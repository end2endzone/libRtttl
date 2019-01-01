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

#ifndef LIBRTTTL_UTILS_H
#define LIBRTTTL_UTILS_H

#include <string>
#include <vector>

namespace librtttl
{

typedef std::vector<int> IntVector;
typedef std::vector<std::string> StringVector;

std::string itostr(int i);
char * toString(char c);
char * toString(char c1, char c2);
char * toHex(char c);
char * toHex(char c, const char * iFormat);
char * toHex(unsigned char c, const char * iFormat);
bool isFileEquals(const char * iFile1, const char * iFile2);
int getFileSize(const char * iFile);
void dumpString(const char * iValue, const char * iFilename);
bool getTextFileContent(const char * iFilename, StringVector & oLines );
int replace(std::string & iString, const char * iOldValue, const char * iNewValue);
StringVector split(const char * iString, const char * iSeparator);
bool findFunctionCall(const char * iString, const char * iFunctionName, int iNumParameters, StringVector & oParameters, int & oFunctionOffsetStart, int & oFunctionOffsetEnd);

template <typename T>
inline T divideCeil(const T & a, const T & b)
{
  return (a/b + (a%b == 0 ? 0 : 1));
}

template <typename T>
inline size_t findMinValueIndex (const T * iArray, size_t iSize)
{
  if (iSize == 0)
    return (size_t)-1; //invalid index

  T minValue = iArray[0];
  size_t index = 0;
  for(size_t i=0; i<iSize; i++)
  {
    const T & testValue = iArray[i];
    if (testValue < minValue)
    {
      minValue = testValue;
      index = i;
    }
  }
  return index;
}

template <typename T>
inline size_t findMaxValueIndex (const T * iArray, size_t iSize)
{
  if (iSize == 0)
    return (size_t)-1; //invalid index

  T maxValue = iArray[0];
  size_t index = 0;
  for(size_t i=0; i<iSize; i++)
  {
    const T & testValue = iArray[i];
    if (testValue > maxValue)
    {
      maxValue = testValue;
      index = i;
    }
  }
  return index;
}

template <typename T>
inline const T & findMinValue(const T * iArray, size_t iSize)
{
  size_t index = findMinValueIndex(iArray, iSize);
  return iArray[index];
}

template <typename T>
inline const T & findMaxValue(const T * iArray, size_t iSize)
{
  size_t index = findMaxValueIndex(iArray, iSize);
  return iArray[index];
}

template <typename T>
inline std::vector<size_t> findIndexOf(const T & iValue, const T * iArray, size_t iSize)
{
  std::vector<size_t> list;
  if (iSize == 0)
    return list;

  for(size_t i=0; i<iSize; i++)
  {
    const T & testValue = iArray[i];
    if (testValue == iValue)
    {
      list.push_back(i);
    }
  }
  return list;
}

template <typename T>
inline const T & maximum (const T & oValue)
{
  static T precalculated = 5;
  if (precalculated == 5)
  {
    int typeSize = sizeof(T);
    T maximumUnsigned = (T)0;
    memset(&maximumUnsigned, 0xFF, typeSize);

    T maximumSigned = (T)0;
    memset(&maximumSigned, 0xFF, typeSize);
    char * bytes = (char *)(&maximumSigned);
    memset( &bytes[typeSize-1] , 0x7F, 1);

    if (maximumSigned > maximumUnsigned)
      precalculated = maximumSigned;
    else
      precalculated = maximumUnsigned;
  }

  return precalculated;
}

template <typename T>
inline const T & minimum (const T & oValue)
{
  static T precalculated = 5;
  if (precalculated == 5)
  {
    T minimumUnsigned = (T)0;

    T minimumSigned = maximum(minimumUnsigned);
    minimumSigned++;

    if (minimumSigned < minimumUnsigned)
      precalculated = minimumSigned;
    else
      precalculated = minimumUnsigned;
  }

  return precalculated;
}

template <typename T>
inline void maximize (T & oValue)
{
  oValue = maximum(oValue);
}

template <typename T>
inline void minimize (T & oValue)
{
  oValue = minimum(oValue);
}

template <typename T>
inline void computeEpsilonValues(const T & iValue, const T & iEpsilon, T & oMin, T & oMax)
{
  oMin = iValue;
  oMax = iValue;

  for(T i=0; i<iEpsilon; i++)
  {
    if (oMin > (minimum(oMin)))
    {
      oMin--;
    }
  }

  for(T i=0; i<iEpsilon; i++)
  {
    if (oMax < (maximum(oMax)) )
    {
      oMax++;
    }
  }
}

template <typename T>
inline void clamp(const T & iMin, T & ioValue, const T & iMax)
{
  if (ioValue < iMin)
    ioValue = iMin;
  if (ioValue > iMax)
    ioValue = iMax;
}

}; //namespace librtttl

#endif //LIBRTTTL_UTILS_H
