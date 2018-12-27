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

#include "rtttl.h"
#include "tone.h"

#include "notes.h"
#include "utils.h"
#include "BitReader.h"

#include <algorithm>

//#define DEBUG_STREAMS

// https://en.wikipedia.org/wiki/Ring_Tone_Transfer_Language
// http://www.mobilefish.com/tutorials/rtttl/rtttl_quickguide_specification.html

namespace librtttl
{

typedef std::vector<DURATION_INDEX> DurationList;
typedef std::vector<OCTAVE_INDEX> OctaveList;
typedef std::vector<RTTTL_BPM> BpmList;
typedef unsigned char byte;

#define isdigit(n) (n >= '0' && n <= '9')
#define OCTAVE_OFFSET 0
#define BUZZER_OUT_PIN 0
#define NOTE_SILENT 0

#define RTTTL_DEFAULT_DURATION 4
#define RTTTL_DEFAULT_OCTAVE 6
#define RTTTL_DEFAULT_BPM 63

#define RTTTL_DEFAULT_DURATION_INDEX 2
#define RTTTL_DEFAULT_OCTAVE_INDEX 2
#define RTTTL_DEFAULT_BPM_INDEX 8

#define BPM_MIN 25
#define BPM_MAX 900
#define MAX_ALLOWED_DURATION_VALUE 32

const TONE_FREQUENCY TONE_FREQUENCY_EPSILON = 5; //in Hertz 
const TONE_DURATION DURATION_EPSILON = 3; //in ms

struct RTTTL_DURATION_RESULT
{
  DURATION_INDEX durationIdx;
  bool dotted;
};

const TONE_FREQUENCY gRtttlFrequencies[] = { NOTE_SILENT,
NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7
};
const size_t gNumRtttlFrequencies = sizeof(gRtttlFrequencies)/sizeof(gRtttlFrequencies[0]);

static RTTTL_SETTINGS gRtttlSettings = RELAXED_RTTTL_SETTINGS;

template <class T>
class HeapArray {
public:
  HeapArray(int count)
  {
    this->count = 0;
    this->items = NULL;

    this->items = new T[count];
    if (this->items)
    {
      this->count = count;
      memset(this->items, 0, sizeof(T)*count);
    }
  }
  ~HeapArray()
  {
    if (this->items)
    {
      delete[] this->items;
      this->items = NULL;
      this->count = 0;
    }
  }

