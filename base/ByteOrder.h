/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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

  template<class TYPE>
  static inline TYPE fromBigEndian(TYPE value) throw() {return value;}

  template<class TYPE>
  static inline TYPE toBigEndian(TYPE value) throw() {return value;}

#elif (_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__LITTLE_ENDIAN)

  template<class TYPE>
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

  template<class TYPE>
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
  unsigned char lowByte;
  unsigned char highByte;

  inline SignedShort() throw() {}

  inline SignedShort(short value) throw()
    : lowByte(value), highByte(value >> 8) {
  }

  inline short operator=(short eq) throw() {
    lowByte = eq;
    highByte = eq >> 8;
    return eq;
  }

  inline operator short() const throw() {return (static_cast<unsigned short>(highByte) << 8) + lowByte;}
} __attribute__ ((packed));

/** 16 bit unsigned integer represented in little endian byte order. */
struct UnsignedShort {
  unsigned char lowByte;
  unsigned char highByte;

  inline UnsignedShort() throw() {}

  inline UnsignedShort(unsigned short value) throw()
    : lowByte(value), highByte(value >> 8) {
  }

  inline unsigned short operator=(unsigned short eq) throw() {
    lowByte = eq;
    highByte = eq >> 8;
    return eq;
  }

  inline operator unsigned short() const throw() {
    return (static_cast<unsigned short>(highByte) << 8) + lowByte;
  }
} __attribute__ ((packed));

/** 32 bit signed integer represented in little endian byte order. */
struct SignedInt {
  unsigned char a;
  unsigned char b;
  unsigned char c;
  unsigned char d;

  inline SignedInt() throw() {}

  inline SignedInt(int value) throw()
    : a(value), b(value >> 8), c(value >> 16), d(value >> 24) {
  }

  inline int operator=(int eq) throw() {
    a = eq;
    b = eq >> 8;
    c = eq >> 16;
    d = eq >> 24;
    return eq;
  }

  inline operator int() const throw() {return (((static_cast<unsigned int>(d) << 8) + c << 8) + b << 8) + a;}
} __attribute__ ((packed));

/** 32 bit unsigned integer represented in little endian byte order. */
struct UnsignedInt {
  unsigned char a;
  unsigned char b;
  unsigned char c;
  unsigned char d;

  inline UnsignedInt() throw() {}

  inline UnsignedInt(unsigned int value) throw()
    : a(value), b(value >> 8), c(value >> 16), d(value >> 24) {
  }

  inline unsigned int operator=(unsigned int eq) throw() {
    a = eq;
    b = eq >> 8;
    c = eq >> 16;
    d = eq >> 24;
    return eq;
  }

  inline operator unsigned int() const throw() {return (((static_cast<unsigned int>(d) << 8) + c << 8) + b << 8) + a;}
} __attribute__ ((packed));

/** 64 bit signed integer represented in little endian byte order. */
struct SignedLongLong {
  UnsignedInt a;
  UnsignedInt b;

  inline SignedLongLong() throw() {}

  inline SignedLongLong(long long value) throw()
    : a(value), b(value >> 32) {
  }

  inline long long operator=(long long eq) throw() {
    a = eq;
    b = eq >> 32;
    return eq;
  }

  inline operator long long() const throw() {
    return (static_cast<unsigned long long>(b) << 32) + a;
  }
} __attribute__ ((packed));

/** 64 bit unsigned integer represented in little endian byte order. */
struct UnsignedLongLong {
  UnsignedInt b;
  UnsignedInt a;

  inline UnsignedLongLong() throw() {}

  inline UnsignedLongLong(unsigned long long value) throw()
    : a(value), b(value >> 32) {
  }

  inline unsigned long long operator=(unsigned long long eq) throw() {
    a = eq;
    b = eq >> 32;
    return eq;
  }

