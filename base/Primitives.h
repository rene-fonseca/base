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

#if (_DK_SDU_MIP__BASE__CHAR_SIZE == 1)
  /** 8 bit unsigned integer (a.k.a. octet). */
  typedef unsigned char byte;
  /** 8 bit signed integer. */
  typedef signed char int8;
  /** 8 bit unsigned integer (a.k.a. octet). */
  typedef unsigned char uint8;
#endif

#if (_DK_SDU_MIP__BASE__SHORT_SIZE == 2)
  /** 16 bit signed integer. */
  typedef short int16;
  /** 16 bit unsigned integer (a.k.a. doublet). */
  typedef unsigned short uint16;
#endif

#if (_DK_SDU_MIP__BASE__INT_SIZE == 4)
  /** 32 bit signed integer. */
  typedef int int32;
  /** 32 bit unsigned integer (a.k.a. quadlet). */
  typedef unsigned int uint32;
#elif (_DK_SDU_MIP__BASE__LONG_SIZE == 4)
  /** 32 bit signed integer. */
  typedef long int32;
  /** 32 bit unsigned integer (a.k.a. quadlet). */
  typedef unsigned long uint32;
#elif (_DK_SDI_MIP__BASE__LONG_LONG_SIZE == 4)
  /** 32 bit signed integer. */
  typedef long long int32;
  /** 32 bit unsigned integer (a.k.a. quadlet). */
  typedef unsigned long long uint32;
#endif

#if (_DK_SDU_MIP__BASE__LONG_LONG_SIZE == 8)
  /** 64 bit signed integer. */
  typedef long long int64;
  /** 64 bit unsigned integer. */
  typedef unsigned long long uint64;
#endif

struct int128 {
  uint64 a;
  uint64 b;
};

struct uint128 {
  uint64 a;
  uint64 b;
};

/** A wide character. */
typedef wchar_t wchar; // TAG: this will change to uint32 in the future



#if (_DK_SDU_MIP__BASE__POINTER_SIZE == _DK_SDU_MIP__BASE__INT_SIZE)
  /** The resulting integral type of pointer subtraction. */
  typedef int MemoryDiff;
  /** The integral type used to represent any memory offset and any size of memory block. */
  typedef unsigned int MemorySize;
#elif (_DK_SDU_MIP__BASE__POINTER_SIZE == _DK_SDU_MIP__BASE__LONG_SIZE)
  /** The result type of pointer subtraction. */
  typedef long MemoryDiff;
  /** The integral type used to represent any memory offset and any size of memory block. */
  typedef unsigned long MemorySize;
#elif (_DK_SDU_MIP__BASE__POINTER_SIZE == _DK_SDU_MIP__BASE__LONG_LONG_SIZE)
  /** The result type of pointer subtraction. */
  typedef int64 MemoryDiff;
  /** The integral type used to represent any memory offset and any size of memory block. */
  typedef uint64 MemorySize;
#endif



/**
  Returns the number of elements in the specified built-in array.
*/
template<class TYPE, MemorySize SIZE>
inline MemorySize getArraySize(TYPE (&)[SIZE]) throw() {
  return SIZE;
}

/**
  Returns the address of the specified variable.
*/
template<class TYPE>
inline MemorySize getAddressOf(const TYPE& value) throw() {
  return reinterpret_cast<const char*>(&value) - static_cast<const char*>(0);
}

/**
  Returns a null pointer of the specified type.
*/
template<class TYPE>
inline const TYPE* getNullPointerOf() throw() {
  return static_cast<const TYPE*>(0);
}

/**
  Returns the offset the specified field within the specified structure.
*/
template<class STRUCT, class FIELD>
inline unsigned int getFieldOffset(const FIELD STRUCT::* field) throw() {
  return reinterpret_cast<const char*>(&(static_cast<const STRUCT*>(0)->*field)) - static_cast<const char*>(0);
}

/**
  Returns the offset of the field within the structure.

  <pre>
  namespace myNamespace {

    struct MyStructure {
      struct {
        int subfield;
      } field;
    };
  };
  
  void MyClass::myMethod() throw() {
    MemorySize offset = OFFSETOF(myNamespace::MyStructure, field.subfield);
    ...
  }
  </pre>
*/
#define OFFSETOF(STRUCT, FIELD) getAddressOf(getNullPointerOf<STRUCT>()->FIELD)



