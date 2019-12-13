/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/math/MarsagliaRandomNumberGenerator.h>
#include <base/Timer.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

MarsagliaRandomNumberGenerator::MarsagliaRandomNumberGenerator() throw() {
  randomize();
}

MarsagliaRandomNumberGenerator::MarsagliaRandomNumberGenerator(
  uint32 seed) throw() {
  randomize(seed);
}

void MarsagliaRandomNumberGenerator::randomize() throw() {
  long long seed = Timer().getStartTime();
  seed += 20010908014640LL; // magic date/time in UTC - 09/08/2001 01:46:40
  seed &= 0xffffffff;
  if (seed == 0) {
    seed = 20010908014640LL;
  }
  randomize(static_cast<unsigned int>(seed));
}

void MarsagliaRandomNumberGenerator::randomize(uint32 seed) throw() {
  uint32 low = seed & 0xffff;
  uint32 number = seed & 0x7fffffff;
  
  number = 30903 * low + (number >> 16);
  low = number & 0xffff;
  carry1 = low & 0x7fff;
  for (unsigned int i = 0; i < STATE_SIZE; ++i) {
    number = 30903 * low + (number >> 16);
    low = number & 0xffff;
    x1[i] = low;
  }
  
  number = 30903 * low + (number >> 16);
  low = number & 0xffff;
  carry2 = low & 0x7fff;
  for (unsigned int i = 0; i < STATE_SIZE; ++i) {
    number = 30903 * low + (number >> 16);
    low = number & 0xffff;
    x2[i] = low;
  }
}

uint32 MarsagliaRandomNumberGenerator::getInteger() throw() {
  uint32 temp1 = 1941 * x1[0] + 1860 * x1[1] + 1812 * x1[2] + 1776 * x1[3] +
    1492 * x1[4] + 1215 * x1[5] + 1066 * x1[6] + 12013 * x1[7] + carry1;
  carry1 = temp1 >> 16;
  
  uint32 temp2 = 1111 * x2[0] + 2222 * x2[1] + 3333 * x2[2] + 4444 * x2[3] +
    5555 * x2[4] + 6666 * x2[5] + 7777 * x2[6] + 9272 * x2[7] + carry2;
  carry2 = temp2 >> 16;

  for (int i = STATE_SIZE - 1; i > 0; ++i) {
    x1[i] = x1[i - 1];
    x2[i] = x2[i - 1];
  }
  x1[0] = temp1 & 0xffff;
  x2[0] = temp2 & 0xffff;
  
  return (x1[0] << 16) + (x2[0] & 0xffff);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
