/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MATHEMATICS__MATH_H
#define _DK_SDU_MIP__BASE_MATHEMATICS__MATH_H

#include <base/Primitives.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @defgroup mathematics Mathematics
*/

namespace isoc {
  extern "C" float fabsf(float);
  extern "C" double fabs(double);
  extern "C" long double fabsl(long double);

  extern "C" float ceilf(float);
  extern "C" double ceil(double);
  extern "C" long double ceill(long double);

  extern "C" float floorf(float);
  extern "C" double floor(double);
  extern "C" long double floorl(long double);

  extern "C" float roundf(float);
  extern "C" double round(double);
  extern "C" long double roundl(long double);

  extern "C" float truncf(float);
  extern "C" double trunc(double);
  extern "C" long double truncl(long double);
  
  extern "C" float sqrtf(float);
  extern "C" double sqrt(double);
  extern "C" long double sqrtl(long double);

  extern "C" float cbrtf(float);
  extern "C" double cbrt(double);
  extern "C" long double cbrtl(long double);

  extern "C" float hypotf(float, float);
  extern "C" double hypot(double, double);
  extern "C" long double hypotl(long double, long double);

  extern "C" float expf(float);
  extern "C" double exp(double);
  extern "C" long double expl(long double);

  extern "C" float logf(float);
  extern "C" double log(double);
  extern "C" long double logl(long double);

  extern "C" float log2f(float);
  extern "C" double log2(double);
  extern "C" long double log2l(long double);

  extern "C" float log10f(float);
  extern "C" double log10(double);
  extern "C" long double log10l(long double);

  extern "C" float powf(float, float);
  extern "C" double pow(double, double);
  extern "C" long double powl(long double, long double);

  extern "C" float sinf(float);
  extern "C" double sin(double);
  extern "C" long double sinl(long double);

  extern "C" float cosf(float);
  extern "C" double cos(double);
  extern "C" long double cosl(long double);

  extern "C" float asinf(float);
  extern "C" double asin(double);
  extern "C" long double asinl(long double);

  extern "C" float acosf(float);
  extern "C" double acos(double);
  extern "C" long double acosl(long double);

  extern "C" float sinhf(float);
  extern "C" double sinh(double);
  extern "C" long double sinhl(long double);

  extern "C" float coshf(float);
  extern "C" double cosh(double);
  extern "C" long double coshl(long double);

  extern "C" float asinhf(float);
  extern "C" double asinh(double);
  extern "C" long double asinhl(long double);

  extern "C" float acoshf(float);
  extern "C" double acosh(double);
  extern "C" long double acoshl(long double);

  extern "C" float tanf(float);
  extern "C" double tan(double);
  extern "C" long double tanl(long double);

  extern "C" float atanf(float);
  extern "C" double atan(double);
  extern "C" long double atanl(long double);

  extern "C" float tanhf(float);
  extern "C" double tanh(double);
  extern "C" long double tanhl(long double);

  extern "C" float atanhf(float);
  extern "C" double atanh(double);
  extern "C" long double atanhl(long double);

  extern "C" float atan2f(float, float);
  extern "C" double atan2(double, double);
  extern "C" long double atan2l(long double, long double);
};

/**
  This class is a placeholder for mathematical functions.

  @short Mathematical functions
  @ingroup mathematics
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Math {
private:

  /** Bit reversal lookup table (8 bit). */
  static const uint8 BIT_REVERSAL[256];