  T * items;
  int count;
}; 




const RTTTL_SETTINGS & getRtttlSettings()
{
  return gRtttlSettings;
}

void setRtttlSettings(const RTTTL_SETTINGS & iSettings)
{
  gRtttlSettings = iSettings;
}

void reset(RTTTL_NOTE & iNote)
{
  iNote.raw = 0;
}

void reset(RTTTL_DEFAULT_VALUE_SECTION & iDefaults)
{
  iDefaults.raw = 0;

  //reset to default values
  iDefaults.durationIdx = RTTTL_DEFAULT_DURATION_INDEX;
  iDefaults.octaveIdx = RTTTL_DEFAULT_OCTAVE_INDEX;
  iDefaults.bpm = RTTTL_DEFAULT_BPM;
}

void reset(RTTTL_SONG & iSong)
{
  iSong.name[0] = '\0';
  reset(iSong.defaults);
}

std::string toString(const RTTTL_NOTE & iNote)
{
  static const unsigned char NOTE_BUFFER_SIZE = 16;
  static char buffer[NOTE_BUFFER_SIZE];

  if (getNoteLetterFromIndex(iNote.noteIdx) == 'p')
  {
    sprintf(buffer, "%d%c%s", 
      getNoteDurationFromIndex(iNote.durationIdx),
      getNoteLetterFromIndex(iNote.noteIdx),
      (iNote.dotted ? "." : "")
      );
  }
  else
  {
    sprintf(buffer, "%d%c%s%s%s", 
      getNoteDurationFromIndex(iNote.durationIdx),
      getNoteLetterFromIndex(iNote.noteIdx),
      (iNote.pound ? "#" : ""),
      (iNote.dotted ? "." : ""),
      getNoteOctaveFromIndex(iNote.octaveIdx)
      );
  }

  return std::string(buffer);
}

std::string toString(const RTTTL_NOTE & iNote, const RTTTL_DEFAULT_VALUE_SECTION & iDefaults)
{
  std::string note;

  //duration
  if (iDefaults.durationIdx != iNote.durationIdx)
  {
    note.append( itostr( getNoteDurationFromIndex(iNote.durationIdx) ) );
  }

  //note character
  note.append( toString(getNoteLetterFromIndex(iNote.noteIdx)) );

  //pound
  note.append( (iNote.pound ? "#" : "") );

  //dotted
  note.append( (iNote.dotted ? "." : "") );

  //octave
  if (iDefaults.octaveIdx != iNote.octaveIdx && getNoteLetterFromIndex(iNote.noteIdx) != 'p')
  {
    note.append( itostr( getNoteOctaveFromIndex(iNote.octaveIdx) ) );
  }

  return note;
}

std::string toString(const RTTTL_SONG & iSong)
{
  return toString(iSong, gRtttlSettings.forceDefaultSection);
}

std::string toString(const RTTTL_DEFAULT_VALUE_SECTION & iDefaults)
{
  return toString(iDefaults, gRtttlSettings.forceDefaultSection);
}

std::string toString(const RTTTL_DEFAULT_VALUE_SECTION & iDefaults, bool iForceIncludeDefaults)
{
  std::string defaultSection;
  if (iForceIncludeDefaults || iDefaults.durationIdx != RTTTL_DEFAULT_DURATION_INDEX)
  {
    defaultSection.append("d=" + itostr( getNoteDurationFromIndex(iDefaults.durationIdx) ));
  }
  if (iForceIncludeDefaults || iDefaults.octaveIdx != RTTTL_DEFAULT_OCTAVE_INDEX)
  {
    if (defaultSection != "")
      defaultSection.append(",");
    defaultSection.append("o=" + itostr( getNoteOctaveFromIndex(iDefaults.octaveIdx) ));
  }
  if (iForceIncludeDefaults || iDefaults.bpm != RTTTL_DEFAULT_BPM)
  {
    if (defaultSection != "")
      defaultSection.append(",");
    defaultSection.append("b=" + itostr(iDefaults.bpm));
  }

  return defaultSection;
}

std::string toString(const RTTTL_SONG & iSong, bool iForceIncludeDefaults)
{
  std::string rtttl;
  if (iSong.name != NULL)
  {
    rtttl.append(iSong.name);
  }
  rtttl.append(":");

  //defaults
  std::string defaultSection = toString(iSong.defaults, iForceIncludeDefaults);

  //add default section to rtttl string
  rtttl.append(defaultSection);
  if (defaultSection != "")
  {
    rtttl.append(":");
  }

  //for each note
  for(size_t i=0; i<iSong.notes.size(); i++)
  {
    const RTTTL_NOTE & n = iSong.notes[i];
    std::string noteStr = toString(n, iSong.defaults);
    rtttl.append(noteStr);

    bool isLastNote = (i+1 >= iSong.notes.size());
    if (!isLastNote)
      rtttl.append(",");
  }

  return rtttl;
}

void sortBpms(BpmList & ioBpms)
{
  struct BpmSorter
  {
    inline bool operator() (const RTTTL_BPM & bpm1, const RTTTL_BPM & bpm2)
    {
      BPM_INDEX index1 = findBpmIndex(bpm1);
      BPM_INDEX index2 = findBpmIndex(bpm2);
      bool isBpm1Official = (index1 != INVALID_BPM_INDEX);
      bool isBpm2Official = (index2 != INVALID_BPM_INDEX);
      
      if (isBpm1Official && isBpm2Official)
      {
        return bpm1 < bpm2;
      }
      else if (isBpm1Official && !isBpm2Official)
      {
        return true; //bpm1 is < bpm2
      }
      else if (!isBpm1Official && isBpm2Official)
      {
        return false; //bpm1 is > bpm2
      }
      else if (!isBpm1Official && !isBpm2Official)
      {
        return bpm1 < bpm2;
      }

      //dont know what to do
      return (bpm1 < bpm1);
    }
  };

  std::sort(ioBpms.begin(), ioBpms.end(), BpmSorter());
}

RTTTL_RESULT applyBpm(RTTTL_DEFAULT_VALUE_SECTION & ioDefaults, const RTTTL_BPM & iBpm)
{
  BPM_INDEX bpmIdx = findBpmIndex(iBpm);
  if ( 1 > iBpm && iBpm > 900 )
  {
    return BPM_OUT_OF_BOUNDS;
  }
  else if ( gRtttlSettings.allowAnyBpm && bpmIdx == INVALID_BPM_INDEX )
  {
    //not an official bpm
    ioDefaults.bpm = iBpm;
  }
  else if ( bpmIdx != INVALID_BPM_INDEX )
  {
    ioDefaults.bpm = iBpm;
  }
  else
  {
    return UNKNOWN_BPM;
  }

  return SUCCESS;
}

RTTTL_SONG parseRtttlString(const char * iCode)
{
  RTTTL_SONG song;
  reset(song);
  RTTTL_RESULT parseResult = parseRtttlString(iCode, song);
  if (parseResult != SUCCESS)
  {
    reset(song);
  }
  return song;
}

LIBRTTTL_API RTTTL_RESULT parseRtttlString(const char * iCode, RTTTL_SONG & oSong)
{
  RTTTL_SONG s;
  reset(s);

  // Absolutely no error checking in here
  const char * p = iCode;

  RTTTL_DURATION default_dur = RTTTL_DEFAULT_DURATION;
  RTTTL_OCTAVE_VALUE default_oct = RTTTL_DEFAULT_OCTAVE;
  RTTTL_BPM bpm = RTTTL_DEFAULT_BPM;
  int num;

  // format: d=N,o=N,b=NNN:
  // find the start (skip name, etc)

  num = 0;
  memset(s.name, 0, RTTTL_SONG_NAME_SIZE);
  while(*p != ':')
  {
    if (num < RTTTL_SONG_NAME_SIZE - 1)
      s.name[num] = *p;
    num++;
    p++;    // next char
  }
  s.name[RTTTL_SONG_NAME_SIZE-1] = '\0'; //force null terminated string
  p++;                     // skip ':'

  // get default duration
  if(*p == 'd')
  {
    p++; p++;              // skip "d="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    if(num > 0) default_dur = num;
    p++;                   // skip comma
  }

  // get default octave
  if(*p == 'o')
  {
    p++; p++;              // skip "o="
    num = *p++ - '0';
    if(num >= 3 && num <=7) default_oct = num;
    p++;                   // skip comma
  }

  // get BPM
  if(*p == 'b')
  {
    p++; p++;              // skip "b="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    bpm = num;
    p++;                   // skip colon
  }

  s.defaults.durationIdx = findNoteDurationIndex(default_dur);
  s.defaults.octaveIdx = findNoteOctaveIndex(default_oct);

  RTTTL_RESULT bpmSetResult = applyBpm(s.defaults, bpm);
  if (bpmSetResult != SUCCESS)
  {
    //FAIL.
    reset(oSong);
    return bpmSetResult;
  }

  RTTTL_NOTE note;

  // now begin note loop
  while(*p)
  {

#ifdef _DEBUG
    const char * noteStart = p;
#endif

    reset(note);

    //apply defaults on the note in case they are missing from the rtttl code
    note.durationIdx = s.defaults.durationIdx;
    note.octaveIdx = s.defaults.octaveIdx;

    // first, get note duration, if available
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    if (num)
    {
      note.durationIdx = findNoteDurationIndex(num);
    }

    // now get the note
    int noteIndex = findNoteLetterIndex(*p);
    if (noteIndex == -1)
    {
      note.noteIdx = findNoteLetterIndex('p');
    }
    else
    {
      note.noteIdx = noteIndex;
    }
    p++;

    // now, get optional '#' sharp
    if(*p == '#')
    {
      note.pound = true;
      p++;
    }

    // now, get optional '.' dotted note
    if(*p == '.')
    {
      note.dotted = true;
      p++;
    }

    // now, get scale
    if(isdigit(*p))
    {
      note.octaveIdx = findNoteOctaveIndex(*p - '0');
      p++;
    }

    if(*p == ',')
      p++;       // skip comma for next note (or we may be at the end)

#ifdef _DEBUG
    std::string noteStr = toString(note, s.defaults);
#endif

    s.notes.push_back(note);
  }

  oSong = s;
  return SUCCESS;
}

size_t findRtttlFrequencyIndex(TONE_FREQUENCY iFrequency, TONE_FREQUENCY iEpsilon)
{
  TONE_FREQUENCY frequencyMin = 0;
  TONE_FREQUENCY frequencyMax = 0;
  computeEpsilonValues<TONE_FREQUENCY>(iFrequency, iEpsilon, frequencyMin, frequencyMax);

  for(size_t i=0; i<gNumRtttlFrequencies; i++)
  {
    TONE_FREQUENCY noteFreq = gRtttlFrequencies[i];
    if (frequencyMin <= noteFreq && noteFreq <= frequencyMax)
    {
      //found
      return i;
    }
  }

  return (size_t)-1; //not found
}

size_t findClosestRtttlFrequencyIndex(TONE_FREQUENCY iFrequency)
{
  TONE_FREQUENCY gRtttlNotes_delta[gNumRtttlFrequencies] = {0};

  //compute delta
  for(int i=0; i<gNumRtttlFrequencies; i++)
  {
    TONE_FREQUENCY noteFreq = gRtttlFrequencies[i];
    gRtttlNotes_delta[i] = abs(noteFreq - iFrequency);
  }

  size_t closestNoteindex = findMinValueIndex(gRtttlNotes_delta, sizeof(gRtttlNotes_delta)/sizeof(gRtttlNotes_delta[0]));
  return closestNoteindex;
}

RTTTL_DURATION_RESULT findDurationIndex(RTTTL_BPM iBpm, TONE_DURATION iToneDuration)
{
  // BPM usually expresses the number of quarter notes per minute
  int wholenote = (60 * 1000L / iBpm) * 4;  // this is the time for whole note (in milliseconds)

  //look for a perfect match
  TONE_DURATION toneDurationMin = 0;
  TONE_DURATION toneDurationMax = 0;
  computeEpsilonValues(iToneDuration, DURATION_EPSILON, toneDurationMin, toneDurationMax);
  for(DURATION_INDEX durationIdx = 0; durationIdx < getNoteDurationsCount(); durationIdx++)
  {
    int durationCode = getNoteDurationFromIndex(durationIdx);
    int durationMs = wholenote / durationCode;
    if (toneDurationMin <= durationMs && durationMs <= toneDurationMax)
    {
      //perfect match
      RTTTL_DURATION_RESULT d = {durationIdx, false};
      return d;
    }
  }

  //perfect match not found, check for a dotted note
  TONE_DURATION dottedDuration = (iToneDuration*2)/3;
  computeEpsilonValues<TONE_DURATION>(dottedDuration, DURATION_EPSILON, toneDurationMin, toneDurationMax);
  for(DURATION_INDEX durationIdx = 0; durationIdx < getNoteDurationsCount(); durationIdx++)
  {
    int durationCode = getNoteDurationFromIndex(durationIdx);
    int durationMs = wholenote / durationCode;
    if (toneDurationMin <= durationMs && durationMs <= toneDurationMax)
    {
      //perfect dotted match
      RTTTL_DURATION_RESULT d = {durationIdx, true};
      return d;
    }
  }

  //invalid
  RTTTL_DURATION_RESULT invalidDuration = {INVALID_DURATION_INDEX, false};
  return invalidDuration;
}

RTTTL_DURATION_RESULT findClosestDurationIndex(RTTTL_BPM iBpm, TONE_DURATION iToneDuration)
{
  //invalid
  RTTTL_DURATION_RESULT result = {INVALID_DURATION_INDEX, false};

  // BPM usually expresses the number of quarter notes per minute
  int wholenote = (60 * 1000L / iBpm) * 4;  // this is the time for whole note (in milliseconds)

  //search for a perfect match
  result = findDurationIndex(iBpm, iToneDuration);
  if (result.durationIdx != INVALID_DURATION_INDEX)
  {
    //a perfect match found
    return result;
  }

  uint16_t durationsCount = getNoteDurationsCount();

  //find the closest values for duration
  //store time diff for each durationIdx in counters
  //countes in range [0, count-1]       are normal durations
  //countes in range [count, count*2-1] are dotted durations
  HeapArray<int> diffs(durationsCount*2);//from 0 to 5

  //init diffs with huge values
  for(int i=0; i<diffs.count; i++)
  {
    diffs.items[i] = 0X0FFFFFFE;
  }

  //find closest duration
  for(DURATION_INDEX durationIdx = 0; durationIdx < durationsCount; durationIdx++)
  {
    int durationCode = getNoteDurationFromIndex(durationIdx);
    int durationMs = wholenote / durationCode;
    int durrationDiff = abs(iToneDuration - durationMs);

    //store the result
    diffs.items[durationIdx] = durrationDiff;
  }

  //find the closest dotted duration
  TONE_DURATION dottedDuration = (iToneDuration*2)/3;
  for(DURATION_INDEX durationIdx = 0; durationIdx < durationsCount; durationIdx++)
  {
    int durationCode = getNoteDurationFromIndex(durationIdx);
    int durationMs = wholenote / durationCode;
    int durrationDiff = abs(dottedDuration - durationMs);

    //store the result
    durationIdx += durationsCount;
    diffs.items[durationIdx] = durrationDiff;
  }

  //find the duration which is the closest to iToneDuration
  size_t closestDurationIdx = findMinValueIndex (diffs.items, diffs.count);
  int closestDurationDiffValue = diffs.items[closestDurationIdx];

  if (closestDurationIdx >= 0 && closestDurationIdx < durationsCount)
  {
    result.durationIdx = closestDurationIdx;
    result.dotted = false;
  }
  else
  {
    //that's a dotted duration
    closestDurationIdx -= durationsCount;
    result.durationIdx = closestDurationIdx;
    result.dotted = true;
  }
  return result;
}

BpmList findValidBpms(TONE_DURATION iDuration)
{
  BpmList bpms;

  const TONE_CALL_INFO_LIST & calls = getRecordedToneCalls();
  if (gRtttlSettings.allowAnyBpm)
  {
    for(RTTTL_BPM bpm = BPM_MIN; bpm<BPM_MAX; bpm++)
    {
      RTTTL_DURATION_RESULT result = findDurationIndex(bpm, iDuration);
      if (result.durationIdx != INVALID_DURATION_INDEX)
      {
        bpms.push_back(bpm);
      }
    }
  }
  else
  {
    for(BPM_INDEX bpmIdx = 0; bpmIdx<getBpmsCount(); bpmIdx++)
    {
      RTTTL_BPM bpm = getBpmFromIndex(bpmIdx);
      RTTTL_DURATION_RESULT result = findDurationIndex(bpm, iDuration);
      if (result.durationIdx != INVALID_DURATION_INDEX)
      {
        bpms.push_back(bpm);
      }
    }
  }

  return bpms;
}

BpmList findAllBpms(const TONE_CALL_INFO_LIST & iCalls)
{
  TONE_CALL_INFO_LIST calls = iCalls; //make a copy
  removeExpectedDelays(calls);

  //find the best values for bpm by looking at all the note durations
  RTTTL_BPM bpmCount[BPM_MAX+1] = {0};
  for(size_t i=0; i<calls.size(); i++)
  {
    const TONE_CALL_INFO & c = calls[i];
    if (c._tone != INVALID_TONE_INFO)
    {
      //identify note
      int noteDuration = c._tone.duration;

      //what are the possible bpm for this duration ?
      BpmList bpms = findValidBpms(c._tone.duration);

      //dump bmp stats
      for(size_t j=0; j<bpms.size(); j++)
      {
        RTTTL_BPM bpmIndex = bpms[j];
        bpmCount[bpmIndex]++;
      }
    }
    if (c.delay != INVALID_TONE_DURATION)
    {
      //what are the possible bpm for this duration ?
      BpmList bpms = findValidBpms(c.delay);

      //dump bmp stats
      for(size_t j=0; j<bpms.size(); j++)
      {
        RTTTL_BPM bpmIndex = bpms[j];
        bpmCount[bpmIndex]++;
      }
    }
  }
  //find most popular bpm as low as possible
  size_t popularBpmIndex = (RTTTL_BPM)findMaxValueIndex(bpmCount, BPM_MAX+1);
  RTTTL_BPM popularBpm = bpmCount[popularBpmIndex];

  //find locations where the maximum value is found
  std::vector<size_t> allBpm = findIndexOf(popularBpm, bpmCount, BPM_MAX+1);

  //convert allBpm to BpmList
  BpmList bpms;
  for(size_t i=0; i<allBpm.size(); i++)
  {
    RTTTL_BPM bpm = (RTTTL_BPM)allBpm[i];
    bpms.push_back(bpm);
  }

  //make sure that bpms that are official are 
  //end up at the beginning of the list.
  sortBpms(bpms);

  return bpms;
}

RTTTL_BPM findBestBpm(const TONE_CALL_INFO_LIST & iCalls)
{
  BpmList allBpm = findAllBpms(iCalls);

  //prefer default bmp 63
  for(size_t i=0; i<allBpm.size(); i++)
  {
    RTTTL_BPM bpm = allBpm[i];
    if (bpm == RTTTL_DEFAULT_BPM)
      return RTTTL_DEFAULT_BPM;
  }

  //prefer official bpm between 100 and 200
  for(size_t i=0; i<allBpm.size(); i++)
  {
    RTTTL_BPM foundBpm = allBpm[i];
    for(BPM_INDEX j=0; j<getBpmsCount(); j++)
    {
      RTTTL_BPM allowedBpm = getBpmFromIndex(j);
      if (allowedBpm >= 100 && allowedBpm <= 200 && allowedBpm == foundBpm)
        return foundBpm;
    }
  }

  //prefer official bpm 60 and over. ie 50 has same priority as 250
  for(size_t i=0; i<allBpm.size(); i++)
  {
    RTTTL_BPM foundBpm = allBpm[i];
    for(BPM_INDEX j=0; j<getBpmsCount(); j++)
    {
      RTTTL_BPM allowedBpm = getBpmFromIndex(j);
      if (allowedBpm >= 60 && allowedBpm <= 250 && allowedBpm == foundBpm)
        return foundBpm;
    }
  }

  //prefer any official bpm
  for(size_t i=0; i<allBpm.size(); i++)
  {
    RTTTL_BPM foundBpm = allBpm[i];
    for(BPM_INDEX j=0; j<getBpmsCount(); j++)
    {
      RTTTL_BPM allowedBpm = getBpmFromIndex(j);
      if (allowedBpm == foundBpm)
        return foundBpm;
    }
  }

  if (gRtttlSettings.allowAnyBpm)
  {
    //prefer unofficial bpm between 100 and 160
    for(size_t i=0; i<allBpm.size(); i++)
    {
      RTTTL_BPM bpm = allBpm[i];
      if (bpm >= 100 && bpm <= 160)
        return bpm;
    }

    //prefer unofficial bpm between 160 and 200
    for(size_t i=0; i<allBpm.size(); i++)
    {
      RTTTL_BPM bpm = allBpm[i];
      if (bpm >= 160 && bpm <= 200)
        return bpm;
    }

    //prefer unofficial bpm 60 and over. ie 50 has same priority as 250
    for(size_t i=0; i<allBpm.size(); i++)
    {
      RTTTL_BPM bpm = allBpm[i];
      if (bpm >= 60)
        return bpm;
    }
  }

  //no bpm preferences
  return allBpm[0];
}

OctaveList getSongOctavesList(const RTTTL_NOTE_LIST & iNotes)
{
  //find the best values for octave by looking at all the notes
  HeapArray<int> counters(getNoteOctavesCount());//from 0 to 3

  for(size_t i=0; i<iNotes.size(); i++)
  {
    const RTTTL_NOTE & note = iNotes[i];
    counters.items[note.octaveIdx]++;
  }

  //find used octave
  OctaveList octaves;
  for(OCTAVE_INDEX octaveIdx=0; octaveIdx<getNoteOctavesCount(); octaveIdx++)
  {
    int octaveCount = counters.items[octaveIdx];
    if (octaveCount > 0)
    {
      octaves.push_back(octaveIdx);
    }
  }

  return octaves;
}

RTTTL_OCTAVE_VALUE getBestOctaveOptimization(const RTTTL_NOTE_LIST & iNotes)
{
  //find the best values for octave by looking at all the notes
  HeapArray<int> counters(getNoteOctavesCount());  //from 0 to 3

  for(size_t i=0; i<iNotes.size(); i++)
  {
    const RTTTL_NOTE & note = iNotes[i];
    counters.items[note.octaveIdx]++;
  }

  //find most popular octave as low as possible
  size_t popularOctaveIndex = findMaxValueIndex(counters.items, counters.count);

  return (RTTTL_OCTAVE_VALUE)popularOctaveIndex;
}

DurationList getSongDurationsList(const RTTTL_NOTE_LIST & iNotes)
{
  //find the best values for duration by looking at all the note durations
  HeapArray<int> counters(getNoteDurationsCount());

  for(size_t i=0; i<iNotes.size(); i++)
  {
    const RTTTL_NOTE & note = iNotes[i];
    counters.items[note.durationIdx]++;
  }

  //find used octave
  DurationList durations;
  for(DURATION_INDEX durationIdx=0; durationIdx<getNoteOctavesCount(); durationIdx++)
  {
    int durationCount = counters.items[durationIdx];
    if (durationCount > 0)
    {
      durations.push_back(durationIdx);
    }
  }

  return durations;
}


RTTTL_DURATION getBestDurationOptimization(const RTTTL_NOTE_LIST & iNotes)
{
  //find the best values for duration by looking at all the note durations
  HeapArray<int> counters(getNoteDurationsCount());

  for(size_t i=0; i<iNotes.size(); i++)
  {
    const RTTTL_NOTE & note = iNotes[i];
    counters.items[note.durationIdx]++;
  }

  //find most popular duration as low as possible
  size_t popularDurationIndex = findMaxValueIndex(counters.items, counters.count);
  return (RTTTL_DURATION)popularDurationIndex;
}

RTTTL_NOTE getRtttlNote(RTTTL_BPM iBpm, TONE_FREQUENCY iFrequency, TONE_DURATION iDuration) //iDuration in ms
{
  RTTTL_NOTE n;
  reset(n);

  //find note matching the given frequency +-5 Hertz
  size_t noteFrequencyIndex = findRtttlFrequencyIndex(iFrequency, TONE_FREQUENCY_EPSILON);

  //if a perfect match not found, find the closest note possible.
  if (noteFrequencyIndex == (size_t)-1)
    noteFrequencyIndex = findClosestRtttlFrequencyIndex(iFrequency);

  if (noteFrequencyIndex == 0)
  {
    n.noteIdx = findNoteLetterIndex('p');
  }
  else
  {
    //find a match where ((scale - 4) * 12 + note) == noteIndex
    //since note e# == note f, search for perfect note match before trying to find a # match
    //to give priority to "f" since it is a smaller string
    bool noteFound = false;
    for(NOTE_LETTER_INDEX noteIdx = 0; noteIdx<getNoteLettersCount(); noteIdx++)
    {
      int noteOffset = getNoteOffsetFromLetterIndex(noteIdx);
      for(OCTAVE_INDEX octaveIdx = 0; octaveIdx < getNoteOctavesCount(); octaveIdx++)
      {
        RTTTL_OCTAVE_VALUE scale = getNoteOctaveFromIndex(octaveIdx);

        //evaluate
        int tmp = ((scale - 4) * 12 + noteOffset);
        if (tmp == noteFrequencyIndex)
        {
          //found!
          n.noteIdx = noteIdx;
          n.octaveIdx = octaveIdx;
          noteFound = true;
          scale = 99; noteIdx = 250;//exit both for
          break;
        }
      }
    }

    //if perfect note is still not found, check for a # note
    if (!noteFound)
    {
      for(NOTE_LETTER_INDEX noteIdx = 0; noteIdx<getNoteLettersCount(); noteIdx++)
      {
        int noteOffset = getNoteOffsetFromLetterIndex(noteIdx);
        for(OCTAVE_INDEX octaveIdx = 0; octaveIdx < getNoteOctavesCount(); octaveIdx++)
        {
          RTTTL_OCTAVE_VALUE scale = getNoteOctaveFromIndex(octaveIdx);

          //evaluate
          int tmp = ((scale - 4) * 12 + (noteOffset) + 1);
          if (tmp == noteFrequencyIndex)
          {
            //found!
            n.noteIdx = noteIdx;
            n.octaveIdx = octaveIdx;
            n.pound = true;
            noteFound = true;
            scale = 99; noteIdx = 250;//exit both for
            break;
          }
        }
      }
    }
  }

  //compute duration
  RTTTL_DURATION_RESULT durationResult = findDurationIndex(iBpm, iDuration);
  if (durationResult.durationIdx == INVALID_DURATION_INDEX)
  {
    //Unable to compute duration for the given bpm.
    //Use the closest duration found
    durationResult = findClosestDurationIndex(iBpm, iDuration);
  }
  if (durationResult.durationIdx == INVALID_DURATION_INDEX)
  {
    //Unable to resolve duration with the closest duration
    //generate the longest note duration
    durationResult.durationIdx = 0;
    durationResult.dotted = false;
  }

  //apply duration
  n.durationIdx = durationResult.durationIdx;
  n.dotted = durationResult.dotted;

  return n;
}

void _playRTTTL(const char *p)
{
  // Absolutely no error checking in here

  RTTTL_DURATION default_dur = RTTTL_DEFAULT_DURATION;
  RTTTL_OCTAVE_VALUE default_oct = RTTTL_DEFAULT_OCTAVE;
  RTTTL_BPM bpm = RTTTL_DEFAULT_BPM;
  int num;
  TONE_DURATION wholenote;
  TONE_DURATION duration;
  byte note;
  RTTTL_OCTAVE_VALUE scale;

  // format: d=N,o=N,b=NNN:
  // find the start (skip name, etc)

  while(*p != ':') p++;    // ignore name
  p++;                     // skip ':'

  // get default duration
  if(*p == 'd')
  {
    p++; p++;              // skip "d="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    if(num > 0) default_dur = num;
    p++;                   // skip comma
  }

  // get default octave
  if(*p == 'o')
  {
    p++; p++;              // skip "o="
    num = *p++ - '0';
    if(num >= 3 && num <=7) default_oct = num;
    p++;                   // skip comma
  }

  // get BPM
  if(*p == 'b')
  {
    p++; p++;              // skip "b="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    bpm = num;
    p++;                   // skip colon
  }

  // BPM usually expresses the number of quarter notes per minute
  wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)

