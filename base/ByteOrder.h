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

#include <base/Primitives.h>
#include <base/string/FormatOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This namespace contains functions used for explicit conversion between the
  supported byte orders for primitives. The supported byte orders are big
  endian and little endian.

  @short Byte order manipulation functions
*/

namespace ByteOrder {

#if (_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__BIG_ENDIAN)

  /** Convert value from big endian. */
  template<class TYPE>
  inline TYPE fromBigEndian(TYPE value) noexcept
  {
    return value;
  }

  /** Convert value to big endian. */
  template<class TYPE>
  inline TYPE toBigEndian(TYPE value) noexcept
  {
    return value;
  }

#elif (_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN)

  /** Convert value from big endian. */
  template<class TYPE>
  inline TYPE fromBigEndian(TYPE value) noexcept
  {
    return value;
  }

  /** Convert value from big endian. */
  template<>
  inline uint16 fromBigEndian<uint16>(uint16 value) noexcept
  {
    return (value >> 8) | (value << 8);
  }

  /** Convert value from big endian. */
  template<>
  inline uint32 fromBigEndian<uint32>(uint32 value) noexcept
  {
    return (value >> 24) | ((value & 0xff0000) >> 8) | ((value & 0xff00) << 8) | (value << 24);
  }

  /** Convert value from big endian. */
  template<>
  inline uint64 fromBigEndian<uint64>(uint64 value) noexcept
  {
    return fromBigEndian<uint32>(static_cast<uint32>(value >> 32)) |
      static_cast<uint64>(fromBigEndian<uint32>(static_cast<uint32>(value))) << 32;
  }

  /** Convert value to big endian. */
  template<class TYPE>
  inline TYPE toBigEndian(TYPE value) noexcept
  {
    return value;
  }

  /** Convert value to big endian. */
  template<>
  inline uint16 toBigEndian<uint16>(uint16 value) noexcept
  {
    return (value >> 8) | (value << 8);
  }

  /** Convert value to big endian. */
  template<>
  inline uint32 toBigEndian<uint32>(uint32 value) noexcept
  {
    return (value >> 24) | ((value & 0xff0000) >> 8) | ((value & 0xff00) << 8) | (value << 24);
  }

  /** Convert value to big endian. */
  template<>
  inline uint64 toBigEndian<uint64>(uint64 value) noexcept
  {
    return toBigEndian<uint32>(value >> 32) | static_cast<uint64>(toBigEndian<uint32>(value & 0xffffffff)) << 32;
  }

#endif

}; // end of namespace ByteOrder



#if (_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__BIG_ENDIAN)

  /**
    This class is used to store primitives in little endian byte order
    independently of the native byte order of the platform.
    
    @short Little endian representation.
    @version 2.0
  */

  template<class TYPE>
  struct LittleEndian {
  };

