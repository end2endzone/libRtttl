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

#ifndef LIBRTTTL_STREAM_H
#define LIBRTTTL_STREAM_H

#include "librtttl/config.h"
#include "librtttl/version.h"

#include <string>
#include <cstring> //for memset()

namespace librtttl
{

struct LIBRTTTL_EXPORT STREAM
{
  unsigned char * buffer;
  typedef size_t STREAM_SIZE;
  STREAM_SIZE length;

  STREAM()
  {
    buffer = NULL;
    length = 0;
  }

  void clear()
  {
    if (buffer != NULL)
    {
      delete[] buffer;
      buffer = NULL;
    }
  }

  bool alloc(STREAM_SIZE iSize)
  {
    clear();

    buffer = new unsigned char[iSize];
    if (buffer)
    {
      length = iSize;
      return true;
    }
    return false;
  }

  void zeroize()
  {
    if (buffer != NULL)
    {
      memset(buffer, 0, length);
    }
  }

  ~STREAM()
  {
    clear();
  }
};

LIBRTTTL_EXPORT std::string toCppArray(const STREAM & iStream);

}; //namespace librtttl

#endif //LIBRTTTL_STREAM_H
