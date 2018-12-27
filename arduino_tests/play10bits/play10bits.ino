// Note:
//   10 Bits RTTTL format requires the BitReader library.
//   The code & updates for the BitReader library can be found on http://end2endzone.com
#include <bitreader.h>

#define RTTTL_SERIAL_DEBUG

// *************** RTTTL STRUCTURE DEFINITION START ***************

typedef unsigned char DURATION_INDEX;
typedef unsigned char NOTE_CHARACTER_INDEX;
typedef unsigned char OCTAVE_INDEX;
typedef unsigned short RTTTL_DURATION;
typedef          char  RTTTL_NOTE_CHARACTER;
typedef unsigned char  RTTTL_OCTAVE;
typedef unsigned short RTTTL_BPM;
typedef unsigned short TONE_DURATION;
typedef unsigned short TONE_FREQUENCY;

#define NOTE_SILENT 0

#ifndef NOTE_B0

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

#endif //NOTE_B0

const TONE_FREQUENCY rtttlNotes[] = { NOTE_SILENT,
NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7
};

#pragma pack(push, 1) // exact fit - no padding
union RTTTL_NOTE
{
  unsigned short raw;
  struct
  {
    DURATION_INDEX durationIdx    : 3; //ranges from 0 to 7. Matches index of durationValues[]
    NOTE_CHARACTER_INDEX noteIdx  : 3; //ranges from 0 to 7. Matches index of noteCharacterCodes[]
    bool pound                    : 1; //ranges from 0 to 1. True if the note is pound
    bool dotted                   : 1; //ranges from 0 to 1. True if the duration is dotted
    OCTAVE_INDEX octaveIdx        : 2; //ranges from 0 to 3. Matches index of octaveValues[]
    unsigned char padding         : 6;
  };
};

union RTTTL_DEFAULT_VALUE_SECTION
{
  unsigned short raw;
  //struct 
  //{
  //  DURATION_INDEX durationIdx :  3; //ranges from 0 to 7.  Matches index of durationValues[]
  //  OCTAVE_INDEX octaveIdx     :  2; //ranges from 0 to 3.  Matches index of octaveValues[]
  //  RTTTL_BPM              bpm : 10; //ranges from 1 to 900.
  //};
  struct //aligned on 8 bits types
  {
    DURATION_INDEX durationIdx :  3; //ranges from 0 to 7.  Matches index of durationValues[]
    OCTAVE_INDEX     octaveIdx :  2; //ranges from 0 to 3.  Matches index of octaveValues[]
  };
  struct //aligned on 16 bits types
  {
    unsigned short        :  5; //padding for durationIdx and octaveIdx
    RTTTL_BPM         bpm : 10; //ranges from 1 to 900.
  };
};
#pragma pack(pop) //back to whatever the previous packing mode was

// *************** RTTTL STRUCTURE DEFINITION END ***************

const unsigned char noteCharacterCodes[] = { 1, 3, 5, 6, 8, 10, 12, 0};
const RTTTL_DURATION durationValues[] = {1, 2, 4, 8, 16, 32};
const RTTTL_OCTAVE octaveValues[] = {4, 5, 6, 7};

void play10BitsRtttl(int pin, const unsigned char * iBuffer, int numNotes) {
  // Absolutely no error checking in here

  RTTTL_DEFAULT_VALUE_SECTION * defaultSection = (RTTTL_DEFAULT_VALUE_SECTION *)iBuffer;
  RTTTL_NOTE * notesBuffer = (RTTTL_NOTE *)&iBuffer[2];

  #ifndef USE_BITADDRESS_READ_WRITE
  BitReader bitreader;
  #else
  BitAddress bitreader;
  #endif
  
  bitreader.setBuffer(notesBuffer);

  RTTTL_BPM bpm = defaultSection->bpm;

  #ifdef RTTTL_SERIAL_DEBUG
  Serial.print("numNotes=");
  Serial.println(numNotes);
  // format: d=N,o=N,b=NNN:
  Serial.print("d=");
  Serial.print(durationValues[defaultSection->durationIdx]);
  Serial.print(",o=");
  Serial.print(octaveValues[defaultSection->octaveIdx]);
  Serial.print(",b=");
  Serial.println(bpm);
  #endif
  
  TONE_DURATION duration;
  byte note;
  RTTTL_OCTAVE scale;

  // BPM usually expresses the number of quarter notes per minute
  TONE_DURATION wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)

  // now begin note loop
  for(int i=0; i<numNotes; i++) {
    RTTTL_NOTE n;
    n.raw = 0;
    bitreader.read(10, &n.raw);

    // first, get note duration, if available
    duration = wholenote / durationValues[n.durationIdx];

    // now get the note
    note = noteCharacterCodes[n.noteIdx];

    // now, get optional '#' sharp
    if(n.pound)
    {
      note++;
    }

    // now, get optional '.' dotted note
    if(n.dotted)
    {
      duration += duration/2;
    }

    // now, get scale
    scale = octaveValues[n.octaveIdx];

    if(note)
    {
      #ifdef RTTTL_SERIAL_DEBUG
      Serial.print(durationValues[n.durationIdx]);
      static const char noteCharacterValues[] =   {'c','d','e','f','g','a','b','p'};
      Serial.print(noteCharacterValues[n.noteIdx]);
      Serial.print( (n.pound ? "#" : "") );
      Serial.print( (n.dotted ? "." : "") );
      Serial.println(octaveValues[n.octaveIdx]);
      #endif
      
      tone(pin, rtttlNotes[(scale - 4) * 12 + note], duration);
      delay(duration+1);
      noTone(pin);
    }
    else
    {
      #ifdef RTTTL_SERIAL_DEBUG
      Serial.print(durationValues[n.durationIdx]);
      static const char noteCharacterValues[] =   {'c','d','e','f','g','a','b','p'};
      Serial.print(noteCharacterValues[n.noteIdx]);
      Serial.print( (n.pound ? "#" : "") );
      Serial.print( (n.dotted ? "." : "") );
      Serial.println();
      #endif

      delay(duration);
    }
  }
}

