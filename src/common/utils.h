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
