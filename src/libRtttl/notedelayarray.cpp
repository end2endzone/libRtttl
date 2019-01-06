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

#include "notedelayarray.h"
#include "librtttl/tone.h"
#include "librtttl/rtttl.h"
#include "utils.h"

namespace librtttl
{

void _playToneDelayArray(const int & iPin, const int * iArray, int iArraySize)
{
  for (int i = 0; i < iArraySize-1; i += 2) {
    const int & noteFreq = iArray[i];
    const int & noteDelay = iArray[i+1];
    if (noteFreq != 0)
    {
      librtttl::arduino::tone(iPin, (TONE_FREQUENCY)noteFreq, (TONE_DURATION)noteDelay);
      librtttl::arduino::delay(noteDelay + 1);
    }
    else
    {
      librtttl::arduino::delay(noteDelay);
    }
    librtttl::arduino::noTone(iPin);
  }
}

TONE_CALL_INFO_LIST playToneDelayArray(const int & iPin, const int * iArray, int iArraySize)
{
  clearToneCalls();
  _playToneDelayArray(iPin, iArray, iArraySize);
  return getRecordedToneCalls();
}

void convertRtttl2NoteDelayArray(const int & iPin, const char * iRtttl, const char * iArrayName, const char * iFilename)
{
  const TONE_CALL_INFO_LIST toneCalls = playRTTTL(iRtttl);
  std::string arrayDefinition = calls2NoteDelayArray(toneCalls, iArrayName);

  dumpString(arrayDefinition.c_str(), iFilename);
}

}; //namespace librtttl