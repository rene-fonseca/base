/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__PRIMITIVES_H
#define _DK_SDU_MIP__BASE__PRIMITIVES_H

#include <base/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

// Primitive constraints
// TAG: check char
// TAG: check short
// TAG: check int
// TAG: check long
// TAG: check long long

#if (_DK_SDU_MIP__BASE__CHAR_SIZE == 1)
  /** 8 bit unsigned integer (a.k.a. octet). */
  typedef unsigned char byte;
#else
  #error char primitive is not 8 bits
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
  template<class TYPE> class Void {public: enum {IS_VOID = false /**< True if type is void. */};};
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
  template<class TYPE> class Cardinal {public: enum {IS_CARDINAL = false /**< True if type is a cardinal type. */};};
  template<> class Cardinal<bool> {public: enum {IS_CARDINAL = true};};
  template<> class Cardinal<char> {public: enum {IS_CARDINAL = true};};
  template<> class Cardinal<signed char> {public: enum {IS_CARDINAL = true};};
  template<> class Cardinal<unsigned char> {public: enum {IS_CARDINAL = true};};
  template<> class Cardinal<wchar_t> {public: enum {IS_CARDINAL = true};};
  template<> class Cardinal<short> {public: enum {IS_CARDINAL = true};};
  template<> class Cardinal<unsigned short> {public: enum {IS_CARDINAL = true};};
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
  template<class TYPE> class FloatingPoint {public: enum {IS_FLOATING_POINT = false /**< True if type is a floating point type. */};};
  template<> class FloatingPoint<float> {public: enum {IS_FLOATING_POINT = true};};
  template<> class FloatingPoint<double> {public: enum {IS_FLOATING_POINT = true};};
  template<> class FloatingPoint<long double> {public: enum {IS_FLOATING_POINT = true};};

  /**
    Specifies whether or not the given type is an arithmetic (i.e. cardinal or floating) type.
  */
  template<class TYPE>
  class Arithmetic {
  public:
    enum {
      /** True if the type is an arithmetic type. */
      IS_ARITHMETIC = Cardinal<TYPE>::IS_CARDINAL || FloatingPoint<TYPE>::IS_FLOATING_POINT
    };
  };

  /**
    Specifies whether or not the given type is a primitive (built-in) type.
  */
  template<class TYPE>
  class Primitive {
  public:
    enum {
      /** True if the type a built-in type. */
      IS_PRIMITIVE = Void<TYPE>::IS_VOID || Arithmetic<TYPE>::IS_ARITHMETIC
    };
  };

}; // end of primitives namespace

/**
  Contains information about the primitive type.
*/
template<class PRIMITIVE>
class PrimitiveTraits {
public:
};

template<class TYPE>
class PrimitiveTraits<TYPE*> {
public:
  
  /** The type of pointer differences. */
#if (_DK_SDU_MIP__BASE__POINTER_SIZE == _DK_SDU_MIP__BASE__LONG_SIZE)
  typedef long Distance;
#elif (_DK_SDU_MIP__BASE__POINTER_SIZE == _DK_SDU_MIP__BASE__LONG_LONG_SIZE)
  typedef long long Distance;
#else
  #error Pointer not compatible with long or long long primitives
#endif
};

template<>
class PrimitiveTraits<bool> {
public:
  
  typedef bool Signed;
  typedef bool Unsigned;
  
  static const bool MINIMUM = false;
  static const bool MAXIMUM = true;
};

template<>
class PrimitiveTraits<signed char> {
public:
  
  typedef signed char Signed;
  typedef unsigned char Unsigned;
  
#if (_DK_SDU_MIP__BASE__CHAR_SIZE == 1)
  static const signed char MAXIMUM = 127;
  static const signed char MINIMUM = -MAXIMUM - 1;
#endif
};

template<>
class PrimitiveTraits<unsigned char> {
public:
  
  typedef signed char Signed;
  typedef unsigned char Unsigned;
  
#if (_DK_SDU_MIP__BASE__CHAR_SIZE == 1)
  static const unsigned char MAXIMUM = 255;
  static const unsigned char MINIMUM = 0;
#endif
};

template<>
class PrimitiveTraits<short> {
public:
  
