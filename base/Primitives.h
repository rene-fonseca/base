/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/features.h>
#include <vector>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if (!defined(_COM_AZURE_DEV__BASE__COMPILER_PRIMITIVES))
#if (_COM_AZURE_DEV__BASE__CHAR_SIZE == 1)
  /** 8 bit signed integer. */
  typedef signed char int8;
  /** 8 bit unsigned integer (a.k.a. octet). */
  typedef unsigned char uint8;
#endif

#if (_COM_AZURE_DEV__BASE__SHORT_SIZE == 2)
  /** 16 bit signed integer. */
  typedef short int16;
  /** 16 bit unsigned integer (a.k.a. doublet). */
  typedef unsigned short uint16;
#endif

#if (_COM_AZURE_DEV__BASE__INT_SIZE == 4)
  /** 32 bit signed integer. */
  typedef int int32;
  /** 32 bit unsigned integer (a.k.a. quadlet). */
  typedef unsigned int uint32;
#elif (_COM_AZURE_DEV__BASE__LONG_SIZE == 4)
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

#if (_COM_AZURE_DEV__BASE__LONG_LONG_SIZE == 8)
  /** 64 bit signed integer. */
  typedef long long int64;
  /** 64 bit unsigned integer. */
  typedef unsigned long long uint64;
#endif
#endif

/* 128 bit signed integer. */
struct int128 {
  uint64 a;
  uint64 b;
};

/* 128 bit unsigned integer. */
struct uint128 {
  uint64 a;
  uint64 b;
};

/** A native wide character. */
typedef wchar_t wchar;
/** UCS-2 encoded character. */
typedef uint16 ucs2;
/** UCS-4 encoded character (ISO/IEC 10646). */
typedef uint32 ucs4;

/* A void type. */
struct nothing {
};



// TAG: typeof is a GCC extension
/** The resulting integral type of pointer subtraction. */
typedef decltype(static_cast<char*>(nullptr) - static_cast<char*>(nullptr)) MemoryDiff;
/**
  The integral type used to represent any memory offset and any size of memory
  block.
*/
typedef decltype(sizeof(void*)) MemorySize;



/** The largets signed integer supported by the architecture. */
typedef MemoryDiff LargestInt; // TAG: could be different
/** The largets unsigned integer supported by the architecture. */
typedef MemorySize ULargestInt; // TAG: could be different



/**
  This macro returns a Literal object from a string literal (e.g.
  MESSAGE("Hello, World")).
  
  @deprecated
*/
#define MESSAGE(literal) Literal(literal)

/**
  This macro constructs a WideLiteral object from the given string
  literal.

  @deprecated
*/
#define WIDEMESSAGE(message) WideLiteral(L ## message)


/**
  Native string.
*/
class _COM_AZURE_DEV__BASE__API NativeString {
private:

  const char* value = nullptr;
public:

  inline NativeString(const char* _value) noexcept : value(_value) {
  }
  
  inline const char* getValue() const noexcept {
    return value;
  }

  inline operator const char*() const noexcept {
    return value;
  }

  /** Returns the length of the string. */
  MemorySize getLength() const noexcept;
};

/**
  Native wide string.
*/
class _COM_AZURE_DEV__BASE__API NativeWideString {
private:

  const wchar* value = nullptr;
public:

  inline NativeWideString(const wchar* _value) noexcept : value(_value) {
  }
  
  inline const wchar* getValue() const noexcept {
    return value;
  }

  inline operator const wchar*() const noexcept {
    return value;
  }

  /** Returns the length of the string. */
  MemorySize getLength() const noexcept;
};



/**
  Returns the number of elements in the specified built-in array.
*/
template<class TYPE, MemorySize SIZE>
inline constexpr MemorySize getArraySize(const TYPE (&)[SIZE]) noexcept {
  return SIZE;
}

/**
  Returns the address of the specified variable.
*/
template<class TYPE>
inline MemorySize getAddressOf(const TYPE& value) noexcept {
  return reinterpret_cast<const char*>(&value) - static_cast<const char*>(nullptr);
}

/**
  Returns the address of the specified pointer.
*/
inline MemorySize getAddressOf(const void* value) noexcept {
  return reinterpret_cast<const char*>(value) - static_cast<const char*>(nullptr);
}

/**
  Returns a null pointer of the specified type.
*/
template<class TYPE>
inline const TYPE* getNullPointerOf() noexcept {
  return static_cast<const TYPE*>(nullptr);
}