  // now begin note loop
  while(*p)
  {
    // first, get note duration, if available
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }

    if(num) duration = wholenote / num;
    else duration = wholenote / default_dur;  // we will need to check if we are a dotted note after

    // now get the note
    note = 0;

    switch(*p)
    {
    case 'c':
      note = 1;
      break;
    case 'd':
      note = 3;
      break;
    case 'e':
      note = 5;
      break;
    case 'f':
      note = 6;
      break;
    case 'g':
      note = 8;
      break;
    case 'a':
      note = 10;
      break;
    case 'b':
      note = 12;
      break;
    case 'p':
    default:
      note = 0;
    }
    p++;

    // now, get optional '#' sharp
    if(*p == '#')
    {
      note++;
      p++;
    }

    // now, get optional '.' dotted note
    if(*p == '.')
    {
      duration += duration/2;
      p++;
    }

    // now, get scale
    if(isdigit(*p))
    {
      scale = *p - '0';
      p++;
    }
    else
    {
      scale = default_oct;
    }

    scale += OCTAVE_OFFSET;

    if(*p == ',')
      p++;       // skip comma for next note (or we may be at the end)

    // now play the note

    if(note)
    {
      librtttl::arduino::tone(BUZZER_OUT_PIN, gRtttlFrequencies[(scale - 4) * 12 + note], duration);
      librtttl::arduino::delay(duration+1);
      librtttl::arduino::noTone(BUZZER_OUT_PIN);
    }
    else
    {
      librtttl::arduino::delay(duration);
    }
  }
}

