#include "ImperialMarch.h"
#include "tone.h"
#include "utils.h"

void playImperialMarch(int iPinNumber) {

  //http://www.instructables.com/id/How-to-easily-play-music-with-buzzer-on-arduino-Th/

  //tone(pin, note, duration)
  tone(iPinNumber,LA3,Q); 
  delay(1+Q); //delay duration should always be 1 ms more than the note in order to separate them.
  tone(iPinNumber,LA3,Q);
  delay(1+Q);
  tone(iPinNumber,LA3,Q);
  delay(1+Q);
  tone(iPinNumber,F3,E+S);
  delay(1+E+S);
  tone(iPinNumber,C4,S);
  delay(1+S);

  tone(iPinNumber,LA3,Q);
  delay(1+Q);
  tone(iPinNumber,F3,E+S);
  delay(1+E+S);
  tone(iPinNumber,C4,S);
  delay(1+S);
  tone(iPinNumber,LA3,H);
  delay(1+H);

  tone(iPinNumber,E4,Q); 
  delay(1+Q); 
  tone(iPinNumber,E4,Q);
  delay(1+Q);
  tone(iPinNumber,E4,Q);
  delay(1+Q);
  tone(iPinNumber,F4,E+S);
  delay(1+E+S);
  tone(iPinNumber,C4,S);
  delay(1+S);

  tone(iPinNumber,Ab3,Q);
  delay(1+Q);
  tone(iPinNumber,F3,E+S);
  delay(1+E+S);
  tone(iPinNumber,C4,S);
  delay(1+S);
  tone(iPinNumber,LA3,H);
  delay(1+H);

  tone(iPinNumber,LA4,Q);
  delay(1+Q);
  tone(iPinNumber,LA3,E+S);
  delay(1+E+S);
  tone(iPinNumber,LA3,S);
  delay(1+S);
  tone(iPinNumber,LA4,Q);
  delay(1+Q);
  tone(iPinNumber,Ab4,E+S);
  delay(1+E+S);
  tone(iPinNumber,G4,S);
  delay(1+S);

  tone(iPinNumber,Gb4,S);
  delay(1+S);
  tone(iPinNumber,E4,S);
  delay(1+S);
  tone(iPinNumber,F4,E);
  delay(1+E);
  delay(1+E);//PAUSE
  tone(iPinNumber,Bb3,E);
  delay(1+E);
  tone(iPinNumber,Eb4,Q);
  delay(1+Q);
  tone(iPinNumber,D4,E+S);
  delay(1+E+S);
  tone(iPinNumber,Db4,S);
  delay(1+S);

  tone(iPinNumber,C4,S);
  delay(1+S);
  tone(iPinNumber,B3,S);
  delay(1+S);
  tone(iPinNumber,C4,E);
  delay(1+E);
  delay(1+E);//PAUSE QUASI FINE RIGA
  tone(iPinNumber,F3,E);
  delay(1+E);
  tone(iPinNumber,Ab3,Q);
  delay(1+Q);
  tone(iPinNumber,F3,E+S);
  delay(1+E+S);
  tone(iPinNumber,LA3,S);
  delay(1+S);

  tone(iPinNumber,C4,Q);
  delay(1+Q);
  tone(iPinNumber,LA3,E+S);
  delay(1+E+S);
  tone(iPinNumber,C4,S);
  delay(1+S);
  tone(iPinNumber,E4,H);
  delay(1+H);

  tone(iPinNumber,LA4,Q);
  delay(1+Q);
  tone(iPinNumber,LA3,E+S);
  delay(1+E+S);
  tone(iPinNumber,LA3,S);
  delay(1+S);
  tone(iPinNumber,LA4,Q);
  delay(1+Q);
  tone(iPinNumber,Ab4,E+S);
  delay(1+E+S);
  tone(iPinNumber,G4,S);
  delay(1+S);

  tone(iPinNumber,Gb4,S);
  delay(1+S);
  tone(iPinNumber,E4,S);
  delay(1+S);
  tone(iPinNumber,F4,E);
  delay(1+E);
  delay(1+E);//PAUSE
  tone(iPinNumber,Bb3,E);
  delay(1+E);
  tone(iPinNumber,Eb4,Q);
  delay(1+Q);
  tone(iPinNumber,D4,E+S);
  delay(1+E+S);
  tone(iPinNumber,Db4,S);
  delay(1+S);

  tone(iPinNumber,C4,S);
  delay(1+S);
  tone(iPinNumber,B3,S);
  delay(1+S);
  tone(iPinNumber,C4,E);
  delay(1+E);
  delay(1+E);//PAUSE QUASI FINE RIGA
  tone(iPinNumber,F3,E);
  delay(1+E);
  tone(iPinNumber,Ab3,Q);
  delay(1+Q);
  tone(iPinNumber,F3,E+S);
  delay(1+E+S);
  tone(iPinNumber,C4,S);
  delay(1+S);

  tone(iPinNumber,LA3,Q);
  delay(1+Q);
  tone(iPinNumber,F3,E+S);
  delay(1+E+S);
  tone(iPinNumber,C4,S);
  delay(1+S);
  tone(iPinNumber,LA3,H);
  delay(1+H);

  noTone(iPinNumber);
}

void dumpImperialMarchToneCalls(const char * iFilename)
{
  clearToneCalls();
  playImperialMarch(0);
  dumpRecordedToneCalls(iFilename);
  printf("Imperial march has %d function calls\n", getRecordedToneCalls().size());
}

void convertImperialMarch2NoteDelayArray(const char * iArrayName, const char * iFilename)
{
  clearToneCalls();
  playImperialMarch(0);

  const TONE_CALL_INFO_LIST & toneCalls = getRecordedToneCalls(); 
  std::string arrayDefinition = calls2NoteDelayArray(toneCalls, iArrayName);

  dumpString(arrayDefinition.c_str(), iFilename);
}
