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
#include <base/string/InvalidFormat.h>
#include <base/string/String.h>

// TAG: what about rounding

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This class provides support for floating point types.
  
  @short Floating point type support.
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API FloatingPoint {
public:

  /**
    Specifies the maximum explicit exponent for string representations in
    decimal format.
  */
  static constexpr unsigned int MAXIMUM_DECIMAL_EXPONENT = 9999;
  
  /**
    Specifies the maximum explicit exponent for string representations in
    hexadecimal format.
  */
  static constexpr unsigned int MAXIMUM_BINARY_EXPONENT = 9999;
  
  /**
    Converts hex representation to a float.
  */
  float getFloatAsHex(const String& value);

  /**
    Converts the string to a float.
  */
  static float getFloat(const String& value);
  
  /**
    Converts the string to a double.
  */
  static double getDouble(const String& value);
  
  /**
    Converts the string to a long double.
  */
  static long double getLongDouble(const String& value);
  
  /** Floating point flags. */
  enum Flags {
    /** Specifies a negative value. */
    FP_NEGATIVE = 1,
    /** Specifies a denormalized value. */
    FP_DENORMALIZED = 2,
    /** Specifies that the value is a quite NaN. */
    FP_QUITE_NAN = 4,
    /** Specifies that the value is a signaling NaN. */
    FP_SIGNALING_NAN = 8,
    /** Specifies that the value is a NaN. */
    FP_ANY_NAN = FP_QUITE_NAN | FP_SIGNALING_NAN,
    /** Value is zero (the sign is determined by the FP_NEGATIVE flag). */
    FP_ANY_ZERO = 16,
    /** Value is infinity (the sign is determined by the FP_NEGATIVE flag). */
    FP_INFINITY = 32,
    /** Specifies an ordinary value representable in [-]d.ddde[-]dd form. */
    FP_VALUE = 64
  };
  
  class Representation {
  public:
    
    /*
      Representation of single precision (32-bit) floating point type as
      specified by IEEE 754.
    */
_COM_AZURE_DEV__BASE__PACKED__BEGIN
    struct IEEE754SinglePrecision {
      static constexpr bool HAS_IMPLIED_ONE = true;
      static constexpr int MINIMUM_EXPONENT = -125;
      static constexpr int MAXIMUM_EXPONENT = 128;
      static constexpr int ALL_BITS_EXPONENT = (1 << 8) - 1;
      static constexpr int BIAS = 0x7f;
      static constexpr unsigned int SIGNIFICANT = 24;
#if ((_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__BIG_ENDIAN) && \
     (_COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER == _COM_AZURE_DEV__BASE__BIG_ENDIAN))
      uint32 negative : 1;
      uint32 exponent : 8;
      uint32 mantissa0 : 23;
#elif ((_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__BIG_ENDIAN) && \
       (_COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN))
      uint32 negative : 1;
      uint32 exponent : 8;
      uint32 mantissa0 : 23;
#elif ((_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN) && \
       (_COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN))
      uint32 mantissa0 : 23;
      uint32 exponent : 8;
      uint32 negative : 1;
#elif ((_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN) && \
       (_COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER == _COM_AZURE_DEV__BASE__BIG_ENDIAN))
      uint32 mantissa0 : 23;
      uint32 exponent : 8;
      uint32 negative : 1;
#endif // bit allocation
    } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

    /*
      Representation of double precision (64-bit) floating point type as
      specified by IEEE 754.
    */
_COM_AZURE_DEV__BASE__PACKED__BEGIN
    struct IEEE754DoublePrecision {
      static constexpr bool HAS_IMPLIED_ONE = true;
      static constexpr int MINIMUM_EXPONENT = -1021;
      static constexpr int MAXIMUM_EXPONENT = 1024;
      static constexpr int ALL_BITS_EXPONENT = (1 << 11) - 1;
      static constexpr int BIAS = 0x3ff;
      static constexpr unsigned int SIGNIFICANT = 53;
#if ((_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__BIG_ENDIAN) && \
     (_COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER == _COM_AZURE_DEV__BASE__BIG_ENDIAN))
      uint32 negative : 1;
      uint32 exponent : 11;
      uint32 mantissa1 : 20;
      uint32 mantissa0 : 32;
#elif ((_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__BIG_ENDIAN) && \
       (_COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN))
      uint32 mantissa0 : 32;
      uint32 negative : 1;
      uint32 exponent : 11;
      uint32 mantissa1 : 20;
#elif ((_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN) && \
       (_COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN))
      uint32 mantissa0 : 32;
      uint32 mantissa1 : 20;
      uint32 exponent : 11;
      uint32 negative : 1;
#elif ((_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN) && \
       (_COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER == _COM_AZURE_DEV__BASE__BIG_ENDIAN))
      uint32 mantissa1 : 20;
      uint32 exponent : 11;
      uint32 negative : 1;
      uint32 mantissa0 : 32;
#endif // bit allocation
    } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

    /*
      Representation of double-extended precision (96-bit) floating point type.
    */
_COM_AZURE_DEV__BASE__PACKED__BEGIN
    struct IEEEExtendedDoublePrecision96 {
      static constexpr bool HAS_IMPLIED_ONE = false;
      static constexpr int MINIMUM_EXPONENT = -16381;
      static constexpr int MAXIMUM_EXPONENT = 16384;
      static constexpr int ALL_BITS_EXPONENT = (1 << 15) - 1;
      static constexpr int BIAS = 0x3fff;
      static constexpr unsigned int SIGNIFICANT = 64;
#if ((_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__BIG_ENDIAN) && \
     (_COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER == _COM_AZURE_DEV__BASE__BIG_ENDIAN))
      uint32 negative : 1;
      uint32 exponent : 15;
      uint32 empty : 16;
      uint32 mantissa1 : 32;
      uint32 mantissa0 : 32;
#elif ((_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__BIG_ENDIAN) && \
       (_COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN))
      uint32 mantissa0 : 32;
      uint32 mantissa1 : 32;
      uint32 negative : 1;
      uint32 exponent : 15;
      uint32 empty : 16;
#elif ((_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN) && \
       (_COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN))
      uint32 mantissa0 : 32;
      uint32 mantissa1 : 32;
      uint32 exponent : 15;
      uint32 negative : 1;
      uint32 empty : 16;
#elif ((_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN) && \
       (_COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER == _COM_AZURE_DEV__BASE__BIG_ENDIAN))
      uint32 exponent : 15;
      uint32 negative : 1;
      uint32 empty : 16;
      uint32 mantissa1 : 32;
      uint32 mantissa0 : 32;
#endif // bit allocation
    } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

    /*
      Representation of double-extended precision (96-bit) floating point type. 16 bytes alignment.
    */
_COM_AZURE_DEV__BASE__PACKED__BEGIN
    struct IEEEExtendedDoublePrecision96Align16 {
      static constexpr bool HAS_IMPLIED_ONE = false;
      static constexpr int MINIMUM_EXPONENT = -16381;
      static constexpr int MAXIMUM_EXPONENT = 16384;
      static constexpr int ALL_BITS_EXPONENT = (1 << 15) - 1;
      static constexpr int BIAS = 0x3fff;
      static constexpr unsigned int SIGNIFICANT = 64;
#if ((_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__BIG_ENDIAN) && \
     (_COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER == _COM_AZURE_DEV__BASE__BIG_ENDIAN))
      uint32 unused : 32;
      uint32 negative : 1;
      uint32 exponent : 15;
      uint32 empty : 16;
      uint32 mantissa1 : 32;
      uint32 mantissa0 : 32;
#elif ((_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__BIG_ENDIAN) && \
       (_COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN))
      uint32 mantissa0 : 32;
      uint32 mantissa1 : 32;
      uint32 negative : 1;
      uint32 exponent : 15;
      uint32 empty : 16;
      uint32 unused : 32;
#elif ((_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN) && \
       (_COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN))
      uint32 mantissa0 : 32;
      uint32 mantissa1 : 32;
      uint32 exponent : 15;
      uint32 negative : 1;
      uint32 empty : 16;
      uint32 unused : 32;
#elif ((_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN) && \
       (_COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER == _COM_AZURE_DEV__BASE__BIG_ENDIAN))
      uint32 unused : 32;
      uint32 exponent : 15;
      uint32 negative : 1;
      uint32 empty : 16;
      uint32 mantissa1 : 32;
      uint32 mantissa0 : 32;
#endif // bit allocation
    } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END
    
    /*
      Representation of double-extended precision (128-bit) floating point
      type.
    */
_COM_AZURE_DEV__BASE__PACKED__BEGIN
    struct IEEEExtendedDoublePrecision128 {
      static constexpr bool HAS_IMPLIED_ONE = false;
      static constexpr int MINIMUM_EXPONENT = -16381;
      static constexpr int MAXIMUM_EXPONENT = 16384;
      static constexpr int ALL_BITS_EXPONENT = (1 << 15) - 1;
      static constexpr int BIAS = 0x3fff;
      static constexpr unsigned int SIGNIFICANT = 64;
#if ((_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__BIG_ENDIAN) && \
     (_COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER == _COM_AZURE_DEV__BASE__BIG_ENDIAN))
      uint32 empty1 : 32;
      uint32 negative : 1;
      uint32 exponent : 15;
      uint32 empty0 : 16;
      uint32 mantissa1 : 32;
      uint32 mantissa0 : 32;
#elif ((_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__BIG_ENDIAN) && \
       (_COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN))
      uint32 mantissa0 : 32;
      uint32 mantissa1 : 32;
      uint32 negative : 1;
      uint32 exponent : 15;
      uint32 empty0 : 16;
      uint32 empty1 : 32;
#elif ((_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN) && \
       (_COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN))
      uint32 mantissa0 : 32;
      uint32 mantissa1 : 32;
      uint32 exponent : 15;
      uint32 negative : 1;
      uint32 empty0 : 16;
      uint32 empty1 : 32;
#elif ((_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN) && \
       (_COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER == _COM_AZURE_DEV__BASE__BIG_ENDIAN))
      uint32 empty1 : 32;
      uint32 exponent : 15;
      uint32 negative : 1;
      uint32 empty0 : 16;
      uint32 mantissa1 : 32;
      uint32 mantissa0 : 32;
#endif // bit allocation
    } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

    /* Representation of quadruple precision (128-bit) floating point type. */
