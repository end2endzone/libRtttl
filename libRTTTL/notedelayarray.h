#pragma once

#include "librtttl.h"
#include "tone.h"

TONE_CALL_INFO_LIST playToneDelayArray(const int & iPin, const int * iArray, int iArraySize);
void convertRtttl2NoteDelayArray(const int & iPin, const char * iRtttl, const char * iArrayName, const char * iFilename);