/**
  Returns the offset the specified field within the specified structure.
*/
template<class STRUCT, class FIELD>
inline unsigned int getFieldOffset(const FIELD STRUCT::* field) noexcept {
  return reinterpret_cast<const char*>(&(static_cast<const STRUCT*>(nullptr)->*field))
    - static_cast<const char*>(nullptr);
}

/**
  Returns the offset of the field within the structure.

  @code
  namespace myNamespace {

    struct MyStructure {
      struct {
        int subfield;
      } field;
    };
  };
  
  void MyClass::myMethod() noexcept {
    MemorySize offset = OFFSETOF(myNamespace::MyStructure, field.subfield);
    ...
  }
  @endcode
*/
#define OFFSETOF(STRUCT, FIELD) getAddressOf(getNullPointerOf<STRUCT>()->FIELD)



/**
  Returns an unsigned 16 bit integer from 2 bytes.
  
  @param lowWord The low bits.
  @param highWord The high bits.
*/
inline uint16 merge(uint8 lowWord, uint8 highWord) noexcept {
  return (static_cast<uint16>(highWord) << (sizeof(lowWord) * 8)) | lowWord;
}

/**
  Returns an unsigned 32 bit integer from 2 16 bit integers.
  
  @param lowWord The low bits.
  @param highWord The high bits.
*/
inline uint32 merge(uint16 lowWord, uint16 highWord) noexcept {
  return (static_cast<uint32>(highWord) << (sizeof(lowWord) * 8)) | lowWord;
}

/**
  Returns an unsigned 64 bit integer from 2 32 bit integers.
  
  @param lowWord The low bits.
  @param highWord The high bits.
*/
inline uint64 merge(uint32 lowWord, uint32 highWord) noexcept {
  return (static_cast<uint64>(highWord) << (sizeof(lowWord) * 8)) | lowWord;
}



/**
  Returns the higher half-word of the specified value.
*/
inline uint8 getHighWordOf16(uint16 value) noexcept {
  return value >> (sizeof(value) * 8/2);
}

/**
  Returns the lower half-word of the specified value.
*/
inline uint8 getLowWordOf16(uint16 value) noexcept {
  return static_cast<uint8>(value & 0xff);
}

/**
  Returns the higher half-word of the specified value.
*/
inline uint16 getHighWordOf32(uint32 value) noexcept {
  return value >> (sizeof(value) * 8/2);
}

/**
  Returns the lower half-word of the specified value.
*/
inline uint16 getLowWordOf32(uint32 value) noexcept {
  return value;
}

/**
  Returns the higher half-word of the specified value.
*/
inline uint32 getHighWordOf64(uint64 value) noexcept {
  return value >> (sizeof(value) * 8/2);
}

/**
  Returns the lower half-word of the specified value.
*/
inline uint32 getLowWordOf64(uint64 value) noexcept {
  return static_cast<uint32>(value & 0xfffffff);
}

/**
  Returns true if the primitive variable is aligned properly.
*/
template<class TYPE>
inline bool isAligned(const TYPE& value) noexcept {
  unsigned int alignment = 0;
  if (sizeof(value) <= sizeof(long)) {
    alignment = sizeof(long);
  } else if (sizeof(value) <= 2 * sizeof(long)) {
    alignment = 2 * sizeof(long);
  } else {
    alignment = 4 * sizeof(long);
  }
  return (reinterpret_cast<const char*>(&value) - static_cast<const char*>(nullptr)) && ((alignment - 1) == 0);
}

/**
  Returns the requested primitive data type aligned at an appropriate address
  for the architecture. The buffer is expected to be of at least
  sizeof(TYPE) + n * sizeof(long) - 1 bytes (where n is the minimum value for
  which sizeof(TYPE) <= n * sizeof(long) is true).
  
  @param buffer The buffer.
*/
template<class TYPE>
inline TYPE& getAligned(char* buffer) noexcept {
  unsigned int alignment = 0;
  if (sizeof(buffer) <= sizeof(long)) {
    alignment = sizeof(long);
  } else if (sizeof(buffer) <= 2 * sizeof(long)) {
    alignment = 2 * sizeof(long);
  } else {
    alignment = 4 * sizeof(long);
  }
  return reinterpret_cast<TYPE*>(
    (((buffer - static_cast<char*>(nullptr)) + alignment - 1) & ~(alignment - 1)) +
    static_cast<char*>(nullptr)
  );
}

/**
  This namespace is a wrapper for information concerning the primitive
  (built-in) types.
  
  @short Primitive type information.
*/
namespace primitives {

