/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__FLOATING_POINT_H
#define _DK_SDU_MIP__BASE__FLOATING_POINT_H

#include <base/Primitives.h>
#include <base/string/InvalidFormat.h>
#include <base/string/String.h>

// TAG: what about rounding

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class provides support for floating point types.
  
  @short Floating point type support.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class FloatingPoint {
public:

  /**
    Specifies the maximum explicit exponent for string representations in
    decimal format.
  */
  static const unsigned int MAXIMUM_DECIMAL_EXPONENT = 9999;
  
  /**
    Specifies the maximum explicit exponent for string representations in
    hexadecimal format.
  */
  static const unsigned int MAXIMUM_BINARY_EXPONENT = 9999;
  
  /**
    Converts hex representation to a float.
  */
  float getFloatAsHex(const String& value) throw(InvalidFormat);

  /**
    Converts the string to a float.
  */
  static float getFloat(const String& value) throw(InvalidFormat);
  
  /**
    Converts the string to a double.
  */
  static double getDouble(const String& value) throw(InvalidFormat);
  
  /**
    Converts the string to a long double.
  */
  static long double getLongDouble(const String& value) throw(InvalidFormat);
  
  /** Floating point flags. */
  enum Flags {
    FP_NEGATIVE = 1, /**< Specifies a negative value. */
    FP_DENORMALIZED = 2, /**< Specifies a denormalized value. */
    FP_NAN = 4, /**< Specifies that the value isn't a number. */
    FP_ZERO = 8, /**< Value is zero (sign is determined by the FP_NEGATIVE flag). */
    FP_INFINITY = 16, /**< Value is infinity (sign is determined by the FP_NEGATIVE flag). */
    FP_VALUE = 32 /**< Specifies an ordinary value representable in [-]d.ddde[-]dd form. */
  };  
  
  class Representation {
  public:
    // TAG: problem if sizeof(uint32) != sizeof(unsigned int)
    
    /** Representation of single precision (32-bit) floating point type as specified by IEEE 754. */
    struct IEEE754SinglePrecision {
      static const bool HAS_IMPLIED_ONE = true;
      static const int MINIMUM_EXPONENT = -125;
      static const int MAXIMUM_EXPONENT = 128;
      static const int BIAS = 0x7f;
      static const unsigned int SIGNIFICANT = 24;
#if ((_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__BIG_ENDIAN) && \
     (_DK_SDU_MIP__BASE__FLOAT_WORD_ORDER == _DK_SDU_MIP__BASE__BIG_ENDIAN))
      uint32 negative : 1;
      uint32 exponent : 8;
      uint32 mantissa0 : 23;
#elif ((_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__BIG_ENDIAN) && \
       (_DK_SDU_MIP__BASE__FLOAT_WORD_ORDER == _DK_SDU_MIP__BASE__LITTLE_ENDIAN))
      uint32 negative : 1;
      uint32 exponent : 8;
      uint32 mantissa0 : 23;
#elif ((_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__LITTLE_ENDIAN) && \
       (_DK_SDU_MIP__BASE__FLOAT_WORD_ORDER == _DK_SDU_MIP__BASE__LITTLE_ENDIAN))
      uint32 mantissa0 : 23;
      uint32 exponent : 8;
      uint32 negative : 1;
#elif ((_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__LITTLE_ENDIAN) && \
       (_DK_SDU_MIP__BASE__FLOAT_WORD_ORDER == _DK_SDU_MIP__BASE__BIG_ENDIAN))
      uint32 mantissa0 : 23;
      uint32 exponent : 8;
      uint32 negative : 1;
#endif // bit allocation
    } _DK_SDU_MIP__BASE__PACKED;
    
    /** Representation of double precision (64-bit) floating point type as specified by IEEE 754. */
    struct IEEE754DoublePrecision {
      static const bool HAS_IMPLIED_ONE = true;
      static const int MINIMUM_EXPONENT = -1021;
      static const int MAXIMUM_EXPONENT = 1024;
      static const int BIAS = 0x3ff;
      static const unsigned int SIGNIFICANT = 53;
#if ((_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__BIG_ENDIAN) && \
     (_DK_SDU_MIP__BASE__FLOAT_WORD_ORDER == _DK_SDU_MIP__BASE__BIG_ENDIAN))
      uint32 negative : 1;
      uint32 exponent : 11;
      uint32 mantissa1 : 20;
      uint32 mantissa0 : 32;
#elif ((_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__BIG_ENDIAN) && \
       (_DK_SDU_MIP__BASE__FLOAT_WORD_ORDER == _DK_SDU_MIP__BASE__LITTLE_ENDIAN))
      uint32 mantissa0 : 32;
      uint32 negative : 1;
      uint32 exponent : 11;
      uint32 mantissa1 : 20;
#elif ((_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__LITTLE_ENDIAN) && \
       (_DK_SDU_MIP__BASE__FLOAT_WORD_ORDER == _DK_SDU_MIP__BASE__LITTLE_ENDIAN))
      uint32 mantissa0 : 32;
      uint32 mantissa1 : 20;
      uint32 exponent : 11;
      uint32 negative : 1;
