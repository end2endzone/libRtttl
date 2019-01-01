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

#ifndef COUNTER5BITS_H
#define COUNTER5BITS_H

#include "binary.h"

const unsigned char fiveBitCounters[] = {
  b00100000, /* 0x20    32d */
  b10001000, /* 0x88   136d */
  b01000001, /* 0x41    65d */
  b10001010, /* 0x8A   138d */
  b00111001, /* 0x39    57d */
  b00101000, /* 0x28    40d */
  b10101001, /* 0xA9   169d */
  b11000101, /* 0xC5   197d */
  b10011010, /* 0x9A   154d */
  b01111011, /* 0x7B   123d */
  b00110000, /* 0x30    48d */
  b11001010, /* 0xCA   202d */
  b01001001, /* 0x49    73d */
  b10101011, /* 0xAB   171d */
  b10111101, /* 0xBD   189d */
  b00111000, /* 0x38    56d */
  b11101011, /* 0xEB   235d */
  b11001101, /* 0xCD   205d */
  b10111011, /* 0xBB   187d */
  b11111111, /* 0xFF   255d */
  b00100000, /* 0x20    32d */
  b10001000, /* 0x88   136d */
  b01000001, /* 0x41    65d */
  b10001010, /* 0x8A   138d */
  b00111001, /* 0x39    57d */
  b00101000, /* 0x28    40d */
  b10101001, /* 0xA9   169d */
  b11000101, /* 0xC5   197d */
  b10011010, /* 0x9A   154d */
  b01111011, /* 0x7B   123d */
  b00110000, /* 0x30    48d */
  b11001010, /* 0xCA   202d */
  b01001001, /* 0x49    73d */
  b10101011, /* 0xAB   171d */
  b10111101, /* 0xBD   189d */
  b00111000, /* 0x38    56d */
  b11101011, /* 0xEB   235d */
  b11001101, /* 0xCD   205d */
  b10111011, /* 0xBB   187d */
  b11111111, /* 0xFF   255d */
  b00100000, /* 0x20    32d */
  b10001000, /* 0x88   136d */
  b01000001, /* 0x41    65d */
  b10001010, /* 0x8A   138d */
  b00000001, /* 0x01     1d */
};
const size_t fiveBitCountersSize = sizeof(fiveBitCounters)/sizeof(fiveBitCounters[0]);

#endif //COUNTER5BITS_H