  typedef short Signed;
  typedef unsigned short Unsigned;
  
#if (_DK_SDU_MIP__BASE__SHORT_SIZE == 2)
  static const short MAXIMUM = 32767;
#elif (_DK_SDU_MIP__BASE__SHORT_SIZE = 4)
  static const short MAXIMUM = 2147483647;
#endif
  static const short MINIMUM = -MAXIMUM - 1;
};

template<>
class PrimitiveTraits<unsigned short> {
public:
  
  typedef short Signed;
  typedef unsigned short Unsigned;
  
#if (_DK_SDU_MIP__BASE__SHORT_SIZE == 2)
  static const unsigned short MAXIMUM = 65535U;
#elif (_DK_SDU_MIP__BASE__SHORT_SIZE = 4)
  static const unsigned short MAXIMUM = 4294967295U;
#endif
  static const unsigned short MINIMUM = 0;
};

template<>
class PrimitiveTraits<int> {
public:
  
  typedef int Signed;
  typedef unsigned int Unsigned;
  
#if (_DK_SDU_MIP__BASE__LONG_SIZE == 4)
  static const int MAXIMUM = 2147483647;
#elif (_DK_SDU_MIP__BASE__LONG_SIZE == 8)
  static const int MAXIMUM = 9223372036854775807;
#endif
  static const int MINIMUM = -MAXIMUM - 1L;
};

template<>
class PrimitiveTraits<unsigned int> {
public:
  
  typedef int Signed;
  typedef unsigned int Unsigned;
  
#if (_DK_SDU_MIP__BASE__LONG_SIZE == 4)
  static const unsigned int MAXIMUM = 4294967295U;
#elif (_DK_SDU_MIP__BASE__LONG_SIZE == 8)
  static const unsigned int MAXIMUM = 18446744073709551615U;
#endif
  static const unsigned int MINIMUM = 0;
};

template<>
class PrimitiveTraits<long> {
public:
  
  typedef long Signed;
  typedef unsigned long Unsigned;
  
#if (_DK_SDU_MIP__BASE__LONG_SIZE == 4)
  static const long MAXIMUM = 2147483647L;
#elif (_DK_SDU_MIP__BASE__LONG_SIZE == 8)
  static const long MAXIMUM = 9223372036854775807L;
#endif
  static const long MINIMUM = -MAXIMUM - 1L;
};

template<>
class PrimitiveTraits<unsigned long> {
public:
  
  typedef long Signed;
  typedef unsigned long Unsigned;
  
#if (_DK_SDU_MIP__BASE__LONG_SIZE == 4)
  static const unsigned long MAXIMUM = 4294967295UL;
#elif (_DK_SDU_MIP__BASE__LONG_SIZE == 8)
  static const unsigned long MAXIMUM = 18446744073709551615UL;
#endif
  static const unsigned long MINIMUM = 0;
};

template<>
class PrimitiveTraits<long long> {
public:
  
  typedef long long Signed;
  typedef unsigned long long Unsigned;
  
#if (_DK_SDU_MIP__BASE__LONG_LONG_SIZE == 8)
  static const long long MAXIMUM = 9223372036854775807LL;
#endif
  static const long long MINIMUM = -MAXIMUM - 1LL;
};

template<>
class PrimitiveTraits<unsigned long long> {
public:
  
  typedef long long Signed;
  typedef unsigned long long Unsigned;
  
#if (_DK_SDU_MIP__BASE__LONG_LONG_SIZE == 8)
  static const unsigned long long MAXIMUM = 18446744073709551615ULL;
#endif
  static const unsigned long long MINIMUM = 0;
};

template<>
class PrimitiveTraits<float> {
public:
  
  typedef float Signed;
  typedef float Unsigned;
  
  static const float MAXIMUM = 0; // TAG: get maximum from proper representation
  static const float MINIMUM = -MAXIMUM;
};

template<>
class PrimitiveTraits<double> {
public:
  
  typedef double Signed;
  typedef double Unsigned;
  
  static const double MAXIMUM = 0; // TAG: get maximum from proper representation
  static const double MINIMUM = -MAXIMUM;
};

template<>
class PrimitiveTraits<long double> {
public:
  
  typedef long double Signed;
  typedef long double Unsigned;
  
  static const long double MAXIMUM = 0; // TAG: get maximum from proper representation
  static const long double MINIMUM = -MAXIMUM;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