#elif ((_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__LITTLE_ENDIAN) && \
       (_DK_SDU_MIP__BASE__FLOAT_WORD_ORDER == _DK_SDU_MIP__BASE__BIG_ENDIAN))
      uint32 mantissa1 : 20;
      uint32 exponent : 11;
      uint32 negative : 1;
      uint32 mantissa0 : 32;
#endif // bit allocation
    } _DK_SDU_MIP__BASE__PACKED;
    
    /** Representation of double-extended precision (96-bit) floating point type. */
    struct IEEEExtendedDoublePrecision96 {
      static const bool HAS_IMPLIED_ONE = false;
      static const int MINIMUM_EXPONENT = -16381;
      static const int MAXIMUM_EXPONENT = 16384;
      static const int BIAS = 0x3fff;
      static const unsigned int SIGNIFICANT = 64;
#if ((_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__BIG_ENDIAN) && \
     (_DK_SDU_MIP__BASE__FLOAT_WORD_ORDER == _DK_SDU_MIP__BASE__BIG_ENDIAN))
      uint32 negative : 1;
      uint32 exponent : 15;
      uint32 empty : 16;
      uint32 mantissa1 : 32;
      uint32 mantissa0 : 32;
#elif ((_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__BIG_ENDIAN) && \
       (_DK_SDU_MIP__BASE__FLOAT_WORD_ORDER == _DK_SDU_MIP__BASE__LITTLE_ENDIAN))
      uint32 mantissa0 : 32;
      uint32 mantissa1 : 32;
      uint32 negative : 1;
      uint32 exponent : 15;
      uint32 empty : 16;
#elif ((_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__LITTLE_ENDIAN) && \
       (_DK_SDU_MIP__BASE__FLOAT_WORD_ORDER == _DK_SDU_MIP__BASE__LITTLE_ENDIAN))
      uint32 mantissa0 : 32;
      uint32 mantissa1 : 32;
      uint32 exponent : 15;
      uint32 negative : 1;
      uint32 empty : 16;
#elif ((_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__LITTLE_ENDIAN) && \
       (_DK_SDU_MIP__BASE__FLOAT_WORD_ORDER == _DK_SDU_MIP__BASE__BIG_ENDIAN))
      uint32 exponent : 15;
      uint32 negative : 1;
      uint32 empty : 16;
      uint32 mantissa1 : 32;
      uint32 mantissa0 : 32;
#endif // bit allocation
    } _DK_SDU_MIP__BASE__PACKED;
    
    /** Representation of double-extended precision (128-bit) floating point type. */
    struct IEEEExtendedDoublePrecision128 {
      static const bool HAS_IMPLIED_ONE = false;
      static const int BIAS = 0x3fff;
      static const unsigned int SIGNIFICANT = 64;
#if ((_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__BIG_ENDIAN) && \
     (_DK_SDU_MIP__BASE__FLOAT_WORD_ORDER == _DK_SDU_MIP__BASE__BIG_ENDIAN))
      uint32 empty1 : 32;
      uint32 negative : 1;
      uint32 exponent : 15;
      uint32 empty0 : 16;
      uint32 mantissa1 : 32;
      uint32 mantissa0 : 32;
#elif ((_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__BIG_ENDIAN) && \
       (_DK_SDU_MIP__BASE__FLOAT_WORD_ORDER == _DK_SDU_MIP__BASE__LITTLE_ENDIAN))
      uint32 mantissa0 : 32;
      uint32 mantissa1 : 32;
      uint32 negative : 1;
      uint32 exponent : 15;
      uint32 empty0 : 16;
      uint32 empty1 : 32;
#elif ((_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__LITTLE_ENDIAN) && \
       (_DK_SDU_MIP__BASE__FLOAT_WORD_ORDER == _DK_SDU_MIP__BASE__LITTLE_ENDIAN))
      uint32 mantissa0 : 32;
      uint32 mantissa1 : 32;
      uint32 exponent : 15;
      uint32 negative : 1;
      uint32 empty0 : 16;
      uint32 empty1 : 32;
#elif ((_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__LITTLE_ENDIAN) && \
       (_DK_SDU_MIP__BASE__FLOAT_WORD_ORDER == _DK_SDU_MIP__BASE__BIG_ENDIAN))
      uint32 empty1 : 32;
      uint32 exponent : 15;
      uint32 negative : 1;
      uint32 empty0 : 16;
      uint32 mantissa1 : 32;
      uint32 mantissa0 : 32;
#endif // bit allocation
    } _DK_SDU_MIP__BASE__PACKED;
  
    /** Representation of quadruple precision (128-bit) floating point type. */
    struct IEEEQuadruplePrecision {
      static const bool HAS_IMPLIED_ONE = true;
      static const int MINIMUM_EXPONENT = -16381;
      static const int MAXIMUM_EXPONENT = 16384;
      static const int BIAS = 0x3fff;
      static const unsigned int SIGNIFICANT = 113;
#if ((_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__BIG_ENDIAN) && \
     (_DK_SDU_MIP__BASE__FLOAT_WORD_ORDER == _DK_SDU_MIP__BASE__BIG_ENDIAN))
      uint32 negative : 1;
      uint32 exponent : 15;
      uint32 mantissa3 : 16;
      uint32 mantissa2 : 32;
      uint32 mantissa1 : 32;
      uint32 mantissa0 : 32;
