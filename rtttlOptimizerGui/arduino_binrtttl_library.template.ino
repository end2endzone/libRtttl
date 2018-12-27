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
