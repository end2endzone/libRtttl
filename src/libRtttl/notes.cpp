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

#include "libRtttl/tone.h"
#include "notes.h"
#include "utils.h"

namespace librtttl
{

NOTEDEFLIST buildNotesList();
static NOTEDEFLIST notes = buildNotesList();

NOTEDEFLIST buildNotesList()
{
  NOTEDEFLIST n;
#define DECLARE_NOTE(note_name) {NOTEDEF d; d.name= #note_name ; d.freq=note_name; n.push_back(d);}
  DECLARE_NOTE(NOTE_H   );

  DECLARE_NOTE(NOTE_B0  );
  DECLARE_NOTE(NOTE_C1  );
  DECLARE_NOTE(NOTE_CS1 );
  DECLARE_NOTE(NOTE_D1  );
  DECLARE_NOTE(NOTE_DS1 );
  DECLARE_NOTE(NOTE_E1  );
  DECLARE_NOTE(NOTE_F1  );
  DECLARE_NOTE(NOTE_FS1 );
  DECLARE_NOTE(NOTE_G1  );
  DECLARE_NOTE(NOTE_GS1 );
  DECLARE_NOTE(NOTE_A1  );
  DECLARE_NOTE(NOTE_AS1 );
  DECLARE_NOTE(NOTE_B1  );
  DECLARE_NOTE(NOTE_C2  );
  DECLARE_NOTE(NOTE_CS2 );
  DECLARE_NOTE(NOTE_D2  );
  DECLARE_NOTE(NOTE_DS2 );
  DECLARE_NOTE(NOTE_E2  );
  DECLARE_NOTE(NOTE_F2  );
  DECLARE_NOTE(NOTE_FS2 );
  DECLARE_NOTE(NOTE_G2  );
  DECLARE_NOTE(NOTE_GS2 );
  DECLARE_NOTE(NOTE_A2  );
  DECLARE_NOTE(NOTE_AS2 );
  DECLARE_NOTE(NOTE_B2  );
  DECLARE_NOTE(NOTE_C3  );
  DECLARE_NOTE(NOTE_CS3 );
  DECLARE_NOTE(NOTE_D3  );
  DECLARE_NOTE(NOTE_DS3 );
  DECLARE_NOTE(NOTE_E3  );
  DECLARE_NOTE(NOTE_F3  );
  DECLARE_NOTE(NOTE_FS3 );
  DECLARE_NOTE(NOTE_G3  );
  DECLARE_NOTE(NOTE_GS3 );
  DECLARE_NOTE(NOTE_A3  );
  DECLARE_NOTE(NOTE_AS3 );
  DECLARE_NOTE(NOTE_B3  );
  DECLARE_NOTE(NOTE_C4  );
  DECLARE_NOTE(NOTE_CS4 );
  DECLARE_NOTE(NOTE_D4  );
  DECLARE_NOTE(NOTE_DS4 );
  DECLARE_NOTE(NOTE_E4  );
  DECLARE_NOTE(NOTE_F4  );
  DECLARE_NOTE(NOTE_FS4 );
  DECLARE_NOTE(NOTE_G4  );
  DECLARE_NOTE(NOTE_GS4 );
  DECLARE_NOTE(NOTE_A4  );
  DECLARE_NOTE(NOTE_AS4 );
  DECLARE_NOTE(NOTE_B4  );
  DECLARE_NOTE(NOTE_C5  );
  DECLARE_NOTE(NOTE_CS5 );
  DECLARE_NOTE(NOTE_D5  );
  DECLARE_NOTE(NOTE_DS5 );
  DECLARE_NOTE(NOTE_E5  );
  DECLARE_NOTE(NOTE_F5  );
  DECLARE_NOTE(NOTE_FS5 );
  DECLARE_NOTE(NOTE_G5  );
  DECLARE_NOTE(NOTE_GS5 );
  DECLARE_NOTE(NOTE_A5  );
  DECLARE_NOTE(NOTE_AS5 );
  DECLARE_NOTE(NOTE_B5  );
  DECLARE_NOTE(NOTE_C6  );
  DECLARE_NOTE(NOTE_CS6 );
  DECLARE_NOTE(NOTE_D6  );
  DECLARE_NOTE(NOTE_DS6 );
  DECLARE_NOTE(NOTE_E6  );
  DECLARE_NOTE(NOTE_F6  );
  DECLARE_NOTE(NOTE_FS6 );
  DECLARE_NOTE(NOTE_G6  );
  DECLARE_NOTE(NOTE_GS6 );
  DECLARE_NOTE(NOTE_A6  );
  DECLARE_NOTE(NOTE_AS6 );
  DECLARE_NOTE(NOTE_B6  );
  DECLARE_NOTE(NOTE_C7  );
  DECLARE_NOTE(NOTE_CS7 );
  DECLARE_NOTE(NOTE_D7  );
  DECLARE_NOTE(NOTE_DS7 );
  DECLARE_NOTE(NOTE_E7  );
  DECLARE_NOTE(NOTE_F7  );
  DECLARE_NOTE(NOTE_FS7 );
  DECLARE_NOTE(NOTE_G7  );
  DECLARE_NOTE(NOTE_GS7 );
  DECLARE_NOTE(NOTE_A7  );
  DECLARE_NOTE(NOTE_AS7 );
  DECLARE_NOTE(NOTE_B7  );
  DECLARE_NOTE(NOTE_C8  );
  DECLARE_NOTE(NOTE_CS8 );
  DECLARE_NOTE(NOTE_D8  );
  DECLARE_NOTE(NOTE_DS8 );

#pragma warning( push )
#pragma warning( disable : 4244)
  DECLARE_NOTE(C0  );
  DECLARE_NOTE(Db0 );
  DECLARE_NOTE(D0  );
  DECLARE_NOTE(Eb0 );
  DECLARE_NOTE(E0  );
  DECLARE_NOTE(F0  );
  DECLARE_NOTE(Gb0 );
  DECLARE_NOTE(G0  );
  DECLARE_NOTE(Ab0 );
  DECLARE_NOTE(LA0 );
  DECLARE_NOTE(Bb0 );
  DECLARE_NOTE(B0  );
  DECLARE_NOTE(C1  );
  DECLARE_NOTE(Db1 );
  DECLARE_NOTE(D1  );
  DECLARE_NOTE(Eb1 );
  DECLARE_NOTE(E1  );
  DECLARE_NOTE(F1  );
  DECLARE_NOTE(Gb1 );
  DECLARE_NOTE(G1  );
  DECLARE_NOTE(Ab1 );
  DECLARE_NOTE(LA1 );
  DECLARE_NOTE(Bb1 );
  DECLARE_NOTE(B1  );
  DECLARE_NOTE(C2  );
  DECLARE_NOTE(Db2 );
  DECLARE_NOTE(D2  );
  DECLARE_NOTE(Eb2 );
  DECLARE_NOTE(E2  );
  DECLARE_NOTE(F2  );
  DECLARE_NOTE(Gb2 );
  DECLARE_NOTE(G2  );
  DECLARE_NOTE(Ab2 );
  DECLARE_NOTE(LA2 );
  DECLARE_NOTE(Bb2 );
  DECLARE_NOTE(B2  );
  DECLARE_NOTE(C3  );
  DECLARE_NOTE(Db3 );
  DECLARE_NOTE(D3  );
  DECLARE_NOTE(Eb3 );
  DECLARE_NOTE(E3  );
  DECLARE_NOTE(F3  );
  DECLARE_NOTE(Gb3 );
  DECLARE_NOTE(G3  );
  DECLARE_NOTE(Ab3 );
  DECLARE_NOTE(LA3 );
  DECLARE_NOTE(Bb3 );
  DECLARE_NOTE(B3  );
  DECLARE_NOTE(C4  );
  DECLARE_NOTE(Db4 );
  DECLARE_NOTE(D4  );
  DECLARE_NOTE(Eb4 );
  DECLARE_NOTE(E4  );
  DECLARE_NOTE(F4  );
  DECLARE_NOTE(Gb4 );
  DECLARE_NOTE(G4  );
  DECLARE_NOTE(Ab4 );
  DECLARE_NOTE(LA4 );
  DECLARE_NOTE(Bb4 );
  DECLARE_NOTE(B4  );
  DECLARE_NOTE(C5  );
  DECLARE_NOTE(Db5 );
  DECLARE_NOTE(D5  );
  DECLARE_NOTE(Eb5 );
  DECLARE_NOTE(E5  );
  DECLARE_NOTE(F5  );
  DECLARE_NOTE(Gb5 );
  DECLARE_NOTE(G5  );
  DECLARE_NOTE(Ab5 );
  DECLARE_NOTE(LA5 );
  DECLARE_NOTE(Bb5 );
  DECLARE_NOTE(B5  );
  DECLARE_NOTE(C6  );
  DECLARE_NOTE(Db6 );
  DECLARE_NOTE(D6  );
  DECLARE_NOTE(Eb6 );
  DECLARE_NOTE(E6  );
  DECLARE_NOTE(F6  );
  DECLARE_NOTE(Gb6 );
  DECLARE_NOTE(G6  );
  DECLARE_NOTE(Ab6 );
  DECLARE_NOTE(LA6 );
  DECLARE_NOTE(Bb6 );
  DECLARE_NOTE(B6  );
  DECLARE_NOTE(C7  );
  DECLARE_NOTE(Db7 );
  DECLARE_NOTE(D7  );
  DECLARE_NOTE(Eb7 );
  DECLARE_NOTE(E7  );
  DECLARE_NOTE(F7  );
  DECLARE_NOTE(Gb7 );
  DECLARE_NOTE(G7  );
  DECLARE_NOTE(Ab7 );
  DECLARE_NOTE(LA7 );
  DECLARE_NOTE(Bb7 );
  DECLARE_NOTE(B7  );
  DECLARE_NOTE(C8  );
  DECLARE_NOTE(Db8 );
  DECLARE_NOTE(D8  );
  DECLARE_NOTE(Eb8 );
#pragma warning( pop ) 

#undef DECLARE_NOTE
  return n;
}

NOTEDEF findNote2(int iFreq)
{
  for(size_t i=0; i<notes.size(); i++)
  {
    NOTEDEF & n = notes[i];
    if (n.freq == iFreq)
    {
      return n;
    }
  }
  return INVALID_NOTE;
}

NOTEDEF findNote(int iFreq)
{
  //look for a perfect match of iFreq-1 or iFreq+1 before.
  //This is required to allow priority to NOTE_x instead of AS4 type of notes...
  NOTEDEF n = findNote2(iFreq+1);
  if (n.name != INVALID_NOTE.name)
  {
    //found!
    return n;
  }

  NOTEDEF n2 = findNote2(iFreq-1);
  if (n.name != INVALID_NOTE.name)
  {
    //found!
    return n;
  }

  //not found. Search for a perfect frequency match
  return findNote2(iFreq);
}

NOTEDEF findNote(const char * iName)
{
  for(size_t i=0; i<notes.size(); i++)
  {
    NOTEDEF & n = notes[i];
    if (n.name == iName)
    {
      return n;
    }
  }
  return INVALID_NOTE;
}

//define fake notes for offsetting
#define NOTE_E8 NOTE_DS8
#define NOTE_F8 NOTE_DS8
#define NOTE_FS8 NOTE_DS8
#define NOTE_G8 NOTE_DS8
#define NOTE_GS8 NOTE_DS8
#define NOTE_A8 NOTE_DS8
#define NOTE_AS8 NOTE_DS8
#define NOTE_B8 NOTE_DS8

const unsigned short gAllNotes[] = { 
NOTE_C1, NOTE_CS1, NOTE_D1, NOTE_DS1, NOTE_E1, NOTE_F1, NOTE_FS1, NOTE_G1, NOTE_GS1, NOTE_A1, NOTE_AS1, NOTE_B1,
NOTE_C2, NOTE_CS2, NOTE_D2, NOTE_DS2, NOTE_E2, NOTE_F2, NOTE_FS2, NOTE_G2, NOTE_GS2, NOTE_A2, NOTE_AS2, NOTE_B2,
NOTE_C3, NOTE_CS3, NOTE_D3, NOTE_DS3, NOTE_E3, NOTE_F3, NOTE_FS3, NOTE_G3, NOTE_GS3, NOTE_A3, NOTE_AS3, NOTE_B3,
NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7,
NOTE_C8, NOTE_CS8, NOTE_D8, NOTE_DS8, NOTE_E8, NOTE_F8, NOTE_FS8, NOTE_G8, NOTE_GS8, NOTE_A8, NOTE_AS8, NOTE_B8
};
const unsigned short gNumNotes = sizeof(gAllNotes)/sizeof(gAllNotes[0]);
const unsigned short gNoteOctaveOffset = gNumNotes/8;

unsigned short increaseOctave(unsigned short iFrequency)
{
  return increaseOctave(iFrequency, 0);
}

unsigned short decreaseOctave(unsigned short iFrequency)
{
  return decreaseOctave(iFrequency, 0);
}

unsigned short increaseOctave(unsigned short iFrequency, unsigned short iFrequencyEpsilon)
{
  unsigned short frequencyMin = 0;
  unsigned short frequencyMax = 0;
  computeEpsilonValues<unsigned short>(iFrequency, iFrequencyEpsilon, frequencyMin, frequencyMax);

  for(unsigned short i=0; i<gNumNotes; i++)
  {
    if (frequencyMin <= gAllNotes[i] && gAllNotes[i] <= frequencyMax)
    {
      //found given note frequency
      unsigned short newIndex = i + gNoteOctaveOffset;
      if (newIndex >= 0 && newIndex < gNumNotes)
      {
        return gAllNotes[newIndex];
      }
    }
  }
  return iFrequency;
}

unsigned short decreaseOctave(unsigned short iFrequency, unsigned short iFrequencyEpsilon)
{
  unsigned short frequencyMin = 0;
  unsigned short frequencyMax = 0;
  computeEpsilonValues<unsigned short>(iFrequency, iFrequencyEpsilon, frequencyMin, frequencyMax);

  for(unsigned short i=0; i<gNumNotes; i++)
  {
    if (frequencyMin <= gAllNotes[i] && gAllNotes[i] <= frequencyMax)
    {
      //found given note frequency
      unsigned short newIndex = i - gNoteOctaveOffset;
      if (newIndex >= 0 && newIndex < gNumNotes)
      {
        return gAllNotes[newIndex];
      }
    }
  }
  return iFrequency;
}

}; //namespace librtttl