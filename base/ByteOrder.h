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

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This namespace contains functions used for explicit conversion between the
  supported byte orders for primitives. The supported byte orders are big
  endian and little endian.

  @short Byte order manipulation functions
*/

namespace ByteOrder {

#if (_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__BIG_ENDIAN)

  template<class TYPE>
  inline TYPE fromBigEndian(TYPE value) throw() {
    return value;
  }

  template<class TYPE>
  inline TYPE toBigEndian(TYPE value) throw() {
    return value;
  }

#elif (_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__LITTLE_ENDIAN)

  template<class TYPE>
  inline TYPE fromBigEndian(TYPE value) throw() {
    return value;
  }

  template<>
  inline uint16 fromBigEndian<uint16>(uint16 value) throw() {
    return (value >> 8) | (value << 8);
  }

  template<>
  inline uint32 fromBigEndian<uint32>(uint32 value) throw() {
    return (value >> 24) | ((value & 0xff0000) >> 8) | ((value & 0xff00) << 8) | (value << 24);
  }

  template<>
  inline uint64 fromBigEndian<uint64>(uint64 value) throw() {
    return fromBigEndian<uint32>(value >> 32) | static_cast<uint64>(fromBigEndian<uint32>(value)) << 32;
  }

  template<class TYPE>
  inline TYPE toBigEndian(TYPE value) throw() {
    return value;
  }

  template<>
  inline uint16 toBigEndian<uint16>(uint16 value) throw() {
    return (value >> 8) | (value << 8);
  }

  template<>
  inline uint32 toBigEndian<uint32>(uint32 value) throw() {
    return (value >> 24) | ((value & 0xff0000) >> 8) | ((value & 0xff00) << 8) | (value << 24);
  }

  template<>
  inline uint64 toBigEndian<uint64>(uint64 value) throw() {
    return toBigEndian<uint32>(value >> 32) | static_cast<uint64>(toBigEndian<uint32>(value & 0xffffffff)) << 32;
  }

#endif

}; // end of namespace ByteOrder



#if (_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__BIG_ENDIAN)

  /**
    This class is used to store primitives in little endian byte order
    independently of the native byte order of the platform.
    
    @short Little endian representation.
    @version 2.0
  */

  template<class TYPE>
  struct LittleEndian {
  };

_DK_SDU_MIP__BASE__PACKED__BEGIN
  /* 16 bit signed integer represented in little endian byte order. */
  template<>
  struct LittleEndian<int16> {
    uint8 a;
    uint8 b;
    
    inline LittleEndian() throw() {
    }
    
    inline LittleEndian(int16 value) throw()
      : a(value), b(value >> 8) {
    }
    
    inline int16 operator=(int16 eq) throw() {
      b = eq >> 8;
      a = eq;
      return eq;
    }
    
    inline operator int16() const throw() {
      return (static_cast<uint16>(b) << 8) + a;
    }
  } _DK_SDU_MIP__BASE__PACKED;
_DK_SDU_MIP__BASE__PACKED__END
  
_DK_SDU_MIP__BASE__PACKED__BEGIN
  /* 16 bit unsigned integer represented in little endian byte order. */
  template<>
  struct LittleEndian<uint16> {
    uint8 a;
    uint8 b;
    
    inline LittleEndian() throw() {
    }
    
    inline LittleEndian(uint16 value) throw()
      : a(value), b(value >> 8) {
    }
    
    inline uint16 operator=(uint16 eq) throw() {
      b = eq >> 8;
      a = eq;
      return eq;
    }
    
    inline operator uint16() const throw() {
      return (static_cast<uint16>(b) << 8) + a;
    }
  } _DK_SDU_MIP__BASE__PACKED;
_DK_SDU_MIP__BASE__PACKED__END

_DK_SDU_MIP__BASE__PACKED__BEGIN
  /* 32 bit signed integer represented in little endian byte order. */
  template<>
  struct LittleEndian<int32> {
    uint8 a;
    uint8 b;
    uint8 c;
    uint8 d;
    
    inline LittleEndian() throw() {
    }
    
    inline LittleEndian(int32 value) throw()
      : a(value), b(value >> 8), c(value >> 16), d(value >> 24) {
    }
    
    inline int32 operator=(int32 eq) throw() {
      d = eq >> 24;
      c = eq >> 16;
      b = eq >> 8;
      a = eq;
      return eq;
    }
    
    inline operator int32() const throw() {
      return (((static_cast<uint32>(d) << 8) + c << 8) + b << 8) + a;
    }
  } _DK_SDU_MIP__BASE__PACKED;