  inline operator unsigned long long() const throw() {
    return (static_cast<unsigned long long>(b) << 32) + a;
  }
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
/** 64 bit signed integer represented in little endian byte order. */
typedef long long SignedLongLong;
/** 64 bit unsigned integer represented in little endian byte order. */
typedef unsigned long long UnsignedLongLong;

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
/** 64 bit signed integer represented in little endian byte order. */
typedef long long SignedLongLong;
/** 64 bit unsigned integer represented in little endian byte order. */
typedef unsigned long long UnsignedLongLong;

#else

/** 16 bit signed integer represented in big endian byte order. */
struct SignedShort {
  unsigned char highByte;
  unsigned char lowByte;

  inline SignedShort() throw() {}

  inline SignedShort(short value) throw()
    : highByte(value >> 8), lowByte(value) {
  }

  inline short operator=(short eq) throw() {
    highByte = eq >> 8;
    lowByte = eq;
    return eq;
  }

  inline operator short() const throw() {return (static_cast<unsigned short>(highByte) << 8) + lowByte;}
} __attribute__ ((packed));

/** 16 bit unsigned integer represented in big endian byte order. */
struct UnsignedShort {
  unsigned char highByte;
  unsigned char lowByte;

  inline UnsignedShort() throw() {}

  inline UnsignedShort(unsigned short value) throw()
    : highByte(value >> 8), lowByte(value) {
  }
  
  inline unsigned short operator=(unsigned short eq) throw() {
    highByte = eq >> 8;
    lowByte = eq;
    return eq;
  }

  inline operator unsigned short() const throw() {return (static_cast<unsigned short>(highByte) << 8) + lowByte;}
} __attribute__ ((packed));

/** 32 bit signed integer represented in big endian byte order. */
struct SignedInt {
  unsigned char d;
  unsigned char c;
  unsigned char b;
  unsigned char a;

  inline SignedInt() throw() {}

  inline SignedInt(int value) throw()
    : d(value >> 24), c(value >> 16), b(value >> 8), a(value) {
  }

  inline int operator=(int eq) throw() {
    d = eq >> 24;
    c = eq >> 16;
    b = eq >> 8;
    a = eq;
    return eq;
  }

  inline operator int() const throw() {return (((static_cast<unsigned int>(d) << 8) + c << 8) + b << 8) + a;}
} __attribute__ ((packed));

/** 32 bit unsigned integer represented in big endian byte order. */
struct UnsignedInt {
  unsigned char d;
  unsigned char c;
  unsigned char b;
  unsigned char a;

  inline UnsignedInt() throw() {}

  inline UnsignedInt(int value) throw()
    : d(value >> 24), c(value >> 16), b(value >> 8), a(value) {
  }

  inline unsigned int operator=(unsigned int eq) throw() {
    d = eq >> 24;
    c = eq >> 16;
    b = eq >> 8;
    a = eq;
    return eq;
  }

  inline operator unsigned int() const throw() {return (((static_cast<unsigned int>(d) << 8) + c << 8) + b << 8) + a;}
} __attribute__ ((packed));

/** 64 bit signed integer represented in big endian byte order. */
struct SignedLongLong {
  UnsignedInt b;
  UnsignedInt a;

  inline SignedLongLong() throw() {}

  inline SignedLongLong(long long value) throw()
    : b(value >> 32), a(value) {
  }

  inline long long operator=(long long eq) throw() {
    b = eq >> 32;
    a = eq;
    return eq;
  }

  inline operator long long() const throw() {
    return (static_cast<unsigned long long>(b) << 32) + a;
  }
} __attribute__ ((packed));

/** 64 bit unsigned integer represented in big endian byte order. */
struct UnsignedLongLong {
  UnsignedInt b;
  UnsignedInt a;

  inline UnsignedLongLong() throw() {}
  
  inline UnsignedLongLong(unsigned long long value) throw()
    : b(value >> 32), a(value) {
  }
  
  inline UnsignedLongLong& operator=(unsigned long long eq) throw() {
    b = eq >> 32;
    a = eq;
    return *this;
  }
  
  inline operator unsigned long long() const throw() {
    return (static_cast<unsigned long long>(b) << 32) + a;
  }
} __attribute__ ((packed));

#endif

}; // end of class BigEndian

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