  /**
    Specifies whether or not the type is void.

    @short Is void primitive type.
  */
  template<class TYPE> class Void {
  public:
    
    enum {IS_VOID = false /**< True if type is void. */};
  };
  
  template<> class Void<void> {
  public:

    enum {IS_VOID = true};
  };

  /**
    Specifies whether or not the type is a cardinal type (this includes the
    'bool' primitive).
    
    Example:
    
    @code
    template<class TYPE>
    void myFunction(const TYPE& value) noexcept {
      if (Primitives::Cardinal<TYPE>::IS_CARDINAL) {
        // do something
      } else {
        // do something
      }
    }
    @endcode

    @short Is cardinal primitive type.
  */
  template<class TYPE> class Cardinal {
  public:
    
    enum {IS_CARDINAL = false /**< True if type is a cardinal type. */};
  };
  
  template<> class Cardinal<bool> {
  public:

    enum {IS_CARDINAL = true};
  };
  
  template<> class Cardinal<char> {
  public:

    enum {IS_CARDINAL = true};
  };

  template<> class Cardinal<signed char> {
  public:

    enum {IS_CARDINAL = true};
  };

  template<> class Cardinal<unsigned char> {
  public:

    enum {IS_CARDINAL = true};
  };

  template<> class Cardinal<wchar_t> {
  public:

    enum {IS_CARDINAL = true};
  };
  
  template<> class Cardinal<short> {
  public:

    enum {IS_CARDINAL = true};
  };
  
  template<> class Cardinal<unsigned short> {
  public:

    enum {IS_CARDINAL = true};
  };
  
  template<> class Cardinal<int> {
  public:

    enum {IS_CARDINAL = true};
  };
  
  template<> class Cardinal<unsigned int> {
  public:

    enum {IS_CARDINAL = true};
  };
  
  template<> class Cardinal<long> {
  public:

    enum {IS_CARDINAL = true};
  };
  
  template<> class Cardinal<unsigned long> {
  public:

    enum {IS_CARDINAL = true};
  };
  
  template<> class Cardinal<long long> {
  public:

    enum {IS_CARDINAL = true};
  };
  
  template<> class Cardinal<unsigned long long> {
  public:

    enum {IS_CARDINAL = true};
  };
  
  /**
    Specifies whether or not the type is a floating point type.

    Example:
    
    @code
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
    @endcode

    @short Is floating point primitive type.
  */
  template<class TYPE> class FloatingPoint {
  public:

    enum {IS_FLOATING_POINT = false /**< True if type is a floating point type. */};
  };
  
  template<> class FloatingPoint<float> {
  public:

    enum {IS_FLOATING_POINT = true};
  };
  
  template<> class FloatingPoint<double> {
  public:
    
    enum {IS_FLOATING_POINT = true};
  };
  
  template<> class FloatingPoint<long double> {
  public:
    
    enum {IS_FLOATING_POINT = true};
  };

  /**
    Specifies whether or not the given type is an arithmetic (i.e. cardinal or
    floating) type.
    
    @short Is arithmetic primitive type.
  */
  template<class TYPE>
  class Arithmetic {
  public:
    enum {
      /** True if the type is an arithmetic type. */
      IS_ARITHMETIC = Cardinal<TYPE>::IS_CARDINAL ||
        FloatingPoint<TYPE>::IS_FLOATING_POINT
    };
  };

  /**
    Specifies whether or not the given type is a primitive (built-in) type.
    
    @short Is primitive type.
  */
  template<class TYPE>
  class Primitive {
  public:
    enum {
      /** True if the type is a built-in type. */
      IS_PRIMITIVE = Void<TYPE>::IS_VOID || Arithmetic<TYPE>::IS_ARITHMETIC
    };
  };

}; // end of primitives namespace

