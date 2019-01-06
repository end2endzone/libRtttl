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

#ifndef LIBRTTTL_TONE_H
#define LIBRTTTL_TONE_H

#include "librtttl/config.h"
#include "librtttl/version.h"

#include <vector>
#include <string>

namespace librtttl
{

typedef unsigned short TONE_DURATION;
typedef unsigned short TONE_FREQUENCY;

static TONE_DURATION INVALID_TONE_DURATION = (TONE_DURATION)-1;
static TONE_FREQUENCY INVALID_TONE_FREQUENCY = (TONE_FREQUENCY)-1;

struct LIBRTTTL_EXPORT TONE_INFO
{
  TONE_FREQUENCY freq;
  TONE_DURATION duration;
  TONE_INFO()
  {
    freq = INVALID_TONE_FREQUENCY;
    duration = INVALID_TONE_DURATION;
  }
  bool operator != (const TONE_INFO & i) const
  {
    bool equal = i.freq == freq && i.duration == duration;
    return !equal;
  }
};
static TONE_INFO INVALID_TONE_INFO;
struct LIBRTTTL_EXPORT TONE_CALL_INFO
{
  TONE_INFO _tone;
  TONE_DURATION delay;

  TONE_CALL_INFO(){
    _tone = INVALID_TONE_INFO;
    delay = INVALID_TONE_DURATION;
  }
};

typedef std::vector<TONE_CALL_INFO> TONE_CALL_INFO_LIST;

LIBRTTTL_EXPORT void reset(TONE_CALL_INFO & oCall);

namespace arduino
{

LIBRTTTL_EXPORT void tone(int pin, TONE_FREQUENCY freq, TONE_DURATION duration);
LIBRTTTL_EXPORT void tone(int pin, double freq, double duration);
LIBRTTTL_EXPORT void noTone(int pin);
LIBRTTTL_EXPORT void delay(TONE_DURATION iDuration);

}; //namespace arduino

LIBRTTTL_EXPORT void clearToneCalls();
LIBRTTTL_EXPORT const TONE_CALL_INFO_LIST & getRecordedToneCalls();
LIBRTTTL_EXPORT void dumpRecordedToneCalls(const char * iFilename);
LIBRTTTL_EXPORT void dumpToneCalls(const TONE_CALL_INFO_LIST & iToneCalls, const char * iFileName);
LIBRTTTL_EXPORT std::string calls2NoteDelayArray(const TONE_CALL_INFO_LIST & iToneCalls, const char * iArrayName);
LIBRTTTL_EXPORT void removeExpectedDelays(TONE_CALL_INFO_LIST & iCalls);

LIBRTTTL_EXPORT void increaseOctave(TONE_CALL_INFO_LIST & ioCalls);
LIBRTTTL_EXPORT void decreaseOctave(TONE_CALL_INFO_LIST & ioCalls);
LIBRTTTL_EXPORT void increaseOctave(TONE_CALL_INFO_LIST & ioCalls, unsigned short iFrequencyEpsilon);
LIBRTTTL_EXPORT void decreaseOctave(TONE_CALL_INFO_LIST & ioCalls, unsigned short iFrequencyEpsilon);

LIBRTTTL_EXPORT bool parseToneCall(const char * iText, TONE_CALL_INFO & oCall);
LIBRTTTL_EXPORT bool parseToneCallsFile(const char * iFilename, TONE_CALL_INFO_LIST & oToneCalls);
LIBRTTTL_EXPORT bool parseToneCallsString(const char * iFilename, TONE_CALL_INFO_LIST & oToneCalls);

}; //namespace librtttl

#endif //LIBRTTTL_TONE_H
