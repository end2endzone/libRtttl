#ifndef BEEP_H
#define BEEP_H

#include "beep_api.h"

BEEP_API void beep(unsigned short iFrequency, unsigned short iDuration);
BEEP_API void sleep(unsigned short iDuration);

/// <sumary>
/// Play a MIDI file.
/// </sumary>
/// <remarks>
/// This function is highly platform dependent. It may not be supported by another platform.
/// </remarks>
/// <param name="iPath">The path to the MIDI file.</param>
/// <returns>Returns 0 on success. Non-zero on error.</returns>
BEEP_API unsigned long playMidiFile(const char * iPath);

#endif //BEEP_H
