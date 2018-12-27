#pragma once

#include "librtttl.h"

#include <vector>
#include <string>

typedef unsigned short TONE_DURATION;
typedef unsigned short TONE_FREQUENCY;

static TONE_DURATION INVALID_TONE_DURATION = (TONE_DURATION)-1;
static TONE_FREQUENCY INVALID_TONE_FREQUENCY = (TONE_FREQUENCY)-1;

struct LIBRTTTL_API TONE_INFO
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
struct LIBRTTTL_API TONE_CALL_INFO
{
  TONE_INFO _tone;
  TONE_DURATION delay;

  TONE_CALL_INFO(){
    _tone = INVALID_TONE_INFO;
    delay = INVALID_TONE_DURATION;
  }
};

typedef std::vector<TONE_CALL_INFO> TONE_CALL_INFO_LIST;

LIBRTTTL_API void reset(TONE_CALL_INFO & oCall);

LIBRTTTL_API void tone(int pin, TONE_FREQUENCY freq, TONE_DURATION duration);
LIBRTTTL_API void tone(int pin, double freq, double duration);
LIBRTTTL_API void noTone(int pin);
LIBRTTTL_API void delay(TONE_DURATION iDuration);
LIBRTTTL_API void clearToneCalls();
LIBRTTTL_API const TONE_CALL_INFO_LIST & getRecordedToneCalls();
LIBRTTTL_API void dumpRecordedToneCalls(const char * iFilename);
LIBRTTTL_API void dumpToneCalls(const TONE_CALL_INFO_LIST & iToneCalls, const char * iFileName);
LIBRTTTL_API std::string calls2NoteDelayArray(const TONE_CALL_INFO_LIST & iToneCalls, const char * iArrayName);
LIBRTTTL_API void removeExpectedDelays(TONE_CALL_INFO_LIST & iCalls);

LIBRTTTL_API void increaseOctave(TONE_CALL_INFO_LIST & ioCalls);
LIBRTTTL_API void decreaseOctave(TONE_CALL_INFO_LIST & ioCalls);
LIBRTTTL_API void increaseOctave(TONE_CALL_INFO_LIST & ioCalls, unsigned short iFrequencyEpsilon);
LIBRTTTL_API void decreaseOctave(TONE_CALL_INFO_LIST & ioCalls, unsigned short iFrequencyEpsilon);

LIBRTTTL_API bool parseToneCall(const char * iText, TONE_CALL_INFO & oCall);
LIBRTTTL_API bool parseToneCallsFile(const char * iFilename, TONE_CALL_INFO_LIST & oToneCalls);
LIBRTTTL_API bool parseToneCallsString(const char * iFilename, TONE_CALL_INFO_LIST & oToneCalls);