_DK_SDU_MIP__BASE__PACKED__END
  
_DK_SDU_MIP__BASE__PACKED__BEGIN
  /* 32 bit unsigned integer represented in little endian byte order. */
  template<>
  struct LittleEndian<uint32> {
    uint8 a;
    uint8 b;
    uint8 c;
    uint8 d;
    
    inline LittleEndian() throw() {
    }
    
    inline LittleEndian(uint32 value) throw()
      : a(value), b(value >> 8), c(value >> 16), d(value >> 24) {
    }
    
    inline uint32 operator=(uint32 eq) throw() {
      d = eq >> 24;
      c = eq >> 16;
      b = eq >> 8;
      a = eq;
      return eq;
    }
  
    inline operator uint32() const throw() {
      return (((static_cast<uint32>(d) << 8) + c << 8) + b << 8) + a;
    }
  } _DK_SDU_MIP__BASE__PACKED;
_DK_SDU_MIP__BASE__PACKED__END
  
_DK_SDU_MIP__BASE__PACKED__BEGIN
  /* 64 bit signed integer represented in little endian byte order. */
  template<>
  struct LittleEndian<int64> {
    LittleEndian<uint32> a;
    LittleEndian<uint32> b;
    
    inline LittleEndian() throw() {
    }
  
    inline LittleEndian(int64 value) throw()
      : a(value), b(value >> 32) {
    }
    
    inline int64 operator=(int64 eq) throw() {
      b = eq >> 32;
      a = eq;
      return eq;
    }
    
    inline operator int64() const throw() {
      return (static_cast<uint64>(b) << 8) + a;
    }
  } _DK_SDU_MIP__BASE__PACKED;
_DK_SDU_MIP__BASE__PACKED__END

_DK_SDU_MIP__BASE__PACKED__BEGIN
  /* 64 bit unsigned integer represented in little endian byte order. */
  template<>
  struct LittleEndian<uint64> {
    LittleEndian<uint32> a;
    LittleEndian<uint32> b;
    
    inline LittleEndian() throw() {
    }
    
    inline LittleEndian(uint64 value) throw()
      : a(value), b(value >> 32) {
    }
    
    inline uint64 operator=(uint64 eq) throw() {
      b = eq >> 32;
      a = eq;
      return eq;
    }
    
    inline operator uint64() const throw() {
      return (static_cast<uint64>(b) << 32) + a;
    }
  } _DK_SDU_MIP__BASE__PACKED;
_DK_SDU_MIP__BASE__PACKED__END

  /**
    This class is used to store primitives in big endian byte order
    independently of the native byte order of the platform.
    
    @short Big endian representation.
    @version 2.0
  */

_DK_SDU_MIP__BASE__PACKED__BEGIN
  template<class TYPE>
  struct BigEndian {
    TYPE value;
    
    inline BigEndian() throw() {
    }
    
    inline BigEndian(TYPE _value) throw()
      : value(_value) {
    }
    
    inline TYPE operator=(TYPE eq) throw() {
      value = eq;
      return eq;
    }
    
    inline operator TYPE() const throw() {
      return value;
    }
  } _DK_SDU_MIP__BASE__PACKED;
_DK_SDU_MIP__BASE__PACKED__END
  
#endif

#if (_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__LITTLE_ENDIAN)

  /**
    This class is used to store primitives in little endian byte order
    independently of the native byte order of the platform.
    
    @short Little endian representation.
    @version 2.0
  */
  
_DK_SDU_MIP__BASE__PACKED__BEGIN
  template<class TYPE>
  struct LittleEndian {
    TYPE value;
    
    inline LittleEndian() throw() {
    }
    
    inline LittleEndian(TYPE _value) throw()
      : value(_value) {
    }
    
    inline TYPE operator=(TYPE eq) throw() {
      value = eq;
      return eq;
    }
    
    inline operator TYPE() const throw() {
      return value;
    }
  } _DK_SDU_MIP__BASE__PACKED;
_DK_SDU_MIP__BASE__PACKED__END
  
  /**
    This class is used to store primitives in big endian byte order
    independently of the native byte order of the platform.
    
    @short Big endian representation.
    @version 2.0
  */

  template<class TYPE>
  struct BigEndian {
  };

