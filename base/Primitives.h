/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__PRIMITIVES_H
#define _DK_SDU_MIP__BASE__PRIMITIVES_H

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
  typedef unsigned short uint16;
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

/**
  Returns the higher half-word of the specified value.
*/
inline byte getHighWord(unsigned short value) throw() {
  return value >> (sizeof(unsigned short) * 8/2);
}

/**
  Returns the lower half-word of the specified value.
*/
inline byte getLowWord(unsigned short value) throw() {
  return value;
}

/**
  Returns the higher half-word of the specified value.
*/
inline unsigned short getHighWord(unsigned int value) throw() {
  return value >> (sizeof(unsigned int) * 8/2);
}

/**
  Returns the lower half-word of the specified value.
*/
inline unsigned short getLowWord(unsigned int value) throw() {
  return value;
}

/**
  Returns the higher half-word of the specified value.
*/
inline unsigned int getHighWord(unsigned long long value) throw() {
  return value >> (sizeof(unsigned long long) * 8/2);
}

/**
  Returns the lower half-word of the specified value.
*/
inline unsigned int getLowWord(unsigned long long value) throw() {
  return value;
}

/**
  Returns the address of the specified object as a byte pointer.
*/
template<class TYPE>
inline byte* getByteAddress(TYPE& value) throw() {return reinterpret_cast<byte*>(&value);}

template<class TYPE>
inline const byte* getByteAddress(const TYPE& value) throw() {return reinterpret_cast<const byte*>(&value);}

template<class TYPE>
inline char* getCharAddress(TYPE& value) throw() {return reinterpret_cast<char*>(&value);}

template<class TYPE>
inline const char* getCharAddress(const TYPE& value) throw() {return reinterpret_cast<const char*>(&value);}

/**
  This namespace is a wrapper for information concerning the primitive (built-in) types.
  
  @short Primitive type information
*/
namespace primitives {

  /**
    Specifies whether or not the type is void.
  */
  template<class TYPE> class Void {public: enum {IS_VOID = false};};
  template<> class Void<void> {public: enum {IS_VOID = true};};

  /**
    Specifies whether or not the type is a cardinal type (this includes the 'bool' primitive).

    Example:
    
    <pre>
    template<class TYPE>
    void myFunction(const TYPE& value) throw() {
      if (Primitives::Cardinal<TYPE>::IS_CARDINAL) {
        // do something
      } else {
        // do something
      }
    }
    </pre>
  */
  template<class TYPE> class Cardinal {public: enum {IS_CARDINAL = false};};
  template<> class Cardinal<bool> {public: enum {IS_CARDINAL = true};};
  template<> class Cardinal<char> {public: enum {IS_CARDINAL = true};};
  template<> class Cardinal<signed char> {public: enum {IS_CARDINAL = true};};
  template<> class Cardinal<unsigned char> {public: enum {IS_CARDINAL = true};};
  template<> class Cardinal<wchar_t> {public: enum {IS_CARDINAL = true};};
  template<> class Cardinal<int> {public: enum {IS_CARDINAL = true};};
  template<> class Cardinal<unsigned int> {public: enum {IS_CARDINAL = true};};
  template<> class Cardinal<long> {public: enum {IS_CARDINAL = true};};
  template<> class Cardinal<unsigned long> {public: enum {IS_CARDINAL = true};};
  template<> class Cardinal<long long> {public: enum {IS_CARDINAL = true};};
  template<> class Cardinal<unsigned long long> {public: enum {IS_CARDINAL = true};};
  // TAG: what about signed types
  
  /**
    Specifies whether or not the type is a floating point type.

    Example:
    
    <pre>
    template<class TYPE>
    void myFunction(const TYPE& value) throw() {
      if (Primitives::FloatingPoint<TYPE>::IS_FLOATING_POINT) {
        // do something
      } else if (Primitives::Cardinal<TYPE>::IS_CARDINAL) {
        // do something
      } else {
        // do something
      }
    }
    </pre>
  */
  template<class TYPE> class FloatingPoint {public: enum {IS_FLOATING_POINT = false};};
  template<> class FloatingPoint<float> {public: enum {IS_FLOATING_POINT = true};};
  template<> class FloatingPoint<double> {public: enum {IS_FLOATING_POINT = true};};
  template<> class FloatingPoint<long double> {public: enum {IS_FLOATING_POINT = true};};

  /**
    Specifies whether or not the given type is an arithmetic (i.e. cardinal or floating) type.
  */
  template<class TYPE>
  class Arithmetic {
  public:
    enum {IS_ARITHMETIC = Cardinal<TYPE>::IS_CARDINAL || FloatingPoint<TYPE>::IS_FLOATING_POINT};
  };

  /**
    Specifies whether or not the given type is a primitive (built-in) type.
  */
  template<class TYPE>
  class Primitive {
  public:
    enum {IS_PRIMITIVE = Void<TYPE>::IS_VOID || Arithmetic<TYPE>::IS_ARITHMETIC};
  };
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
