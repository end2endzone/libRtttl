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

#ifndef LIBRTTTL_RTTTL_H
#define LIBRTTTL_RTTTL_H

#include "libRtttl/config.h"
#include "libRtttl/version.h"
#include "libRtttl/tone.h"
#include "libRtttl/stream.h"

#include "binrtttl.h"

#include <vector>
#include <string>

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
LIBRTTTL_EXPORT const RTTTL_SETTINGS & getRtttlSettings();
LIBRTTTL_EXPORT void setRtttlSettings(const RTTTL_SETTINGS & iSettings);

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

LIBRTTTL_EXPORT std::string getErrorDescription(RTTTL_RESULT iResult);

LIBRTTTL_EXPORT TONE_CALL_INFO_LIST playRTTTL(const char *p);
LIBRTTTL_EXPORT TONE_CALL_INFO_LIST playRTTTL(const RTTTL_SONG & iSong);

//backward compatibility.
LIBRTTTL_EXPORT std::string calls2Rtttl(const char * iSongName);
LIBRTTTL_EXPORT std::string calls2Rtttl(const char * iSongName,
                                     const RTTTL_DURATION * iForcedDefaultDuration,
                                     const RTTTL_OCTAVE_VALUE * iForcedDefaultOctave,
                                     const RTTTL_BPM * iForcedDefaultBpm);

LIBRTTTL_EXPORT RTTTL_RESULT calls2Rtttl( const char * iSongName,
                                       std::string & oRtttlCode,
                                       const RTTTL_DURATION * iForcedDefaultDuration = NULL,
                                       const RTTTL_OCTAVE_VALUE * iForcedDefaultOctave = NULL,
                                       const RTTTL_BPM * iForcedDefaultBpm = NULL);
LIBRTTTL_EXPORT RTTTL_RESULT calls2Rtttl( const TONE_CALL_INFO_LIST & iCalls,
                                       const char * iSongName,
                                       std::string & oRtttlCode,
                                       const RTTTL_DURATION * iForcedDefaultDuration = NULL,
                                       const RTTTL_OCTAVE_VALUE * iForcedDefaultOctave = NULL,
                                       const RTTTL_BPM * iForcedDefaultBpm = NULL);

LIBRTTTL_EXPORT RTTTL_RESULT findAlternateRtttlEncoding(const char * iCode, StringVector & oAlternateEncodings);
LIBRTTTL_EXPORT RTTTL_RESULT findAlternateRtttlEncoding(const RTTTL_SONG & iSong, StringVector & oAlternateEncodings);
LIBRTTTL_EXPORT RTTTL_RESULT optimizeEncoding(const char * iCode, std::string & oRtttlCode);

LIBRTTTL_EXPORT void dumpRtttlToneCalls(const char * iArray, const char * iFilename);

LIBRTTTL_EXPORT void reset(RTTTL_NOTE & iNote);
LIBRTTTL_EXPORT void reset(RTTTL_DEFAULT_VALUE_SECTION & iDefaults);
LIBRTTTL_EXPORT void reset(RTTTL_SONG & iSong);

LIBRTTTL_EXPORT std::string toString(const RTTTL_NOTE & iNote);
LIBRTTTL_EXPORT std::string toString(const RTTTL_NOTE & iNote, const RTTTL_DEFAULT_VALUE_SECTION & iDefaults);
LIBRTTTL_EXPORT std::string toString(const RTTTL_DEFAULT_VALUE_SECTION & iDefaults);
LIBRTTTL_EXPORT std::string toString(const RTTTL_DEFAULT_VALUE_SECTION & iDefaults, bool iForceIncludeDefaults);
LIBRTTTL_EXPORT std::string toString(const RTTTL_SONG & iSong);
LIBRTTTL_EXPORT std::string toString(const RTTTL_SONG & iSong, bool iForceIncludeDefaults);

LIBRTTTL_EXPORT void toStream(const RTTTL_SONG & iSong, STREAM & oStream);
LIBRTTTL_EXPORT void toStream10(const RTTTL_SONG & iSong, STREAM & oStream);
LIBRTTTL_EXPORT void toStream16(const RTTTL_SONG & iSong, STREAM & oStream);
LIBRTTTL_EXPORT bool parseStream(const STREAM & iStream, RTTTL_SONG & oSong);
LIBRTTTL_EXPORT RTTTL_SONG parseRtttlString(const char * iCode);
LIBRTTTL_EXPORT RTTTL_RESULT parseRtttlString(const char * iCode, RTTTL_SONG & oSong);

}; //namespace librtttl

#endif //LIBRTTTL_RTTTL_H