_COM_AZURE_DEV__BASE__PACKED__BEGIN
    struct IEEEQuadruplePrecision {
      static constexpr bool HAS_IMPLIED_ONE = true;
      static constexpr int MINIMUM_EXPONENT = -16381;
      static constexpr int MAXIMUM_EXPONENT = 16384;
      static constexpr int ALL_BITS_EXPONENT = (1 << 15) - 1;
      static constexpr int BIAS = 0x3fff;
      static constexpr unsigned int SIGNIFICANT = 113;
#if ((_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__BIG_ENDIAN) && \
     (_COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER == _COM_AZURE_DEV__BASE__BIG_ENDIAN))
      uint32 negative : 1;
      uint32 exponent : 15;
      uint32 mantissa3 : 16;
      uint32 mantissa2 : 32;
      uint32 mantissa1 : 32;
      uint32 mantissa0 : 32;
#elif ((_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__BIG_ENDIAN) && \
       (_COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN))
      uint32 mantissa0 : 32;
      uint32 mantissa1 : 32;
      uint32 mantissa2 : 32;
      uint32 negative : 1;
      uint32 exponent : 15;
      uint32 mantissa3 : 16;
#elif ((_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN) && \
       (_COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN))
      uint32 mantissa0 : 32;
      uint32 mantissa1 : 32;
      uint32 mantissa2 : 32;
      uint32 mantissa3 : 16;
      uint32 exponent : 15;
      uint32 negative : 1;
#elif ((_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN) && \
       (_COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER == _COM_AZURE_DEV__BASE__BIG_ENDIAN))
      uint32 mantissa3 : 16;
      uint32 exponent : 15;
      uint32 negative : 1;
      uint32 mantissa2 : 32;
      uint32 mantissa1 : 32;
      uint32 mantissa0 : 32;
#endif // bit allocation
    } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

    /* Representation of IBM extended precision (128-bit) floating point type. */
_COM_AZURE_DEV__BASE__PACKED__BEGIN
    struct IBMExtendedPrecision { // see https://gcc.gnu.org/wiki/Ieee128PowerPC // ATTENTION: there are different IBM formats!
      static constexpr bool HAS_IMPLIED_ONE = true;
      static constexpr int MINIMUM_EXPONENT = -1021;
      static constexpr int MAXIMUM_EXPONENT = 1024;
      static constexpr int ALL_BITS_EXPONENT = (1 << 11) - 1;
      static constexpr int BIAS = (1 << 10) - 1;
      static constexpr unsigned int SIGNIFICANT = 53 * 2;
#if (1 || \
     (_COM_AZURE_DEV__BASE__BYTE_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN) && \
     (_COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER == _COM_AZURE_DEV__BASE__LITTLE_ENDIAN))
      uint32 mantissa0 : 32;
      uint32 mantissa1 : 20;
      uint32 exponent : 11;
      uint32 negative : 1;
      uint32 mantissa2_0 : 32;
      uint32 mantissa2_1 : 20;
      uint32 exponent2 : 11;
      uint32 negative2 : 1;
#else
#  error Unsupported IBM extended precision
#endif // bit allocation
    } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END
  };

#if (_COM_AZURE_DEV__BASE__FLOAT == _COM_AZURE_DEV__BASE__IEEE_754_SINGLE_PRECISION)
  typedef Representation::IEEE754SinglePrecision FloatRepresentation;
#elif (_COM_AZURE_DEV__BASE__FLOAT == _COM_AZURE_DEV__BASE__IEEE_754_DOUBLE_PRECISION)
  typedef Representation::IEEE754DoublePrecision FloatRepresentation;
#elif (_COM_AZURE_DEV__BASE__FLOAT == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96)
  typedef Representation::IEEEExtendedDoublePrecision96 FloatRepresentation;
#elif (_COM_AZURE_DEV__BASE__FLOAT == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16)
  typedef Representation::IEEEExtendedDoublePrecision96Align16 FloatRepresentation;
#elif (_COM_AZURE_DEV__BASE__FLOAT == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128)
  typedef Representation::IEEEExtendedDoublePrecision128 FloatRepresentation;
#elif (_COM_AZURE_DEV__BASE__FLOAT == _COM_AZURE_DEV__BASE__IEEE_QUADRUPLE_PRECISION)
  typedef Representation::IEEEQuadruplePrecision FloatRepresentation;
#elif (_COM_AZURE_DEV__BASE__FLOAT == _COM_AZURE_DEV__BASE__IBM_EXTENDED_PRECISION)
  typedef Representation::IBMExtendedPrecision FloatRepresentation;
#else
#  error Invalid floating-point representation of type float
#endif

#if (_COM_AZURE_DEV__BASE__DOUBLE == _COM_AZURE_DEV__BASE__IEEE_754_SINGLE_PRECISION)
  typedef Representation::IEEE754SinglePrecision DoubleRepresentation;
#elif (_COM_AZURE_DEV__BASE__DOUBLE == _COM_AZURE_DEV__BASE__IEEE_754_DOUBLE_PRECISION)
  typedef Representation::IEEE754DoublePrecision DoubleRepresentation;
#elif (_COM_AZURE_DEV__BASE__DOUBLE == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96)
  typedef Representation::IEEEExtendedDoublePrecision96 DoubleRepresentation;
#elif (_COM_AZURE_DEV__BASE__DOUBLE == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16)
  typedef Representation::IEEEExtendedDoublePrecision96Align16 DoubleRepresentation;
#elif (_COM_AZURE_DEV__BASE__DOUBLE == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128)
  typedef Representation::IEEEExtendedDoublePrecision128 DoubleRepresentation;
#elif (_COM_AZURE_DEV__BASE__DOUBLE == _COM_AZURE_DEV__BASE__IEEE_QUADRUPLE_PRECISION)
  typedef Representation::IEEEQuadruplePrecision DoubleRepresentation;
#elif (_COM_AZURE_DEV__BASE__DOUBLE == _COM_AZURE_DEV__BASE__IBM_EXTENDED_PRECISION)
  typedef Representation::IBMExtendedPrecision DoubleRepresentation;
#else
#  error Invalid floating-point representation of type double
#endif

#if (_COM_AZURE_DEV__BASE__LONG_DOUBLE == _COM_AZURE_DEV__BASE__IEEE_754_SINGLE_PRECISION)
  typedef Representation::IEEE754SinglePrecision LongDoubleRepresentation;
#elif (_COM_AZURE_DEV__BASE__LONG_DOUBLE == _COM_AZURE_DEV__BASE__IEEE_754_DOUBLE_PRECISION)
  typedef Representation::IEEE754DoublePrecision LongDoubleRepresentation;
#elif (_COM_AZURE_DEV__BASE__LONG_DOUBLE == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96)
  typedef Representation::IEEEExtendedDoublePrecision96 LongDoubleRepresentation;
#elif (_COM_AZURE_DEV__BASE__LONG_DOUBLE == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16)
  typedef Representation::IEEEExtendedDoublePrecision96Align16 LongDoubleRepresentation;
#elif (_COM_AZURE_DEV__BASE__LONG_DOUBLE == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128)
  typedef Representation::IEEEExtendedDoublePrecision128 LongDoubleRepresentation;
#elif (_COM_AZURE_DEV__BASE__LONG_DOUBLE == _COM_AZURE_DEV__BASE__IEEE_QUADRUPLE_PRECISION)
  typedef Representation::IEEEQuadruplePrecision LongDoubleRepresentation;
#elif (_COM_AZURE_DEV__BASE__LONG_DOUBLE == _COM_AZURE_DEV__BASE__IBM_EXTENDED_PRECISION)
  typedef Representation::IBMExtendedPrecision LongDoubleRepresentation;
#else
#  error Invalid floating-point representation of type long double
#endif

#if (_COM_AZURE_DEV__BASE__FLOAT128 == _COM_AZURE_DEV__BASE__IEEE_754_SINGLE_PRECISION)
  typedef Representation::IEEE754SinglePrecision Float128Representation;
#elif (_COM_AZURE_DEV__BASE__FLOAT128 == _COM_AZURE_DEV__BASE__IEEE_754_DOUBLE_PRECISION)
  typedef Representation::IEEE754DoublePrecision Float128Representation;
#elif (_COM_AZURE_DEV__BASE__FLOAT128 == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96)
  typedef Representation::IEEEExtendedDoublePrecision96 Float128Representation;
#elif (_COM_AZURE_DEV__BASE__FLOAT128 == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16)
  typedef Representation::IEEEExtendedDoublePrecision96Align16 Float128Representation;
#elif (_COM_AZURE_DEV__BASE__FLOAT128 == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128)
  typedef Representation::IEEEExtendedDoublePrecision128 Float128Representation;
#elif (_COM_AZURE_DEV__BASE__FLOAT128 == _COM_AZURE_DEV__BASE__IEEE_QUADRUPLE_PRECISION)
  typedef Representation::IEEEQuadruplePrecision Float128Representation;
#elif (_COM_AZURE_DEV__BASE__FLOAT128 == _COM_AZURE_DEV__BASE__IBM_EXTENDED_PRECISION)
  typedef Representation::IBMExtendedPrecision Float128Representation;
#else
#  error Invalid floating-point representation of type float128
#endif

  /** @short IEEE 754 single precision conversion support. */
