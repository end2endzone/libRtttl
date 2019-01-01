#include "ImperialMarch.h"
#include "libRtttl/tone.h"
#include "utils.h"
#include "../../src/libRtttl/notes.h" //temporary fix

using namespace librtttl;

void playImperialMarch(int iPinNumber) {

  //http://www.instructables.com/id/How-to-easily-play-music-with-buzzer-on-arduino-Th/

  //librtttl::arduino::tone(pin, note, duration)
  librtttl::arduino::tone(iPinNumber,LA3,Q); 
  librtttl::arduino::delay(1+Q); //librtttl::arduino::delay duration should always be 1 ms more than the note in order to separate them.
  librtttl::arduino::tone(iPinNumber,LA3,Q);
  librtttl::arduino::delay(1+Q);
  librtttl::arduino::tone(iPinNumber,LA3,Q);
  librtttl::arduino::delay(1+Q);
  librtttl::arduino::tone(iPinNumber,F3,E+S);
  librtttl::arduino::delay(1+E+S);
  librtttl::arduino::tone(iPinNumber,C4,S);
  librtttl::arduino::delay(1+S);

  librtttl::arduino::tone(iPinNumber,LA3,Q);
  librtttl::arduino::delay(1+Q);
  librtttl::arduino::tone(iPinNumber,F3,E+S);
  librtttl::arduino::delay(1+E+S);
  librtttl::arduino::tone(iPinNumber,C4,S);
  librtttl::arduino::delay(1+S);
  librtttl::arduino::tone(iPinNumber,LA3,H);
  librtttl::arduino::delay(1+H);

  librtttl::arduino::tone(iPinNumber,E4,Q); 
  librtttl::arduino::delay(1+Q); 
  librtttl::arduino::tone(iPinNumber,E4,Q);
  librtttl::arduino::delay(1+Q);
  librtttl::arduino::tone(iPinNumber,E4,Q);
  librtttl::arduino::delay(1+Q);
  librtttl::arduino::tone(iPinNumber,F4,E+S);
  librtttl::arduino::delay(1+E+S);
  librtttl::arduino::tone(iPinNumber,C4,S);
  librtttl::arduino::delay(1+S);

  librtttl::arduino::tone(iPinNumber,Ab3,Q);
  librtttl::arduino::delay(1+Q);
  librtttl::arduino::tone(iPinNumber,F3,E+S);
  librtttl::arduino::delay(1+E+S);
  librtttl::arduino::tone(iPinNumber,C4,S);
  librtttl::arduino::delay(1+S);
  librtttl::arduino::tone(iPinNumber,LA3,H);
  librtttl::arduino::delay(1+H);

  librtttl::arduino::tone(iPinNumber,LA4,Q);
  librtttl::arduino::delay(1+Q);
  librtttl::arduino::tone(iPinNumber,LA3,E+S);
  librtttl::arduino::delay(1+E+S);
  librtttl::arduino::tone(iPinNumber,LA3,S);
  librtttl::arduino::delay(1+S);
  librtttl::arduino::tone(iPinNumber,LA4,Q);
  librtttl::arduino::delay(1+Q);
  librtttl::arduino::tone(iPinNumber,Ab4,E+S);
  librtttl::arduino::delay(1+E+S);
  librtttl::arduino::tone(iPinNumber,G4,S);
  librtttl::arduino::delay(1+S);

  librtttl::arduino::tone(iPinNumber,Gb4,S);
  librtttl::arduino::delay(1+S);
  librtttl::arduino::tone(iPinNumber,E4,S);
  librtttl::arduino::delay(1+S);
  librtttl::arduino::tone(iPinNumber,F4,E);
  librtttl::arduino::delay(1+E);
  librtttl::arduino::delay(1+E);//PAUSE
  librtttl::arduino::tone(iPinNumber,Bb3,E);
  librtttl::arduino::delay(1+E);
  librtttl::arduino::tone(iPinNumber,Eb4,Q);
  librtttl::arduino::delay(1+Q);
  librtttl::arduino::tone(iPinNumber,D4,E+S);
  librtttl::arduino::delay(1+E+S);
  librtttl::arduino::tone(iPinNumber,Db4,S);
  librtttl::arduino::delay(1+S);

  librtttl::arduino::tone(iPinNumber,C4,S);
  librtttl::arduino::delay(1+S);
  librtttl::arduino::tone(iPinNumber,B3,S);
  librtttl::arduino::delay(1+S);
  librtttl::arduino::tone(iPinNumber,C4,E);
  librtttl::arduino::delay(1+E);
  librtttl::arduino::delay(1+E);//PAUSE QUASI FINE RIGA
  librtttl::arduino::tone(iPinNumber,F3,E);
  librtttl::arduino::delay(1+E);
  librtttl::arduino::tone(iPinNumber,Ab3,Q);
  librtttl::arduino::delay(1+Q);
  librtttl::arduino::tone(iPinNumber,F3,E+S);
  librtttl::arduino::delay(1+E+S);
  librtttl::arduino::tone(iPinNumber,LA3,S);
  librtttl::arduino::delay(1+S);

  librtttl::arduino::tone(iPinNumber,C4,Q);
  librtttl::arduino::delay(1+Q);
  librtttl::arduino::tone(iPinNumber,LA3,E+S);
  librtttl::arduino::delay(1+E+S);
  librtttl::arduino::tone(iPinNumber,C4,S);
  librtttl::arduino::delay(1+S);
  librtttl::arduino::tone(iPinNumber,E4,H);
  librtttl::arduino::delay(1+H);

  librtttl::arduino::tone(iPinNumber,LA4,Q);
  librtttl::arduino::delay(1+Q);
  librtttl::arduino::tone(iPinNumber,LA3,E+S);
  librtttl::arduino::delay(1+E+S);
  librtttl::arduino::tone(iPinNumber,LA3,S);
  librtttl::arduino::delay(1+S);
  librtttl::arduino::tone(iPinNumber,LA4,Q);
  librtttl::arduino::delay(1+Q);
  librtttl::arduino::tone(iPinNumber,Ab4,E+S);
  librtttl::arduino::delay(1+E+S);
  librtttl::arduino::tone(iPinNumber,G4,S);
  librtttl::arduino::delay(1+S);

  librtttl::arduino::tone(iPinNumber,Gb4,S);
  librtttl::arduino::delay(1+S);
  librtttl::arduino::tone(iPinNumber,E4,S);
  librtttl::arduino::delay(1+S);
  librtttl::arduino::tone(iPinNumber,F4,E);
  librtttl::arduino::delay(1+E);
  librtttl::arduino::delay(1+E);//PAUSE
  librtttl::arduino::tone(iPinNumber,Bb3,E);
  librtttl::arduino::delay(1+E);
  librtttl::arduino::tone(iPinNumber,Eb4,Q);
  librtttl::arduino::delay(1+Q);
  librtttl::arduino::tone(iPinNumber,D4,E+S);
  librtttl::arduino::delay(1+E+S);
  librtttl::arduino::tone(iPinNumber,Db4,S);
  librtttl::arduino::delay(1+S);

  librtttl::arduino::tone(iPinNumber,C4,S);
  librtttl::arduino::delay(1+S);
  librtttl::arduino::tone(iPinNumber,B3,S);
  librtttl::arduino::delay(1+S);
  librtttl::arduino::tone(iPinNumber,C4,E);
  librtttl::arduino::delay(1+E);
  librtttl::arduino::delay(1+E);//PAUSE QUASI FINE RIGA
  librtttl::arduino::tone(iPinNumber,F3,E);
  librtttl::arduino::delay(1+E);
  librtttl::arduino::tone(iPinNumber,Ab3,Q);
  librtttl::arduino::delay(1+Q);
  librtttl::arduino::tone(iPinNumber,F3,E+S);
  librtttl::arduino::delay(1+E+S);
  librtttl::arduino::tone(iPinNumber,C4,S);
  librtttl::arduino::delay(1+S);

  librtttl::arduino::tone(iPinNumber,LA3,Q);
  librtttl::arduino::delay(1+Q);
  librtttl::arduino::tone(iPinNumber,F3,E+S);
  librtttl::arduino::delay(1+E+S);
  librtttl::arduino::tone(iPinNumber,C4,S);
  librtttl::arduino::delay(1+S);
  librtttl::arduino::tone(iPinNumber,LA3,H);
  librtttl::arduino::delay(1+H);

  librtttl::arduino::noTone(iPinNumber);
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
