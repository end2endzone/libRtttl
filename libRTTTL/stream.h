#pragma once

#include "librtttl.h"

#include <string>

struct LIBRTTTL_API STREAM
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

LIBRTTTL_API std::string toCppArray(const STREAM & iStream);
