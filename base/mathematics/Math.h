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

#include <base/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

namespace isoc {
  extern "C" float fabsf(float);
  extern "C" double fabs(double);
  extern "C" long double fabsl(long double);

  extern "C" float sqrtf(float);
  extern "C" double sqrt(double);
  extern "C" long double sqrtl(long double);

  extern "C" float expf(float);
  extern "C" double exp(double);
  extern "C" long double expl(long double);

  extern "C" float logf(float);
  extern "C" double log(double);
  extern "C" long double logl(long double);

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
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Math {
public:

  static float inline abs(float value) throw() {
    return isoc::fabsf(value);
  }

  static double inline abs(double value) throw() {
    return isoc::fabs(value);
  }

  static long double inline abs(long double value) throw() {
    return isoc::fabsl(value);
  }

  static float inline sqrt(float value) throw() {
    return isoc::sqrtf(value);
  }

  static double inline sqrt(double value) throw() {
    return isoc::sqrt(value);
  }

  static long double inline sqrt(long double value) throw() {
    return isoc::sqrtl(value);
  }


  static float inline exp(float value) throw() {
    return isoc::expf(value);
  }

  static double inline exp(double value) throw() {
    return isoc::exp(value);
  }

  static long double inline exp(long double value) throw() {
    return isoc::expl(value);
  }

  static float inline log(float value) throw() {
    return isoc::logf(value);
  }

  static double inline log(double value) throw() {
    return isoc::log(value);
  }

  static long double inline log(long double value) throw() {
    return isoc::logl(value);
  }

  static float inline pow(float a, float b) throw() {
    return isoc::powf(a, b);
  }

  static double inline pow(double a, double b) throw() {
    return isoc::pow(a, b);
  }

  static long double inline pow(long double a, long double b) throw() {
    return isoc::powl(a, b);
  }


  static float inline sin(float value) throw() {
    return isoc::sinf(value);
  }

  static double inline sin(double value) throw() {
    return isoc::sin(value);
  }

  static long double inline sin(long double value) throw() {
    return isoc::sinl(value);
  }

  static float inline asin(float value) throw() {
    return isoc::asinf(value);
  }

  static double inline asin(double value) throw() {
    return isoc::asin(value);
  }

  static long double inline asin(long double value) throw() {
    return isoc::asinl(value);
  }


  static float inline cos(float value) throw() {
    return isoc::cosf(value);
  }

  static double inline cos(double value) throw() {
    return isoc::cos(value);
  }

  static long double inline cos(long double value) throw() {
    return isoc::cosl(value);
  }

  static float inline acos(float value) throw() {
    return isoc::acosf(value);
  }

  static double inline acos(double value) throw() {
    return isoc::acos(value);
  }

  static long double inline acos(long double value) throw() {
    return isoc::acosl(value);
  }


  static float inline tan(float value) throw() {
    return isoc::tanf(value);
  }

  static double inline tan(double value) throw() {
    return isoc::tan(value);
  }

  static long double inline tan(long double value) throw() {
    return isoc::tanl(value);
  }

  static float inline atan(float value) throw() {
    return isoc::atanf(value);
  }

  static double inline atan(double value) throw() {
    return isoc::atan(value);
  }

  static long double inline atan(long double value) throw() {
    return isoc::atanl(value);
  }



  static float inline sinh(float value) throw() {
    return isoc::sinhf(value);
  }

  static double inline sinh(double value) throw() {
    return isoc::sinh(value);
  }

  static long double inline sinh(long double value) throw() {
    return isoc::sinhl(value);
  }

  static float inline asinh(float value) throw() {
    return isoc::asinhf(value);
  }

  static double inline asinh(double value) throw() {
    return isoc::asinh(value);
  }

  static long double inline asinh(long double value) throw() {
    return isoc::asinhl(value);
  }


  static float inline cosh(float value) throw() {
    return isoc::coshf(value);
  }

  static double inline cosh(double value) throw() {
    return isoc::cosh(value);
  }

  static long double inline cosh(long double value) throw() {
    return isoc::coshl(value);
  }

  static float inline acosh(float value) throw() {
    return isoc::acoshf(value);
  }

  static double inline acosh(double value) throw() {
    return isoc::acosh(value);
  }

  static long double inline acosh(long double value) throw() {
    return isoc::acoshl(value);
  }


  static float inline tanh(float value) throw() {
    return isoc::tanhf(value);
  }

  static double inline tanh(double value) throw() {
    return isoc::tanh(value);
  }

  static long double inline tanh(long double value) throw() {
    return isoc::tanhl(value);
  }

  static float inline atanh(float value) throw() {
    return isoc::atanhf(value);
  }

  static double inline atanh(double value) throw() {
    return isoc::atanh(value);
  }

  static long double inline atanh(long double value) throw() {
    return isoc::atanhl(value);
  }


  static float inline atan2(float y, float x) throw() {
    return isoc::atan2f(y, x);
  }

  static double inline atan2(double y, double x) throw() {
    return isoc::atan2(y, x);
  }

  static long double inline atan2(long double y, long double x) throw() {
    return isoc::atan2l(y, x);
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
    Returns the specified integer if it's a power of 2 and otherwise returns 0.
  */
  inline unsigned char isPowerOf2(unsigned char value) throw() {
    static const bool IS_POWER[16] = {
      0, 1, 2, 0, 4, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0
    };
    if (value & 0x0f == 0) {
      return IS_POWER[value >> 4];
    } else if (value & 0xf0 == 0) {
      return IS_POWER[value];
    } else {
      return 0;
    }
  }

  /**
    Returns the specified integer if it's a power of 2 and otherwise returns 0.
  */
  static inline unsigned int isPowerOf2(unsigned int value) throw() {
    if (value != 0) {
      unsigned int shift = 0;
      while (value & 0xff == 0) {
        value >>= 8;
        shift += 8;
      }
      if (value <= 0xff) {
        return static_cast<unsigned int>(isPowerOf2(value)) << shift;
      }
    }
    return 0;
  }

  /**
    Returns the specified integer if it's a power of 2 and otherwise returns 0.
  */
  static inline unsigned long long isPowerOf2(unsigned long long value) throw() {
    if (value != 0) {
      unsigned int shift = 0;
      while (value & 0xff == 0) {
        value >>= 8;
        shift += 8;
      }
      if (value <= 0xff) {
        return static_cast<unsigned long long>(isPowerOf2(value)) << shift;
      }
    }
    return 0;
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
