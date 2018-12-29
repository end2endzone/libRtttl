// beep.cpp : Defines the exported functions for the DLL application.
//
#include "beep.h"
#include <string>

#ifdef _WIN32
#include <windows.h>
#pragma comment(lib, "Winmm.lib")
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

BEEP_API unsigned long playMidiFile(const char * iPath)
{
#ifdef _WIN32
  //http://www.dreamincode.net/forums/topic/95186-playing-a-midmidi-file/
  std::string command;
  command += "play ";
  command += iPath;
  mciSendString(command.c_str(),NULL,0,NULL);
  return 0;
#else
  return 1; //NOT SUPPORTED
#endif
}
