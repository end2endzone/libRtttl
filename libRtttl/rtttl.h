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

#ifndef LIBRTTTL_RTTTL_H
#define LIBRTTTL_RTTTL_H

#include "librtttl.h"
#include "binrtttl.h"
#include "tone.h"

#include <vector>
#include <string>
#include "stream.h"

using namespace anyrtttl;

namespace librtttl
{

typedef std::vector<std::string> StringVector;
typedef std::vector<RTTTL_NOTE> RTTTL_NOTE_LIST;

struct RTTTL_SONG
{
  char name[RTTTL_SONG_NAME_SIZE];
  RTTTL_DEFAULT_VALUE_SECTION defaults;
  RTTTL_NOTE_LIST notes;
};

struct RTTTL_SETTINGS
{
  //relaxed RTTTL format settings
  bool forceDefaultSection; //always insert default section values (even if they match official default values)
  //bool allowOctave1To8; //allow octave from 1 to 8 instead of the official 4 to 7.
  bool allowAnyBpm; //allow any beat per minute between 25 and 900. (Not only official RTTTL BPM)
  bool allowToneFrequencyClamping; //allow the RTTTL encoder to clamp the given frequencies between minimum and maximum RTTTL supported frequencies.
  bool allowOctaveOffseting; //allow the RTTTL encoder to offset all the given frequencies by +- 1 to 4 octave to match minimum and maximum RTTTL supported frequencies.
};

static RTTTL_SETTINGS       OFFICIAL_RTTTL_SETTINGS = {false, false, false, false};
static RTTTL_SETTINGS         STRICT_RTTTL_SETTINGS = {true,  false, false, false};
static RTTTL_SETTINGS   BPM_FRIENDLY_RTTTL_SETTINGS = {true,  false,  true,  true};
static RTTTL_SETTINGS        RELAXED_RTTTL_SETTINGS = {true,   true,  true,  true};
LIBRTTTL_API const RTTTL_SETTINGS & getRtttlSettings();
LIBRTTTL_API void setRtttlSettings(const RTTTL_SETTINGS & iSettings);

enum RTTTL_RESULT
{
  SUCCESS,
  UNKNOWN,
  NOT_SUPPORTED,
  FAILED_OCTAVE_OFFSETTING,
  FAILED_TONE_FREQUENCIES_CLAMPING,
  TONE_FREQUENCIES_OUT_OF_BOUNDS,
  BPM_OUT_OF_BOUNDS, //BPM not in [1,900] range
  UNKNOWN_BPM, //BPM outside of official values
};

LIBRTTTL_API std::string getErrorDescription(RTTTL_RESULT iResult);

LIBRTTTL_API TONE_CALL_INFO_LIST playRTTTL(const char *p);
LIBRTTTL_API TONE_CALL_INFO_LIST playRTTTL(const RTTTL_SONG & iSong);

//backward compatibility.
LIBRTTTL_API std::string calls2Rtttl(const char * iSongName);
LIBRTTTL_API std::string calls2Rtttl(const char * iSongName,
                                     const RTTTL_DURATION * iForcedDefaultDuration,
                                     const RTTTL_OCTAVE_VALUE * iForcedDefaultOctave,
                                     const RTTTL_BPM * iForcedDefaultBpm);

LIBRTTTL_API RTTTL_RESULT calls2Rtttl( const char * iSongName,
                                       std::string & oRtttlCode,
                                       const RTTTL_DURATION * iForcedDefaultDuration = NULL,
                                       const RTTTL_OCTAVE_VALUE * iForcedDefaultOctave = NULL,
                                       const RTTTL_BPM * iForcedDefaultBpm = NULL);
LIBRTTTL_API RTTTL_RESULT calls2Rtttl( const TONE_CALL_INFO_LIST & iCalls,
                                       const char * iSongName,
                                       std::string & oRtttlCode,
                                       const RTTTL_DURATION * iForcedDefaultDuration = NULL,
                                       const RTTTL_OCTAVE_VALUE * iForcedDefaultOctave = NULL,
                                       const RTTTL_BPM * iForcedDefaultBpm = NULL);

LIBRTTTL_API RTTTL_RESULT findAlternateRtttlEncoding(const char * iCode, StringVector & oAlternateEncodings);
LIBRTTTL_API RTTTL_RESULT findAlternateRtttlEncoding(const RTTTL_SONG & iSong, StringVector & oAlternateEncodings);
LIBRTTTL_API RTTTL_RESULT optimizeEncoding(const char * iCode, std::string & oRtttlCode);

LIBRTTTL_API void dumpRtttlToneCalls(const char * iArray, const char * iFilename);

LIBRTTTL_API void reset(RTTTL_NOTE & iNote);
LIBRTTTL_API void reset(RTTTL_DEFAULT_VALUE_SECTION & iDefaults);
LIBRTTTL_API void reset(RTTTL_SONG & iSong);

LIBRTTTL_API std::string toString(const RTTTL_NOTE & iNote);
LIBRTTTL_API std::string toString(const RTTTL_NOTE & iNote, const RTTTL_DEFAULT_VALUE_SECTION & iDefaults);
LIBRTTTL_API std::string toString(const RTTTL_DEFAULT_VALUE_SECTION & iDefaults);
LIBRTTTL_API std::string toString(const RTTTL_DEFAULT_VALUE_SECTION & iDefaults, bool iForceIncludeDefaults);
LIBRTTTL_API std::string toString(const RTTTL_SONG & iSong);
LIBRTTTL_API std::string toString(const RTTTL_SONG & iSong, bool iForceIncludeDefaults);

LIBRTTTL_API void toStream(const RTTTL_SONG & iSong, STREAM & oStream);
LIBRTTTL_API void toStream10(const RTTTL_SONG & iSong, STREAM & oStream);
LIBRTTTL_API void toStream16(const RTTTL_SONG & iSong, STREAM & oStream);
LIBRTTTL_API bool parseStream(const STREAM & iStream, RTTTL_SONG & oSong);
LIBRTTTL_API RTTTL_SONG parseRtttlString(const char * iCode);
LIBRTTTL_API RTTTL_RESULT parseRtttlString(const char * iCode, RTTTL_SONG & oSong);

}; //namespace librtttl

#endif //LIBRTTTL_RTTTL_H
