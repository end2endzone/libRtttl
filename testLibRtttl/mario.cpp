#include "mario.h"
#include "tone.h"
#include "utils.h"

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
  printf("Mario theme song \"%s\" has %d function calls\n", iFilename, getRecordedToneCalls().size());
}

void convertMarioTheme2NoteDelayArray(const int & iPin, const int * iMelodyArray, const char * iArrayName, const char * iFilename)
{
  clearToneCalls();
  playMarioThemeSong(iPin, iMelodyArray);

  const TONE_CALL_INFO_LIST & toneCalls = getRecordedToneCalls(); 
  std::string arrayDefinition = calls2NoteDelayArray(toneCalls, iArrayName);

  dumpString(arrayDefinition.c_str(), iFilename);
}