//rtttl 10 bits binary format for the following: Zelda1:d=4,o=5,b=125:a#,f.,8a#,16a#,16c6,16d6,16d#6,2f6,8p,8f6,16f.6,16f#6,16g#.6,2a#.6,16a#.6,16g#6,16f#.6,8g#.6,16f#.6,2f6,f6,8d#6,16d#6,16f6,2f#6,8f6,8d#6,8c#6,16c#6,16d#6,2f6,8d#6,8c#6,8c6,16c6,16d6,2e6,g6,8f6,16f,16f,8f,16f,16f,8f,16f,16f,8f,8f,a#,f.,8a#,16a#,16c6,16d6,16d#6,2f6,8p,8f6,16f.6,16f#6,16g#.6,2a#.6,c#7,c7,2a6,f6,2f#.6,a#6,a6,2f6,f6,2f#.6,a#6,a6,2f6,d6,2d#.6,f#6,f6,2c#6,a#,c6,16d6,2e6,g6,8f6,16f,16f,8f,16f,16f,8f,16f,16f,8f,8f
const unsigned char Zelda1[] = {0xAA, 0x8F, 0x6A, 0x69, 0xB6, 0x16, 0x5B, 0x04, 0x32, 0xC8, 0x64, 0x86, 0x3B, 0x6D, 0xC8, 0x29, 0x97, 0xE4, 0xA6, 0xCB, 0x2E, 0x99, 0xDC, 0x8E, 0xCB, 0x6D, 0x86, 0x1A, 0x2E, 0xC9, 0x24, 0x87, 0x59, 0x6E, 0xB8, 0xE4, 0x90, 0x44, 0x32, 0x99, 0xE1, 0x92, 0x43, 0x0E, 0x48, 0x20, 0x83, 0x11, 0x8A, 0xB8, 0x21, 0x47, 0x1C, 0x6D, 0xC4, 0x11, 0x47, 0x1B, 0x71, 0xC4, 0xD1, 0x46, 0x1B, 0xA9, 0xA5, 0xD9, 0x5A, 0x6C, 0x11, 0xC8, 0x20, 0x93, 0x19, 0xEE, 0xB4, 0x21, 0xA7, 0x5C, 0x92, 0x9B, 0xAE, 0xD0, 0x02, 0xA7, 0xA8, 0x61, 0xB6, 0x6A, 0xAA, 0x98, 0xA1, 0x86, 0xD9, 0xAA, 0xA9, 0x62, 0x86, 0x0A, 0x26, 0xAB, 0xA5, 0x86, 0x41, 0xAA, 0x25, 0x20, 0x83, 0x11, 0x8A, 0xB8, 0x21, 0x47, 0x1C, 0x6D, 0xC4, 0x11, 0x47, 0x1B, 0x71, 0xC4, 0xD1, 0x46, 0x1B, 0x01};
const int Zelda1_note_length = 97;

#define BUZZER_OUT_PIN 3

void setup() {
  // put your setup code here, to run once:
  pinMode(BUZZER_OUT_PIN, OUTPUT);

  Serial.begin(115200);

  play10BitsRtttl(BUZZER_OUT_PIN, Zelda1, Zelda1_note_length);
}

void loop() {
  // put your main code here, to run repeatedly:

}