/**
  Contains information about the primitive type.
  
  @short Provides information for a primitive type.
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
  
  static constexpr bool MINIMUM = false;
  static constexpr bool MAXIMUM = true;
};

template<>
class PrimitiveTraits<signed char> {
public:
  
  typedef signed char Signed;
  typedef unsigned char Unsigned;
  
#if (_COM_AZURE_DEV__BASE__CHAR_SIZE == 1)
  static constexpr signed char MAXIMUM = 127;
  static constexpr signed char MINIMUM = -MAXIMUM - 1;
#endif
};

template<>
class PrimitiveTraits<unsigned char> {
public:
  
  typedef signed char Signed;
  typedef unsigned char Unsigned;
  
#if (_COM_AZURE_DEV__BASE__CHAR_SIZE == 1)
  static constexpr unsigned char MAXIMUM = 255;
  static constexpr unsigned char MINIMUM = 0;
#endif
};

template<>
class PrimitiveTraits<short> {
public:
  
  typedef short Signed;
  typedef unsigned short Unsigned;
  
#if (_COM_AZURE_DEV__BASE__SHORT_SIZE == 2)
  static constexpr short MAXIMUM = 32767;
#elif (_COM_AZURE_DEV__BASE__SHORT_SIZE = 4)
  static constexpr short MAXIMUM = 2147483647;
#endif
  static constexpr short MINIMUM = -MAXIMUM - 1;
};

template<>
class PrimitiveTraits<unsigned short> {
public:
  
  typedef short Signed;
  typedef unsigned short Unsigned;
  
#if (_COM_AZURE_DEV__BASE__SHORT_SIZE == 2)
  static constexpr unsigned short MAXIMUM = 65535U;
#elif (_COM_AZURE_DEV__BASE__SHORT_SIZE = 4)
  static constexpr unsigned short MAXIMUM = 4294967295U;
#endif
  static constexpr unsigned short MINIMUM = 0;
};

template<>
class PrimitiveTraits<int> {
public:
  
  typedef int Signed;
  typedef unsigned int Unsigned;
  
#if (_COM_AZURE_DEV__BASE__INT_SIZE == 4)
  static constexpr int MAXIMUM = 2147483647;
#elif (_COM_AZURE_DEV__BASE__INT_SIZE == 8)
  static constexpr int MAXIMUM = 9223372036854775807;
#endif
  static constexpr int MINIMUM = -MAXIMUM - 1L;
};

template<>
class PrimitiveTraits<unsigned int> {
public:
  
  typedef int Signed;
  typedef unsigned int Unsigned;
  
#if (_COM_AZURE_DEV__BASE__INT_SIZE == 4)
  static constexpr unsigned int MAXIMUM = 4294967295U;
#elif (_COM_AZURE_DEV__BASE__INT_SIZE == 8)
  static constexpr unsigned int MAXIMUM = 18446744073709551615U;
#endif
  static constexpr unsigned int MINIMUM = 0;
};

template<>
class PrimitiveTraits<long> {
public:
  
  typedef long Signed;
  typedef unsigned long Unsigned;
  
#if (_COM_AZURE_DEV__BASE__LONG_SIZE == 4)
  static constexpr long MAXIMUM = 2147483647L;
#elif (_COM_AZURE_DEV__BASE__LONG_SIZE == 8)
  static constexpr long MAXIMUM = 9223372036854775807L;
#endif
  static constexpr long MINIMUM = -MAXIMUM - 1L;
};

template<>
class PrimitiveTraits<unsigned long> {
public:
  
  typedef long Signed;
  typedef unsigned long Unsigned;
  
#if (_COM_AZURE_DEV__BASE__LONG_SIZE == 4)
  static constexpr unsigned long MAXIMUM = 4294967295UL;
#elif (_COM_AZURE_DEV__BASE__LONG_SIZE == 8)
  static constexpr unsigned long MAXIMUM = 18446744073709551615UL;
#endif
  static constexpr unsigned long MINIMUM = 0;
};

template<>
class PrimitiveTraits<long long> {
public:
  
  typedef long long Signed;
  typedef unsigned long long Unsigned;
  
#if (_COM_AZURE_DEV__BASE__LONG_LONG_SIZE == 8)
  static constexpr long long MAXIMUM = 9223372036854775807LL;
#endif
  static constexpr long long MINIMUM = -MAXIMUM - 1LL;
};

template<>
class PrimitiveTraits<unsigned long long> {
public:
  
  typedef long long Signed;
  typedef unsigned long long Unsigned;
  
#if (_COM_AZURE_DEV__BASE__LONG_LONG_SIZE == 8)
  static constexpr unsigned long long MAXIMUM = 18446744073709551615ULL;
#endif
  static constexpr unsigned long long MINIMUM = 0;
};

template<>
class _COM_AZURE_DEV__BASE__API PrimitiveTraits<float> {
public:  
  
  static const float MAXIMUM;
  static const float MINIMUM;
};

template<>
class _COM_AZURE_DEV__BASE__API PrimitiveTraits<double> {
public:
  
  static const double MAXIMUM;
  static const double MINIMUM;
};

template<>
class _COM_AZURE_DEV__BASE__API PrimitiveTraits<long double> {
public:
  
  static const long double MAXIMUM;
  static const long double MINIMUM;
};

/** Primitive array. */
template<class TYPE>
class PrimitiveArray : public std::vector<TYPE> {
public:

