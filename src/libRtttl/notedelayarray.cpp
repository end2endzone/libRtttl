//
//  libRTTTL Library - v1.2 - 05/27/2016
//  Copyright (C) 2016 Antoine Beauchamp
//  The code & updates for the library can be found on http://end2endzone.com
//
// AUTHOR/LICENSE:
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3.0 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License (LGPL-3.0) for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// DISCLAIMER:
//  This software is furnished "as is", without technical support, and with no 
//  warranty, express or implied, as to its usefulness for any purpose.
//
// PURPOSE:
//  The library allows one to convert an RTTTL melody to other formats like:
//  - Smaller RTTTL code
//  - Arduino code (tone() and delay() function calls)
//  - Binary RTTTL. See AnyRtttl library at http://end2endzone.com
//
//  The library also support converting arduino code (tones) to RTTTL.
//  
// HISTORY:
// 05/27/2016 v1.2 - First public release.
//

#include "NoteDelayArray.h"
#include "libRtttl/tone.h"
#include "libRtttl/rtttl.h"
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