void _playRTTTL(const RTTTL_SONG & iSong)
{
  // Absolutely no error checking in here

  RTTTL_DURATION default_dur = RTTTL_DEFAULT_DURATION;
  RTTTL_OCTAVE_VALUE default_oct = RTTTL_DEFAULT_OCTAVE;
  RTTTL_BPM bpm = RTTTL_DEFAULT_BPM;
  //int num;
  TONE_DURATION wholenote;
  TONE_DURATION duration;
  byte note;
  RTTTL_OCTAVE_VALUE scale;

  // BPM usually expresses the number of quarter notes per minute
  wholenote = (60 * 1000L / iSong.defaults.bpm) * 4;  // this is the time for whole note (in milliseconds)

  //for each notes
  for(size_t i=0; i<iSong.notes.size(); i++)
  {
    const RTTTL_NOTE & n = iSong.notes[i];

    // first, get note duration, if available
    duration = wholenote / getNoteDurationFromIndex(n.durationIdx);

    // now get the note
    RTTTL_NOTE_LETTER noteLetter = getNoteLetterFromIndex(n.noteIdx);
    switch(noteLetter)
    {
    case 'c':
      note = 1;
      break;
    case 'd':
      note = 3;
      break;
    case 'e':
      note = 5;
      break;
    case 'f':
      note = 6;
      break;
    case 'g':
      note = 8;
      break;
    case 'a':
      note = 10;
      break;
    case 'b':
      note = 12;
      break;
    case 'p':
    default:
      note = 0;
    }

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
    scale = getNoteOctaveFromIndex(n.octaveIdx);

    if(note)
    {
      librtttl::arduino::tone(BUZZER_OUT_PIN, gRtttlFrequencies[(scale - 4) * 12 + note], duration);
      librtttl::arduino::delay(duration+1);
      librtttl::arduino::noTone(BUZZER_OUT_PIN);
    }
    else
    {
      librtttl::arduino::delay(duration);
    }
  }
}

