#include "mario.h"
#include "tone.h"
#include "utils.h"

void playMarioThemeSong(const int & iPin, const int * iArray )
{
  for (int thisNote = 1; thisNote < (iArray[0] * 2 + 1); thisNote = thisNote + 2) { // Run through the notes one at a time
    tone(iPin, (TONE_FREQUENCY)iArray[thisNote], (TONE_DURATION)(1000 / iArray[thisNote + 1]));     // Play the single note
#pragma warning( push )
#pragma warning( disable : 4244) // conversion from 'double' to 'int', possible loss of data
    //delay((1000 / iArray[thisNote + 1]) * 1.30);                          // Delay for the specified time

    int toneDuration = (1000 / iArray[thisNote + 1]);
    int firstDelay = toneDuration+1;
    int fullDelay = toneDuration * 1.30;
    int secondDelay = fullDelay-firstDelay;
    delay(firstDelay);
    delay(secondDelay);

#pragma warning( pop ) 
    noTone(iPin);                                                 // Silence the note that was playing
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
