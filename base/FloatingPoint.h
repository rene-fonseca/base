/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Ren� M�ller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__FLOATING_POINT_H
#define _DK_SDU_MIP__BASE__FLOATING_POINT_H

#include <base/features.h>
#include <base/platforms/arch/arch.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/** Floating point flags. */
enum FloatingPointFlags {
  FP_NEGATIVE = 1, /**< Specifies a negative value. */
  FP_DENORMALIZED = 2, /**< Specifies a denormalized value. */
  FP_NAN = 4, /**< Specifies that the value isn't a number. */
  FP_ZERO = 8, /**< Value is zero (sign is determined by the FP_NEGATIVE flag). */
  FP_INFINITY = 16, /**< Value is infinity (sign is determined by the FP_NEGATIVE flag). */
  FP_VALUE = 32 /**< Specifies an ordinary value representable in [-]d.ddde[-]dd form. */
};

#if defined(_DK_SDU_MIP__BASE__BIG_ENDIAN)
  /** Representation of single precision (32-bit) floating point type as specified by IEEE 754. */
  struct IEEE_754_SinglePrecision {
    static const bool HAS_IMPLIED_ONE = true;
    static const int BIAS = 0x7f;
    static const unsigned int SIGNIFICANT = 24;

    bool negative : 1;
    unsigned int exponent : 8;
    unsigned int mantissa0 : 23;
  };
#else
  /** Representation of single precision (32-bit) floating point type as specified by IEEE 754. */
  struct IEEE_754_SinglePrecision {
    static const bool HAS_IMPLIED_ONE = true;
    static const int BIAS = 0x7f;
    static const unsigned int SIGNIFICANT = 24;

    unsigned int mantissa0 : 23;
    unsigned int exponent : 8;
    bool negative : 1;
  };
#endif

#if defined(_DK_SDU_MIP__BASE__BIG_ENDIAN)
  /** Representation of double precision (64-bit) floating point type as specified by IEEE 754. */
  struct IEEE_754_DoublePrecision {
    static const bool HAS_IMPLIED_ONE = true;
    static const int BIAS = 0x3ff;
    static const unsigned int SIGNIFICANT = 53;

    bool negative : 1;
    unsigned int exponent : 11;
    unsigned int mantissa1 : 20;
    unsigned int mantissa0 : 32;
  };
#else
  /** Representation of double precision (64-bit) floating point type as specified by IEEE 754. */
  struct IEEE_754_DoublePrecision {
    static const bool HAS_IMPLIED_ONE = true;
    static const int BIAS = 0x3ff;
    static const unsigned int SIGNIFICANT = 53;

    unsigned int mantissa0 : 32;
    unsigned int mantissa1 : 20;
    unsigned int exponent : 11;
    bool negative : 1;
  };
#endif

#if defined(_DK_MIP_SDU__BASE__BIG_ENDIAN)
  /** Representation of double-extended precision (96-bit) floating point type. */
  struct IEEE_DoubleExtendedPrecision96 {
    static const bool HAS_IMPLIED_ONE = false;
    static const int BIAS = 0x3fff;
    static const unsigned int SIGNIFICANT = 64;

    unsigned int negative : 1;
    unsigned int exponent : 15;
    unsigned int empty : 16;
    unsigned int mantissa1 : 32;
    unsigned int mantissa0 : 32;
  };
#else
  /** Representation of double-extended precision (96-bit) floating point type. */
  struct IEEE_DoubleExtendedPrecision96 {
    static const bool HAS_IMPLIED_ONE = false;
    static const int BIAS = 0x3fff;
    static const unsigned int SIGNIFICANT = 64;

    unsigned int mantissa0 : 32;
    unsigned int mantissa1 : 32;
    unsigned int exponent : 15;
    unsigned int negative : 1;
    unsigned int empty : 16;
  };
#endif

#if defined(_DK_MIP_SDU__BASE__BIG_ENDIAN)
  /** Representation of quadruple precision (128-bit) floating point type. */
  struct IEEE_QuadruplePrecision {
    static const bool HAS_IMPLIED_ONE = true;
    static const int BIAS = 0x3fff;
    static const unsigned int SIGNIFICANT = 113;

    unsigned int negative : 1;
    unsigned int exponent : 15;
    unsigned int mantissa3 : 16;
    unsigned int mantissa2 : 32;
    unsigned int mantissa1 : 32;
    unsigned int mantissa0 : 32;
  };
#else
  /** Representation of quadruple precision (128-bit) floating point type. */
  struct IEEE_QuadruplePrecision {
    static const bool HAS_IMPLIED_ONE = true;
    static const int BIAS = 0x3fff;
    static const unsigned int SIGNIFICANT = 113;

    unsigned int mantissa0 : 32;
    unsigned int mantissa1 : 32;
    unsigned int mantissa2 : 32;
    unsigned int mantissa3 : 16;
    unsigned int exponent : 15;
    unsigned int negative : 1;
  };
#endif



#if defined(_DK_SDU_MIP__BASE__FLOAT_IEEE_754_SINGLE)
  typedef IEEE_754_SinglePrecision FloatRepresentation;
#else
  #error Invalid representation of type float specified by arch.h
#endif

#if defined(_DK_SDU_MIP__BASE__DOUBLE_IEEE_754_DOUBLE)
  typedef IEEE_754_DoublePrecision DoubleRepresentation;
#else
  #error Invalid representation of type double specified by arch.h
#endif

#if defined(_DK_SDU_MIP__BASE__LONG_DOUBLE_IEEE_754_DOUBLE)
  typedef IEEE_754_DoublePrecision LongDoubleRepresentation;
#elif defined(_DK_SDU_MIP__BASE__LONG_DOUBLE_IEEE_DOUBLE_EXTENDED_96)
  typedef IEEE_DoubleExtendedPrecision96 LongDoubleRepresentation;
#elif defined(_DK_SDU_MIP__BASE__LONG_DOUBLE_IEEE_QUADRUPLE)
  typedef IEEE_QuadruplePrecision LongDoubleRepresentation;
#else
  #error Invalid representation of type long double specified by arch.h
#endif

/**
  Analyses the specified floating point number of a given representation.

  @param value The floating point number.
  @param significant The number of significant digits in the mantissa (OUT).
  @param mantissa The array to receive the mantissa of the number (OUT). The size of the array must be (TYPE::SIGNIFICANT+31/32).
  @param exponent The exponent (base 2) of the number (OUT).
  @param flags Flags specifying the number type (OUT).
*/
template<typename TYPE>
inline void analyseFloatingPoint(const TYPE& value, unsigned int& precision, unsigned int* mantissa, int& exponent, unsigned int& flags) throw() {}

template<>
void analyseFloatingPoint<IEEE_754_SinglePrecision>(const IEEE_754_SinglePrecision& value, unsigned int& precision, unsigned int* mantissa, int& exponent, unsigned int& flags) throw();
template<>
void analyseFloatingPoint<IEEE_754_DoublePrecision>(const IEEE_754_DoublePrecision& value, unsigned int& precision, unsigned int* mantissa, int& exponent, unsigned int& flags) throw();
template<>
void analyseFloatingPoint<IEEE_DoubleExtendedPrecision96>(const IEEE_DoubleExtendedPrecision96& value, unsigned int& precision, unsigned int* mantissa, int& exponent, unsigned int& flags) throw();
template<>
void analyseFloatingPoint<IEEE_QuadruplePrecision>(const IEEE_QuadruplePrecision& value, unsigned int& precision, unsigned int* mantissa, int& exponent, unsigned int& flags) throw();

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