_COM_AZURE_DEV__BASE__PACKED__BEGIN
  struct IEEE754SinglePrecision {
    Representation::IEEE754SinglePrecision value;
    
    inline void setValue(const Representation::IEEE754SinglePrecision& value) noexcept
    {
      this->value = value;
    }
    
    inline void setValue(const Representation::IEEE754DoublePrecision& _value) noexcept
    {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent; // possible overflow and underflow
      // both representations have implied one
      unsigned int mantissa =
        (static_cast<uint32>(_value.mantissa1) << 3) | (_value.mantissa0 >> 29);
      value.mantissa0 = mantissa;
    }
    
    void setValue(const Representation::IEEEExtendedDoublePrecision96& value) noexcept;

    void setValue(const Representation::IEEEExtendedDoublePrecision96Align16& value) noexcept;

    void setValue(const Representation::IEEEExtendedDoublePrecision128& value) noexcept;
    
    inline void setValue(const Representation::IEEEQuadruplePrecision& _value) noexcept
    {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent; // possible overflow and underflow
      // both representations have implied one
      value.mantissa0 = (static_cast<uint32>(_value.mantissa3) << (23 - 16)) |
        (_value.mantissa2 >> (32 - 23 + 16 + 1));
    }
    
    void setValue(const Representation::IBMExtendedPrecision& value) noexcept;

    inline void setValue(float value) noexcept
    {
      setValue(*reinterpret_cast<const FloatRepresentation*>(&value));
    }
    
    inline void setValue(double value) noexcept
    {
      setValue(*reinterpret_cast<const DoubleRepresentation*>(&value));
    }
    
    inline void setValue(long double value) noexcept
    {
      setValue(*reinterpret_cast<const LongDoubleRepresentation*>(&value));
    }

    inline void setValue(float128 value) noexcept
    {
      setValue(*reinterpret_cast<const Float128Representation*>(&value));
    }

    inline IEEE754SinglePrecision() noexcept
    {
      clear(value);
    }

    /**
      Initializes representation from floating point components.
      
      @param negative True if value is negative.
      @param mantissa The mantissa hexadecimal values (i.e. 0-15).
      @param size The number of mantissa digits.
      @param exponent The exponent (power of 2).
    */
    IEEE754SinglePrecision(bool negative, const uint8* mantissa, unsigned int size, int exponent);
    
    inline IEEE754SinglePrecision(const Representation::IEEE754SinglePrecision& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEE754SinglePrecision(const Representation::IEEE754DoublePrecision& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEE754SinglePrecision(const Representation::IEEEExtendedDoublePrecision96& value) noexcept
    {
      setValue(value);
    }

    inline IEEE754SinglePrecision(const Representation::IEEEExtendedDoublePrecision96Align16& value) noexcept
    {
      setValue(value);
    }

    inline IEEE754SinglePrecision(const Representation::IEEEExtendedDoublePrecision128& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEE754SinglePrecision(const Representation::IEEEQuadruplePrecision& value) noexcept
    {
      setValue(value);
    }

    inline IEEE754SinglePrecision(const Representation::IBMExtendedPrecision& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEE754SinglePrecision(float value) noexcept
    {
      setValue(*reinterpret_cast<const FloatRepresentation*>(&value));
    }
    
    inline IEEE754SinglePrecision(double value) noexcept
    {
      setValue(*reinterpret_cast<const DoubleRepresentation*>(&value));
    }
    
    inline IEEE754SinglePrecision(long double value) noexcept
    {
      setValue(*reinterpret_cast<const LongDoubleRepresentation*>(&value));
    }

    inline IEEE754SinglePrecision(float128 value) noexcept
    {
      setValue(*reinterpret_cast<const Float128Representation*>(&value));
    }

    /**
      Returns true if the value is negative.
    */
    inline bool isNegative() const noexcept
    {
      return value.negative != 0;
    }
    
    /**
      Returns true if the value is an ordinary number (not infinity or NaN).
    */
    inline bool isOrdinary() const noexcept
    {
      return value.exponent != value.ALL_BITS_EXPONENT;
    }
    
    /**
      Returns true if the value is either +INFINITY or -INFINITY.
    */
    inline bool isInfinity() const noexcept
    {
      return (value.exponent == value.ALL_BITS_EXPONENT) && (value.mantissa0 == 0);
    }
    
    /**
      Returns true if the value is Not-a-Number (NaN).
    */
    inline bool isNaN() const noexcept
    {
      return (value.exponent == value.ALL_BITS_EXPONENT) && (value.mantissa0 != 0);
    }
    
    /**
      Returns true if the value is a quite NaN. A NaN that behaves predictably
      and does not raise exceptions in arithmetic operations is called a quiet
      NaN.
    */
    inline bool isQuiteNaN() const noexcept
    {
      return (value.exponent == value.ALL_BITS_EXPONENT) &&
        ((value.mantissa0 & (1 << (23 - 1))) != 0);
    }
    
    /**
      Returns true if the value is a signaling NaN. A NaN that generally raises
      an exception when encountered as an operand of arithmetic operations is
      called a signaling NaN.
    */
    inline bool isSignalingNaN() const noexcept
    {
      return (value.exponent == value.ALL_BITS_EXPONENT) &&
        ((value.mantissa0 & (1 << (23 - 1))) == 0) && (value.mantissa0 != 0); // but not infitity
    }

    /** Returns the exponent. */
    inline int getExponent() const noexcept
    {
      BASSERT(isOrdinary());
      BASSERT(static_cast<int>(value.exponent - value.BIAS) <= value.MAXIMUM_EXPONENT);
      BASSERT(static_cast<int>(value.exponent - value.BIAS) >= value.MINIMUM_EXPONENT);
      return value.exponent - value.BIAS;
    }

    /** Sets the exponent. */
    inline void setExponent(int exponent) noexcept
    {
      BASSERT(exponent <= value.MAXIMUM_EXPONENT);
      BASSERT(exponent >= value.MINIMUM_EXPONENT);
      value.exponent = exponent + value.BIAS;
    }
    
    /** Returns true if supported exponent. Ignoring denomalized values. */
    static inline bool isSupportedExponent(int exponent) noexcept
    {
      return (exponent >= Representation::IEEE754SinglePrecision::MINIMUM_EXPONENT) &&
        (exponent <= Representation::IEEE754SinglePrecision::MAXIMUM_EXPONENT);
    }

    operator float() const noexcept;
    
    operator double() const noexcept;
    
    operator long double() const noexcept;

    operator float128() const noexcept;
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

  /** @short IEEE 754 double precision conversion support. */
_COM_AZURE_DEV__BASE__PACKED__BEGIN
  struct IEEE754DoublePrecision {
    Representation::IEEE754DoublePrecision value;

    inline void setValue(const Representation::IEEE754SinglePrecision& _value) noexcept
    {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent; // possible overflow and underflow
      // both representations have implied one
      value.mantissa1 = _value.mantissa0 >> (23 - 20);
      value.mantissa0 = static_cast<uint32>(_value.mantissa0) << (32 - (23 - 20));
    }
    
    inline void setValue(const Representation::IEEE754DoublePrecision& value) noexcept
    {
      this->value = value;
    }
    
    void setValue(const Representation::IEEEExtendedDoublePrecision96& value) noexcept;
    
    void setValue(const Representation::IEEEExtendedDoublePrecision96Align16& value) noexcept;
    
    void setValue(const Representation::IEEEExtendedDoublePrecision128& value) noexcept;
    
    inline void setValue(const Representation::IEEEQuadruplePrecision& _value) noexcept
    {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent; // possible overflow and underflow
      // both representations have implied one
      value.mantissa1 = (static_cast<uint32>(_value.mantissa3) << (20 - 16)) |
        (_value.mantissa2 >> (32 - (20 - 16)));
      value.mantissa0 = (static_cast<uint32>(_value.mantissa2) << (32 - (20 - 16))) |
        (_value.mantissa1 >> (32 - (20 - 16)));
    }

    void setValue(const Representation::IBMExtendedPrecision& value) noexcept;

    inline void setValue(float value) noexcept
    {
      setValue(*reinterpret_cast<const FloatRepresentation*>(&value));
    }
    
    inline void setValue(double value) noexcept
    {
      setValue(*reinterpret_cast<const DoubleRepresentation*>(&value));
    }
    
    inline void setValue(long double value) noexcept
    {
      setValue(*reinterpret_cast<const LongDoubleRepresentation*>(&value));
    }

    inline void setValue(float128 value) noexcept
    {
      setValue(*reinterpret_cast<const Float128Representation*>(&value));
    }

    inline IEEE754DoublePrecision() noexcept
    {
      clear(value);
    }
    
    inline IEEE754DoublePrecision(const Representation::IEEE754SinglePrecision& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEE754DoublePrecision(const Representation::IEEE754DoublePrecision& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEE754DoublePrecision(const Representation::IEEEExtendedDoublePrecision96& value) noexcept
    {
      setValue(value);
    }

    inline IEEE754DoublePrecision(const Representation::IEEEExtendedDoublePrecision96Align16& value) noexcept
    {
      setValue(value);
    }

    inline IEEE754DoublePrecision(const Representation::IEEEExtendedDoublePrecision128& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEE754DoublePrecision(const Representation::IEEEQuadruplePrecision& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEE754DoublePrecision(const Representation::IBMExtendedPrecision& value) noexcept
    {
      setValue(value);
    }

    inline IEEE754DoublePrecision(float value) noexcept
    {
      setValue(*reinterpret_cast<const FloatRepresentation*>(&value));
    }
    
    inline IEEE754DoublePrecision(double value) noexcept
    {
      setValue(*reinterpret_cast<const DoubleRepresentation*>(&value));
    }
    
    inline IEEE754DoublePrecision(long double value) noexcept
    {
      setValue(*reinterpret_cast<const LongDoubleRepresentation*>(&value));
    }

    inline IEEE754DoublePrecision(float128 value) noexcept
    {
      setValue(*reinterpret_cast<const Float128Representation*>(&value));
    }

    /**
      Returns true if the value is negative.
    */
    inline bool isNegative() const noexcept
    {
      return value.negative != 0;
    }
    
    /**
      Returns true if the value is an ordinary number (not infinity or NaN).
    */
    inline bool isOrdinary() const noexcept
    {
      return value.exponent != value.ALL_BITS_EXPONENT;
    }
    
    /**
      Returns true if the value is either +INFINITY or -INFINITY.
    */
    inline bool isInfinity() const noexcept
    {
      return (value.exponent == value.ALL_BITS_EXPONENT) && (value.mantissa1 == 0) &&
        (value.mantissa0 == 0);
    }
    
    /**
      Returns true if the value is Not-a-Number (NaN).
    */
    inline bool isNaN() const noexcept
    {
      return (value.exponent == value.ALL_BITS_EXPONENT) &&
        ((value.mantissa1 != 0) || (value.mantissa0 != 0));
    }
    
    /**
      Returns true if the value is a quite NaN. A NaN that behaves predictably
      and does not raise exceptions in arithmetic operations is called a quiet
      NaN.
    */
    inline bool isQuiteNaN() const noexcept
    {
      return (value.exponent == value.ALL_BITS_EXPONENT) &&
        ((value.mantissa1 & (1 << (20 - 1))) != 0);
    }
    
    /**
      Returns true if the value is a signaling NaN. A NaN that generally raises
      an exception when encountered as an operand of arithmetic operations is
      called a signaling NaN.
    */
    inline bool isSignalingNaN() const noexcept
    {
      return (value.exponent == value.ALL_BITS_EXPONENT) &&
        ((value.mantissa1 & (1 << (20 - 1))) == 0) && // high bit not set
        ((value.mantissa1 != 0) || (value.mantissa0 != 0)); // but not infitity
    }
    
    /** Returns the exponent. */
    inline int getExponent() const noexcept
    {
      BASSERT(isOrdinary());
      BASSERT(static_cast<int>(value.exponent - value.BIAS) <= value.MAXIMUM_EXPONENT);
      BASSERT(static_cast<int>(value.exponent - value.BIAS) >= value.MINIMUM_EXPONENT);
      return value.exponent - value.BIAS;
    }

    /** Sets the exponent. */
    inline void setExponent(int exponent) noexcept
    {
      BASSERT(exponent <= value.MAXIMUM_EXPONENT);
      BASSERT(exponent >= value.MINIMUM_EXPONENT);
      value.exponent = exponent + value.BIAS;
    }
    
    /** Returns true if supported exponent. Ignoring denomalized values. */
    static inline bool isSupportedExponent(int exponent) noexcept
    {
      return (exponent >= Representation::IEEE754DoublePrecision::MINIMUM_EXPONENT) &&
        (exponent <= Representation::IEEE754DoublePrecision::MAXIMUM_EXPONENT);
    }

    operator float() const noexcept;
    
    operator double() const noexcept;
    
    operator long double() const noexcept;

    operator float128() const noexcept;
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

  /** @short IEEE extended double precision (96 bit) conversion support. 16 byte alignment. */
_COM_AZURE_DEV__BASE__PACKED__BEGIN
  struct IEEEExtendedDoublePrecision96Align16 {
    Representation::IEEEExtendedDoublePrecision96Align16 value;

    inline void setValue(const Representation::IEEE754SinglePrecision& _value) noexcept
    {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent;
      value.mantissa1 = (1 << 31) | (static_cast<uint32>(_value.mantissa0) << (32 - 23 - 1));
      value.mantissa0 = 0;
    }
    
    inline void setValue(const Representation::IEEE754DoublePrecision& _value) noexcept
    {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent;
      value.mantissa1 = (1 << 31) | (static_cast<uint32>(_value.mantissa1) << (32 - 20 - 1)) |
        (_value.mantissa0 >> (32 - 20 - 1));
      value.mantissa0 = static_cast<uint32>(_value.mantissa0) << (32 - 20 - 1);
    }
    
    inline void setValue(const Representation::IEEEExtendedDoublePrecision96& _value) noexcept
    {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent;
      value.mantissa1 = _value.mantissa1;
      value.mantissa0 = _value.mantissa0;
    }

    inline void setValue(const Representation::IEEEExtendedDoublePrecision96Align16& value) noexcept
    {
      this->value = value;
    }
    
    inline void setValue(const Representation::IEEEExtendedDoublePrecision128& _value) noexcept
    {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent;
      value.mantissa1 = _value.mantissa1;
      value.mantissa0 = _value.mantissa0;
    }
    
    inline void setValue(const Representation::IEEEQuadruplePrecision& _value) noexcept
    {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent;
      value.mantissa1 = (1 << 31) | (static_cast<uint32>(_value.mantissa3) << (32 - 16 - 1)) |
        (_value.mantissa2 >> (32 - (32 - 16 - 1)));
      value.mantissa0 = (static_cast<uint32>(_value.mantissa2) << (32 - (32 - 16 - 1))) |
        (_value.mantissa1 >> (32 - (32 - 16 - 1)));
    }

    void setValue(const Representation::IBMExtendedPrecision& value) noexcept;

    inline void setValue(float value) noexcept
    {
      setValue(*reinterpret_cast<const FloatRepresentation*>(&value));
    }
    
    inline void setValue(double value) noexcept
    {
      setValue(*reinterpret_cast<const DoubleRepresentation*>(&value));
    }
    
    inline void setValue(long double value) noexcept
    {
      setValue(*reinterpret_cast<const LongDoubleRepresentation*>(&value));
    }

    inline void setValue(float128 value) noexcept
    {
      setValue(*reinterpret_cast<const Float128Representation*>(&value));
    }

    inline IEEEExtendedDoublePrecision96Align16() noexcept
    {
      clear(value);
    }
    
    inline IEEEExtendedDoublePrecision96Align16(const Representation::IEEE754SinglePrecision& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision96Align16(const Representation::IEEE754DoublePrecision& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision96Align16(const Representation::IEEEExtendedDoublePrecision96& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision96Align16(const Representation::IEEEExtendedDoublePrecision96Align16& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision96Align16(const Representation::IEEEExtendedDoublePrecision128& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision96Align16(const Representation::IEEEQuadruplePrecision& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision96Align16(const Representation::IBMExtendedPrecision& value) noexcept
    {
      setValue(value);
    }

    inline IEEEExtendedDoublePrecision96Align16(float value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision96Align16(double value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision96Align16(long double value) noexcept
    {
      setValue(value);
    }

    inline IEEEExtendedDoublePrecision96Align16(float128 value) noexcept
    {
      setValue(value);
    }

    /**
      Returns true if the value is negative.
    */
    inline bool isNegative() const noexcept
    {
      return value.negative != 0;
    }
    
    /**
      Returns true if the value is an ordinary number (not infinity or NaN).
    */
    inline bool isOrdinary() const noexcept
    {
      return value.exponent != value.ALL_BITS_EXPONENT;
    }
    
    /**
      Returns true if the value is either +INFINITY or -INFINITY.
    */
    inline bool isInfinity() const noexcept
    {
#if 1 // temporary workaround until we have x86 representation
      constexpr uint32 HIGH = 1 << (32 - 1);
      return (value.exponent == value.ALL_BITS_EXPONENT) &&
        (((value.mantissa1 & ~HIGH) == 0) && (value.mantissa0 == 0));
#else
      return (value.exponent == value.ALL_BITS_EXPONENT) && (value.mantissa1 == 0) &&
        (value.mantissa0 == 0);
#endif
    }
    
    /**
      Returns true if the value is Not-a-Number (NaN).
    */
    inline bool isNaN() const noexcept
    {
#if 1 // temporary workaround until we have x86 representation
      return (value.exponent == value.ALL_BITS_EXPONENT) &&
        !isInfinity();
#else
      return (value.exponent == value.ALL_BITS_EXPONENT) &&
        ((value.mantissa1 != 0) || (value.mantissa0 != 0));
#endif
    }
    
    /**
      Returns true if the value is a quite NaN. A NaN that behaves predictably
      and does not raise exceptions in arithmetic operations is called a quiet
      NaN.
    */
    inline bool isQuiteNaN() const noexcept
    {
#if 1 // temporary workaround until we have x86 representation
      return (value.exponent == value.ALL_BITS_EXPONENT) &&
        !isInfinity() && !isSignalingNaN();
#else
      return (value.exponent == value.ALL_BITS_EXPONENT) &&
        ((value.mantissa1 & (1 << (32 - 1))) != 0);
#endif
    }
    
    /**
      Returns true if the value is a signaling NaN. A NaN that generally raises
      an exception when encountered as an operand of arithmetic operations is
      called a signaling NaN.
    */
    inline bool isSignalingNaN() const noexcept
    {
#if 1 // temporary workaround until we have x86 representation
      return (value.exponent == value.ALL_BITS_EXPONENT) &&
        (((value.mantissa1 & 0xc0000000) == 0x80000000) && ((value.mantissa1 != 0) || (value.mantissa0 != 0)));
#else
      return (value.exponent == value.ALL_BITS_EXPONENT) &&
        ((value.mantissa1 & (1 << (32 - 1))) == 0) &&
        ((value.mantissa1 != 0) || (value.mantissa0 != 0)); // but not infinity
#endif
    }

    /** Returns the exponent. */
    inline int getExponent() const noexcept
    {
      BASSERT(isOrdinary());
      BASSERT(static_cast<int>(value.exponent - value.BIAS) <= value.MAXIMUM_EXPONENT);
      BASSERT(static_cast<int>(value.exponent - value.BIAS) >= value.MINIMUM_EXPONENT);
      return value.exponent - value.BIAS;
    }

    /** Sets the exponent. */
    inline void setExponent(int exponent) noexcept
    {
      BASSERT(exponent <= value.MAXIMUM_EXPONENT);
      BASSERT(exponent >= value.MINIMUM_EXPONENT);
      value.exponent = exponent + value.BIAS;
    }
    
    /** Returns true if supported exponent. Ignoring denomalized values. */
    static inline bool isSupportedExponent(int exponent) noexcept
    {
      return (exponent >= Representation::IEEEExtendedDoublePrecision96Align16::MINIMUM_EXPONENT) &&
        (exponent <= Representation::IEEEExtendedDoublePrecision96Align16::MAXIMUM_EXPONENT);
    }

    operator float() const noexcept;
    
    operator double() const noexcept;
    
    operator long double() const noexcept;

    operator float128() const noexcept;
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

    /** @short IEEE extended double precision (96 bit) conversion support. */
_COM_AZURE_DEV__BASE__PACKED__BEGIN
  struct IEEEExtendedDoublePrecision96 {
    Representation::IEEEExtendedDoublePrecision96 value;
    
    inline void setValue(const Representation::IEEE754SinglePrecision& _value) noexcept
    {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent;
      value.mantissa1 = (1 << 31) | (static_cast<uint32>(_value.mantissa0) << (32 - 23 - 1));
      value.mantissa0 = 0;
    }
    
    inline void setValue(const Representation::IEEE754DoublePrecision& _value) noexcept
    {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent;
      value.mantissa1 = (1 << 31) | (static_cast<uint32>(_value.mantissa1) << (32 - 20 - 1)) |
        (_value.mantissa0 >> (32 - 20 - 1));
      value.mantissa0 = static_cast<uint32>(_value.mantissa0) << (32 - 20 - 1);
    }
    
    inline void setValue(const Representation::IEEEExtendedDoublePrecision96& value) noexcept
    {
      this->value = value;
    }

    inline void setValue(const Representation::IEEEExtendedDoublePrecision96Align16& _value) noexcept
    {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent;
      value.mantissa1 = _value.mantissa1;
      value.mantissa0 = _value.mantissa0;
    }
    
    inline void setValue(const Representation::IEEEExtendedDoublePrecision128& _value) noexcept
    {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent;
      value.mantissa1 = _value.mantissa1;
      value.mantissa0 = _value.mantissa0;
    }
    
    inline void setValue(const Representation::IEEEQuadruplePrecision& _value) noexcept
    {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent;
      value.mantissa1 = (1 << 31) | (static_cast<uint32>(_value.mantissa3) << (32 - 16 - 1)) |
        (_value.mantissa2 >> (32 - (32 - 16 - 1)));
      value.mantissa0 = (static_cast<uint32>(_value.mantissa2) << (32 - (32 - 16 - 1))) |
        (_value.mantissa1 >> (32 - (32 - 16 - 1)));
    }

    void setValue(const Representation::IBMExtendedPrecision& value) noexcept;

    inline void setValue(float value) noexcept
    {
      setValue(*reinterpret_cast<const FloatRepresentation*>(&value));
    }
    
    inline void setValue(double value) noexcept
    {
      setValue(*reinterpret_cast<const DoubleRepresentation*>(&value));
    }
    
    inline void setValue(long double value) noexcept
    {
      setValue(*reinterpret_cast<const LongDoubleRepresentation*>(&value));
    }

    inline void setValue(float128 value) noexcept
    {
      setValue(*reinterpret_cast<const Float128Representation*>(&value));
    }

    inline IEEEExtendedDoublePrecision96() noexcept
    {
      clear(value);
    }
    
    inline IEEEExtendedDoublePrecision96(const Representation::IEEE754SinglePrecision& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision96(const Representation::IEEE754DoublePrecision& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision96(const Representation::IEEEExtendedDoublePrecision96& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision96(const Representation::IEEEExtendedDoublePrecision96Align16& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision96(const Representation::IEEEExtendedDoublePrecision128& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision96(const Representation::IEEEQuadruplePrecision& value) noexcept
    {
      setValue(value);
    }

    inline IEEEExtendedDoublePrecision96(const Representation::IBMExtendedPrecision& value) noexcept
    {
      setValue(value);
    }

    inline IEEEExtendedDoublePrecision96(float value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision96(double value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision96(long double value) noexcept
    {
      setValue(value);
    }

    inline IEEEExtendedDoublePrecision96(float128 value) noexcept
    {
      setValue(value);
    }

    /**
      Returns true if the value is negative.
    */
    inline bool isNegative() const noexcept
    {
      return value.negative != 0;
    }
    
    /**
      Returns true if the value is an ordinary number (not infinity or NaN).
    */
    inline bool isOrdinary() const noexcept
    {
      return value.exponent != value.ALL_BITS_EXPONENT;
    }
    
    /**
      Returns true if the value is either +INFINITY or -INFINITY.
    */
    inline bool isInfinity() const noexcept
    {
#if 1 // temporary workaround until we have x86 representation
      constexpr uint32 HIGH = 1 << (32 - 1);
      return (value.exponent == value.ALL_BITS_EXPONENT) &&
        (((value.mantissa1 & ~HIGH) == 0) && (value.mantissa0 == 0));
#else
      return (value.exponent == value.ALL_BITS_EXPONENT) && (value.mantissa1 == 0) &&
        (value.mantissa0 == 0);
#endif
    }
    
    /**
      Returns true if the value is Not-a-Number (NaN).
    */
    inline bool isNaN() const noexcept
    {
#if 1 // temporary workaround until we have x86 representation
      return (value.exponent == value.ALL_BITS_EXPONENT) &&
        !isInfinity();
#else
      return (value.exponent == value.ALL_BITS_EXPONENT) &&
        ((value.mantissa1 != 0) || (value.mantissa0 != 0));
#endif
    }
    
    /**
      Returns true if the value is a quite NaN. A NaN that behaves predictably
      and does not raise exceptions in arithmetic operations is called a quiet
      NaN.
    */
    inline bool isQuiteNaN() const noexcept
    {
#if 1 // temporary workaround until we have x86 representation
      return (value.exponent == value.ALL_BITS_EXPONENT) &&
        !isInfinity() && !isSignalingNaN();
#else
      return (value.exponent == value.ALL_BITS_EXPONENT) &&
        ((value.mantissa1 & (1 << (32 - 1))) != 0);
#endif
    }
    
    /**
      Returns true if the value is a signaling NaN. A NaN that generally raises
      an exception when encountered as an operand of arithmetic operations is
      called a signaling NaN.
    */
    inline bool isSignalingNaN() const noexcept
    {
#if 1 // temporary workaround until we have x86 representation
      return (value.exponent == value.ALL_BITS_EXPONENT) &&
        (((value.mantissa1 & 0xc0000000) == 0x80000000) && ((value.mantissa1 != 0) || (value.mantissa0 != 0)));
#else
      return (value.exponent == value.ALL_BITS_EXPONENT) &&
        ((value.mantissa1 & (1 << (32 - 1))) == 0) &&
        ((value.mantissa1 != 0) || (value.mantissa0 != 0)); // but not infinity
#endif
    }
    
    /** Returns the exponent. */
    inline int getExponent() const noexcept
    {
      BASSERT(isOrdinary());
      BASSERT(static_cast<int>(value.exponent - value.BIAS) <= value.MAXIMUM_EXPONENT);
      BASSERT(static_cast<int>(value.exponent - value.BIAS) >= value.MINIMUM_EXPONENT);
      return value.exponent - value.BIAS;
    }

    /** Sets the exponent. */
    inline void setExponent(int exponent) noexcept
    {
      BASSERT(exponent <= value.MAXIMUM_EXPONENT);
      BASSERT(exponent >= value.MINIMUM_EXPONENT);
      value.exponent = exponent + value.BIAS;
    }
    
    /** Returns true if supported exponent. Ignoring denomalized values. */
    static inline bool isSupportedExponent(int exponent) noexcept
    {
      return (exponent >= Representation::IEEEExtendedDoublePrecision96::MINIMUM_EXPONENT) &&
        (exponent <= Representation::IEEEExtendedDoublePrecision96::MAXIMUM_EXPONENT);
    }

    operator float() const noexcept;
    
    operator double() const noexcept;
    
    operator long double() const noexcept;

    operator float128() const noexcept;
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

  /** @short IEEE extended double precision (128 bit) conversion support. */
_COM_AZURE_DEV__BASE__PACKED__BEGIN
  struct IEEEExtendedDoublePrecision128 {
    Representation::IEEEExtendedDoublePrecision128 value;

    inline void setValue(const Representation::IEEE754SinglePrecision& _value) noexcept
    {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent;
      value.mantissa1 = (1 << 31) | (static_cast<uint32>(_value.mantissa0) << (32 - 23 - 1));
      value.mantissa0 = 0;
    }
    
    inline void setValue(const Representation::IEEE754DoublePrecision& _value) noexcept
    {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent;
      value.mantissa1 = (1 << 31) | (static_cast<uint32>(_value.mantissa1) << (32 - 20 - 1)) |
        (_value.mantissa0 >> (32 - 20 - 1));
      value.mantissa0 = static_cast<uint32>(_value.mantissa0) << (32 - 20 - 1);
    }
    
    inline void setValue(const Representation::IEEEExtendedDoublePrecision96& _value) noexcept
    {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent;
      value.mantissa1 = _value.mantissa1;
      value.mantissa0 = _value.mantissa0;
    }

    inline void setValue(const Representation::IEEEExtendedDoublePrecision96Align16& _value) noexcept
    {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent;
      value.mantissa1 = _value.mantissa1;
      value.mantissa0 = _value.mantissa0;
    }
    
    inline void setValue(const Representation::IEEEExtendedDoublePrecision128& value) noexcept
    {
      this->value = value;
    }
    
    inline void setValue(const Representation::IEEEQuadruplePrecision& _value) noexcept
    {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent;
      value.mantissa1 = (1 << 31) | (static_cast<uint32>(_value.mantissa3) << (32 - 16 - 1)) |
        (_value.mantissa2 >> (32 - (32 - 16 - 1)));
      value.mantissa0 = (static_cast<uint32>(_value.mantissa2) << (32 - (32 - 16 - 1))) |
        (_value.mantissa1 >> (32 - (32 - 16 - 1)));
    }
    
    void setValue(const Representation::IBMExtendedPrecision& value) noexcept;

    inline void setValue(float value) noexcept
    {
      setValue(*reinterpret_cast<const FloatRepresentation*>(&value));
    }
    
    inline void setValue(double value) noexcept
    {
      setValue(*reinterpret_cast<const DoubleRepresentation*>(&value));
    }
    
    inline void setValue(long double value) noexcept
    {
      setValue(*reinterpret_cast<const LongDoubleRepresentation*>(&value));
    }

    inline void setValue(float128 value) noexcept
    {
      setValue(*reinterpret_cast<const Float128Representation*>(&value));
    }

    inline IEEEExtendedDoublePrecision128() noexcept
    {
      clear(value);
    }
    
    inline IEEEExtendedDoublePrecision128(const Representation::IEEE754SinglePrecision& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision128(const Representation::IEEE754DoublePrecision& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision128(const Representation::IEEEExtendedDoublePrecision96& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision128(const Representation::IEEEExtendedDoublePrecision96Align16& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision128(const Representation::IEEEExtendedDoublePrecision128& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision128(const Representation::IEEEQuadruplePrecision& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision128(const Representation::IBMExtendedPrecision& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision128(float value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision128(double value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEExtendedDoublePrecision128(long double value) noexcept
    {
      setValue(value);
    }

    inline IEEEExtendedDoublePrecision128(float128 value) noexcept
    {
      setValue(value);
    }

    /**
      Returns true if the value is negative.
    */
    inline bool isNegative() const noexcept
    {
      return value.negative != 0;
    }
    
    /**
      Returns true if the value is an ordinary number (not infinity or NaN).
    */
    inline bool isOrdinary() const noexcept
    {
      return value.exponent != value.ALL_BITS_EXPONENT;
    }
    
    /**
      Returns true if the value is either +INFINITY or -INFINITY.
    */
    inline bool isInfinity() const noexcept
    {
      return (value.exponent == value.ALL_BITS_EXPONENT) && (value.mantissa1 == 0) &&
        (value.mantissa0 == 0);
    }
    
    /**
      Returns true if the value is Not-a-Number (NaN).
    */
    inline bool isNaN() const noexcept
    {
      return (value.exponent == value.ALL_BITS_EXPONENT) &&
        ((value.mantissa1 != 0) || (value.mantissa0 != 0));
    }
    
    /**
      Returns true if the value is a quite NaN. A NaN that behaves predictably
      and does not raise exceptions in arithmetic operations is called a quiet
      NaN.
    */
    inline bool isQuiteNaN() const noexcept
    {
      return (value.exponent == value.ALL_BITS_EXPONENT) &&
        ((value.mantissa1 & (1 << (32 - 1))) != 0);
    }
    
    /**
      Returns true if the value is a signaling NaN. A NaN that generally raises
      an exception when encountered as an operand of arithmetic operations is
      called a signaling NaN.
    */
    inline bool isSignalingNaN() const noexcept
    {
      return (value.exponent == value.ALL_BITS_EXPONENT) &&
        ((value.mantissa1 & (1 << (32 - 1))) == 0) &&
        ((value.mantissa1 != 0) || (value.mantissa0 != 0)); // but not infitity
    }
    
    /** Returns the exponent. */
    inline int getExponent() const noexcept
    {
      BASSERT(isOrdinary());
      BASSERT(static_cast<int>(value.exponent - value.BIAS) <= value.MAXIMUM_EXPONENT);
      BASSERT(static_cast<int>(value.exponent - value.BIAS) >= value.MINIMUM_EXPONENT);
      return value.exponent - value.BIAS;
    }

    /** Sets the exponent. */
    inline void setExponent(int exponent) noexcept
    {
      BASSERT(exponent <= value.MAXIMUM_EXPONENT);
      BASSERT(exponent >= value.MINIMUM_EXPONENT);
      value.exponent = exponent + value.BIAS;
    }
    
    /** Returns true if supported exponent. Ignoring denomalized values. */
    static inline bool isSupportedExponent(int exponent) noexcept
    {
      return (exponent >= Representation::IEEEExtendedDoublePrecision128::MINIMUM_EXPONENT) &&
        (exponent <= Representation::IEEEExtendedDoublePrecision128::MAXIMUM_EXPONENT);
    }

    operator float() const noexcept;
    
    operator double() const noexcept;
    
    operator long double() const noexcept;

    operator float128() const noexcept;
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

  /** @short IEEE quadruple precision conversion support. */
_COM_AZURE_DEV__BASE__PACKED__BEGIN
  struct IEEEQuadruplePrecision {
    Representation::IEEEQuadruplePrecision value;

    inline void setValue(const Representation::IEEE754SinglePrecision& _value) noexcept
    {
      value.negative = _value.negative;
      int exponent = _value.exponent - _value.BIAS + value.BIAS;
      value.exponent = exponent; // possible overflow and underflow
      // both representations have implied one
      value.mantissa3 = _value.mantissa0 >> (23 - 16);
      value.mantissa2 = static_cast<uint32>(_value.mantissa0) << (32 - (23 - 16));
      value.mantissa1 = 0;
      value.mantissa0 = 0;
    }
    
    inline void setValue(
      const Representation::IEEE754DoublePrecision& _value) noexcept
    {
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
    
    void setValue(const Representation::IEEEExtendedDoublePrecision96& value) noexcept;

    void setValue(const Representation::IEEEExtendedDoublePrecision96Align16& value) noexcept;

    void setValue(const Representation::IEEEExtendedDoublePrecision128& value) noexcept;
    
    inline void setValue(const Representation::IEEEQuadruplePrecision& value) noexcept
    {
      this->value = value;
    }

    void setValue(const Representation::IBMExtendedPrecision& value) noexcept;

    inline void setValue(float value) noexcept
    {
      setValue(*reinterpret_cast<const FloatRepresentation*>(&value));
    }
    
    inline void setValue(double value) noexcept
    {
      setValue(*reinterpret_cast<const DoubleRepresentation*>(&value));
    }
    
    inline void setValue(long double value) noexcept
    {
      setValue(*reinterpret_cast<const LongDoubleRepresentation*>(&value));
    }

    inline void setValue(float128 value) noexcept
    {
      setValue(*reinterpret_cast<const Float128Representation*>(&value));
    }

    inline IEEEQuadruplePrecision() noexcept
    {
      clear(value);
    }
    
    inline IEEEQuadruplePrecision(const Representation::IEEE754SinglePrecision& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEQuadruplePrecision(const Representation::IEEE754DoublePrecision& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEQuadruplePrecision(const Representation::IEEEExtendedDoublePrecision96& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEQuadruplePrecision(const Representation::IEEEExtendedDoublePrecision96Align16& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEQuadruplePrecision(const Representation::IEEEExtendedDoublePrecision128& value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEQuadruplePrecision(const Representation::IEEEQuadruplePrecision& value) noexcept
    {
      setValue(value);
    }

    inline IEEEQuadruplePrecision(const Representation::IBMExtendedPrecision& value) noexcept
    {
      setValue(value);
    }

    inline IEEEQuadruplePrecision(float value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEQuadruplePrecision(double value) noexcept
    {
      setValue(value);
    }
    
    inline IEEEQuadruplePrecision(long double value) noexcept
    {
      setValue(value);
    }

    inline IEEEQuadruplePrecision(float128 value) noexcept
    {
      setValue(value);
    }

    /**
      Returns true if the value is negative.
    */
    inline bool isNegative() const noexcept
    {
      return value.negative != 0;
    }
    
    /**
      Returns true if the value is an ordinary number (not infinity or NaN).
    */
    inline bool isOrdinary() const noexcept
    {
      return value.exponent != value.ALL_BITS_EXPONENT;
    }
    
    /**
      Returns true if the value is either +INFINITY or -INFINITY.
    */
    inline bool isInfinity() const noexcept
    {
      return (value.exponent == value.ALL_BITS_EXPONENT) && (value.mantissa3 == 0) &&
        (value.mantissa2 == 0) && (value.mantissa1 == 0) &&
        (value.mantissa0 == 0);
    }
    
    /**
      Returns true if the value is Not-a-Number (NaN).
    */
    inline bool isNaN() const noexcept
    {
      return (value.exponent == value.ALL_BITS_EXPONENT) &&
        ((value.mantissa3 != 0) || (value.mantissa2 != 0) ||
         (value.mantissa1 != 0) || (value.mantissa0 != 0));
    }
    
    /**
      Returns true if the value is a quite NaN. A NaN that behaves predictably
      and does not raise exceptions in arithmetic operations is called a quiet
      NaN.
    */
    inline bool isQuiteNaN() const noexcept
    {
      return (value.exponent == value.ALL_BITS_EXPONENT) &&
        ((value.mantissa3 & (1 << (16 - 1))) != 0);
    }
    
    /**
      Returns true if the value is a signaling NaN. A NaN that generally raises
      an exception when encountered as an operand of arithmetic operations is
      called a signaling NaN.
    */
    inline bool isSignalingNaN() const noexcept
    {
      return (value.exponent == value.ALL_BITS_EXPONENT) &&
        ((value.mantissa3 & (1 << (16 - 1))) == 0) &&
        ((value.mantissa3 != 0) || (value.mantissa2 != 0) ||
         (value.mantissa1 != 0) || (value.mantissa0 != 0)); // but not infitity
    }

    /** Returns the exponent. */
    inline int getExponent() const noexcept
    {
      BASSERT(isOrdinary());
      BASSERT(static_cast<int>(value.exponent - value.BIAS) <= value.MAXIMUM_EXPONENT);
      BASSERT(static_cast<int>(value.exponent - value.BIAS) >= value.MINIMUM_EXPONENT);
      return value.exponent - value.BIAS;
    }

    /** Sets the exponent. */
    inline void setExponent(int exponent) noexcept
    {
      BASSERT(exponent <= value.MAXIMUM_EXPONENT);
      BASSERT(exponent >= value.MINIMUM_EXPONENT);
      value.exponent = exponent + value.BIAS;
    }
    
    /** Returns true if supported exponent. Ignoring denomalized values. */
    static inline bool isSupportedExponent(int exponent) noexcept
    {
      return (exponent >= Representation::IEEEQuadruplePrecision::MINIMUM_EXPONENT) &&
        (exponent <= Representation::IEEEQuadruplePrecision::MAXIMUM_EXPONENT);
    }

    operator float() const noexcept;
    
    operator double() const noexcept;
    
    operator long double() const noexcept;

    operator float128() const noexcept;
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

  /** @short IBM extended precision conversion support. */
_COM_AZURE_DEV__BASE__PACKED__BEGIN
  struct IBMExtendedPrecision {
    Representation::IBMExtendedPrecision value;
    
    inline void setValue(const Representation::IEEE754SinglePrecision& _value) noexcept
    {
      IEEE754DoublePrecision temp(_value);
      setValue(temp.value);
    }
    
    inline void setValue(const Representation::IEEE754DoublePrecision& _value) noexcept
    {
      Representation::IEEE754DoublePrecision* dd =
        reinterpret_cast<Representation::IEEE754DoublePrecision*>(&value);
      dd[0] = _value;
      clear(dd[1]);
    }
    
    void setValue(const Representation::IEEEExtendedDoublePrecision96& value) noexcept;

    void setValue(const Representation::IEEEExtendedDoublePrecision96Align16& value) noexcept;

    void setValue(const Representation::IEEEExtendedDoublePrecision128& value) noexcept;

    void setValue(const Representation::IEEEQuadruplePrecision& value) noexcept;

    inline void setValue(const Representation::IBMExtendedPrecision& value) noexcept
    {
      this->value = value;
    }
    
    inline void setValue(float value) noexcept
    {
      setValue(*reinterpret_cast<const FloatRepresentation*>(&value));
    }
    
    inline void setValue(double value) noexcept
    {
      setValue(*reinterpret_cast<const DoubleRepresentation*>(&value));
    }
    
    inline void setValue(long double value) noexcept
    {
      setValue(*reinterpret_cast<const LongDoubleRepresentation*>(&value));
    }

    inline void setValue(float128 value) noexcept
    {
      setValue(*reinterpret_cast<const Float128Representation*>(&value));
    }

    inline IBMExtendedPrecision() noexcept
    {
      clear(value);
    }
    
    inline IBMExtendedPrecision(const Representation::IEEE754SinglePrecision& value) noexcept
    {
      setValue(value);
    }
    
    inline IBMExtendedPrecision(const Representation::IEEE754DoublePrecision& value) noexcept
    {
      setValue(value);
    }
    
    inline IBMExtendedPrecision(const Representation::IEEEExtendedDoublePrecision96& value) noexcept
    {
      setValue(value);
    }
    
    inline IBMExtendedPrecision(const Representation::IEEEExtendedDoublePrecision96Align16& value) noexcept
    {
      setValue(value);
    }
    
    inline IBMExtendedPrecision(const Representation::IEEEExtendedDoublePrecision128& value) noexcept
    {
      setValue(value);
    }
    
    inline IBMExtendedPrecision(const Representation::IEEEQuadruplePrecision& value) noexcept
    {
      setValue(value);
    }
    
    inline IBMExtendedPrecision(const Representation::IBMExtendedPrecision& value) noexcept
    {
      setValue(value);
    }

    inline IBMExtendedPrecision(float value) noexcept
    {
      setValue(value);
    }
    
    inline IBMExtendedPrecision(double value) noexcept
    {
      setValue(value);
    }
    
    inline IBMExtendedPrecision(long double value) noexcept
    {
      setValue(value);
    }

    inline IBMExtendedPrecision(float128 value) noexcept
    {
      setValue(value);
    }

    /**
      Returns true if the value is negative.
    */
    inline bool isNegative() const noexcept
    {
      return value.negative != 0;
    }
    
    /**
      Returns true if the value is an ordinary number (not infinity or NaN).
    */
    inline bool isOrdinary() const noexcept
    {
      return value.exponent != value.ALL_BITS_EXPONENT;
    }
    
    /**
      Returns true if the value is either +INFINITY or -INFINITY.
    */
    inline bool isInfinity() const noexcept
    {
      return reinterpret_cast<const IEEE754DoublePrecision&>(value).isInfinity();
    }
    
    /**
      Returns true if the value is Not-a-Number (NaN).
    */
    inline bool isNaN() const noexcept
    {
      return reinterpret_cast<const IEEE754DoublePrecision&>(value).isNaN();
    }
    
    /**
      Returns true if the value is a quite NaN. A NaN that behaves predictably
      and does not raise exceptions in arithmetic operations is called a quiet
      NaN.
    */
    inline bool isQuiteNaN() const noexcept
    {
      return reinterpret_cast<const IEEE754DoublePrecision&>(value).isQuiteNaN();
    }
    
    /**
      Returns true if the value is a signaling NaN. A NaN that generally raises
      an exception when encountered as an operand of arithmetic operations is
      called a signaling NaN.
    */
    inline bool isSignalingNaN() const noexcept
    {
      return reinterpret_cast<const IEEE754DoublePrecision&>(value).isSignalingNaN();
    }

    /** Returns the exponent. */
    inline int getExponent() const noexcept
    {
      BASSERT(isOrdinary());
      BASSERT(static_cast<int>(value.exponent - value.BIAS) <= value.MAXIMUM_EXPONENT);
      BASSERT(static_cast<int>(value.exponent - value.BIAS) >= value.MINIMUM_EXPONENT);
      return value.exponent - value.BIAS;
    }

    /** Sets the exponent. */
    inline void setExponent(int exponent) noexcept
    {
      BASSERT(exponent <= value.MAXIMUM_EXPONENT);
      BASSERT(exponent >= value.MINIMUM_EXPONENT);
      value.exponent = exponent + value.BIAS;
    }
    
    /** Returns true if supported exponent. Ignoring denomalized values. */
    static inline bool isSupportedExponent(int exponent) noexcept
    {
      return (exponent >= Representation::IBMExtendedPrecision::MINIMUM_EXPONENT) &&
        (exponent <= Representation::IBMExtendedPrecision::MAXIMUM_EXPONENT);
    }

    operator float() const noexcept;
    
    operator double() const noexcept;
    
    operator long double() const noexcept;

    operator float128() const noexcept;
  } _COM_AZURE_DEV__BASE__PACKED;
_COM_AZURE_DEV__BASE__PACKED__END

#if (_COM_AZURE_DEV__BASE__FLOAT == _COM_AZURE_DEV__BASE__IEEE_754_SINGLE_PRECISION)
  typedef IEEE754SinglePrecision ToFloat;
#elif (_COM_AZURE_DEV__BASE__FLOAT == _COM_AZURE_DEV__BASE__IEEE_754_DOUBLE_PRECISION)
  typedef IEEE754DoublePrecision ToFloat;
#elif (_COM_AZURE_DEV__BASE__FLOAT == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96)
  typedef IEEEExtendedDoublePrecision96 ToFloat;
#elif (_COM_AZURE_DEV__BASE__FLOAT == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16)
  typedef IEEEExtendedDoublePrecision96Align16 ToFloat;
#elif (_COM_AZURE_DEV__BASE__FLOAT == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128)
  typedef IEEEExtendedDoublePrecision128 ToFloat;
#elif (_COM_AZURE_DEV__BASE__FLOAT == _COM_AZURE_DEV__BASE__IEEE_QUADRUPLE_PRECISION)
  typedef IEEEQuadruplePrecision ToFloat;
#elif (_COM_AZURE_DEV__BASE__FLOAT == _COM_AZURE_DEV__BASE__IBM_EXTENDED_PRECISION)
  typedef IBMExtendedPrecision ToFloat;
#else
#  error Invalid floating-point representation of type float
#endif

#if (_COM_AZURE_DEV__BASE__DOUBLE == _COM_AZURE_DEV__BASE__IEEE_754_SINGLE_PRECISION)
  typedef IEEE754SinglePrecision ToDouble;
#elif (_COM_AZURE_DEV__BASE__DOUBLE == _COM_AZURE_DEV__BASE__IEEE_754_DOUBLE_PRECISION)
  typedef IEEE754DoublePrecision ToDouble;
#elif (_COM_AZURE_DEV__BASE__DOUBLE == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96)
  typedef IEEEExtendedDoublePrecision96 ToDouble;
#elif (_COM_AZURE_DEV__BASE__DOUBLE == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16)
  typedef IEEEExtendedDoublePrecision96Align16 ToDouble;
#elif (_COM_AZURE_DEV__BASE__DOUBLE == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128)
  typedef IEEEExtendedDoublePrecision128 ToDouble;
#elif (_COM_AZURE_DEV__BASE__DOUBLE == _COM_AZURE_DEV__BASE__IEEE_QUADRUPLE_PRECISION)
  typedef IEEEQuadruplePrecision ToDouble;
#elif (_COM_AZURE_DEV__BASE__DOUBLE == _COM_AZURE_DEV__BASE__IBM_EXTENDED_PRECISION)
  typedef IBMExtendedPrecision ToDouble;
#else
#  error Invalid floating-point representation of type double
#endif

#if (_COM_AZURE_DEV__BASE__LONG_DOUBLE == _COM_AZURE_DEV__BASE__IEEE_754_SINGLE_PRECISION)
  typedef IEEE754SinglePrecision ToLongDouble;
#elif (_COM_AZURE_DEV__BASE__LONG_DOUBLE == _COM_AZURE_DEV__BASE__IEEE_754_DOUBLE_PRECISION)
  typedef IEEE754DoublePrecision ToLongDouble;
#elif (_COM_AZURE_DEV__BASE__LONG_DOUBLE == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96)
  typedef IEEEExtendedDoublePrecision96 ToLongDouble;
#elif (_COM_AZURE_DEV__BASE__LONG_DOUBLE == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16)
  typedef IEEEExtendedDoublePrecision96Align16 ToLongDouble;
#elif (_COM_AZURE_DEV__BASE__LONG_DOUBLE == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128)
  typedef IEEEExtendedDoublePrecision128 ToLongDouble;
#elif (_COM_AZURE_DEV__BASE__LONG_DOUBLE == _COM_AZURE_DEV__BASE__IEEE_QUADRUPLE_PRECISION)
  typedef IEEEQuadruplePrecision ToLongDouble;
#elif (_COM_AZURE_DEV__BASE__LONG_DOUBLE == _COM_AZURE_DEV__BASE__IBM_EXTENDED_PRECISION)
  typedef IBMExtendedPrecision ToLongDouble;
#else
#  error Invalid floating-point representation of type long double
#endif

#if (_COM_AZURE_DEV__BASE__FLOAT128 == _COM_AZURE_DEV__BASE__IEEE_754_SINGLE_PRECISION)
  typedef IEEE754SinglePrecision ToFloat128;
#elif (_COM_AZURE_DEV__BASE__FLOAT128 == _COM_AZURE_DEV__BASE__IEEE_754_DOUBLE_PRECISION)
  typedef IEEE754DoublePrecision ToFloat128;
#elif (_COM_AZURE_DEV__BASE__FLOAT128 == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96)
  typedef IEEEExtendedDoublePrecision96 ToFloat128;
#elif (_COM_AZURE_DEV__BASE__FLOAT128 == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16)
  typedef IEEEExtendedDoublePrecision96Align16 ToFloat128;
#elif (_COM_AZURE_DEV__BASE__FLOAT128 == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128)
  typedef IEEEExtendedDoublePrecision128 ToFloat128;
#elif (_COM_AZURE_DEV__BASE__FLOAT128 == _COM_AZURE_DEV__BASE__IEEE_QUADRUPLE_PRECISION)
  typedef IEEEQuadruplePrecision ToFloat128;
#elif (_COM_AZURE_DEV__BASE__FLOAT128 == _COM_AZURE_DEV__BASE__IBM_EXTENDED_PRECISION)
  typedef IBMExtendedPrecision ToFloat128;
#else
#  error Invalid floating-point representation of type float128
#endif

  /** Converts float primitive to representation type. */
  template<class TYPE>
  class ToFloatPrimitive {};
};

template<>
class FloatingPoint::ToFloatPrimitive<float> {
public:
  
  typedef FloatingPoint::ToFloat Type;
};

template<>
class FloatingPoint::ToFloatPrimitive<double> {
public:
  
  typedef FloatingPoint::ToDouble Type;
};

template<>
class FloatingPoint::ToFloatPrimitive<long double> {
public:
  
  typedef FloatingPoint::ToLongDouble Type;
};

template<>
class FloatingPoint::ToFloatPrimitive<float128> {
public:

  typedef FloatingPoint::ToFloat128 Type;
};

inline FloatingPoint::IEEE754SinglePrecision::operator float() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLOAT == _COM_AZURE_DEV__BASE__IEEE_754_SINGLE_PRECISION)
  return *reinterpret_cast<const float*>(&value);
#else
  return ToFloat(value);
#endif // float
}

inline FloatingPoint::IEEE754SinglePrecision::operator double() const noexcept
{
#if (_COM_AZURE_DEV__BASE__DOUBLE == _COM_AZURE_DEV__BASE__IEEE_754_SINGLE_PRECISION)
  return *reinterpret_cast<const double*>(&value);
#else
  return ToDouble(value);
#endif // double
}

inline FloatingPoint::IEEE754SinglePrecision::operator long double() const noexcept
{
#if (_COM_AZURE_DEV__BASE__LONG_DOUBLE == _COM_AZURE_DEV__BASE__IEEE_754_SINGLE_PRECISION)
  return *reinterpret_cast<const long double*>(&value);
#else
  return ToLongDouble(value);
#endif // long double
}

inline FloatingPoint::IEEE754SinglePrecision::operator float128() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLOAT128 == _COM_AZURE_DEV__BASE__IEEE_754_SINGLE_PRECISION)
  return *reinterpret_cast<const float128*>(&value);
#else
  return ToFloat128(value);
#endif
}

inline FloatingPoint::IEEE754DoublePrecision::operator float() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLOAT == _COM_AZURE_DEV__BASE__IEEE_754_DOUBLE_PRECISION)
  return *reinterpret_cast<const float*>(&value);
#else
  return ToFloat(value);
#endif // float
}

inline FloatingPoint::IEEE754DoublePrecision::operator double() const noexcept
{
#if (_COM_AZURE_DEV__BASE__DOUBLE == _COM_AZURE_DEV__BASE__IEEE_754_DOUBLE_PRECISION)
  return *reinterpret_cast<const double*>(&value);
#else
  return ToDouble(value);
#endif // double
}

inline FloatingPoint::IEEE754DoublePrecision::operator long double() const noexcept
{
#if (_COM_AZURE_DEV__BASE__LONG_DOUBLE == _COM_AZURE_DEV__BASE__IEEE_754_DOUBLE_PRECISION)
  return *reinterpret_cast<const long double*>(&value);
#else
  return ToLongDouble(value);
#endif // long double
}

inline FloatingPoint::IEEE754DoublePrecision::operator float128() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLOAT128 == _COM_AZURE_DEV__BASE__IEEE_754_DOUBLE_PRECISION)
  return *reinterpret_cast<const float128*>(&value);
#else
  return ToFloat128(value);
#endif
}

inline FloatingPoint::IEEEExtendedDoublePrecision96::operator float() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLOAT == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96)
  return *reinterpret_cast<const float*>(&value);
#elif (_COM_AZURE_DEV__BASE__FLOAT == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16)
  return *reinterpret_cast<const long double*>(&value);
#else
  return ToFloat(value);
#endif // float
}

inline FloatingPoint::IEEEExtendedDoublePrecision96::operator double() const noexcept
{
#if (_COM_AZURE_DEV__BASE__DOUBLE == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96)
  return *reinterpret_cast<const double*>(&value);
#elif (_COM_AZURE_DEV__BASE__DOUBLE == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16)
  return *reinterpret_cast<const long double*>(&value);
#else
  return ToDouble(value);
#endif // double
}
  
inline FloatingPoint::IEEEExtendedDoublePrecision96::operator long double() const noexcept
{
#if (_COM_AZURE_DEV__BASE__LONG_DOUBLE == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96)
  return *reinterpret_cast<const long double*>(&value);
#elif (_COM_AZURE_DEV__BASE__LONG_DOUBLE == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16)
  return *reinterpret_cast<const long double*>(&value);
#else
  return ToLongDouble(value);
#endif // long double
}

inline FloatingPoint::IEEEExtendedDoublePrecision96::operator float128() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLOAT128 == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96)
  return *reinterpret_cast<const float128*>(&value);
#else
  return ToFloat128(value);
#endif
}

inline FloatingPoint::IEEEExtendedDoublePrecision96Align16::operator float() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLOAT == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16)
  return *reinterpret_cast<const long double*>(&value);
#else
  return ToFloat(value);
#endif // float
}

inline FloatingPoint::IEEEExtendedDoublePrecision96Align16::operator double() const noexcept
{
#if (_COM_AZURE_DEV__BASE__DOUBLE == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16)
  return *reinterpret_cast<const long double*>(&value);
#else
  return ToDouble(value);
#endif // double
}
  
inline FloatingPoint::IEEEExtendedDoublePrecision96Align16::operator long double() const noexcept
{
#if (_COM_AZURE_DEV__BASE__LONG_DOUBLE == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16)
  return *reinterpret_cast<const long double*>(&value);
#else
  return ToLongDouble(value);
#endif // long double
}

inline FloatingPoint::IEEEExtendedDoublePrecision96Align16::operator float128() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLOAT128 == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16)
  return *reinterpret_cast<const float128*>(&value);
#else
  return ToFloat128(value);
#endif
}

inline FloatingPoint::IEEEExtendedDoublePrecision128::operator float() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLOAT == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128)
  return *reinterpret_cast<const float*>(&value);
#else
  return ToFloat(value);
#endif // float
}

inline FloatingPoint::IEEEExtendedDoublePrecision128::operator double() const noexcept
{
#if (_COM_AZURE_DEV__BASE__DOUBLE == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128)
  return *reinterpret_cast<const double*>(&value);
#else
  return ToDouble(value);
#endif // double
}

inline FloatingPoint::IEEEExtendedDoublePrecision128::operator long double() const noexcept
{
#if (_COM_AZURE_DEV__BASE__LONG_DOUBLE == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128)
  return *reinterpret_cast<const long double*>(&value);
#else
  return ToLongDouble(value);
#endif // long double
}

inline FloatingPoint::IEEEExtendedDoublePrecision128::operator float128() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLOAT128 == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128)
  return *reinterpret_cast<const float128*>(&value);
#else
  return ToFloat128(value);
#endif
}

inline FloatingPoint::IEEEQuadruplePrecision::operator float() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLOAT == _COM_AZURE_DEV__BASE__IEEE_QUADRUPLE_PRECISION)
  return *reinterpret_cast<const float*>(&value);
#else
  return ToFloat(value);
#endif // float
}

inline FloatingPoint::IEEEQuadruplePrecision::operator double() const noexcept
{
#if (_COM_AZURE_DEV__BASE__DOUBLE == _COM_AZURE_DEV__BASE__IEEE_QUADRUPLE_PRECISION)
  return *reinterpret_cast<const double*>(&value);
#else
  return ToDouble(value);
#endif // double
}
  
inline FloatingPoint::IEEEQuadruplePrecision::operator long double() const noexcept
{
#if (_COM_AZURE_DEV__BASE__LONG_DOUBLE == _COM_AZURE_DEV__BASE__IEEE_QUADRUPLE_PRECISION)
  return *reinterpret_cast<const long double*>(&value);
#else
  return ToLongDouble(value);
#endif // long double
}

inline FloatingPoint::IEEEQuadruplePrecision::operator float128() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLOAT128 == _COM_AZURE_DEV__BASE__IEEE_QUADRUPLE_PRECISION)
  return *reinterpret_cast<const float128*>(&value);
#else
  return ToFloat128(value);
#endif
}

inline FloatingPoint::IBMExtendedPrecision::operator float() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLOAT == _COM_AZURE_DEV__BASE__IBM_EXTENDED_PRECISION)
  return *reinterpret_cast<const float*>(&value);
#else
  return ToFloat(value);
#endif // float
}

inline FloatingPoint::IBMExtendedPrecision::operator double() const noexcept
{
#if (_COM_AZURE_DEV__BASE__DOUBLE == _COM_AZURE_DEV__BASE__IBM_EXTENDED_PRECISION)
  return *reinterpret_cast<const double*>(&value);
#else
  return ToDouble(value);
#endif // double
}
  
inline FloatingPoint::IBMExtendedPrecision::operator long double() const noexcept
{
#if (_COM_AZURE_DEV__BASE__LONG_DOUBLE == _COM_AZURE_DEV__BASE__IBM_EXTENDED_PRECISION)
  return *reinterpret_cast<const long double*>(&value);
#else
  return ToLongDouble(value);
#endif // long double
}

inline FloatingPoint::IBMExtendedPrecision::operator float128() const noexcept
{
#if (_COM_AZURE_DEV__BASE__FLOAT128 == _COM_AZURE_DEV__BASE__IBM_EXTENDED_PRECISION)
  return *reinterpret_cast<const float128*>(&value);
#else
  return ToFloat128(value);
#endif
}

/**
  Analyzes the specified floating point number of a given representation.
  
  @param value The floating point number.
  @param significant The number of significant digits in the mantissa (OUT).
  @param mantissa The array to receive the mantissa of the number (OUT). The
  size of the array must be (TYPE::SIGNIFICANT+31/32).
  @param exponent The exponent (base 2) of the number (OUT).
  @param flags Flags specifying the number type (OUT).
*/
template<typename TYPE>
inline void analyzeFloatingPoint(
  const TYPE& value,
  unsigned int& precision,
  unsigned int* mantissa,
  int& exponent,
  unsigned int& flags) noexcept
{
  BASSERT(!"Not supported");
}

template<>
void analyzeFloatingPoint<FloatingPoint::Representation::IEEE754SinglePrecision>(
  const FloatingPoint::Representation::IEEE754SinglePrecision& value,
  unsigned int& precision,
  unsigned int* mantissa,
  int& exponent,
  unsigned int& flags) noexcept;

template<>
void analyzeFloatingPoint<FloatingPoint::Representation::IEEE754DoublePrecision>(
  const FloatingPoint::Representation::IEEE754DoublePrecision& value,
  unsigned int& precision,
  unsigned int* mantissa,
  int& exponent,
  unsigned int& flags) noexcept;

template<>
void analyzeFloatingPoint<FloatingPoint::Representation::IEEEExtendedDoublePrecision96>(
  const FloatingPoint::Representation::IEEEExtendedDoublePrecision96& value,
  unsigned int& precision,
  unsigned int* mantissa,
  int& exponent,
  unsigned int& flags) noexcept;

template<>
void analyzeFloatingPoint<FloatingPoint::Representation::IEEEExtendedDoublePrecision96Align16>(
  const FloatingPoint::Representation::IEEEExtendedDoublePrecision96Align16& value,
  unsigned int& precision,
  unsigned int* mantissa,
  int& exponent,
  unsigned int& flags) noexcept;

template<>
void analyzeFloatingPoint<FloatingPoint::Representation::IEEEExtendedDoublePrecision128>(
  const FloatingPoint::Representation::IEEEExtendedDoublePrecision128& value,
  unsigned int& precision,
  unsigned int* mantissa,
  int& exponent,
  unsigned int& flags) noexcept;

template<>
void analyzeFloatingPoint<FloatingPoint::Representation::IEEEQuadruplePrecision>(
  const FloatingPoint::Representation::IEEEQuadruplePrecision& value,
  unsigned int& precision,
  unsigned int* mantissa,
  int& exponent,
  unsigned int& flags) noexcept;

template<>
void analyzeFloatingPoint<FloatingPoint::Representation::IBMExtendedPrecision>(
  const FloatingPoint::Representation::IBMExtendedPrecision& value,
  unsigned int& precision,
  unsigned int* mantissa,
  int& exponent,
  unsigned int& flags) noexcept;

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
