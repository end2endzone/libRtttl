#include "NoteDelayArray.h"
#include "tone.h"
#include "utils.h"
#include "rtttl.h"

void _playToneDelayArray(const int & iPin, const int * iArray, int iArraySize)
{
  for (int i = 0; i < iArraySize-1; i += 2) {
    const int & noteFreq = iArray[i];
    const int & noteDelay = iArray[i+1];
    if (noteFreq != 0)
    {
      tone(iPin, (TONE_FREQUENCY)noteFreq, (TONE_DURATION)noteDelay);
      delay(noteDelay + 1);
    }
    else
    {
      delay(noteDelay);
    }
    noTone(iPin);
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