_DK_SDU_MIP__BASE__PACKED__BEGIN
  /* 16 bit signed integer represented in big endian byte order. */
  template<>
  struct BigEndian<int16> {
    uint8 b;
    uint8 a;
    
    inline BigEndian() throw() {
    }
    
    inline BigEndian(int16 value) throw()
      : b(value >> 8), a(value) {
    }
    
    inline int16 operator=(int16 eq) throw() {
      b = eq >> 8;
      a = eq;
      return eq;
    }
    
    inline operator int16() const throw() {
      return (static_cast<uint16>(b) << 8) + a;
    }
  } _DK_SDU_MIP__BASE__PACKED;
_DK_SDU_MIP__BASE__PACKED__END
  
_DK_SDU_MIP__BASE__PACKED__BEGIN
  /* 16 bit unsigned integer represented in big endian byte order. */
  template<>
  struct BigEndian<uint16> {
    uint8 b;
    uint8 a;

    inline BigEndian() throw() {
    }
    
    inline BigEndian(uint16 value) throw()
      : b(value >> 8), a(value) {
    }
    
    inline uint16 operator=(uint16 eq) throw() {
      b = eq >> 8;
      a = eq;
      return eq;
    }
  
    inline operator uint16() const throw() {
      return (static_cast<uint16>(b) << 8) + a;
    }
  } _DK_SDU_MIP__BASE__PACKED;
_DK_SDU_MIP__BASE__PACKED__END

_DK_SDU_MIP__BASE__PACKED__BEGIN
  /* 32 bit signed integer represented in big endian byte order. */
  template<>
  struct BigEndian<int32> {
    uint8 d;
    uint8 c;
    uint8 b;
    uint8 a;
    
    inline BigEndian() throw() {
    }
    
    inline BigEndian(int32 value) throw()
      : d(value >> 24), c(value >> 16), b(value >> 8), a(value) {
    }
    
    inline int32 operator=(int32 eq) throw() {
      d = eq >> 24;
      c = eq >> 16;
      b = eq >> 8;
      a = eq;
      return eq;
    }
    
    inline operator int32() const throw() {
      return (((((static_cast<uint32>(d) << 8) + c) << 8) + b) << 8) + a;
    }
  } _DK_SDU_MIP__BASE__PACKED;
_DK_SDU_MIP__BASE__PACKED__END
  
_DK_SDU_MIP__BASE__PACKED__BEGIN
  /* 32 bit unsigned integer represented in big endian byte order. */
  template<>
  struct BigEndian<uint32> {
    uint8 d;
    uint8 c;
    uint8 b;
    uint8 a;
    
    inline BigEndian() throw() {
    }
    
    inline BigEndian(uint32 value) throw()
      : d(value >> 24), c(value >> 16), b(value >> 8), a(value) {
    }
    
    inline uint32 operator=(uint32 eq) throw() {
      d = eq >> 24;
      c = eq >> 16;
      b = eq >> 8;
      a = eq;
      return eq;
    }
  
    inline operator uint32() const throw() {
      return (((((static_cast<uint32>(d) << 8) + c) << 8) + b) << 8) + a;
    }
  } _DK_SDU_MIP__BASE__PACKED;
_DK_SDU_MIP__BASE__PACKED__END
  
_DK_SDU_MIP__BASE__PACKED__BEGIN
  /* 64 bit signed integer represented in big endian byte order. */
  template<>
  struct BigEndian<int64> {
    BigEndian<uint32> b;
    BigEndian<uint32> a;
    
    inline BigEndian() throw() {
    }
  
    inline BigEndian(int64 value) throw()
      : b(value >> 32), a(value) {
    }
    
    inline int64 operator=(int64 eq) throw() {
      b = eq >> 32;
      a = eq;
      return eq;
    }
    
    inline operator int64() const throw() {
      return (static_cast<uint64>(b) << 8) + a;
    }
  } _DK_SDU_MIP__BASE__PACKED;
_DK_SDU_MIP__BASE__PACKED__END
  
_DK_SDU_MIP__BASE__PACKED__BEGIN
  /* 64 bit unsigned integer represented in big endian byte order. */
  template<>
  struct BigEndian<uint64> {
    BigEndian<uint32> b;
    BigEndian<uint32> a;
    
    inline BigEndian() throw() {
    }
    
    inline BigEndian(uint64 value) throw()
      : b(value >> 32), a(value) {
    }
    
    inline uint64 operator=(uint64 eq) throw() {
      b = eq >> 32;
      a = eq;
      return eq;
    }
    
    inline operator uint64() const throw() {
      return (static_cast<uint64>(b) << 32) + a;
    }
  } _DK_SDU_MIP__BASE__PACKED;
_DK_SDU_MIP__BASE__PACKED__END
  
#endif // little endian

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