TONE_CALL_INFO_LIST playRTTTL(const char *p)
{
  clearToneCalls();
  _playRTTTL(p);
  return getRecordedToneCalls();
}

TONE_CALL_INFO_LIST playRTTTL(const RTTTL_SONG & iSong)
{
  clearToneCalls();
  _playRTTTL(iSong);
  return getRecordedToneCalls();
}

bool validateRtttlFrequencies(const TONE_CALL_INFO_LIST & iCalls)
{
  TONE_FREQUENCY freqencyMin = NOTE_C4 - TONE_FREQUENCY_EPSILON;
  TONE_FREQUENCY freqencyMax = NOTE_B7 + TONE_FREQUENCY_EPSILON;

  for(size_t i=0; i<iCalls.size(); i++)
  {
    const TONE_CALL_INFO & c = iCalls[i];
    if (c._tone != INVALID_TONE_INFO)
    {
      bool inRange = (freqencyMin <= c._tone.freq && c._tone.freq <= freqencyMax);
      if (!inRange)
      {
        return false;
      }
    }
  }
  return true;
}

void processOctaveOffsetting(TONE_CALL_INFO_LIST & iCalls)
{
  bool notesFixed = false;
  for(int numOctave = 1; numOctave <= 5 && !notesFixed; numOctave++)
  {
    //try increasing the octave
    TONE_CALL_INFO_LIST newCalls = iCalls; //make a copy
    for(int i=0; i<numOctave; i++)
    {
      increaseOctave(newCalls, TONE_FREQUENCY_EPSILON);
    }

    //did that fixed all notes ?
    if (validateRtttlFrequencies(newCalls))
    {
      //all notes are within RTTTL note range!
      iCalls = newCalls;
      notesFixed = true;
    }
    else
    {
      //try decreasing the octave
      TONE_CALL_INFO_LIST newCalls = iCalls; //make a copy
      for(int i=0; i<numOctave; i++)
      {
        decreaseOctave(newCalls, TONE_FREQUENCY_EPSILON);
      }

      //did that fixed all notes ?
      if (validateRtttlFrequencies(newCalls))
      {
        //all notes are within RTTTL note range!
        iCalls = newCalls;
        notesFixed = true;
      }
    }
  }
}

