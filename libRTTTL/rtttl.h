#pragma once

#include "librtttl.h"
#include "tone.h"

#include <vector>
#include <string>
#include "stream.h"

#define RTTTL_SONG_NAME_SIZE 11
#define RTTTL_NOTE_SIZE_BITS 10

typedef std::vector<std::string> StringVector;

typedef unsigned char DURATION_INDEX;
typedef unsigned char NOTE_CHARACTER_INDEX;
typedef unsigned char OCTAVE_INDEX;
typedef unsigned char BPM_INDEX;

static DURATION_INDEX       INVALID_DURATION_INDEX        = (DURATION_INDEX)-1;
static NOTE_CHARACTER_INDEX INVALID_NOTE_CHARACTER_INDEX  = (NOTE_CHARACTER_INDEX)-1;
static OCTAVE_INDEX         INVALID_OCTAVE_INDEX          = (OCTAVE_INDEX)-1;
static BPM_INDEX            INVALID_BPM_INDEX             = (BPM_INDEX)-1;

typedef unsigned short RTTTL_DURATION;
typedef          char  RTTTL_NOTE_CHARACTER;
typedef unsigned char  RTTTL_OCTAVE;
typedef unsigned short RTTTL_BPM;

#pragma pack(push, 1) // exact fit - no padding
union RTTTL_NOTE
{
  unsigned short raw;
  struct
  {
    DURATION_INDEX durationIdx    : 3; //ranges from 0 to 7. Matches index of getNoteDurationFromIndex()
    NOTE_CHARACTER_INDEX noteIdx  : 3; //ranges from 0 to 7. Matches index of getNoteCharacterFromIndex()
    bool pound                    : 1; //ranges from 0 to 1. True if the note is pound
    bool dotted                   : 1; //ranges from 0 to 1. True if the duration is dotted
    OCTAVE_INDEX octaveIdx        : 2; //ranges from 0 to 3. Matches index of getNoteOctaveFromIndex()
    unsigned char padding         : 6;
  };
};

union RTTTL_DEFAULT_VALUE_SECTION
{
  unsigned short raw;
  //struct 
  //{
  //  DURATION_INDEX durationIdx :  3; //ranges from 0 to 7.  Matches index of getNoteDurationFromIndex()
  //  OCTAVE_INDEX octaveIdx     :  2; //ranges from 0 to 3.  Matches index of getNoteOctaveFromIndex()
  //  RTTTL_BPM bpm              : 10; //ranges from 0 to 900.
  //  bool             hasAnyBpm :  1;
  //};
  struct //aligned on 8 bits types
  {
    DURATION_INDEX durationIdx :  3; //ranges from 0 to 7.  Matches index of getNoteDurationFromIndex()
    OCTAVE_INDEX     octaveIdx :  2; //ranges from 0 to 3.  Matches index of getNoteOctaveFromIndex()
    unsigned char              :  3; //padding for anyBpm
    unsigned char              :  7; //padding for anyBpm
    bool             hasAnyBpm :  1; //defines if anyBpm has priority over bpmIdx
  };
  struct //aligned on 8 bits types
  {
    unsigned char              :  5; //padding for durationIdx and octaveIdx
    BPM_INDEX       bpmIdxLow  :  3; //matches  low bits of bpmIdx
    BPM_INDEX       bpmIdxHigh :  2; //matches high bits of bpmIdx
  };
  struct //aligned on 16 bits types
  {
    unsigned short         :  5; //padding for durationIdx and octaveIdx
    unsigned short  bpmIdx :  5; //ranges from 0 to 31. Matches index of getOfficialBpmFromIndex()
  };
  struct //aligned on 16 bits types
  {
    unsigned short        :  5; //padding for durationIdx and octaveIdx
    RTTTL_BPM      anyBpm : 10; //ranges from 1 to 900.
    unsigned short        :  1; //padding
  };
};
#pragma pack(pop) //back to whatever the previous packing mode was

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

//Allowed values: 'c','d','e','f','g','a','b','p'
LIBRTTTL_API RTTTL_NOTE_CHARACTER getNoteCharacterFromIndex(NOTE_CHARACTER_INDEX iIndex);
LIBRTTTL_API size_t getNoteCharactersCount();
LIBRTTTL_API NOTE_CHARACTER_INDEX findNoteCharacterIndex(RTTTL_NOTE_CHARACTER n);

//Allowed values: 1, 2, 4, 8, 16, 32
LIBRTTTL_API RTTTL_DURATION getNoteDurationFromIndex(DURATION_INDEX iIndex);
LIBRTTTL_API size_t getNoteDurationsCount();
LIBRTTTL_API DURATION_INDEX findNoteDurationIndex(RTTTL_DURATION n);

//Allowed values: 4, 5, 6, 7
LIBRTTTL_API RTTTL_OCTAVE getNoteOctaveFromIndex(OCTAVE_INDEX iIndex);
LIBRTTTL_API size_t getNoteOctavesCount();
LIBRTTTL_API OCTAVE_INDEX findNoteOctaveIndex(RTTTL_OCTAVE n);

//Allowed values: 25, 28, 31, 35, 40, 45, 50, 56, 63, 70, 80, 90, 100, 112, 125, 140, 160, 180, 200, 225, 250, 285, 320, 355, 400, 450, 500, 565, 635, 715, 800 and 900.
LIBRTTTL_API RTTTL_BPM getOfficialBpmFromIndex(BPM_INDEX iIndex);
LIBRTTTL_API size_t getOfficialBpmCount();
LIBRTTTL_API BPM_INDEX findOfficialBpmIndex(RTTTL_BPM n);

enum RTTTL_RESULT
{
  SUCCESS,
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
                                     const RTTTL_OCTAVE * iForcedDefaultOctave,
                                     const RTTTL_BPM * iForcedDefaultBpm);

LIBRTTTL_API RTTTL_RESULT calls2Rtttl( const char * iSongName,
                                       std::string & oRtttlCode,
                                       const RTTTL_DURATION * iForcedDefaultDuration = NULL,
                                       const RTTTL_OCTAVE * iForcedDefaultOctave = NULL,
                                       const RTTTL_BPM * iForcedDefaultBpm = NULL);
LIBRTTTL_API RTTTL_RESULT calls2Rtttl( const TONE_CALL_INFO_LIST & iCalls,
                                       const char * iSongName,
                                       std::string & oRtttlCode,
                                       const RTTTL_DURATION * iForcedDefaultDuration = NULL,
                                       const RTTTL_OCTAVE * iForcedDefaultOctave = NULL,
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
