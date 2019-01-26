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

#include "utils.h"

#include <cstdio>   //for sprintf
#include <cstdio>   //for fopen, fseek, fgets

namespace librtttl
{

std::string itostr(int i)
{
  char buffer[1024];
  sprintf(buffer, "%d", i);
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

void dumpString(const char * iValue, const char * iFilename)
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