public:
  
  /**
    Returns the absolute value.
  */
  static inline float abs(float value) throw() {
    #if defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_FABSF)
      return _DK_SDU_MIP__BASE__COMPILER_ISOC_FABSF(value);
    #else
      return isoc::fabsf(value);
    #endif
  }

  /**
    Returns the absolute value.
  */
  static inline double abs(double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_FABS)
      return _DK_SDU_MIP__BASE__COMPILER_ISOC_FABS(value);
    #else
      return isoc::fabs(value);
    #endif
  }

  /**
    Returns the absolute value.
  */
  static inline long double abs(long double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_FABSL)
      return _DK_SDU_MIP__BASE__COMPILER_ISOC_FABSL(value);
    #else
      return isoc::fabsl(value);
    #endif
  }

  /**
    Rounds (towards +infinity) the value to the nearest integer.
  */
  static inline float ceil(float value) throw() {
    return isoc::ceilf(value);
  }

  /**
    Rounds (towards +infinity) the value to the nearest integer.
  */
  static inline double ceil(double value) throw() {
    return isoc::ceil(value);
  }

  /**
    Rounds (towards +infinity) the value to the nearest integer.
  */
  static inline long double ceil(long double value) throw() {
    return isoc::ceill(value);
  }

  /**
    Rounds (towards -infinity) the value to the nearest integer.
  */
  static inline float floor(float value) throw() {
    return isoc::floorf(value);
  }

  /**
    Rounds (towards -infinity) the value to the nearest integer.
  */
  static inline double floor(double value) throw() {
    return isoc::floor(value);
  }

  /**
    Rounds (towards -infinity) the value to the nearest integer.
  */
  static inline long double floor(long double value) throw() {
    return isoc::floorl(value);
  }

  /**
    Rounds the value to the nearest integer.
  */
  static inline float round(float value) throw() {
    return isoc::roundf(value);
  }

  /**
    Rounds the value to the nearest integer.
  */
  static inline double round(double value) throw() {
    return isoc::round(value);
  }

  /**
    Rounds the value to the nearest integer.
  */
  static inline long double round(long double value) throw() {
    return isoc::roundl(value);
  }

  /**
    Rounds (towards zero) the value to the nearest integer.
  */
  static inline float trunc(float value) throw() {
    return isoc::truncf(value);
  }

  /**
    Rounds (towards zero) the value to the nearest integer.
  */
  static inline double trunc(double value) throw() {
    return isoc::trunc(value);
  }

  /**
    Rounds (towards zero) the value to the nearest integer.
  */
  static inline long double trunc(long double value) throw() {
    return isoc::truncl(value);
  }



  /**
    Returns the square root of the value.
  */
  static inline float sqrt(float value) throw() {
    #if defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_SQRTF)
      return _DK_SDU_MIP__BASE__COMPILER_ISOC_SQRTF(value);
    #else
      return isoc::sqrtf(value);
    #endif
  }

  /**
    Returns the square root of the value.
  */
  static inline double sqrt(double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_SQRT)
      return _DK_SDU_MIP__BASE__COMPILER_ISOC_SQRT(value);
    #else
      return isoc::sqrt(value);
    #endif
  }

  /**
    Returns the square root of the value.
  */
  static inline long double sqrt(long double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_SQRTL)
      return _DK_SDU_MIP__BASE__COMPILER_ISOC_SQRTL(value);
    #else
      return isoc::sqrtl(value);
    #endif
  }

  /**
    Returns the cube root of the value.
  */
  static inline float cbrt(float value) throw() {
    return isoc::cbrtf(value);
  }

  /**
    Returns the cube root of the value.
  */
  static inline double cbrt(double value) throw() {
    return isoc::cbrt(value);
  }

  /**
    Returns the cube root of the value.
  */
  static inline long double cbrt(long double value) throw() {
    return isoc::cbrtl(value);
  }

  /**
    Returns the Euclidian distance - sqrt(x^2 + y^2).
  */
  static inline float hypot(float x, float y) throw() {
    return isoc::hypotf(x, y);
  }

  /**
    Returns the Euclidian distance - sqrt(x^2 + y^2).
  */
  static inline double hypot(double x, double y) throw() {
    return isoc::hypot(x, y);
  }

  /**
    Returns the Euclidian distance - sqrt(x^2 + y^2).
  */
  static inline long double hypot(long double x, long double y) throw() {
    return isoc::hypotl(x, y);
  }

  /**
    Returns the exponential of the value.
  */
  static inline float exp(float value) throw() {
    return isoc::expf(value);
  }

  /**
    Returns the exponential of the value.
  */
  static inline double exp(double value) throw() {
    return isoc::exp(value);
  }

  /**
    Returns the exponential of the value.
  */
  static inline long double exp(long double value) throw() {
    return isoc::expl(value);
  }

  /**
    Returns the natural logarithm of the value.
  */
  static inline float log(float value) throw() {
    return isoc::logf(value);
  }

  /**
    Returns the natural logarithm of the value.
  */
  static inline double log(double value) throw() {
    return isoc::log(value);
  }

  /**
    Returns the natural logarithm of the value.
  */
  static inline long double log(long double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_LOGL)
      return _DK_SDU_MIP__BASE__COMPILER_ISOC_LOGL(value);
    #else
      return isoc::logl(value);
    #endif
  }

  /**
    Returns the logarithm with base 2 of the value.
  */
  static inline float log2(float value) throw() {
    return isoc::log2f(value);
  }

  /**
    Returns the logarithm with base 2 of the value.
  */
  static inline double log2(double value) throw() {
    return isoc::log2(value);
  }

  /**
    Returns the logarithm with base 2 of the value.
  */
  static inline long double log2(long double value) throw() {
    return isoc::log2l(value);
  }

  /**
    Returns the logarithm with base 10 of the value.
  */
  static inline float log10(float value) throw() {
    return isoc::log10f(value);
  }

  /**
    Returns the logarithm with base 10 of the value.
  */
  static inline double log10(double value) throw() {
    return isoc::log10(value);
  }

  /**
    Returns the logarithm with base 10 of the value.
  */
  static inline long double log10(long double value) throw() {
    return isoc::log10l(value);
  }

  /**
    Returns the power a^b.
  */
  static inline float pow(float a, float b) throw() {
    return isoc::powf(a, b);
  }

  /**
    Returns the power a^b.
  */
  static inline double pow(double a, double b) throw() {
    return isoc::pow(a, b);
  }

  /**
    Returns the power a^b.
  */
  static inline long double pow(long double a, long double b) throw() {
    return isoc::powl(a, b);
  }



  /**
    Returns the sine.
  */
  static inline float sin(float value) throw() {
    #if defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_SINF)
      return _DK_SDU_MIP__BASE__COMPILER_ISOC_SINF(value);
    #else
      return isoc::sinf(value);
    #endif
  }

  /**
    Returns the sine.
  */
  static inline double sin(double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_SIN)
      return _DK_SDU_MIP__BASE__COMPILER_ISOC_SIN(value);
    #else
      return isoc::sin(value);
    #endif
  }

  /**
    Returns the sine.
  */
  static inline long double sin(long double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_SINL)
      return _DK_SDU_MIP__BASE__COMPILER_ISOC_SINL(value);
    #else
      return isoc::sinl(value);
    #endif
  }

  /**
    Returns the inverse sine.
  */
  static inline float asin(float value) throw() {
    return isoc::asinf(value);
  }

  /**
    Returns the inverse sine.
  */
  static inline double asin(double value) throw() {
    return isoc::asin(value);
  }

  /**
    Returns the inverse sine.
  */
  static inline long double asin(long double value) throw() {
    return isoc::asinl(value);
  }

  /**
    Returns the cosine.
  */
  static inline float cos(float value) throw() {
    #if defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_COSF)
      return _DK_SDU_MIP__BASE__COMPILER_ISOC_COSF(value);
    #else
      return isoc::cosf(value);
    #endif
  }

  /**
    Returns the cosine.
  */
  static inline double cos(double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_COS)
      return _DK_SDU_MIP__BASE__COMPILER_ISOC_COS(value);
    #else
      return isoc::cos(value);
    #endif
  }

  /**
    Returns the cosine.
  */
  static inline long double cos(long double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_COSL)
      return _DK_SDU_MIP__BASE__COMPILER_ISOC_COSL(value);
    #else
      return isoc::cosl(value);
    #endif
  }

  /**
    Returns the inverse cosine.
  */
  static inline float acos(float value) throw() {
    return isoc::acosf(value);
  }

  /**
    Returns the inverse cosine.
  */
  static inline double acos(double value) throw() {
    return isoc::acos(value);
  }

  /**
    Returns the inverse cosine.
  */
  static inline long double acos(long double value) throw() {
    return isoc::acosl(value);
  }

  /**
    Returns the tangent.
  */
  static inline float tan(float value) throw() {
    return isoc::tanf(value);
  }

  /**
    Returns the tangent.
  */
  static inline double tan(double value) throw() {
    return isoc::tan(value);
  }

  /**
    Returns the tangent.
  */
  static inline long double tan(long double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_TANL)
      return _DK_SDU_MIP__BASE__COMPILER_ISOC_TANL(value);
    #else
      return isoc::tanl(value);
    #endif
  }

  /**
    Returns the inverse tangent.
  */
  static inline float atan(float value) throw() {
    return isoc::atanf(value);
  }

  /**
    Returns the inverse tangent.
  */
  static inline double atan(double value) throw() {
    return isoc::atan(value);
  }

  /**
    Returns the inverse tangent.
  */
  static inline long double atan(long double value) throw() {
    return isoc::atanl(value);
  }

  /**
    Returns the arc tangent of y/x using the signs of both arguments to determine the proper quadrant.
  */
  static inline float atan2(float y, float x) throw() {
    #if defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_ATAN2F)
      return _DK_SDU_MIP__BASE__COMPILER_ISOC_ATAN2F(y, x);
    #else
      return isoc::atan2f(y, x);
    #endif
  }

  /**
    Returns the arc tangent of y/x using the signs of both arguments to determine the proper quadrant.
  */
  static inline double atan2(double y, double x) throw() {
    return isoc::atan2(y, x);
  }

  /**
    Returns the arc tangent of y/x using the signs of both arguments to determine the proper quadrant.
  */
  static inline long double atan2(long double y, long double x) throw() {
    #if defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_ATAN2L)
      return _DK_SDU_MIP__BASE__COMPILER_ISOC_ATAN2L(y, x);
    #else
      return isoc::atan2l(y, x);
    #endif
  }



  /**
    Returns the hyperbolic sine.
  */
  static inline float sinh(float value) throw() {
    return isoc::sinhf(value);
  }

  /**
    Returns the hyperbolic sine.
  */
  static inline double sinh(double value) throw() {
    return isoc::sinh(value);
  }

  /**
    Returns the hyperbolic sine.
  */
  static inline long double sinh(long double value) throw() {
    return isoc::sinhl(value);
  }

  /**
    Returns the inverse hyperbolic sine.
  */
  static inline float asinh(float value) throw() {
    return isoc::asinhf(value);
  }

  /**
    Returns the inverse hyperbolic sine.
  */
  static inline double asinh(double value) throw() {
    return isoc::asinh(value);
  }

  /**
    Returns the inverse hyperbolic sine.
  */
  static inline long double asinh(long double value) throw() {
    return isoc::asinhl(value);
  }

  /**
    Returns the hyperbolic cosine.
  */
  static inline float cosh(float value) throw() {
    return isoc::coshf(value);
  }

  /**
    Returns the hyperbolic cosine.
  */
  static inline double cosh(double value) throw() {
    return isoc::cosh(value);
  }

  /**
    Returns the hyperbolic cosine.
  */
  static inline long double cosh(long double value) throw() {
    return isoc::coshl(value);
  }

  /**
    Returns the inverse hyperbolic cosine.
  */
  static inline float acosh(float value) throw() {
    return isoc::acoshf(value);
  }

  /**
    Returns the inverse hyperbolic cosine.
  */
  static inline double acosh(double value) throw() {
    return isoc::acosh(value);
  }

  /**
    Returns the inverse hyperbolic cosine.
  */
  static inline long double acosh(long double value) throw() {
    return isoc::acoshl(value);
  }

  /**
    Returns the hyperbolic tangent.
  */
  static inline float tanh(float value) throw() {
    return isoc::tanhf(value);
  }

  /**
    Returns the hyperbolic tangent.
  */
  static inline double tanh(double value) throw() {
    return isoc::tanh(value);
  }

  /**
    Returns the hyperbolic tangent.
  */
  static inline long double tanh(long double value) throw() {
    return isoc::tanhl(value);
  }

  /**
    Returns the inverse hyperbolic tangent.
  */
  static inline float atanh(float value) throw() {
    return isoc::atanhf(value);
  }

  /**
    Returns the inverse hyperbolic tangent.
  */
  static inline double atanh(double value) throw() {
    return isoc::atanh(value);
  }

  /**
    Returns the inverse hyperbolic tangent.
  */
  static inline long double atanh(long double value) throw() {
    return isoc::atanhl(value);
  }



  /**
    Returns the square root of the specified integer. The result is round down.
  */
  static inline unsigned int isqrt(unsigned int value) throw() {
    unsigned int accumulator = 0;
    unsigned int remainder = 0;
    for (unsigned int i = sizeof(unsigned int)*8/2; i > 0; --i) {
      remainder =  (remainder << 2) | (value >> (sizeof(unsigned int)*8 - 2));
      value <<= 2;
      accumulator <<= 1;
      unsigned int e = accumulator << 1;
      if (remainder > e) {
        remainder -= e + 1;
        ++accumulator;
      }
    }
    return accumulator;
  }
  
  /**
    Returns the square root of the specified integer with 8 bit precision. The
    result is round down.
  */
  static inline unsigned int isqrt8(unsigned int value) throw() {
    unsigned int accumulator = 0;
    unsigned int remainder = 0;
    for (unsigned int i = sizeof(unsigned int)*8/2 + 8; i > 0; --i) {
      remainder =  (remainder << 2) | (value >> (sizeof(unsigned int)*8 - 2));
      value <<= 2;
      accumulator <<= 1;
      unsigned int e = accumulator << 1;
      if (remainder > e) {
        remainder -= e + 1;
        ++accumulator;
      }
    }
    return accumulator;
  }

  /**
    Returns the square root of the specified integer with 16 bit precision. The
    result is round down.
  */
  static inline unsigned int isqrt16(unsigned int value) throw() {
    unsigned int accumulator = 0;
    unsigned int remainder = 0;
    for (unsigned int i = sizeof(unsigned int)*8/2 + 16; i > 0; --i) {
      remainder =  (remainder << 2) | (value >> (sizeof(unsigned int)*8 - 2));
      value <<= 2;
      accumulator <<= 1;
      unsigned int e = accumulator << 1;
      if (remainder > e) {
        remainder -= e + 1;
        ++accumulator;
      }
    }
  }
  
  /**
    Returns the square root of the specified integer with 8 bit precision. The
    result is rounded to the nearest 256th of an integer.
  */
  static inline unsigned int isqrt8Round(unsigned int value) throw() {
    unsigned int accumulator = 0;
    unsigned int remainder = 0;
    for (unsigned int i = sizeof(unsigned int)*8/2 + 8; i > 0; --i) {
      remainder =  (remainder << 2) | (value >> (sizeof(unsigned int)*8 - 2));
      value <<= 2;
      accumulator <<= 1;
      unsigned int e = accumulator << 1;
      if (remainder > e) {
        remainder -= e + 1;
        ++accumulator;
      }
    }
    remainder = (remainder << 2) | (value >> (sizeof(unsigned int)*8 - 2));
    if (remainder > (accumulator << 2)) {
      ++accumulator; // round up
    }
    return accumulator;
  }

  /**
    Returns true if the specified integer is a power of 2.
  */
  static inline bool isPowerOf2(unsigned char value) throw() {
    const unsigned int IS_POWER_OF_2 = 0x022c; // shifted by left by 1
    if (value & 0xf0 == 0) {
      return IS_POWER_OF_2 & (1U << value);
    } else if (value & 0x0f == 0) {
      return IS_POWER_OF_2 & (1U << (value >> 4));
    } else {
      return false;
    }
  }

  /**
    Returns true if the specified integer is a power of 2.
  */
  static inline bool isPowerOf2(unsigned int value) throw() {
    if (value != 0) {
      while (value & 0xff == 0) {
        value >>= 8;
      }
      if (value <= 0xff) {
        return isPowerOf2(static_cast<unsigned char>(value));
      }
    }
    return false;
  }

  /**
    Returns true if the specified integer is a power of 2.
  */
  static inline bool isPowerOf2(unsigned long long value) throw() {
    if (value != 0) {
      while (value & 0xff == 0) {
        value >>= 8;
      }
      if (value <= 0xff) {
        return isPowerOf2(static_cast<unsigned char>(value));
      }
    }
    return false;
  }

  /**
    Returns the bit reversal of the specified 8 bit unsigned integer.
  */
  static inline uint8 getBitReversal(uint8 value) throw() {
    return BIT_REVERSAL[value];
  }

  /**
    Returns the bit reversal of the specified 16 bit unsigned integer.
  */
  static inline uint16 getBitReversal(uint16 value) throw() {
    return merge(getBitReversal(getHighWordOf16(value)), getBitReversal(getLowWordOf16(value)));
  }
  
  /**
    Returns the bit reversal of the specified 32 bit unsigned integer.
  */
  static inline uint32 getBitReversal(uint32 value) throw() {
    return merge(getBitReversal(getHighWordOf32(value)), getBitReversal(getLowWordOf32(value)));
  }

  /**
    Returns the bit reversal of the specified 64 bit unsigned integer.
  */
  static inline uint64 getBitReversal(uint64 value) throw() {
    return merge(getBitReversal(getHighWordOf64(value)), getBitReversal(getLowWordOf64(value)));
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