void processToneFrequencyClamping(TONE_CALL_INFO_LIST & iCalls)
{
  for(size_t i=0; i<iCalls.size(); i++)
  {
    TONE_CALL_INFO & c = iCalls[i];
    if (c._tone != INVALID_TONE_INFO)
    {
      clamp<TONE_FREQUENCY>(NOTE_C4, c._tone.freq, NOTE_B7);
    }
  }
}


std::string calls2Rtttl(const char * iSongName)
{
  std::string tmp;
  RTTTL_RESULT result = calls2Rtttl(iSongName, tmp);
  return tmp;
}

std::string calls2Rtttl( const char * iSongName,
                        const RTTTL_DURATION * iForcedDefaultDuration,
                        const RTTTL_OCTAVE_VALUE * iForcedDefaultOctave,
                        const RTTTL_BPM * iForcedDefaultBpm)
{
  std::string tmp;
  RTTTL_RESULT result = calls2Rtttl(iSongName, tmp,
    iForcedDefaultDuration,
    iForcedDefaultOctave,
    iForcedDefaultBpm);
  return tmp;
}

RTTTL_RESULT calls2Rtttl(const char * iSongName,
                         std::string & oRtttlCode,
                         const RTTTL_DURATION * iForcedDefaultDuration,
                         const RTTTL_OCTAVE_VALUE * iForcedDefaultOctave,
                         const RTTTL_BPM * iForcedDefaultBpm)
{
  TONE_CALL_INFO_LIST calls = getRecordedToneCalls(); //make a copy
  return calls2Rtttl(calls, iSongName, oRtttlCode, iForcedDefaultDuration, iForcedDefaultOctave, iForcedDefaultBpm);
}