#elif ((_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__BIG_ENDIAN) && \
       (_DK_SDU_MIP__BASE__FLOAT_WORD_ORDER == _DK_SDU_MIP__BASE__LITTLE_ENDIAN))
      uint32 mantissa0 : 32;
      uint32 mantissa1 : 32;
      uint32 mantissa2 : 32;
      uint32 negative : 1;
      uint32 exponent : 15;
      uint32 mantissa3 : 16;
#elif ((_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__LITTLE_ENDIAN) && \
       (_DK_SDU_MIP__BASE__FLOAT_WORD_ORDER == _DK_SDU_MIP__BASE__LITTLE_ENDIAN))
      uint32 mantissa0 : 32;
      uint32 mantissa1 : 32;
      uint32 mantissa2 : 32;
      uint32 mantissa3 : 16;
      uint32 exponent : 15;
      uint32 negative : 1;
#elif ((_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__LITTLE_ENDIAN) && \
       (_DK_SDU_MIP__BASE__FLOAT_WORD_ORDER == _DK_SDU_MIP__BASE__BIG_ENDIAN))
      uint32 mantissa3 : 16;
      uint32 exponent : 15;
      uint32 negative : 1;
      uint32 mantissa2 : 32;
      uint32 mantissa1 : 32;
      uint32 mantissa0 : 32;
#endif // bit allocation
    } _DK_SDU_MIP__BASE__PACKED;
  };

#if (_DK_SDU_MIP__BASE__FLOAT == _DK_SDU_MIP__BASE__IEEE_754_SINGLE_PRECISION)
  typedef Representation::IEEE754SinglePrecision FloatRepresentation;
#elif (_DK_SDU_MIP__BASE__FLOAT == _DK_SDU_MIP__BASE__IEEE_754_DOUBLE_PRECISION)
  typedef Representation::IEEE754DoublePrecision FloatRepresentation;
#elif (_DK_SDU_MIP__BASE__FLOAT == _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96)
  typedef Representation::IEEEExtendedDoublePrecision96 FloatRepresentation;
#elif (_DK_SDU_MIP__BASE__FLOAT == _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128)
  typedef Representation::IEEEExtendedDoublePrecision128 FloatRepresentation;
#elif (_DK_SDU_MIP__BASE__FLOAT == _DK_SDU_MIP__BASE__IEEE_QUADRUPLE_PRECISION)
  typedef Representation::IEEEQuadruplePrecision FloatRepresentation;
#else
#  error Invalid floating-point representation of type float
#endif

#if (_DK_SDU_MIP__BASE__DOUBLE == _DK_SDU_MIP__BASE__IEEE_754_SINGLE_PRECISION)
  typedef Representation::IEEE754SinglePrecision DoubleRepresentation;
#elif (_DK_SDU_MIP__BASE__DOUBLE == _DK_SDU_MIP__BASE__IEEE_754_DOUBLE_PRECISION)
  typedef Representation::IEEE754DoublePrecision DoubleRepresentation;
#elif (_DK_SDU_MIP__BASE__DOUBLE == _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96)
  typedef Representation::IEEEExtendedDoublePrecision96 DoubleRepresentation;
#elif (_DK_SDU_MIP__BASE__DOUBLE == _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128)
  typedef Representation::IEEEExtendedDoublePrecision128 DoubleRepresentation;
#elif (_DK_SDU_MIP__BASE__DOUBLE == _DK_SDU_MIP__BASE__IEEE_QUADRUPLE_PRECISION)
  typedef Representation::IEEEQuadruplePrecision DoubleRepresentation;
#else
#  error Invalid floating-point representation of type double
#endif

#if (_DK_SDU_MIP__BASE__LONG_DOUBLE == _DK_SDU_MIP__BASE__IEEE_754_SINGLE_PRECISION)
  typedef Representation::IEEE754SinglePrecision LongDoubleRepresentation;
#elif (_DK_SDU_MIP__BASE__LONG_DOUBLE == _DK_SDU_MIP__BASE__IEEE_754_DOUBLE_PRECISION)
  typedef Representation::IEEE754DoublePrecision LongDoubleRepresentation;
#elif (_DK_SDU_MIP__BASE__LONG_DOUBLE == _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96)
  typedef Representation::IEEEExtendedDoublePrecision96 LongDoubleRepresentation;
#elif (_DK_SDU_MIP__BASE__LONG_DOUBLE == _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128)
  typedef Representation::IEEEExtendedDoublePrecision128 LongDoubleRepresentation;
#elif (_DK_SDU_MIP__BASE__LONG_DOUBLE == _DK_SDU_MIP__BASE__IEEE_QUADRUPLE_PRECISION)
  typedef Representation::IEEEQuadruplePrecision LongDoubleRepresentation;
