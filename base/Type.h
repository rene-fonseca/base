/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__TYPE_H
#define _DK_SDU_MIP__BASE__TYPE_H

#include <base/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if (_DK_SDU_MIP__BASE__CHAR_SIZE == 1)
  class Char {
  public:
#if defined(_DK_SDU_MIP__BASE__UNSIGNED_CHAR)
    static const char MAXIMUM = 255;
    static const char MINIMUM = 0;
#else // signed
    static const char MAXIMUM = 127;
    static const char MINIMUM = -128;
#endif
  };

  class UnsignedChar {
  public:
    static const unsigned char MAXIMUM = 255;
    static const unsigned char MINIMUM = 0;
  };
#endif

#if (_DK_SDU_MIP__BASE__SHORT_SIZE == 2)
  class Short {
  public:
    static const short int MAXIMUM = 32767;
    static const short int MINIMUM = -32768;
  };

  class UnsignedShort {
  public:
    static const unsigned short int MAXIMUM = 65535;
    static const unsigned short int MINIMUM = 0;
  };
#elif (_DK_SDU_MIP__BASE__SHORT_SIZE = 4)
  class Short {
  public:
    static const short int MAXIMUM = 2147483647;
    static const short int MINIMUM = -MAXIMUM - 1;
  };

  class UnsignedShort {
  public:
    static const unsigned short int MAXIMUM = 4294967295U;
    static const unsigned short int MINIMUM = 0;
  };
#endif

#if (_DK_SDU_MIP__BASE__INT_SIZE == 4)
  class Int {
  public:
    static const int MAXIMUM = 2147483647;
    static const int MINIMUM = -MAXIMUM - 1;
  };

  class UnsignedInt {
  public:
    static const unsigned int MAXIMUM = 4294967295U;
    static const unsigned int MINIMUM = 0;
  };
#elif (_DK_SDU_MIP__BASE__INT_SIZE == 8)
  class Int {
  public:
    static const int MAXIMUM = 9223372036854775807;
    static const int MINIMUM = -MAXIMUM - 1;
  };

  class UnsignedInt {
  public:
    static const unsigned int MAXIMUM = 18446744073709551615U;
    static const unsigned int MINIMUM = 0;
  };
#endif

#if (_DK_SDU_MIP__BASE__LONG_SIZE == 4)
  class Long {
  public:
    static const long MAXIMUM = 2147483647L;
    static const long MINIMUM = -MAXIMUM - 1L;
  };

  class UnsignedLong {
  public:
    static const unsigned long MAXIMUM = 4294967295UL;
    static const unsigned long MINIMUM = 0;
  };
#elif (_DK_SDU_MIP__BASE__LONG_SIZE == 8)
  class Long {
  public:
    static const long MAXIMUM = 9223372036854775807;
    static const long MINIMUM = -MAXIMUM - 1L;
  };

  class UnsignedLong {
  public:
    static const unsigned long MAXIMUM = 18446744073709551615UL;
    static const unsigned long MINIMUM = 0;
  };
#endif

#if (_DK_SDU_MIP__BASE__LONG_LONG_SIZE == 8)
  class LongLong {
  public:
    static const long long MAXIMUM = 9223372036854775807LL;
    static const long long MINIMUM = -MAXIMUM - 1LL;
  };

  class UnsignedLongLong {
  public:
    static const unsigned long long MAXIMUM = 18446744073709551615ULL;
    static const unsigned long long MINIMUM = 0;
  };
#endif



#if (_DK_SDU_MIP__BASE__CHAR_SIZE == 1)
  /** 8 bit unsigned integer (a.k.a. octet). */
  typedef unsigned char byte;
#endif

#if (_DK_SDU_MIP__BASE__SHORT_SIZE == 2)
  /** 16 bit unsigned integer. */
  typedef unsigned short ushort16;
#endif

#if (_DK_SDU_MIP__BASE__INT_SIZE == 4)
  /** 32 bit unsigned integer. */
  typedef unsigned int uint32;
#elif (_DK_SDU_MIP__BASE__LONG_SIZE == 4)
  /** 32 bit unsigned integer. */
  typedef unsigned long uint32;
#elif (_DK_SDI_MIP__BASE__LONG_LONG_SIZE == 4)
  /** 32 bit unsigned integer. */
  typedef unsigned long long uint32;
#endif

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
