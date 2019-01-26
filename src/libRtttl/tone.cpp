/**********************************************************************************
 * MIT License
 * 
 * Copyright (c) 2018 Antoine Beauchamp
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *********************************************************************************/

#include "librtttl/tone.h"
#include "notes.h"
#include "utils.h"

#include <cstdlib>    // atoi()
#include <cstdio>     // fprintf(), fclose()

namespace librtttl
{

static TONE_CALL_INFO_LIST calls;

void reset(TONE_CALL_INFO & oCall)
{
  oCall.delay = INVALID_TONE_DURATION;
  oCall._tone = INVALID_TONE_INFO;
}

void clearToneCalls()
{
  calls.clear();
}

const TONE_CALL_INFO_LIST & getRecordedToneCalls()
{
  return calls;
}

namespace arduino
{

void tone(int pin, TONE_FREQUENCY freq, TONE_DURATION duration)
{
  TONE_CALL_INFO c;
  c._tone.freq = freq;
  c._tone.duration = duration;
  calls.push_back(c);
}

void tone(int pin, double freq, double duration)
{
  tone(pin, (TONE_FREQUENCY)freq, (TONE_DURATION)duration);
}

void noTone(int pin)
{
}

void delay(TONE_DURATION iDuration)
{
  TONE_CALL_INFO c;
  c.delay = iDuration;
  calls.push_back(c);
}

}; //namespace arduino

std::string calls2NoteDelayArray(const TONE_CALL_INFO_LIST & iToneCalls, const char * iArrayName)
{
  std::string data;

  data.append("const int ");
  data.append(iArrayName);
  data.append("[] = {");

  int ignoreDelay = INVALID_TONE_DURATION;

  for(size_t i=0; i<iToneCalls.size(); i++)
  {
    const TONE_CALL_INFO & c = iToneCalls[i];

    if (ignoreDelay != INVALID_TONE_DURATION && c.delay != INVALID_TONE_DURATION && ignoreDelay == c.delay)
    {
      //ignored
      ignoreDelay = INVALID_TONE_DURATION;
      continue;
    }

    if (i != 0)
    {
      data.append(",");
    }

    if (c._tone != INVALID_TONE_INFO)
    {
      NOTEDEF note = findNote(c._tone.freq);
      int noteDelay = c._tone.duration;

      data.append(note.name);
      data.append(",");
      data.append( itostr(noteDelay) );

      ignoreDelay = c._tone.duration+1; //ignore next delay if time is 1 ms longer which is the delay() call that is usually following the tone() call.
    }
    if (c.delay != INVALID_TONE_DURATION)
    {
      //true delay
      //NOTE_H, 8
      static const char * SILENCE_NOTE = "NOTE_H";
      data.append(SILENCE_NOTE);
      data.append(",");
      data.append( itostr(c.delay) );
    }
  }

  data.append( "};" );

  return data;
}

void removeExpectedDelays(TONE_CALL_INFO_LIST & ioCalls)
{
  //expected delays are the one which is 1 ms longer than a previous tone() call.

  int ignoreDelay = INVALID_TONE_DURATION;
  for(size_t i=0; i<ioCalls.size(); i++)
  {
    TONE_CALL_INFO & c = ioCalls[i];

    if (c._tone != INVALID_TONE_INFO)
    {
      ignoreDelay = c._tone.duration + 1;
    }
    if (c.delay != INVALID_TONE_DURATION && c.delay == ignoreDelay)
    {
      //this is a "safe to delete" delay
      c.delay = INVALID_TONE_DURATION; //slag delay as safe to delete
      ignoreDelay = INVALID_TONE_DURATION; //prevent double delay deletions
    }
    else if (c.delay != INVALID_TONE_DURATION && c.delay > ignoreDelay)
    {
      //this is a delay which is > that tone duration.
      //reduce delay by tone duration
      c.delay -= ignoreDelay;

      ignoreDelay = INVALID_TONE_DURATION; //prevent double delay deletions
    }
  }

  //remove empty TONE_CALL_INFO from iCalls
  TONE_CALL_INFO_LIST tmpCalls;
  for(size_t i=0; i<ioCalls.size(); i++)
  {
    TONE_CALL_INFO & c = ioCalls[i];
    if (c._tone != INVALID_TONE_INFO || c.delay != INVALID_TONE_DURATION)
    {
      //keep this one
      tmpCalls.push_back(c);
    }
    else
    {
      //empty TONE_CALL_INFO
    }
  }
  ioCalls = tmpCalls;
}

void increaseOctave(TONE_CALL_INFO_LIST & ioCalls)
{
  increaseOctave(ioCalls, 0);
}

void decreaseOctave(TONE_CALL_INFO_LIST & ioCalls)
{
  decreaseOctave(ioCalls, 0);
}

void increaseOctave(TONE_CALL_INFO_LIST & ioCalls, unsigned short iFrequencyEpsilon)
{
  for(size_t i=0; i<ioCalls.size(); i++)
  {
    TONE_CALL_INFO & c = ioCalls[i];
    if (c._tone != INVALID_TONE_INFO)
    {
      c._tone.freq = increaseOctave(c._tone.freq, iFrequencyEpsilon);
    }
  }
}

void decreaseOctave(TONE_CALL_INFO_LIST & ioCalls, unsigned short iFrequencyEpsilon)
{
  for(size_t i=0; i<ioCalls.size(); i++)
  {
    TONE_CALL_INFO & c = ioCalls[i];
    if (c._tone != INVALID_TONE_INFO)
    {
      c._tone.freq = decreaseOctave(c._tone.freq, iFrequencyEpsilon);
    }
  }
}

bool parseToneCall(const char * iText, TONE_CALL_INFO & oCall)
{
  const std::string text = iText;

  reset(oCall);

  bool found = false;

  //look for tone() function
  int toneStartPos = 0;
  int toneEndPos = 0;
  StringVector toneParams;
  if (findFunctionCall(text.c_str(), "tone", 3, toneParams, toneStartPos, toneEndPos))
  {
    //that's a valid tone() function call
    oCall._tone.freq =     (TONE_FREQUENCY)atoi(toneParams[1].c_str());
    oCall._tone.duration =  (TONE_DURATION)atoi(toneParams[2].c_str());
    found = true;
  }

  //look for delay() function
  int delayStartPos = 0;
  int delayEndPos = 0;
  StringVector delayParams;
  if (findFunctionCall(text.c_str(), "delay", 1, delayParams, delayStartPos, delayEndPos))
  {
    //that's a valid tone() function call
    oCall.delay = (TONE_DURATION)atoi(delayParams[0].c_str());
    found = true;
  }

  return found;
}

bool parseToneCallsFile(const char * iFilename, TONE_CALL_INFO_LIST & oToneCalls)
{
  //look for "tone(a,b,c);" and "delay(a)" patterns

  oToneCalls.clear();
  
  StringVector lines;
  if ( !getTextFileContent(iFilename,lines ) )
  {
    //failed to read file
    return false;
  }

  //for each line in file
  for(size_t i=0; i<lines.size(); i++)
  {
    const std::string & line = lines[i];
    TONE_CALL_INFO c;
    if ( parseToneCall(line.c_str(), c) )
    {
      oToneCalls.push_back(c);
    }
  }

  return true;
}

bool parseToneCallsString(const char * iValue, TONE_CALL_INFO_LIST & oToneCalls)
{
  //look for "tone(a,b,c);" and "delay(a)" patterns

  oToneCalls.clear();
  std::string value = iValue;

  std::string accumulator;
  for(size_t i=0; i<value.size(); i++)
  {
    char c = iValue[i];

    if (c != '\n')
    {
      //add
      accumulator.append(1, c);
    }
    else
    {
      //process accumulator
      TONE_CALL_INFO call;
      if ( parseToneCall(accumulator.c_str(), call) )
      {
        oToneCalls.push_back(call);
      }
      accumulator = "";
    }
  }

  if (accumulator != "")
  {
    TONE_CALL_INFO call;
    if ( parseToneCall(accumulator.c_str(), call) )
    {
      oToneCalls.push_back(call);
    }
    accumulator = "";
  }

  return true;
}

void dumpToneCalls(const TONE_CALL_INFO_LIST & iToneCalls, const char * iFileName)
{
  FILE * f = fopen(iFileName, "w");
  if (!f)
    return;

  for(size_t i=0; i<iToneCalls.size(); i++)
  {
    const TONE_CALL_INFO & c = iToneCalls[i];
    if (c._tone != INVALID_TONE_INFO)
    {
      fprintf(f, "tone(0, %d, %d);\n", c._tone.freq, c._tone.duration);
    }
    if (c.delay != INVALID_TONE_DURATION)
    {
      fprintf(f, "delay(%d);\n", c.delay);
    }
  }
  fclose(f);
}

void dumpRecordedToneCalls(const char * iFilename)
{
  const TONE_CALL_INFO_LIST & calls = getRecordedToneCalls();

  dumpToneCalls(calls, iFilename);
}

}; //namespace librtttl