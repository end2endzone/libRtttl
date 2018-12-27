// beep.cpp : Defines the exported functions for the DLL application.
//
#include "beep.h"

#ifdef _WIN32
#include <windows.h>
#endif

BEEP_API void beep(unsigned short iFrequency, unsigned short iDuration)
{
#ifdef _WIN32
  Beep(iFrequency, iDuration);
#endif
}

BEEP_API void sleep(unsigned short iDuration)
{
#ifdef _WIN32
  Sleep(iDuration);
#endif
}