#else
#  error Invalid floating-point representation of type long double
#endif
  
  /* IEEE 754 single precision convertion support. */
  struct IEEE754SinglePrecision {
    Representation::IEEE754SinglePrecision value;
    
    inline void setValue(const Representation::IEEE754SinglePrecision& value) throw() {
      this->value = value;
    }
    
    inline void setValue(const Representation::IEEE754DoublePrecision& _value) throw() {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent; // possible overflow and underflow
      // both representations have implied one
      unsigned int mantissa =
        (static_cast<uint32>(_value.mantissa1) << 3) | (_value.mantissa0 >> 29);
      value.mantissa0 = mantissa;
    }
    
    void setValue(const Representation::IEEEExtendedDoublePrecision96& _value) throw();
    
    void setValue(const Representation::IEEEExtendedDoublePrecision128& _value) throw();
    
    inline void setValue(const Representation::IEEEQuadruplePrecision& _value) throw() {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent; // possible overflow and underflow
      // both representations have implied one
      value.mantissa0 = (static_cast<uint32>(_value.mantissa3) << (23 - 16)) |
        (_value.mantissa2 >> 32 - 23 + 16 + 1);
    }
    
    inline void setValue(float value) throw() {
      setValue(*reinterpret_cast<const FloatRepresentation*>(&value));
    }
    
    inline void setValue(double value) throw() {
      setValue(*reinterpret_cast<const DoubleRepresentation*>(&value));
    }
    
    inline void setValue(long double value) throw() {
      setValue(*reinterpret_cast<const LongDoubleRepresentation*>(&value));
    }
    
    inline IEEE754SinglePrecision() throw() {
    }

    /**
      Initializes representation from floating point components.
      
      @param negative True if value is negative.
      @param mantissa The mantissa hexadecimal values (i.e. 0-15).
      @param size The number of mantissa digits.
      @param exponent The exponent (power of 2).
    */
    IEEE754SinglePrecision(bool negative, const uint8* mantissa, unsigned int size, int exponent) throw();
    
    inline IEEE754SinglePrecision(const Representation::IEEE754SinglePrecision& value) throw() {
      setValue(value);
    }
    
    inline IEEE754SinglePrecision(const Representation::IEEE754DoublePrecision& value) throw() {
      setValue(value);
    }
    
    inline IEEE754SinglePrecision(const Representation::IEEEExtendedDoublePrecision96& value) throw() {
      setValue(value);
    }
    
    inline IEEE754SinglePrecision(const Representation::IEEEExtendedDoublePrecision128& value) throw() {
      setValue(value);
    }
    
    inline IEEE754SinglePrecision(const Representation::IEEEQuadruplePrecision& value) throw() {
      setValue(value);
    }
    
    inline IEEE754SinglePrecision(float value) throw() {
      setValue(*reinterpret_cast<const FloatRepresentation*>(&value));
    }
    
    inline IEEE754SinglePrecision(double value) throw() {
      setValue(*reinterpret_cast<const DoubleRepresentation*>(&value));
    }
    
    inline IEEE754SinglePrecision(long double value) throw() {
      setValue(*reinterpret_cast<const LongDoubleRepresentation*>(&value));
    }
    
    operator float() const throw();
    
    operator double() const throw();
    
    operator long double() const throw();
  } _DK_SDU_MIP__BASE__PACKED;

  /* IEEE 754 double precision convertion support. */
  struct IEEE754DoublePrecision {
    Representation::IEEE754DoublePrecision value;
    
    inline void setValue(const Representation::IEEE754SinglePrecision& _value) throw() {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent; // possible overflow and underflow
      // both representations have implied one
      value.mantissa1 = _value.mantissa0 >> (23 - 20);
      value.mantissa0 = static_cast<uint32>(_value.mantissa0) << (32 - (23 - 20));
    }
    
    inline void setValue(const Representation::IEEE754DoublePrecision& _value) throw() {
      this->value = value;
    }
    
    void setValue(const Representation::IEEEExtendedDoublePrecision96& _value) throw();
    
    void setValue(const Representation::IEEEExtendedDoublePrecision128& _value) throw();
    
    inline void setValue(const Representation::IEEEQuadruplePrecision& _value) throw() {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent; // possible overflow and underflow
      // both representations have implied one
      value.mantissa1 = (static_cast<uint32>(_value.mantissa3) << (20 - 16)) |
        (_value.mantissa2 >> (32 - (20 - 16)));
      value.mantissa0 = (static_cast<uint32>(_value.mantissa2) << (32 - (20 - 16))) |
        (_value.mantissa1 >> (32 - (20 - 16)));
    }

    inline void setValue(float value) throw() {
      setValue(*reinterpret_cast<const FloatRepresentation*>(&value));
    }
    
    inline void setValue(double value) throw() {
      setValue(*reinterpret_cast<const DoubleRepresentation*>(&value));
    }
    
    inline void setValue(long double value) throw() {
      setValue(*reinterpret_cast<const LongDoubleRepresentation*>(&value));
    }
    
    inline IEEE754DoublePrecision() throw() {
    }
    
    inline IEEE754DoublePrecision(const Representation::IEEE754SinglePrecision& value) throw() {
      setValue(value);
    }
    
    inline IEEE754DoublePrecision(const Representation::IEEE754DoublePrecision& value) throw() {
      setValue(value);
    }
    
    inline IEEE754DoublePrecision(const Representation::IEEEExtendedDoublePrecision96& value) throw() {
      setValue(value);
    }
    
    inline IEEE754DoublePrecision(const Representation::IEEEExtendedDoublePrecision128& value) throw() {
      setValue(value);
    }
    
    inline IEEE754DoublePrecision(const Representation::IEEEQuadruplePrecision& value) throw() {
      setValue(value);
    }
    
    inline IEEE754DoublePrecision(float value) throw() {
      setValue(*reinterpret_cast<const FloatRepresentation*>(&value));
    }
    
    inline IEEE754DoublePrecision(double value) throw() {
      setValue(*reinterpret_cast<const DoubleRepresentation*>(&value));
    }
    
    inline IEEE754DoublePrecision(long double value) throw() {
      setValue(*reinterpret_cast<const LongDoubleRepresentation*>(&value));
    }
    
    operator float() const throw();
    
    operator double() const throw();
    
    operator long double() const throw();
  } _DK_SDU_MIP__BASE__PACKED;
  
  /* IEEE extended double precision (96 bit) convertion support. */
  struct IEEEExtendedDoublePrecision96 {
    Representation::IEEEExtendedDoublePrecision128 value;
    
    inline void setValue(const Representation::IEEE754SinglePrecision& _value) throw() {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent;
      value.mantissa1 = (1 << 31) | (static_cast<uint32>(_value.mantissa0) << (32 - 23 - 1));
      value.mantissa0 = 0;
    }
    
    inline void setValue(const Representation::IEEE754DoublePrecision& _value) throw() {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent;
      value.mantissa1 = (1 << 31) | (static_cast<uint32>(_value.mantissa1) << (32 - 20 - 1)) |
        (_value.mantissa0 >> (32 - 20 - 1));
      value.mantissa0 = static_cast<uint32>(_value.mantissa0) << (32 - 20 - 1);
    }
    
    inline void setValue(const Representation::IEEEExtendedDoublePrecision96& _value) throw() {
      this->value = value;
    }
    
    inline void setValue(const Representation::IEEEExtendedDoublePrecision128& _value) throw() {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent;
      value.mantissa1 = _value.mantissa1;
      value.mantissa0 = _value.mantissa0;
    }
    
    inline void setValue(const Representation::IEEEQuadruplePrecision& _value) throw() {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent;
      value.mantissa1 = (1 << 31) | (static_cast<uint32>(_value.mantissa3) << (32 - 16 - 1)) |
        (_value.mantissa2 >> (32 - (32 - 16 - 1)));
      value.mantissa0 = (static_cast<uint32>(_value.mantissa2) << (32 - (32 - 16 - 1))) |
        (_value.mantissa1 >> (32 - (32 - 16 - 1)));
    }
    
    inline void setValue(float value) throw() {
      setValue(*reinterpret_cast<const FloatRepresentation*>(&value));
    }
    
    inline void setValue(double value) throw() {
      setValue(*reinterpret_cast<const DoubleRepresentation*>(&value));
    }
    
    inline void setValue(long double value) throw() {
      setValue(*reinterpret_cast<const LongDoubleRepresentation*>(&value));
    }
    
    inline IEEEExtendedDoublePrecision96() throw() {
    }
    
    inline IEEEExtendedDoublePrecision96(const Representation::IEEE754SinglePrecision& value) throw() {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision96(const Representation::IEEE754DoublePrecision& value) throw() {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision96(const Representation::IEEEExtendedDoublePrecision96& value) throw() {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision96(const Representation::IEEEExtendedDoublePrecision128& value) throw() {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision96(const Representation::IEEEQuadruplePrecision& value) throw() {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision96(float value) throw() {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision96(double value) throw() {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision96(long double value) throw() {
      setValue(value);
    }
    
    operator float() const throw();
    
    operator double() const throw();
    
    operator long double() const throw();
  } _DK_SDU_MIP__BASE__PACKED;
  
  /* IEEE extended double precision (128 bit) convertion support. */
  struct IEEEExtendedDoublePrecision128 {
    Representation::IEEEExtendedDoublePrecision128 value;
    
    inline void setValue(const Representation::IEEE754SinglePrecision& _value) throw() {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent;
      value.mantissa1 = (1 << 31) | (static_cast<uint32>(_value.mantissa0) << (32 - 23 - 1));
      value.mantissa0 = 0;
    }
    
    inline void setValue(const Representation::IEEE754DoublePrecision& _value) throw() {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent;
      value.mantissa1 = (1 << 31) | (static_cast<uint32>(_value.mantissa1) << (32 - 20 - 1)) |
        (_value.mantissa0 >> (32 - 20 - 1));
      value.mantissa0 = static_cast<uint32>(_value.mantissa0) << (32 - 20 - 1);
    }
    
    inline void setValue(const Representation::IEEEExtendedDoublePrecision96& _value) throw() {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent;
      value.mantissa1 = _value.mantissa1;
      value.mantissa0 = _value.mantissa0;
    }
    
    inline void setValue(const Representation::IEEEExtendedDoublePrecision128& _value) throw() {
      this->value = value;
    }
    
    inline void setValue(const Representation::IEEEQuadruplePrecision& _value) throw() {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent;
      value.mantissa1 = (1 << 31) | (static_cast<uint32>(_value.mantissa3) << (32 - 16 - 1)) |
        (_value.mantissa2 >> (32 - (32 - 16 - 1)));
      value.mantissa0 = (static_cast<uint32>(_value.mantissa2) << (32 - (32 - 16 - 1))) |
        (_value.mantissa1 >> (32 - (32 - 16 - 1)));
    }
    
    inline void setValue(float value) throw() {
      setValue(*reinterpret_cast<const FloatRepresentation*>(&value));
    }
    
    inline void setValue(double value) throw() {
      setValue(*reinterpret_cast<const DoubleRepresentation*>(&value));
    }
    
    inline void setValue(long double value) throw() {
      setValue(*reinterpret_cast<const LongDoubleRepresentation*>(&value));
    }
    
    inline IEEEExtendedDoublePrecision128() throw() {
    }
    
    inline IEEEExtendedDoublePrecision128(const Representation::IEEE754SinglePrecision& value) throw() {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision128(const Representation::IEEE754DoublePrecision& value) throw() {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision128(const Representation::IEEEExtendedDoublePrecision96& value) throw() {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision128(const Representation::IEEEExtendedDoublePrecision128& value) throw() {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision128(const Representation::IEEEQuadruplePrecision& value) throw() {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision128(float value) throw() {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision128(double value) throw() {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision128(long double value) throw() {
      setValue(value);
    }
    
    operator float() const throw();
    
    operator double() const throw();
    
    operator long double() const throw();
  } _DK_SDU_MIP__BASE__PACKED;
  
  /* IEEE quadruple precision convertion support. */
  struct IEEEQuadruplePrecision {
    Representation::IEEEQuadruplePrecision value;
    
    inline void setValue(const Representation::IEEE754SinglePrecision& _value) throw() {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent; // possible overflow and underflow
      // both representations have implied one
      value.mantissa3 = _value.mantissa0 >> (23 - 16);
      value.mantissa2 = static_cast<uint32>(_value.mantissa0) << (32 - (23 - 16));
      value.mantissa1 = 0;
      value.mantissa0 = 0;
    }
    
    inline void setValue(const Representation::IEEE754DoublePrecision& _value) throw() {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent; // possible overflow and underflow
      // both representations have implied one
      value.mantissa3 = _value.mantissa1 >> (20 - 16);
      value.mantissa2 = static_cast<uint32>(_value.mantissa1) << (32 - (20 - 16)) |
        (_value.mantissa0 >> (32 - 20));
      value.mantissa1 = 0;
      value.mantissa0 = 0;
    }
    
    void setValue(const Representation::IEEEExtendedDoublePrecision96& _value) throw();
    
    void setValue(const Representation::IEEEExtendedDoublePrecision128& _value) throw();
    
    inline void setValue(const Representation::IEEEQuadruplePrecision& _value) throw() {
      this->value = value;
    }
    
    inline void setValue(float value) throw() {
      setValue(*reinterpret_cast<const FloatRepresentation*>(&value));
    }
    
    inline void setValue(double value) throw() {
      setValue(*reinterpret_cast<const DoubleRepresentation*>(&value));
    }
    
    inline void setValue(long double value) throw() {
      setValue(*reinterpret_cast<const LongDoubleRepresentation*>(&value));
    }
    
    inline IEEEQuadruplePrecision() throw() {
    }
    
    inline IEEEQuadruplePrecision(const Representation::IEEE754SinglePrecision& value) throw() {
      setValue(value);
    }
    
    inline IEEEQuadruplePrecision(const Representation::IEEE754DoublePrecision& value) throw() {
      setValue(value);
    }
    
    inline IEEEQuadruplePrecision(const Representation::IEEEExtendedDoublePrecision96& value) throw() {
      setValue(value);
    }
    
    inline IEEEQuadruplePrecision(const Representation::IEEEExtendedDoublePrecision128& value) throw() {
      setValue(value);
    }
    
    inline IEEEQuadruplePrecision(const Representation::IEEEQuadruplePrecision& value) throw() {
      setValue(value);
    }
    
    inline IEEEQuadruplePrecision(float value) throw() {
      setValue(value);
    }
    
    inline IEEEQuadruplePrecision(double value) throw() {
      setValue(value);
    }
    
    inline IEEEQuadruplePrecision(long double value) throw() {
      setValue(value);
    }
    
    operator float() const throw();
    
    operator double() const throw();
    
    operator long double() const throw();
  } _DK_SDU_MIP__BASE__PACKED;

#if (_DK_SDU_MIP__BASE__FLOAT == _DK_SDU_MIP__BASE__IEEE_754_SINGLE_PRECISION)
  typedef IEEE754SinglePrecision ToFloat;
#elif (_DK_SDU_MIP__BASE__FLOAT == _DK_SDU_MIP__BASE__IEEE_754_DOUBLE_PRECISION)
  typedef IEEE754DoublePrecision ToFloat;
#elif (_DK_SDU_MIP__BASE__FLOAT == _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96)
  typedef IEEEExtendedDoublePrecision96 ToFloat;
#elif (_DK_SDU_MIP__BASE__FLOAT == _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128)
  typedef IEEEExtendedDoublePrecision128 ToFloat;
#elif (_DK_SDU_MIP__BASE__FLOAT == _DK_SDU_MIP__BASE__IEEE_QUADRUPLE_PRECISION)
  typedef IEEEQuadruplePrecision ToFloat;
#else
#  error Invalid floating-point representation of type float
#endif

#if (_DK_SDU_MIP__BASE__DOUBLE == _DK_SDU_MIP__BASE__IEEE_754_SINGLE_PRECISION)
  typedef IEEE754SinglePrecision ToDouble;
#elif (_DK_SDU_MIP__BASE__DOUBLE == _DK_SDU_MIP__BASE__IEEE_754_DOUBLE_PRECISION)
  typedef IEEE754DoublePrecision ToDouble;
#elif (_DK_SDU_MIP__BASE__DOUBLE == _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96)
  typedef IEEEExtendedDoublePrecision96 ToDouble;
#elif (_DK_SDU_MIP__BASE__DOUBLE == _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128)
  typedef IEEEExtendedDoublePrecision128 ToDouble;
#elif (_DK_SDU_MIP__BASE__DOUBLE == _DK_SDU_MIP__BASE__IEEE_QUADRUPLE_PRECISION)
  typedef IEEEQuadruplePrecision ToDouble;
#else
#  error Invalid floating-point representation of type double
#endif

#if (_DK_SDU_MIP__BASE__LONG_DOUBLE == _DK_SDU_MIP__BASE__IEEE_754_SINGLE_PRECISION)
  typedef IEEE754SinglePrecision ToLongDouble;
#elif (_DK_SDU_MIP__BASE__LONG_DOUBLE == _DK_SDU_MIP__BASE__IEEE_754_DOUBLE_PRECISION)
  typedef IEEE754DoublePrecision ToLongDouble;
#elif (_DK_SDU_MIP__BASE__LONG_DOUBLE == _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96)
  typedef IEEEExtendedDoublePrecision96 ToLongDouble;
#elif (_DK_SDU_MIP__BASE__LONG_DOUBLE == _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128)
  typedef IEEEExtendedDoublePrecision128 ToLongDouble;
#elif (_DK_SDU_MIP__BASE__LONG_DOUBLE == _DK_SDU_MIP__BASE__IEEE_QUADRUPLE_PRECISION)
  typedef IEEEQuadruplePrecision ToLongDouble;
#else
#  error Invalid floating-point representation of type long double
#endif
};

inline FloatingPoint::IEEE754SinglePrecision::operator float() const throw() {
#if (_DK_SDU_MIP__BASE__FLOAT == _DK_SDU_MIP__BASE__IEEE_754_SINGLE_PRECISION)
  return *reinterpret_cast<const float*>(&value);
#else
  return ToFloat(value);
#endif // float
}

inline FloatingPoint::IEEE754SinglePrecision::operator double() const throw() {
#if (_DK_SDU_MIP__BASE__DOUBLE == _DK_SDU_MIP__BASE__IEEE_754_SINGLE_PRECISION)
  return *reinterpret_cast<const double*>(&value);
#else
  return ToDouble(value);
#endif // double
}

inline FloatingPoint::IEEE754SinglePrecision::operator long double() const throw() {
#if (_DK_SDU_MIP__BASE__LONG_DOUBLE == _DK_SDU_MIP__BASE__IEEE_754_SINGLE_PRECISION)
  return *reinterpret_cast<const long double*>(&value);
#else
  return ToLongDouble(value);
#endif // long double
}

inline FloatingPoint::IEEE754DoublePrecision::operator float() const throw() {
#if (_DK_SDU_MIP__BASE__FLOAT == _DK_SDU_MIP__BASE__IEEE_754_DOUBLE_PRECISION)
  return *reinterpret_cast<const float*>(&value);
#else
  return ToFloat(value);
#endif // float
}

inline FloatingPoint::IEEE754DoublePrecision::operator double() const throw() {
#if (_DK_SDU_MIP__BASE__DOUBLE == _DK_SDU_MIP__BASE__IEEE_754_DOUBLE_PRECISION)
  return *reinterpret_cast<const double*>(&value);
#else
  return ToDouble(value);
#endif // double
}

inline FloatingPoint::IEEE754DoublePrecision::operator long double() const throw() {
#if (_DK_SDU_MIP__BASE__LONG_DOUBLE == _DK_SDU_MIP__BASE__IEEE_754_DOUBLE_PRECISION)
  return *reinterpret_cast<const long double*>(&value);
#else
  return ToLongDouble(value);
#endif // long double
}

inline FloatingPoint::IEEEExtendedDoublePrecision96::operator float() const throw() {
#if (_DK_SDU_MIP__BASE__FLOAT == _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96)
  return *reinterpret_cast<const float*>(&value);
#else
  return ToFloat(value);
#endif // float
}

inline FloatingPoint::IEEEExtendedDoublePrecision96::operator double() const throw() {
#if (_DK_SDU_MIP__BASE__DOUBLE == _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96)
  return *reinterpret_cast<const double*>(&value);
#else
  return ToDouble(value);
#endif // double
}
  
inline FloatingPoint::IEEEExtendedDoublePrecision96::operator long double() const throw() {
#if (_DK_SDU_MIP__BASE__LONG_DOUBLE == _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96)
  return *reinterpret_cast<const long double*>(&value);
#else
  return ToLongDouble(value);
#endif // long double
}

inline FloatingPoint::IEEEExtendedDoublePrecision128::operator float() const throw() {
#if (_DK_SDU_MIP__BASE__FLOAT == _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128)
  return *reinterpret_cast<const float*>(&value);
#else
  return ToFloat(value);
#endif // float
}

inline FloatingPoint::IEEEExtendedDoublePrecision128::operator double() const throw() {
#if (_DK_SDU_MIP__BASE__DOUBLE == _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128)
  return *reinterpret_cast<const double*>(&value);
#else
  return ToDouble(value);
#endif // double
}

inline FloatingPoint::IEEEExtendedDoublePrecision128::operator long double() const throw() {
#if (_DK_SDU_MIP__BASE__LONG_DOUBLE == _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128)
  return *reinterpret_cast<const long double*>(&value);
#else
  return ToLongDouble(value);
#endif // long double
}

inline FloatingPoint::IEEEQuadruplePrecision::operator float() const throw() {
#if (_DK_SDU_MIP__BASE__FLOAT == _DK_SDU_MIP__BASE__IEEE_QUADRUPLE_PRECISION)
  return *reinterpret_cast<const float*>(&value);
#else
  return ToFloat(value);
#endif // float
}

inline FloatingPoint::IEEEQuadruplePrecision::operator double() const throw() {
#if (_DK_SDU_MIP__BASE__DOUBLE == _DK_SDU_MIP__BASE__IEEE_QUADRUPLE_PRECISION)
  return *reinterpret_cast<const double*>(&value);
#else
  return ToDouble(value);
#endif // double
}
  
inline FloatingPoint::IEEEQuadruplePrecision::operator long double() const throw() {
#if (_DK_SDU_MIP__BASE__LONG_DOUBLE == _DK_SDU_MIP__BASE__IEEE_QUADRUPLE_PRECISION)
  return *reinterpret_cast<const long double*>(&value);
#else
  return ToLongDouble(value);
#endif // long double
}

/**
  Analyses the specified floating point number of a given representation.
  
  @param value The floating point number.
  @param significant The number of significant digits in the mantissa (OUT).
  @param mantissa The array to receive the mantissa of the number (OUT). The
  size of the array must be (TYPE::SIGNIFICANT+31/32).
  @param exponent The exponent (base 2) of the number (OUT).
  @param flags Flags specifying the number type (OUT).
*/
template<typename TYPE>
inline void analyseFloatingPoint(
  const TYPE& value,
  unsigned int& precision,
  unsigned int* mantissa,
  int& exponent,
  unsigned int& flags) throw() {
}

template<>
void analyseFloatingPoint<FloatingPoint::Representation::IEEE754SinglePrecision>(
  const FloatingPoint::Representation::IEEE754SinglePrecision& value,
  unsigned int& precision,
  unsigned int* mantissa,
  int& exponent,
  unsigned int& flags) throw();

template<>
void analyseFloatingPoint<FloatingPoint::Representation::IEEE754DoublePrecision>(
  const FloatingPoint::Representation::IEEE754DoublePrecision& value,
  unsigned int& precision,
  unsigned int* mantissa,
  int& exponent,
  unsigned int& flags) throw();

template<>
void analyseFloatingPoint<FloatingPoint::Representation::IEEEExtendedDoublePrecision96>(
  const FloatingPoint::Representation::IEEEExtendedDoublePrecision96& value,
  unsigned int& precision,
  unsigned int* mantissa,
  int& exponent,
  unsigned int& flags) throw();

template<>
void analyseFloatingPoint<FloatingPoint::Representation::IEEEExtendedDoublePrecision128>(
  const FloatingPoint::Representation::IEEEExtendedDoublePrecision128& value,
  unsigned int& precision,
  unsigned int* mantissa,
  int& exponent,
  unsigned int& flags) throw();

template<>
void analyseFloatingPoint<FloatingPoint::Representation::IEEEQuadruplePrecision>(
  const FloatingPoint::Representation::IEEEQuadruplePrecision& value,
  unsigned int& precision,
  unsigned int* mantissa,
  int& exponent,
  unsigned int& flags) throw();

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
