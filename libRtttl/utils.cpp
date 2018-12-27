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

#include "utils.h"

namespace librtttl
{

std::string itostr(int i)
{
  char buffer[1024];

#pragma warning( push )
#pragma warning( disable : 4996) // warning C4996: 'itoa': The POSIX name for this item is deprecated.
  itoa(i, buffer, 10);
#pragma warning( pop ) 

  return std::string(buffer);
}

char * toString(char c)
{
  static char buffer[2];
  buffer[0] = c;
  return buffer;
}

char * toString(char c1, char c2)
{
  static char buffer[3];
  buffer[0] = c1;
  buffer[1] = c2;
  return buffer;
}

char * toHex(char c)
{
  return toHex(c, "%x");
}

char * toHex(unsigned char c, const char * iFormat)
{
  static char buffer[10];
  sprintf(buffer, iFormat, c);
  return buffer;
}

char * toHex(char c, const char * iFormat)
{
  static char buffer[10];
  sprintf(buffer, iFormat, c);
  return buffer;
}

int getFileSize(const char * iFile)
{
  FILE * f = fopen(iFile, "rb");
  if(!f)
    return 0;
  fseek(f, 0L, SEEK_END);
  int size = ftell(f);
  fclose(f);
  return size;
}

bool isFileEquals(const char * iFile1, const char * iFile2)
{
  const int size1 = getFileSize(iFile1);
  const int size2 = getFileSize(iFile2);
  if (size1 != size2)
    return false;

  int size = size1; //or size2
  if (size == 0)
    return true;

  char * buffer1 = new char[size];
  char * buffer2 = new char[size];

  FILE * f1 = fopen(iFile1, "rb");
  if (!f1)
    return false;
  FILE * f2 = fopen(iFile2, "rb");
  if (!f2)
  {
    fclose(f1);
    return false;
  }

  fread(buffer1, 1, size, f1);
  fread(buffer2, 1, size, f2);

  for(int i=0; i<size; i++)
  {
    char & c1 = buffer1[i];
    char & c2 = buffer2[i];
    if (c1 != c2)
    {
      fclose(f1);
      fclose(f2);
      return false;
    }
  }

  fclose(f1);
  fclose(f2);
  return true;
}

void librtttl::dumpString(const char * iValue, const char * iFilename)
{
  FILE * f = fopen(iFilename, "w");
  if (!f)
    return;
  fprintf(f, "%s", iValue);
  fclose(f);
}

bool getTextFileContent(const char * iFilename, StringVector & oLines )
{
  const int BUFFER_SIZE = 10240;
  oLines.clear();
  char wBuffer[BUFFER_SIZE];
  FILE* f = fopen(iFilename, "r");
  if (f)
  {
    while (fgets(wBuffer, BUFFER_SIZE, f))
    {
      //remove CRLF
      std::string tmp = wBuffer;
      replace(tmp, "\n", "");
      oLines.push_back(tmp);
    }

    fclose(f);
    return true;
  }
  return false;
}

int replace(std::string & iString, const char * iOldValue, const char * iNewValue)
{
  std::string tmpOldValue = iOldValue;
  std::string tmpNewValue = iNewValue;

  int numOccurance = 0;

  if (tmpOldValue.size() > 0)
  {
    size_t startPos = 0;
    size_t findPos = std::string::npos;
    do
    {
      findPos = iString.find(tmpOldValue, startPos);
      if (findPos != std::string::npos)
      {
        iString.replace(findPos, tmpOldValue.length(), tmpNewValue);
        startPos = findPos + tmpNewValue.length();
        numOccurance++;
      }
    }
    while (findPos != -1);
  }
  return numOccurance;
}

StringVector split(const char * iString, const char * iSeparator)
{
  StringVector elements;

  size_t stringLen = strlen(iString);
  size_t separatorLength = strlen(iSeparator);

  std::string strAccumulator = "";

  for(size_t i=0; i<stringLen; i++)
  {
    if (strncmp(&iString[i], iSeparator, separatorLength) == 0)
    {
      //delimiter found at current position
      //add the accumulator as a token
      if (strAccumulator != "")
      {
        elements.push_back(strAccumulator);
      }
      strAccumulator = "";
    }
    else
    {
      //current character is not part of a delimiter
      strAccumulator.append(1, iString[i]);
    }
  }

  //if there is still data in the accumulator
  if (strAccumulator != "")
  {
    elements.push_back(strAccumulator);
  }
  strAccumulator = "";

  return elements;
}

bool findFunctionCall(const char * iString, const char * iFunctionName, int iNumParameters, StringVector & oParameters, int & oFunctionOffsetStart, int & oFunctionOffsetEnd)
{
  oParameters.clear();
  oFunctionOffsetStart = -1;
  oFunctionOffsetEnd = -1;

  const std::string code = iString;
  const std::string functionStartPattern = std::string(iFunctionName) + "(";
  const std::string functionEndPattern = ");";

  //search for a iFunctionName(...) call
  size_t functionStartPos = code.find(functionStartPattern);
  if (functionStartPos != std::string::npos)
  {
    size_t parametersStartPos = functionStartPos+functionStartPattern.size();
    size_t functionEndPos = code.find(functionEndPattern, parametersStartPos);

    //extract parameters
    std::string parameterString = code.substr(parametersStartPos, functionEndPos-parametersStartPos);
    StringVector parameters = split(parameterString.c_str(), ",");
    
    //trim all parameters ?
    //no

    //expect 3 parameters
    if (parameters.size() == iNumParameters)
    {
      //found
      oParameters = parameters;
      oFunctionOffsetStart = (int)functionStartPos;
      oFunctionOffsetEnd = (int)(functionEndPos+functionEndPattern.size());
      return true;
    }
  }

  return false;
}

}; //namespace librtttl