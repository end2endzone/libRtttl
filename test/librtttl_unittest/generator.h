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

#ifndef GENERATOR_H
#define GENERATOR_H

#include <string>
#include "binary.h"

union Byte
{
  unsigned char raw;
  struct
  {
    bool b0 : 1;
    bool b1 : 1;
    bool b2 : 1;
    bool b3 : 1;
    bool b4 : 1;
    bool b5 : 1;
    bool b6 : 1;
    bool b7 : 1;
  };

  std::string toBinaryDefinition()
  {
    std::string b = "b";
    b.append(1, b7+'0');
    b.append(1, b6+'0');
    b.append(1, b5+'0');
    b.append(1, b4+'0');
    b.append(1, b3+'0');
    b.append(1, b2+'0');
    b.append(1, b1+'0');
    b.append(1, b0+'0');
    return b;
  }

  std::string toHex()
  {
    const int BUFFER_SIZE = 10;
    char buffer[BUFFER_SIZE];

    sprintf(buffer, "0x%02x", raw);

    return buffer;
  }

  void fromString(const char * iValue)
  {
    if (*iValue == 'b')
      iValue++;

    raw = 0;

    //compute byte value
    b0 = iValue[7] == '1';
    b1 = iValue[6] == '1';
    b2 = iValue[5] == '1';
    b3 = iValue[4] == '1';
    b4 = iValue[3] == '1';
    b5 = iValue[2] == '1';
    b6 = iValue[1] == '1';
    b7 = iValue[0] == '1';
  }
};

void generateBinaryDefines(const char * iFile)
{
  FILE * f = fopen(iFile, "w");

  fprintf(f, "#ifndef BINARY_H\n");
  fprintf(f, "#define BINARY_H\n");
  fprintf(f, "\n");
  fprintf(f, "\n");
  for(unsigned int i=0; i<=255; i++)
  {
    Byte byte;
    byte.raw = i;
    fprintf(f, "#define %s %s\n", byte.toBinaryDefinition().c_str(), byte.toHex().c_str() );
  }

  fprintf(f, "\n");
  fprintf(f, "#endif //BINARY_H\n");

  fclose(f);
}

void generate5BitCounterStream(const char * iFile)
{
  FILE * f = fopen(iFile, "w");

  union Counter
  {
    unsigned char raw;
    struct
    {
      unsigned char value: 5;
      unsigned char reserved: 3;
    };
  };

  fprintf(f, "#ifndef COUNTER5BITS_H\n");
  fprintf(f, "#define COUNTER5BITS_H\n");
  fprintf(f, "#include \"binary.h\"\n\n");

  std::string bitStream;
  Counter c;
  c.value = 0;
  Byte b;
  for(unsigned int i=0; i<=70; i++)
  {
    b.raw = c.value;
    
    //5 bits to string
#ifdef _DEBUG
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    sprintf(buffer, "%d%d%d%d%d",
      b.b4,
      b.b3,
      b.b2,
      b.b1,
      b.b0);
#endif

    /*
    aaaaa
    bbbb1
    ccc1c
    ddd11
    ee1ee
    aaaaabbbb1ccc1cddd11ee1ee

    bb1aaaaa
    1ccc1cbb
    e1eeddd1
        ...e
    */

    //add to bit stream
    bitStream.append( 1, b.b0+'0' );
    bitStream.append( 1, b.b1+'0' );
    bitStream.append( 1, b.b2+'0' );
    bitStream.append( 1, b.b3+'0' );
    bitStream.append( 1, b.b4+'0' );

    c.value++;
  }

  //dump 5 bitStream as 8 bit array
  std::string arrayDefinition = "const unsigned char fiveBitCounters[] = {\n";

  //fill the bitstream to a multiple of 8
  while(bitStream.size() % 8 != 0)
  {
    bitStream.append("0");
  }

  while(bitStream.size() > 0)
  {
    //fill a 8-bit character
    std::string bits;
    for(int i=0; i<8; i++)
    {
      bits.insert(0, 1, bitStream[0]);
      bitStream.erase( 0, 1 );
    }
    Byte byte;
    byte.fromString(bits.c_str());

    const int BUFFER_SIZE = 1024;
    char tmp[BUFFER_SIZE];
    sprintf(tmp, "  %s, /* 0x%02X   %3dd */\n", byte.toBinaryDefinition().c_str(), byte.raw, byte.raw);
    arrayDefinition.append( tmp );

  }

  //complete the array
  arrayDefinition.append("};");

  //output the array
  fprintf(f, "%s\n", arrayDefinition.c_str());
  fprintf(f, "const size_t fiveBitCountersSize = sizeof(fiveBitCounters)/sizeof(fiveBitCounters[0]);\n");

  fprintf(f, "\n");
  fprintf(f, "#endif //COUNTER5BITS_H\n");
  
  fclose(f);
}

#endif //GENERATOR_H
