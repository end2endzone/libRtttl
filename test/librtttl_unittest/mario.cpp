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

#include "mario.h"
#include "librtttl/tone.h"
#include "utils.h"

#include <cstdio> //for printf

using namespace librtttl;

void playMarioThemeSong(const int & iPin, const int * iArray )
{
  for (int thisNote = 1; thisNote < (iArray[0] * 2 + 1); thisNote = thisNote + 2) { // Run through the notes one at a time
    TONE_FREQUENCY freq = (TONE_FREQUENCY)iArray[thisNote];
    TONE_DURATION duration = (TONE_DURATION)(1000 / iArray[thisNote + 1]);
    if (freq == 0)
    {
      librtttl::arduino::delay(duration);
    }
    else
    {
      librtttl::arduino::tone(iPin, freq, duration);     // Play the single note
      librtttl::arduino::delay(duration+1);
      librtttl::arduino::noTone(iPin);                                                 // Silence the note that was playing
    }
  }
}

void dumpMarioThemeToneCalls(const int & iPin, const int * iArray, const char * iFilename)
{
  clearToneCalls();
  playMarioThemeSong(iPin, iArray);
  dumpRecordedToneCalls(iFilename);
  printf("Mario theme song \"%s\" has %lu function calls\n", iFilename, getRecordedToneCalls().size());
}

void convertMarioTheme2NoteDelayArray(const int & iPin, const int * iMelodyArray, const char * iArrayName, const char * iFilename)
{
  clearToneCalls();
  playMarioThemeSong(iPin, iMelodyArray);

  const TONE_CALL_INFO_LIST & toneCalls = getRecordedToneCalls(); 
  std::string arrayDefinition = calls2NoteDelayArray(toneCalls, iArrayName);

  dumpString(arrayDefinition.c_str(), iFilename);
}