  inline PrimitiveArray(MemorySize size) : std::vector<TYPE>(size) {
    ASSERT(size > 0); // we need to be able to get a valid pointer
  }

  inline operator TYPE* () {
    return &std::vector<TYPE>::operator[](0);
  }

  inline operator const TYPE* () const {
    return &std::vector<TYPE>::operator[](0);
  }

  inline MemorySize size() const {
    return std::vector<TYPE>::size();
  }
};

/** Byte IO. */
class _COM_AZURE_DEV__BASE__API ByteIO {
public:

// TAG: need separate methods for byte order

  static inline uint16 readUInt16(const uint8* src) noexcept {
    const uint16 result = (static_cast<uint32>(src[0]) << 8) |
      (static_cast<uint32>(src[1]) << 0);
    return result;
  }

  static inline uint32 readUInt32(const uint8* src) noexcept {
    const uint32 result = (static_cast<uint32>(src[0]) << 24) |
      (static_cast<uint32>(src[1]) << 16) |
      (static_cast<uint32>(src[2]) << 8) |
      (static_cast<uint32>(src[3]) << 0);
    return result;
  }

  static inline uint64 readUInt64(const uint8* src) noexcept {
    const uint64 result = (static_cast<uint64>(src[0]) << 56) |
      (static_cast<uint64>(src[1]) << 48) |
      (static_cast<uint64>(src[2]) << 40) |
      (static_cast<uint64>(src[3]) << 32) |
      (static_cast<uint64>(src[4]) << 24) |
      (static_cast<uint64>(src[5]) << 16) |
      (static_cast<uint64>(src[6]) << 8) |
      (static_cast<uint64>(src[7]) << 0);
    return result;
  }
};

/** Returns the length of the null-terminated array (excluding terminator). */
template<class TYPE>
inline MemorySize getNullTerminatedLength(const TYPE* _src) noexcept
{
  if (!_src) {
    return 0;
  }
  const auto* src = _src;
  while (*src) { // just find end
    ++src;
  }
  return src - _src;
}

/** Returns the length of the null-terminated array (excluding terminator). */
template<class TYPE>
inline MemorySize getNullTerminatedLength(const TYPE* _src, const MemorySize maximumLength) noexcept
{
  if (!_src) {
    return 0;
  }
  const auto* src = _src;
  const auto* end = _src + maximumLength;
  while ((src != end) && *src) { // just find end
    ++src;
  }
  return src - _src;
}

/** Memory span. */
class MemorySpan {
public:

  const uint8* begin = nullptr;
  const uint8* end = nullptr;

  inline MemorySpan() noexcept
  {
  }

  /** Null-terminated span. */
  MemorySpan(const uint8* begin) noexcept;

  /** Null-terminated span. */
  MemorySpan(const char* begin) noexcept;

  inline MemorySpan(const uint8* _begin, const uint8* _end) noexcept
    : begin(_begin), end(_end)
  {
  }

  inline MemorySpan(const uint8* _begin, const MemorySize size) noexcept
    : begin(_begin), end(_begin + size)
  {
  }

  inline MemorySpan(const char* _begin, const char* _end) noexcept
    : begin(reinterpret_cast<const uint8*>(_begin)), end(reinterpret_cast<const uint8*>(_end))
  {
  }

  inline MemorySpan(const char* _begin, const MemorySize size) noexcept
    : begin(reinterpret_cast<const uint8*>(_begin)), end(reinterpret_cast<const uint8*>(_begin + size))
  {
  }

  inline bool isProper() const noexcept
  {
    return begin && (begin <= end);
  }

  inline MemoryDiff getSize() const noexcept
  {
    return end - begin;
  }

  /** Copy data to given destination. */
  uint8* copyTo(uint8* dest, const uint8* end) const noexcept;

  /** Copy data to given destination which must have room for entire memory span. */
  uint8* copyTo(uint8* dest) const noexcept;

  /** Copy data to given destination. */
  inline char* copyTo(char* dest, const uint8* end) const noexcept
  {
    return reinterpret_cast<char*>(copyTo(reinterpret_cast<uint8*>(dest), reinterpret_cast<const uint8*>(end)));
  }

  /** Copy data to given destination which must have room for entire memory span. */
  inline char* copyTo(char* dest) const noexcept
  {
    return reinterpret_cast<char*>(copyTo(reinterpret_cast<uint8*>(dest)));
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