LIBRTTTL_API RTTTL_RESULT calls2Rtttl( const TONE_CALL_INFO_LIST & iCalls,
                                       const char * iSongName,
                                       std::string & oRtttlCode,
                                       const RTTTL_DURATION * iForcedDefaultDuration,
                                       const RTTTL_OCTAVE_VALUE * iForcedDefaultOctave,
                                       const RTTTL_BPM * iForcedDefaultBpm)
{
  //clean up calls
  TONE_CALL_INFO_LIST calls = iCalls; //make a copy
  removeExpectedDelays(calls);

  RTTTL_SONG song;

  RTTTL_DURATION default_dur = RTTTL_DEFAULT_DURATION;
  RTTTL_OCTAVE_VALUE default_oct = RTTTL_DEFAULT_OCTAVE;
  RTTTL_BPM bpm = RTTTL_DEFAULT_BPM;

  //for each tone calls
  //check that each notes frequency are within rtttl accepted range
  bool allNotesInRange = validateRtttlFrequencies(calls);
  if (!allNotesInRange)
  {
    //did we specify relaxed settings for completing conversion anyway ?
    if (gRtttlSettings.allowOctaveOffseting)
    {
      processOctaveOffsetting(calls);
      
      //look again...
      allNotesInRange = validateRtttlFrequencies(calls);

      if (!allNotesInRange)
      {
        if (gRtttlSettings.allowToneFrequencyClamping)
        {
          //we failed. Let's hope that clamping will success...
        }
        else
        {
          return FAILED_OCTAVE_OFFSETTING;
        }
      }
    }

    if (!allNotesInRange && gRtttlSettings.allowToneFrequencyClamping)
    {
      processToneFrequencyClamping(calls);

      //look again...
      allNotesInRange = validateRtttlFrequencies(calls);

      if (!allNotesInRange)
      {
        //we failed.
        return FAILED_TONE_FREQUENCIES_CLAMPING;
      }
    }

    if (!allNotesInRange)
    {
      return TONE_FREQUENCIES_OUT_OF_BOUNDS;
    }
  }

  //find best values for b
  if (iForcedDefaultBpm == NULL)
  {
    bpm = findBestBpm(calls);
  }
  else
  {
    bpm = *iForcedDefaultBpm;
  }

  //for each tone calls
  for(size_t i=0; i<calls.size(); i++)
  {
    const TONE_CALL_INFO & c = calls[i];
    if (c._tone != INVALID_TONE_INFO)
    {
      //identify note
      RTTTL_NOTE note = getRtttlNote(bpm, c._tone.freq, c._tone.duration);
      song.notes.push_back(note);
    }
    if (c.delay != INVALID_TONE_DURATION)
    {
      RTTTL_NOTE note = getRtttlNote(bpm, NOTE_SILENT, c.delay);
      song.notes.push_back(note);
    }
  }

  //copy name
  strcpy_s(song.name, sizeof(song.name), iSongName);

  //find best values for duration and octave
  if (iForcedDefaultDuration == NULL)
    song.defaults.durationIdx = getBestDurationOptimization(song.notes);
  else
    song.defaults.durationIdx = findNoteDurationIndex(*iForcedDefaultDuration);
  if (iForcedDefaultOctave == NULL)
    song.defaults.octaveIdx = getBestOctaveOptimization(song.notes);
  else
    song.defaults.octaveIdx = findNoteOctaveIndex(*iForcedDefaultOctave);

  //compute bpm
  RTTTL_RESULT bpmSetResult = applyBpm(song.defaults, bpm);
  if (bpmSetResult != SUCCESS)
  {
    return bpmSetResult;
  }

  std::string rtttlString = toString(song, gRtttlSettings.forceDefaultSection);

  oRtttlCode = rtttlString;
  return SUCCESS;
}


LIBRTTTL_API RTTTL_RESULT findAlternateRtttlEncoding(const char * iCode, StringVector & oAlternateEncodings)
{
  oAlternateEncodings.clear();
  RTTTL_SONG song;
  RTTTL_RESULT parseResult = parseRtttlString(iCode, song);
  if (parseResult != SUCCESS)
    return parseResult;

  return findAlternateRtttlEncoding(song, oAlternateEncodings);
}

LIBRTTTL_API RTTTL_RESULT findAlternateRtttlEncoding(const RTTTL_SONG & iSong, StringVector & oAlternateEncodings)
{
  TONE_CALL_INFO_LIST calls = playRTTTL(iSong);

  removeExpectedDelays(calls);

  RTTTL_DURATION default_dur = RTTTL_DEFAULT_DURATION;
  RTTTL_OCTAVE_VALUE default_oct = RTTTL_DEFAULT_OCTAVE;
  RTTTL_BPM bpm = RTTTL_DEFAULT_BPM;

  //for each tone calls
  //check that each notes frequency are within rtttl accepted range
  bool allNotesInRange = validateRtttlFrequencies(calls);
  if (!allNotesInRange)
  {
    //did we specify relaxed settings for completing conversion anyway ?
    if (gRtttlSettings.allowOctaveOffseting)
    {
      processOctaveOffsetting(calls);
      
      //look again...
      allNotesInRange = validateRtttlFrequencies(calls);

      if (!allNotesInRange)
      {
        if (gRtttlSettings.allowToneFrequencyClamping)
        {
          //we failed. Let's hope that clamping will success...
        }
        else
        {
          return FAILED_OCTAVE_OFFSETTING;
        }
      }
    }

    if (!allNotesInRange && gRtttlSettings.allowToneFrequencyClamping)
    {
      processToneFrequencyClamping(calls);

      //look again...
      allNotesInRange = validateRtttlFrequencies(calls);

      if (!allNotesInRange)
      {
        //we failed.
        return FAILED_TONE_FREQUENCIES_CLAMPING;
      }
    }

    if (!allNotesInRange)
    {
      return TONE_FREQUENCIES_OUT_OF_BOUNDS;
    }
  }

  //find all values for b
  BpmList bpms = findAllBpms(calls);

  //for each bpm
  for(size_t i=0; i<bpms.size(); i++)
  {
    RTTTL_BPM bpm = bpms[i];

    RTTTL_SONG tempSong;

    //for each tone calls
    for(size_t j=0; j<calls.size(); j++)
    {
      const TONE_CALL_INFO & c = calls[j];
      if (c._tone != INVALID_TONE_INFO)
      {
        //identify note
        RTTTL_NOTE note = getRtttlNote(bpm, c._tone.freq, c._tone.duration);
        tempSong.notes.push_back(note);
      }
      if (c.delay != INVALID_TONE_DURATION)
      {
        RTTTL_NOTE note = getRtttlNote(bpm, NOTE_SILENT, c.delay);
        tempSong.notes.push_back(note);
      }
    }

    //find best values for duration and octave
    DurationList durations = getSongDurationsList(tempSong.notes);
    OctaveList octaves = getSongOctavesList(tempSong.notes);

    //for each durations
    for(size_t j=0; j<durations.size(); j++)
    {
      DURATION_INDEX durationIdx = durations[j];

      //for each octaves
      for(size_t k=0; k<octaves.size(); k++)
      {
        OCTAVE_INDEX octaveIdx = octaves[k];

        //Copy current song
        RTTTL_SONG song = tempSong;

        //copy name
        strcpy_s(song.name, sizeof(song.name), iSong.name);

        //apply default duration & default octave
        song.defaults.durationIdx = durationIdx;
        song.defaults.octaveIdx = octaveIdx;

        //compute bpm
        RTTTL_RESULT bpmSetResult = applyBpm(song.defaults, bpm);
        if (bpmSetResult != SUCCESS)
          continue; //next bpm

        //Convert song to string
        std::string songCode = toString(song);
        oAlternateEncodings.push_back(songCode);
     }
    }
  }

  return SUCCESS;
}

