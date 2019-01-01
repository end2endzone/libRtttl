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
