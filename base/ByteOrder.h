/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__BYTE_ORDER_H
#define _DK_SDU_MIP__BASE__BYTE_ORDER_H

#include <base/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This namespace contains functions used for explicit conversion between the
  supported byte orders for primitives. The supported byte orders are big
  endian and little endian.

  @short Byte order manipulation functions
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
*/
// TAG: need long and long long methods

namespace ByteOrder {

#if (_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__BIG_ENDIAN)

  template<typename TYPE>
  static inline TYPE fromBigEndian(TYPE value) throw() {return value;}

  template<typename TYPE>
  static inline TYPE toBigEndian(TYPE value) throw() {return value;}

#elif (_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__LITTLE_ENDIAN)

  template<typename TYPE>
  static inline TYPE fromBigEndian(TYPE value) throw() {return TYPE::_DK_SDU_MIP__BASE__INVALID;}

  template<>
  static inline unsigned short fromBigEndian<unsigned short>(unsigned short value) throw() {return (value >> 8) | (value << 8);}

  template<>
  static inline unsigned int fromBigEndian<unsigned int>(unsigned int value) throw() {return (value >> 24) | ((value & 0xff0000) >> 8) | ((value & 0xff00) << 8) | (value << 24);}

  template<>
  static inline unsigned long fromBigEndian<unsigned long>(unsigned long value) throw() {
    return fromBigEndian<unsigned int>(value);
  }

  template<>
  static inline unsigned long long fromBigEndian<unsigned long long>(unsigned long long value) throw() {
    return fromBigEndian<unsigned int>(value >> 32) | static_cast<unsigned long long>(fromBigEndian<unsigned int>(value & 0xffffffff)) << 32;
  }

  template<typename TYPE>
    static inline TYPE toBigEndian(TYPE value) throw() {return TYPE::_DK_SDU_MIP__BASE__INVALID;} // invalid invocation

  template<>
  static inline unsigned short toBigEndian<unsigned short>(unsigned short value) throw() {return (value >> 8) | (value << 8);}

  template<>
  static inline unsigned int toBigEndian<unsigned int>(unsigned int value) throw() {return (value >> 24) | ((value & 0xff0000) >> 8) | ((value & 0xff00) << 8) | (value << 24);}

  template<>
  static inline unsigned long long toBigEndian<unsigned long long>(unsigned long long value) throw() {
    return toBigEndian<unsigned int>(value >> 32) | static_cast<unsigned long long>(toBigEndian<unsigned int>(value & 0xffffffff)) << 32;
  }

#endif

}; // end of namespace ByteOrder



/**
  This class provides types for storing primitives in little endian byte order
  independently of the native byte order of the platform.

  @short Little endian storing types.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class LittleEndian {
public:

#if (_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__BIG_ENDIAN)

/** 16 bit signed integer represented in little endian byte order. */
struct SignedShort {
  char lowByte;
  char highByte;

  inline short operator=(short eq) throw() {
    lowByte = eq & 0xff;
    highByte = eq >> 8;
    return eq;
  }

  inline operator short() const throw() {return highByte << 8 + lowByte;}
} __attribute__ ((packed));

/** 16 bit unsigned integer represented in little endian byte order. */
struct UnsignedShort {
  char lowByte;
  char highByte;

  inline unsigned short operator=(unsigned short eq) throw() {
    lowByte = eq & 0xff;
    highByte = eq >> 8;
    return eq;
  }

  inline operator unsigned short() const throw() {return highByte << 8 + lowByte;}
} __attribute__ ((packed));

/** 32 bit signed integer represented in little endian byte order. */
struct SignedInt {
  char a;
  char b;
  char c;
  char d;

  inline int operator=(int eq) throw() {
    a = eq & 0xff;
    b = (eq >> 8) & 0xff;
    c = (eq >> 16) & 0xff;
    d = eq >> 24;
    return eq;
  }

  inline operator int() const throw() {return ((d << 8 + c) << 8 + b) << 8 + a;}
} __attribute__ ((packed));

/** 32 bit unsigned integer represented in little endian byte order. */
struct UnsignedInt {
  char a;
  char b;
  char c;
  char d;

  inline unsigned int operator=(unsigned int eq) throw() {
    a = eq & 0xff;
    b = (eq >> 8) & 0xff;
    c = (eq >> 16) & 0xff;
    d = eq >> 24;
    return eq;
  }

  inline operator unsigned int() const throw() {return ((d << 8 + c) << 8 + b) << 8 + a;}
} __attribute__ ((packed));

#else

/** 16 bit signed integer represented in little endian byte order. */
typedef short SignedShort;
/** 16 bit unsigned integer represented in little endian byte order. */
typedef unsigned short UnsignedShort;
/** 32 bit signed integer represented in little endian byte order. */
typedef int SignedInt;
/** 32 bit unsigned integer represented in little endian byte order. */
typedef unsigned int UnsignedInt;

#endif

}; // end of class LittleEndian



/**
  This class provides types for storing primitives in big endian byte order
  independently of the native byte order of the platform.

  @short Big endian storing types.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class BigEndian {
public:

#if (_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__BIG_ENDIAN)

/** 16 bit signed integer represented in big endian byte order. */
typedef short SignedShort;
/** 16 bit unsigned integer represented in big endian byte order. */
typedef unsigned short UnsignedShort;
/** 32 bit signed integer represented in big endian byte order. */
typedef int SignedInt;
/** 32 bit unsigned integer represented in big endian byte order. */
typedef unsigned int UnsignedInt;

#else

/** 16 bit signed integer represented in big endian byte order. */
struct SignedShort {
  char highByte;
  char lowByte;

  inline short operator=(short eq) throw() {
    highByte = eq >> 8;
    lowByte = eq & 0xff;
    return eq;
  }

  inline operator short() const throw() {return highByte << 8 + lowByte;}
} __attribute__ ((packed));

/** 16 bit unsigned integer represented in big endian byte order. */
struct UnsignedShort {
  char highByte;
  char lowByte;

  inline unsigned short operator=(unsigned short eq) throw() {
    highByte = eq >> 8;
    lowByte = eq & 0xff;
    return eq;
  }

  inline operator unsigned short() const throw() {return highByte << 8 + lowByte;}
} __attribute__ ((packed));

/** 32 bit signed integer represented in big endian byte order. */
struct SignedInt {
  char d;
  char c;
  char b;
  char a;

  inline int operator=(int eq) throw() {
    d = eq >> 24;
    c = (eq >> 16) & 0xff;
    b = (eq >> 8) & 0xff;
    a = eq & 0xff;
    return eq;
  }

  inline operator int() const throw() {return ((d << 8 + c) << 8 + b) << 8 + a;}
} __attribute__ ((packed));

/** 32 bit unsigned integer represented in big endian byte order. */
struct UnsignedInt {
  char d;
  char c;
  char b;
  char a;

  inline unsigned int operator=(unsigned int eq) throw() {
    d = eq >> 24;
    c = (eq >> 16) & 0xff;
    b = (eq >> 8) & 0xff;
    a = eq & 0xff;
    return eq;
  }

  inline operator unsigned int() const throw() {return ((d << 8 + c) << 8 + b) << 8 + a;}
} __attribute__ ((packed));

#endif

}; // end of class BigEndian

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