RTTTL_RESULT optimizeEncoding(const char * iCode, std::string & oRtttlCode)
{
  StringVector alternates;
  RTTTL_RESULT alternateEncodeResult = findAlternateRtttlEncoding(iCode, alternates);
  if (alternateEncodeResult != SUCCESS)
    return alternateEncodeResult;

  if (alternates.size() == 0)
  {
    return NOT_SUPPORTED;
  }

  //find shortest code
  size_t shortestCodeIdx = 0;
  for(size_t i=0; i<alternates.size(); i++)
  {
    std::string & alternate = alternates[i];
    if (alternate.size() < alternates[shortestCodeIdx].size())
    {
      shortestCodeIdx = i;
    }
  }
  oRtttlCode = alternates[shortestCodeIdx];
  return SUCCESS;
}

void dumpRtttlToneCalls(const char * iArray, const char * iFilename)
{
  const TONE_CALL_INFO_LIST toneCalls = playRTTTL(iArray);
  dumpToneCalls(toneCalls, iFilename);
  //printf("Rtttl song \"%s\" has %d function calls\n", iFilename, getRecordedToneCalls().size());
}

void toStream(const RTTTL_SONG & iSong, STREAM & oStream)
{
  toStream10(iSong, oStream);
}

void toStream10(const RTTTL_SONG & iSong, STREAM & oStream)
{
  oStream.clear();

  //compute required stream size
  size_t streamSize = 0;
  streamSize += sizeof(RTTTL_DEFAULT_VALUE_SECTION);
  streamSize += divideCeil<size_t>(RTTTL_NOTE_SIZE_BITS*iSong.notes.size(), BITS_PER_BYTE); //10 bits per note

  //allocate stream
  if (!oStream.alloc((unsigned int)streamSize))
    return; //failed allocating memory
  oStream.zeroize();

#ifndef USE_BITADDRESS_READ_WRITE
  BitWriter bw;
#else
  BitAddress bw;
#endif
  bw.setBuffer(oStream.buffer);

  //dump defaults data
  bw.write((const unsigned char *)&iSong.defaults, BITS_PER_BYTE*sizeof(RTTTL_DEFAULT_VALUE_SECTION));

  //dump notes
  for(size_t i=0; i<iSong.notes.size(); i++)
  {
    const RTTTL_NOTE & note = iSong.notes[i];
    
#ifdef DEBUG_STREAMS
    BitAddress writeAddrStart = bw.getAddress();
    BitAddress writeAddrEnd = writeAddrStart;
    writeAddrEnd.add(RTTTL_NOTE_SIZE_BITS-1);
    printf("about to write note %d of %d at bit address %s up to address %s\n", i+1, iSong.notes.size(),
      writeAddrStart.toString().c_str(),
      writeAddrEnd.toString().c_str()       );
#endif
    bw.write((const unsigned char *)&note.raw, RTTTL_NOTE_SIZE_BITS);
  }
}

void toStream16(const RTTTL_SONG & iSong, STREAM & oStream)
{
  oStream.clear();

  //compute required stream size
  size_t streamSize = 0;
  streamSize += sizeof(RTTTL_DEFAULT_VALUE_SECTION);
  streamSize += iSong.notes.size() * sizeof(RTTTL_NOTE); //16 bits per note

  //allocate stream
  if (!oStream.alloc((unsigned int)streamSize))
    return; //failed allocating memory
  oStream.zeroize();

  //dump defaults data
  unsigned short * defaultBuffer = (unsigned short *)oStream.buffer;
  defaultBuffer[0] = iSong.defaults.raw;

  //dump notes
  unsigned short * notesBuffer = (unsigned short *)&defaultBuffer[1];
  for(size_t i=0; i<iSong.notes.size(); i++)
  {
    const RTTTL_NOTE & note = iSong.notes[i];
    notesBuffer[i] = note.raw;
  }
}

bool parseStream(const STREAM & iStream, RTTTL_SONG & oSong)
{
  reset(oSong);

  if (iStream.buffer == NULL || iStream.length == 0)
  {
    return false;
  }

#ifndef USE_BITADDRESS_READ_WRITE
  BitReader br;
#else
  BitAddress br;
#endif
  br.setBuffer(iStream.buffer);

  //read defaults
  if (iStream.length > sizeof(RTTTL_DEFAULT_VALUE_SECTION))
  {
    //memcpy(&oSong.defaults, iStream.buffer, sizeof(RTTTL_DEFAULT_VALUE_SECTION));
    br.read(BITS_PER_BYTE * sizeof(RTTTL_DEFAULT_VALUE_SECTION), (unsigned char *)&oSong.defaults);
  }

  BitAddress noteEndAddr; //defines the address of the last bit of the next note to be read from the stream
#ifndef USE_BITADDRESS_READ_WRITE
  noteEndAddr = br.getAddress();
#else
  noteEndAddr = br;
#endif
  noteEndAddr.add(RTTTL_NOTE_SIZE_BITS-1);
  while(noteEndAddr.byte < iStream.length)
  {
    RTTTL_NOTE note;
    reset(note);

#ifdef DEBUG_STREAMS
    BitAddress readAddrStart = br.getAddress();
    BitAddress readAddrEnd = readAddrStart;
    readAddrEnd.add(RTTTL_NOTE_SIZE_BITS-1);
#endif

    br.read(RTTTL_NOTE_SIZE_BITS, (unsigned char*)&note.raw);
    
    oSong.notes.push_back(note);

#ifdef DEBUG_STREAMS
    printf("readed note %d located at address %s up to address %s\n", oSong.notes.size(),
      readAddrStart.toString().c_str(), readAddrEnd.toString().c_str());
#endif

    //refresh note end address
#ifndef USE_BITADDRESS_READ_WRITE
    noteEndAddr = br.getAddress();
#else
    noteEndAddr = br;
#endif
    noteEndAddr.add(RTTTL_NOTE_SIZE_BITS-1);
  }

  return true;
}

}; //namespace librtttl