/**
  Returns an unsigned 16 bit integer from 2 bytes.
  
  @param lowWord The low bits.
  @param highWord The high bits.
*/
inline uint16 merge(uint8 lowWord, uint8 highWord) throw() {
  return (static_cast<uint16>(highWord) << (sizeof(lowWord) * 8)) | lowWord;
}

/**
  Returns an unsigned 32 bit integer from 2 16 bit integers.
  
  @param lowWord The low bits.
  @param highWord The high bits.
*/
inline uint32 merge(uint16 lowWord, uint16 highWord) throw() {
  return (static_cast<uint32>(highWord) << (sizeof(lowWord) * 8)) | lowWord;
}

/**
  Returns an unsigned 64 bit integer from 2 32 bit integers.
  
  @param lowWord The low bits.
  @param highWord The high bits.
*/
inline uint64 merge(uint32 lowWord, uint32 highWord) throw() {
  return (static_cast<uint64>(highWord) << (sizeof(lowWord) * 8)) | lowWord;
}



/**
  Returns the higher half-word of the specified value.
*/
inline uint8 getHighWordOf16(uint16 value) throw() {
  return value >> (sizeof(value) * 8/2);
}

/**
  Returns the lower half-word of the specified value.
*/
inline uint8 getLowWordOf16(uint16 value) throw() {
  return value;
}

/**
  Returns the higher half-word of the specified value.
*/
inline uint16 getHighWordOf32(uint32 value) throw() {
  return value >> (sizeof(value) * 8/2);
}

/**
  Returns the lower half-word of the specified value.
*/
inline uint16 getLowWordOf32(uint32 value) throw() {
  return value;
}

/**
  Returns the higher half-word of the specified value.
*/
inline uint32 getHighWordOf64(uint64 value) throw() {
  return value >> (sizeof(value) * 8/2);
}

/**
  Returns the lower half-word of the specified value.
*/
inline uint32 getLowWordOf64(uint64 value) throw() {
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
  Returns true if the primitive variable is aligned properly.
*/
template<class TYPE>
inline bool isAligned(const TYPE& value) throw() {
  unsigned int alignment;
  if (sizeof(value) <= sizeof(long)) {
    alignment = sizeof(long);
  } else if (sizeof(value) <= 2 * sizeof(long)) {
    alignment = 2 * sizeof(long);
  } else {
    alignment = 4 * sizeof(long);
  }
  return (reinterpret_cast<const char*>(&value) - static_cast<const char*>(0)) & (alignment - 1) == 0;
}

/**
  Returns the requested primitive data type aligned at an appropriate address
  for the architecture. The buffer is expected to be of at least
  sizeof(TYPE) + n * sizeof(long) - 1 bytes (where n is the minimum value for
  which sizeof(TYPE) <= n * sizeof(long) is true).
  
  @param buffer The buffer.
*/
template<class TYPE>
inline TYPE& getAligned(char* buffer) throw() {
  unsigned int alignment;
  if (sizeof(value) <= sizeof(long)) {
    alignment = sizeof(long);
  } else if (sizeof(value) <= 2 * sizeof(long)) {
    alignment = 2 * sizeof(long);
  } else {
    alignment = 4 * sizeof(long);
  }
  return reinterpret_cast<TYPE*>(
    (((buffer - static_cast<char*>(0)) + alignment - 1) & ~(alignment - 1)) + static_cast<char*>(0)
  );
}

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
  typedef MemoryDiff Distance;
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
  
#if (_DK_SDU_MIP__BASE__INT_SIZE == 4)
  static const int MAXIMUM = 2147483647;
#elif (_DK_SDU_MIP__BASE__INT_SIZE == 8)
  static const int MAXIMUM = 9223372036854775807;
#endif
  static const int MINIMUM = -MAXIMUM - 1L;
};

template<>
class PrimitiveTraits<unsigned int> {
public:
  
  typedef int Signed;
  typedef unsigned int Unsigned;
  
#if (_DK_SDU_MIP__BASE__INT_SIZE == 4)
  static const unsigned int MAXIMUM = 4294967295U;
#elif (_DK_SDU_MIP__BASE__INT_SIZE == 8)
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

/**
  Returns the maximum value of the type of the specified primitive object.
*/
template<class TYPE>
inline TYPE getMaximum(TYPE value) throw() {
  return PrimitiveTraits<TYPE>::MAXIMUM;
}

/**
  Returns the minimum value of the type of the specified primitive object.
*/
template<class TYPE>
inline TYPE getMinimum(TYPE value) throw() {
  return PrimitiveTraits<TYPE>::MINIMUM;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