_COM_AZURE_DEV__BASE__PACKED__BEGIN
  /* 16 bit signed integer represented in little endian byte order. */
  template<>
  struct LittleEndian<int16> {
    uint8 a;
    uint8 b;
    
    inline LittleEndian() noexcept
    {
    }
    
    inline LittleEndian(int16 value) noexcept
      : a(value), b(value >> 8)
    {
    }
    
    inline int16 operator=(int16 assign) noexcept
    {
      b = assign >> 8;
      a = assign;
      return assign;
    }
    
    inline operator int16() const noexcept
    {
      return (static_cast<uint16>(b) << 8) + a;
    }
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END
  
_COM_AZURE_DEV__BASE__PACKED__BEGIN
  /* 16 bit unsigned integer represented in little endian byte order. */
  template<>
  struct LittleEndian<uint16> {
    uint8 a;
    uint8 b;
    
    inline LittleEndian() noexcept
    {
    }
    
    inline LittleEndian(uint16 value) noexcept
      : a(value), b(value >> 8)
    {
    }
    
    inline uint16 operator=(uint16 assign) noexcept
    {
      b = assign >> 8;
      a = assign;
      return assign;
    }
    
    inline operator uint16() const noexcept
    {
      return (static_cast<uint16>(b) << 8) + a;
    }
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

_COM_AZURE_DEV__BASE__PACKED__BEGIN
  /* 32 bit signed integer represented in little endian byte order. */
  template<>
  struct LittleEndian<int32> {
    uint8 a;
    uint8 b;
    uint8 c;
    uint8 d;
    
    inline LittleEndian() noexcept
    {
    }
    
    inline LittleEndian(int32 value) noexcept
      : a(static_cast<uint8>(value & 0xff)),
        b(static_cast<uint8>((value >> 8) & 0xff)),
        c(static_cast<uint8>((value >> 16) & 0xff)),
        d(static_cast<uint8>((value >> 24) & 0xff))
    {
    }
    
    inline int32 operator=(int32 assign) noexcept
    {
      d = assign >> 24;
      c = assign >> 16;
      b = assign >> 8;
      a = assign;
      return assign;
    }
    
    inline operator int32() const noexcept {
      return (((static_cast<uint32>(d) << 8) + c << 8) + b << 8) + a;
    }
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END
  
_COM_AZURE_DEV__BASE__PACKED__BEGIN
  /* 32 bit unsigned integer represented in little endian byte order. */
  template<>
  struct LittleEndian<uint32> {
    uint8 a;
    uint8 b;
    uint8 c;
    uint8 d;
    
    inline LittleEndian() noexcept
    {
    }
    
    inline LittleEndian(uint32 value) noexcept
      : a(value), b(value >> 8), c(value >> 16), d(value >> 24)
    {
    }
    
    inline uint32 operator=(uint32 assign) noexcept
    {
      d = assign >> 24;
      c = assign >> 16;
      b = assign >> 8;
      a = assign;
      return assign;
    }
  
    inline operator uint32() const noexcept
    {
      return (((static_cast<uint32>(d) << 8) + c << 8) + b << 8) + a;
    }
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END
  
_COM_AZURE_DEV__BASE__PACKED__BEGIN
  /* 64 bit signed integer represented in little endian byte order. */
  template<>
  struct LittleEndian<int64> {
    LittleEndian<uint32> a;
    LittleEndian<uint32> b;
    
    inline LittleEndian() noexcept
    {
    }
  
    inline LittleEndian(int64 value) noexcept
      : a(value), b(value >> 32)
    {
    }
    
    inline int64 operator=(int64 assign) noexcept
    {
      b = assign >> 32;
      a = assign;
      return assign;
    }
    
    inline operator int64() const noexcept
    {
      return (static_cast<uint64>(b) << 8) + a;
    }
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

_COM_AZURE_DEV__BASE__PACKED__BEGIN
  /* 64 bit unsigned integer represented in little endian byte order. */
  template<>
  struct LittleEndian<uint64> {
    LittleEndian<uint32> a;
    LittleEndian<uint32> b;
    
    inline LittleEndian() noexcept
    {
    }
    
    inline LittleEndian(uint64 value) noexcept
      : a(value), b(value >> 32)
    {
    }
    
    inline uint64 operator=(uint64 assign) noexcept
    {
      b = assign >> 32;
      a = assign;
      return assign;
    }
    
    inline operator uint64() const noexcept
    {
      return (static_cast<uint64>(b) << 32) + a;
    }
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

  /**
    This class is used to store primitives in big endian byte order
    independently of the native byte order of the platform.
    
    @short Big endian representation.
    @version 2.0
  */

_COM_AZURE_DEV__BASE__PACKED__BEGIN
  template<class TYPE>
  struct BigEndian {
    TYPE value;
    
    inline BigEndian() noexcept
    {
    }
    
    inline BigEndian(TYPE _value) noexcept
      : value(_value)
    {
    }
    
    inline TYPE operator=(TYPE assign) noexcept
    {
      value = assign;
      return assign;
    }
    
    inline operator TYPE() const noexcept
    {
      return value;
    }
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END
  
#endif

#if (_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN)

  /**
    This class is used to store primitives in little endian byte order
    independently of the native byte order of the platform.
    
    @short Little endian representation.
    @version 2.0
  */
  
_COM_AZURE_DEV__BASE__PACKED__BEGIN
  template<class TYPE>
  struct LittleEndian {
    TYPE value;
    
    inline LittleEndian() noexcept
      : value(0)
    {
    }
    
    inline LittleEndian(TYPE _value) noexcept
      : value(_value) {
    }
    
    inline TYPE operator=(TYPE assign) noexcept
    {
      value = assign;
      return assign;
    }
    
    inline operator TYPE() const noexcept
    {
      return value;
    }
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END
  
  /**
    This class is used to store primitives in big endian byte order
    independently of the native byte order of the platform.
    
    @short Big endian representation.
    @version 2.0
  */

  template<class TYPE>
  struct BigEndian {
  };

_COM_AZURE_DEV__BASE__PACKED__BEGIN
  /* 16 bit signed integer represented in big endian byte order. */
  template<>
  struct BigEndian<int16> {
    uint8 b;
    uint8 a;
    
    inline BigEndian() noexcept
      : b(0), a(0)
    {
    }
    
    inline BigEndian(int16 value) noexcept
      : b(value >> 8), a(value)
    {
    }
    
    inline int16 operator=(int16 assign) noexcept
    {
      b = assign >> 8;
      a = assign;
      return assign;
    }
    
    inline operator int16() const noexcept
    {
      return (static_cast<uint16>(b) << 8) + a;
    }
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END
    
_COM_AZURE_DEV__BASE__PACKED__BEGIN
  /* 16 bit unsigned integer represented in big endian byte order. */
  template<>
  struct BigEndian<uint16> {
    uint8 b;
    uint8 a;

    inline BigEndian() noexcept
      : b(0), a(0)
    {
    }
    
    inline BigEndian(uint16 value) noexcept
      : b(value >> 8), a(value)
    {
    }
    
    inline uint16 operator=(uint16 assign) noexcept
    {
      b = assign >> 8;
      a = assign;
      return assign;
    }
  
    inline operator uint16() const noexcept
    {
      return (static_cast<uint16>(b) << 8) + a;
    }
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

_COM_AZURE_DEV__BASE__PACKED__BEGIN
  /* 32 bit signed integer represented in big endian byte order. */
  template<>
  struct BigEndian<int32> {
    uint8 d;
    uint8 c;
    uint8 b;
    uint8 a;
    
    inline BigEndian() noexcept
      : d(0), c(0), b(0), a(0)
    {
    }
    
    inline BigEndian(int32 value) noexcept
      : d(value >> 24), c(value >> 16), b(value >> 8), a(value)
    {
    }
    
    inline int32 operator=(int32 assign) noexcept
    {
      d = assign >> 24;
      c = assign >> 16;
      b = assign >> 8;
      a = assign;
      return assign;
    }
    
    inline operator int32() const noexcept
    {
      return (((((static_cast<uint32>(d) << 8) + c) << 8) + b) << 8) + a;
    }
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END
  
_COM_AZURE_DEV__BASE__PACKED__BEGIN
  /* 32 bit unsigned integer represented in big endian byte order. */
  template<>
  struct BigEndian<uint32> {
    uint8 d;
    uint8 c;
    uint8 b;
    uint8 a;
    
    inline BigEndian() noexcept
      : d(0), c(0), b(0), a(0)
    {
    }
    
    inline BigEndian(uint32 value) noexcept
      : d(value >> 24), c(value >> 16), b(value >> 8), a(value)
    {
    }
    
    inline uint32 operator=(uint32 assign) noexcept
    {
      d = assign >> 24;
      c = assign >> 16;
      b = assign >> 8;
      a = assign;
      return assign;
    }
  
    inline operator uint32() const noexcept
    {
      return (((((static_cast<uint32>(d) << 8) + c) << 8) + b) << 8) + a;
    }
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END
  
_COM_AZURE_DEV__BASE__PACKED__BEGIN
  /* 64 bit signed integer represented in big endian byte order. */
  template<>
  struct BigEndian<int64> {
    BigEndian<uint32> b;
    BigEndian<uint32> a;
    
    inline BigEndian() noexcept
      : b(0), a(0)
    {
    }
  
    inline BigEndian(int64 value) noexcept
      : b(static_cast<uint32>(value >> 32)), a(static_cast<uint32>(value))
    {
    }
    
    inline int64 operator=(int64 assign) noexcept
    {
      b = static_cast<uint32>(assign >> 32);
      a = static_cast<uint32>(assign);
      return assign;
    }
    
    inline operator int64() const noexcept
    {
      return (static_cast<uint64>(b) << 8) + a;
    }
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END
  
_COM_AZURE_DEV__BASE__PACKED__BEGIN
  /* 64 bit unsigned integer represented in big endian byte order. */
  template<>
  struct BigEndian<uint64> {
    BigEndian<uint32> b;
    BigEndian<uint32> a;
    
    inline BigEndian() noexcept
      : b(0), a(0)
    {
    }
    
    inline BigEndian(uint64 value) noexcept
      : b(static_cast<uint32>(value >> 32)), a(static_cast<uint32>(value))
    {
    }
    
    inline uint64 operator=(uint64 assign) noexcept
    {
      b = static_cast<uint32>(assign >> 32);
      a = static_cast<uint32>(assign);
      return assign;
    }
    
    inline operator uint64() const noexcept
    {
      return (static_cast<uint64>(b) << 32) + a;
    }
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END
  
#endif // little endian

template<class TYPE>
inline FormatOutputStream& operator<<(FormatOutputStream& stream, const LittleEndian<TYPE>& value)
{
  return stream << static_cast<TYPE>(value);
}

template<class TYPE>
inline FormatOutputStream& operator<<(FormatOutputStream& stream, const BigEndian<TYPE>& value)
{
  return stream << static_cast<TYPE>(value);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
