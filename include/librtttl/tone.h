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

#ifndef LIBRTTTL_TONE_H
#define LIBRTTTL_TONE_H

#include "libRtttl/config.h"
#include "libRtttl/version.h"

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
