/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Primitives.h>
#include <base/mathematics/Constants.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  define _DK_SDU_MIP__BASE__REDIR_ISOC_HYPOTF(x, y) _hypotf(x, y)
#  define _DK_SDU_MIP__BASE__REDIR_ISOC_FABSF(x) (float)fabs(x)
#  define _DK_SDU_MIP__BASE__REDIR_ISOC_FABSL(x) fabs((double)x)
#  define _DK_SDU_MIP__BASE__REDIR_ISOC_CEILL(x) ceil((double)x)
#  define _DK_SDU_MIP__BASE__REDIR_ISOC_FLOORL(x) floor((double)x)
#endif

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

  extern "C" float cbrtf(float);
  extern "C" double cbrt(double);

  extern "C" float hypotf(float, float);
  extern "C" double hypot(double, double);

  extern "C" float expf(float);
  extern "C" double exp(double);

  extern "C" float logf(float);
  extern "C" double log(double);

  extern "C" float log2f(float);
  extern "C" double log2(double);

  extern "C" float log10f(float);
  extern "C" double log10(double);

  extern "C" float powf(float, float);
  extern "C" double pow(double, double);

  extern "C" float sinf(float);
  extern "C" double sin(double);

  extern "C" float cosf(float);
  extern "C" double cos(double);

  extern "C" float asinf(float);
  extern "C" double asin(double);

  extern "C" float acosf(float);
  extern "C" double acos(double);

  extern "C" float sinhf(float);
  extern "C" double sinh(double);

  extern "C" float coshf(float);
  extern "C" double cosh(double);

  extern "C" float asinhf(float);
  extern "C" double asinh(double);

  extern "C" float acoshf(float);
  extern "C" double acosh(double);

  extern "C" float tanf(float);
  extern "C" double tan(double);

  extern "C" float atanf(float);
  extern "C" double atan(double);

  extern "C" float tanhf(float);
  extern "C" double tanh(double);

  extern "C" float atanhf(float);
  extern "C" double atanh(double);

  extern "C" float atan2f(float, float);
  extern "C" double atan2(double, double);

#if 1
  inline long double sqrtl(long double x) {
    return sqrt(static_cast<double>(x));
  }

  inline long double cbrtl(long double x) {
    return cbrt(static_cast<double>(x));
  }

  inline long double hypotl(long double x, long double y) {
    return hypot(static_cast<double>(x), static_cast<double>(y));
  }

  inline long double expl(long double x) {
    return exp(static_cast<double>(x));
  }

  inline long double logl(long double x) {
    return log(static_cast<double>(x));
  }

  inline long double log2l(long double x) {
    return log2(static_cast<double>(x));
  }

  inline long double log10l(long double x) {
    return log10(static_cast<double>(x));
  }

  inline long double powl(long double x, long double y) {
    return pow(static_cast<double>(x), static_cast<double>(y));
  }

  inline long double sinl(long double x) {
    return sin(static_cast<double>(x));
  }

  inline long double cosl(long double x) {
    return cos(static_cast<double>(x));
  }

  inline long double asinl(long double x) {
    return asin(static_cast<double>(x));
  }

  inline long double acosl(long double x) {
    return acos(static_cast<double>(x));
  }

  inline long double sinhl(long double x) {
    return sinh(static_cast<double>(x));
  }

  inline long double coshl(long double x) {
    return cosh(static_cast<double>(x));
  }

  inline long double asinhl(long double x) {
    return asinh(static_cast<double>(x));
  }

  inline long double acoshl(long double x) {
    return acosh(static_cast<double>(x));
  }

  inline long double tanl(long double x) {
    return tan(static_cast<double>(x));
  }

  inline long double atanl(long double x) {
    return atan(static_cast<double>(x));
  }

  inline long double tanhl(long double x) {
    return tanh(static_cast<double>(x));
  }

  inline long double atanhl(long double x) {
    return atanh(static_cast<double>(x));
  }

  inline long double atan2l(long double x, long double y) {
    return atan2(static_cast<double>(x), static_cast<double>(y));
  }

#else
  extern "C" long double sqrtl(long double);
  extern "C" long double cbrtl(long double);
  extern "C" long double hypotl(long double, long double);
  extern "C" long double expl(long double);
  extern "C" long double logl(long double);
  extern "C" long double log2l(long double);
  extern "C" long double log10l(long double);
  extern "C" long double powl(long double, long double);
  extern "C" long double sinl(long double);
  extern "C" long double cosl(long double);
  extern "C" long double asinl(long double);
  extern "C" long double acosl(long double);
  extern "C" long double sinhl(long double);
  extern "C" long double coshl(long double);
  extern "C" long double asinhl(long double);
  extern "C" long double acoshl(long double);
  extern "C" long double tanl(long double);
  extern "C" long double atanl(long double);
  extern "C" long double tanhl(long double);
  extern "C" long double atanhl(long double);
  extern "C" long double atan2l(long double, long double);
#endif
};

/**
  This class is a placeholder for mathematical functions.

  @short Mathematical functions
  @ingroup mathematics
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API Math {
private:

  /** Bit reversal lookup table (8 bit). */
  static const uint8 BIT_REVERSAL[256];
public:
  
  /**
    Returns the absolute value.
  */
  static inline float abs(float value) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_FABSF)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_FABSF(value);
    #else
      return isoc::fabsf(value);
    #endif
  }

  /**
    Returns the absolute value.
  */
  static inline double abs(double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_FABS)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_FABS(value);
    #else
      return isoc::fabs(value);
    #endif
  }

  /**
    Returns the absolute value.
  */
  static inline long double abs(long double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_FABSL)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_FABSL(value);
    #else
      return isoc::fabsl(value);
    #endif
  }

  /**
    Rounds (towards +infinity) the value to the nearest integer.
  */
  static inline float ceil(float value) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_CEILF)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_CEILF(value);
    #else
      return isoc::ceilf(value);
    #endif
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
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_CEILL)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_CEILL(value);
    #else
      return isoc::ceill(value);
    #endif
  }

  /**
    Rounds (towards -infinity) the value to the nearest integer.
  */
  static inline float floor(float value) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_FLOORF)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_FLOORF(value);
    #else
      return isoc::floorf(value);
    #endif
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
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_FLOORL)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_FLOORL(value);
    #else
      return isoc::floorl(value);
    #endif
  }

  /**
    Rounds the value to the nearest integer (away from 0).
  */
  static inline float round(float value) throw() {
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_ROUNDF)
      return isoc::roundf(value);
    #elif defined(_DK_SDU_MIP__BASE__HAVE_ISOC_ROUNDL)
      return isoc::roundl(value);
    #elif defined(_DK_SDU_MIP__BASE__HAVE_ISOC_ROUND)
      return isoc::round(value);
    #else
      return (value >= 0) ? Math::ceil(value) : Math::floor(value);
    #endif
  }
  
  /**
    Rounds the value to the nearest integer.
  */
  static inline double round(double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_ROUND)
      return isoc::round(value);
    #elif defined(_DK_SDU_MIP__BASE__HAVE_ISOC_ROUNDL)
      return isoc::roundl(value);
    #elif defined(_DK_SDU_MIP__BASE__HAVE_ISOC_ROUNDF)
      return isoc::roundf(value);
    #else
      return 0; // TAG: fixme
    #endif
  }
  
  /**
    Rounds the value to the nearest integer.
  */
  static inline long double round(long double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_ROUNDL)
      return isoc::roundl(value);
    #elif defined(_DK_SDU_MIP__BASE__HAVE_ISOC_ROUND)
      return isoc::round(value);
    #elif defined(_DK_SDU_MIP__BASE__HAVE_ISOC_ROUNDF)
      return isoc::roundf(value);
    #else
      return 0; // TAG: fixme
    #endif
  }
  
  /**
    Rounds (towards zero) the value to the nearest integer.
  */
  static inline float trunc(float value) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_TRUNCF)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_TRUNCF(value);
    #else
      return isoc::truncf(value);
    #endif
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
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_TRUNCL)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_TRUNCL(value);
    #else
      return isoc::truncl(value);
    #endif
  }



  /**
    Returns the square root of the value.
  */
  static inline float sqrt(float value) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_SQRTF)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_SQRTF(value);
    #else
      return isoc::sqrtf(value);
    #endif
  }

  /**
    Returns the square root of the value.
  */
  static inline double sqrt(double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_SQRT)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_SQRT(value);
    #else
      return isoc::sqrt(value);
    #endif
  }

  /**
    Returns the square root of the value.
  */
  static inline long double sqrt(long double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_SQRTL)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_SQRTL(value);
    #else
      return isoc::sqrtl(value);
    #endif
  }

  /**
    Returns the cube root of the value.
  */
  static inline float cbrt(float value) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_CBRTF)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_CBRTF(value);
    #else
      return isoc::cbrtf(value);
    #endif
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
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_CBRTL)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_CBRTL(value);
    #else
      return isoc::cbrtl(value);
    #endif
  }

  /**
    Returns the Euclidian distance - sqrt(x^2 + y^2).
  */
  static inline float hypot(float x, float y) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_HYPOTF)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_HYPOTF(x, y);
    #else
      return isoc::hypotf(x, y);
    #endif
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
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_HYPOTL)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_HYPOTL(x, y);
    #else
      return isoc::hypotl(x, y);
    #endif
  }

  /**
    Returns the exponential of the value.
  */
  static inline float exp(float value) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_EXPF)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_EXPF(value);
    #else
      return isoc::expf(value);
    #endif
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
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_EXPL)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_EXPL(value);
    #else
      return isoc::expl(value);
    #endif
  }

  /**
    Returns the natural logarithm of the value.
  */
  static inline float ln(float value) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_LOGF)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_LOGF(value);
    #else
      return isoc::logf(value);
    #endif
  }

  /**
    Returns the natural logarithm of the value.
  */
  static inline double ln(double value) throw() {
    return isoc::log(value);
  }

  /**
    Returns the natural logarithm of the value.
  */
  static inline long double ln(long double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_LOGL)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_LOGL(value);
    #else
      return isoc::logl(value);
    #endif
  }

  /**
    Returns the logarithm with base 2 of the value.
  */
  static inline float log2(float value) throw() {
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_LOG2F)
      return isoc::log2f(value);
    #elif defined(_DK_SDU_MIP__BASE__HAVE_ISOC_LOG2L)
      return isoc::log2l(value);
    #elif defined(_DK_SDU_MIP__BASE__HAVE_ISOC_LOG2)
      return isoc::log2(value);
    #else
      return Math::ln(value) * constant::LOG2E;
    #endif
  }

  /**
    Returns the logarithm with base 2 of the value.
  */
  static inline double log2(double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_LOG2)
      return isoc::log2(value);
    #elif defined(_DK_SDU_MIP__BASE__HAVE_ISOC_LOG2L)
      return isoc::log2l(value);
    #elif defined(_DK_SDU_MIP__BASE__HAVE_ISOC_LOG2F)
      return isoc::log2f(value);
    #else
      return Math::ln(value) * constant::LOG2E;
    #endif
  }

  /**
    Returns the logarithm with base 2 of the value.
  */
  static inline long double log2(long double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_LOG2L)
      return isoc::log2l(value);
    #elif defined(_DK_SDU_MIP__BASE__HAVE_ISOC_LOG2)
      return isoc::log2(value);
    #elif defined(_DK_SDU_MIP__BASE__HAVE_ISOC_LOG2F)
      return isoc::log2f(value);
    #else
      return Math::ln(value) * constant::LOG2E;
    #endif
  }
  
  /**
    Returns the logarithm with base 10 of the value.
  */
  static inline float log10(float value) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_LOG10F)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_LOG10F(value);
    #else
      #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_LOG10F)
        return isoc::log10f(value);
      #else
        #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_LOGF)
          return _DK_SDU_MIP__BASE__REDIR_ISOC_LOGF(value) * constant::LOG10E;
        #else
          return isoc::logf(value) * constant::LOG10E;
        #endif
      #endif
    #endif
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
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_LOG10L)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_LOG10L(value);
    #else
      #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_LOG10L)
        return isoc::log10l(value);
      #else
        #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_LOGL)
          return _DK_SDU_MIP__BASE__REDIR_ISOC_LOGL(value) * constant::LOG10E;
        #else
          return isoc::logl(value) * constant::LOG10E;
        #endif
      #endif
    #endif
  }

  /**
    Returns the logarithm with base n of the value.
  */
  static inline long double logn(long double value, long double base) throw() {
    return Math::ln(value)/base;
  }
  
  /**
    Returns the power a^b.
  */
  static inline float power(float a, float b) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_POWF)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_POWF(a, b);
    #else
      return isoc::powf(a, b);
    #endif
  }

  /**
    Returns the power a^b.
  */
  static inline double power(double a, double b) throw() {
    return isoc::pow(a, b);
  }

  /**
    Returns the power a^b.
  */
  static inline long double power(long double a, long double b) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_POWL)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_POWL(a, b);
    #else
      return isoc::powl(a, b);
    #endif
//       if (a > 0) {
//         return Math::exp(b * Math::ln(a));
//       } else if (a == 0) {
//         if (b > 0) {
//           return 0;
//         } else if (b == 0) {
//           return 1;
//         } else {
//           // out of domain
//         }
//       } else if (Math::frac(b) == 0) {
//         if (Math::frac(b * 0.5) == 0) { // equal or unequal
//           return Math::exp(b * Math::ln(-a));
//         } else {
//           return -Math::exp(b * Math::ln(-a));
//         }
//       } else {
//         // out of domain
//       }
  }



  /**
    Returns the sine.
  */
  static inline float sin(float value) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_SINF)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_SINF(value);
    #else
      return isoc::sinf(value);
    #endif
  }

  /**
    Returns the sine.
  */
  static inline double sin(double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_SIN)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_SIN(value);
    #else
      return isoc::sin(value);
    #endif
  }

  /**
    Returns the sine.
  */
  static inline long double sin(long double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_SINL)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_SINL(value);
    #else
      return isoc::sinl(value);
    #endif
  }

  /**
    Returns the inverse sine.
  */
  static inline float asin(float value) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_ASINF)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_ASINF(value);
    #else
      return isoc::asinf(value);
    #endif
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
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_ASINL)
      return isoc::asinl(value);
    #elif defined(_DK_SDU_MIP__BASE__REDIR_ISOC_ASINL)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_ASINL(value);
    #else
      return Math::atan2(value, Math::sqrt(1 - value * value));
      // return Math::atan(value/Math::sqrt((1 - value * value)));
    #endif
  }

  /**
    Returns the cosine.
  */
  static inline float cos(float value) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_COSF)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_COSF(value);
    #else
      return isoc::cosf(value);
    #endif
  }

  /**
    Returns the cosine.
  */
  static inline double cos(double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_COS)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_COS(value);
    #else
      return isoc::cos(value);
    #endif
  }

  /**
    Returns the cosine.
  */
  static inline long double cos(long double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_COSL)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_COSL(value);
    #else
      return isoc::cosl(value);
    #endif
  }

  /**
    Returns the inverse cosine.
  */
  static inline float acos(float value) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_ACOSF)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_ACOSF(value);
    #else
      return isoc::acosf(value);
    #endif
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
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_ACOSL)
      return isoc::acosl(value);
    #elif defined(_DK_SDU_MIP__BASE__REDIR_ISOC_ACOSL)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_ACOSL(value);
    #else
      return Math::atan2(Math::sqrt(1 - value * value), value);
      // return constant::PI_2 - Math::atan(value/Math::sqrt((1 - value * value)));
    #endif
  }

  /**
    Returns the tangent.
  */
  static inline float tan(float value) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_TANF)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_TANF(value);
    #else
      return isoc::tanf(value);
    #endif
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
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_TANL)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_TANL(value);
    #else
      return isoc::tanl(value);
    #endif
  }
  
  
  /**
    Returns the inverse tangent.
  */
  static inline float atan(float value) throw() {
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_ATANF)
      return isoc::atanf(value);
    #elif defined(_DK_SDU_MIP__BASE__REDIR_ISOC_ATANF)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_ATANF(value);
    #else
      return isoc::atan(value);
    #endif
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
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_ATANL)
      return isoc::atanl(value);
    #elif defined(_DK_SDU_MIP__BASE__REDIR_ISOC_ATANL)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_ATANL(value);
    #else
      return isoc::atan(value);
    #endif
  }
  
  /**
    Returns the cotangent of the value.
  */
  static inline long double cotan(long double value) throw() {
    return 1/Math::tan(value);
  }
  
  /**
    Returns the inverse cotangent of the value.
  */
  static inline long double acotan(long double value) throw() {
    return Math::atan2(1, value);
  }
  
  /**
    Returns the secant of the value.
  */
  static inline long double sec(long double value) throw() {
    return 1/Math::cos(value);
  }
  
  /**
    Returns the inverse secant of the value.
  */
  static inline long double asec(long double value) throw() {
    return Math::atan2(value, Math::sqrt(1 - value * value)); // acos(1/value)
  }
  
  /**
    Returns the cosecant of the value.
  */
  static inline long double cosec(long double value) throw() {
    return 1/Math::sin(value);
  }
  
  /**
    Returns the inverse cosecant of the value.
  */
  static inline long double acosec(long double value) throw() {
    return Math::atan2(Math::sqrt(1 - value * value), value); // asin(1/value)
  }
  
  /**
    Returns the exsecant of the value.
  */
  static inline long double exsec(long double value) throw() {
    return 1/Math::cos(value) - 1;
  }
  
  /**
    Returns the inverse exsecant of the value.
  */
  static inline long double aexsec(long double value) throw() {
    return Math::atan(Math::sqrt(value * (value + 2)));
  }
  
  /**
    Returns versine of value.
  */
  static inline long double vers(long double value) throw() {
    return 1 - Math::cos(value);
  }
  
  /**
    Returns the inverse versine of the value.
  */
  static inline long double avers(long double value) throw() {
    return Math::atan(Math::sqrt((2 - value) * value/((value - 1)*(value - 1))));
  }
  
  /**
    Returns the haversine of value.
  */
  static inline long double hav(long double value) throw() {
    return (1 - Math::cos(value))/2;
  }
  
  /**
    Returns the inverse hav of the value.
  */
  static inline long double ahav(long double value) throw() {
    return Math::atan(Math::sqrt(1/((1 - 2 * value) * (1 - 2 * value)) - 1));
  }
  
  /**
    Returns the coversine of value.
  */
  static inline long double covers(long double value) throw() {
    return 1 - Math::sin(value);
  }
  
  /**
    Returns the inverse coversine of the value.
  */
  static inline long double acovers(long double value) throw() {
    return Math::atan(Math::sqrt(-1/(value * (value - 2)) - 1));
  }
  
  /**
    Returns the arc tangent of y/x using the signs of both arguments to
    determine the proper quadrant.
  */
  static inline float atan2(float y, float x) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_ATAN2F)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_ATAN2F(y, x);
    #else
      return isoc::atan2f(y, x);
    #endif
  }

  /**
    Returns the arc tangent of y/x using the signs of both arguments to
    determine the proper quadrant.
  */
  static inline double atan2(double y, double x) throw() {
    return isoc::atan2(y, x);
  }

  /**
    Returns the arc tangent of y/x using the signs of both arguments to
    determine the proper quadrant.
  */
  static inline long double atan2(long double y, long double x) throw() {
    #if defined(_DK_SDU_MIP__BASE__REDIR_ISOC_ATAN2L)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_ATAN2L(y, x);
    #else
      return isoc::atan2l(y, x);
    #endif
  }



  /**
    Returns the hyperbolic sine.
  */
  static inline float sinh(float value) throw() {
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_SINHF)
      return isoc::sinhf(value);
    #elif defined(_DK_SDU_MIP__BASE__REDIR_ISOC_SINHF)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_SINHF(value);
    #else
      return (Math::exp(value) - Math::exp(-value))/2;
    #endif
  }

  /**
    Returns the hyperbolic sine.
  */
  static inline double sinh(double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_SINH)
      return isoc::sinh(value);
    #elif defined(_DK_SDU_MIP__BASE__REDIR_ISOC_SINH)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_SINH(value);
    #else
      return (Math::exp(value) - Math::exp(-value))/2;
    #endif
  }

  /**
    Returns the hyperbolic sine.
  */
  static inline long double sinh(long double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_SINH)
      return isoc::sinhl(value);
    #elif defined(_DK_SDU_MIP__BASE__REDIR_ISOC_SINHL)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_SINHL(value);
    #else
      return (Math::exp(value) - Math::exp(-value))/2;
    #endif
  }

  /**
    Returns the inverse hyperbolic sine.
  */
  static inline float asinh(float value) throw() {
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_ASINHF)
      return isoc::asinhf(value);
    #elif defined(_DK_SDU_MIP__BASE__REDIR_ISOC_ASINHF)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_ASINHF(value);
    #else
      return Math::ln(value + Math::sqrt(1 + value * value));
    #endif
  }

  /**
    Returns the inverse hyperbolic sine.
  */
  static inline double asinh(double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_ASINH)
      return isoc::asinh(value);
    #elif defined(_DK_SDU_MIP__BASE__REDIR_ISOC_ASINH)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_ASINH(value);
    #else
      return Math::ln(value + Math::sqrt(1 + value * value));
    #endif
  }

  /**
    Returns the inverse hyperbolic sine.
  */
  static inline long double asinh(long double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_ASINHL)
      return isoc::asinhl(value);
    #elif defined(_DK_SDU_MIP__BASE__REDIR_ISOC_ASINHL)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_ASINHL(value);
    #else
      return Math::ln(value + Math::sqrt(1 + value * value));
    #endif
  }

  /**
    Returns the hyperbolic cosine.
  */
  static inline float cosh(float value) throw() {
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_COSHF)
      return isoc::coshf(value);
    #elif defined(_DK_SDU_MIP__BASE__REDIR_ISOC_COSHF)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_COSHF(value);
    #else
      return (Math::exp(value) + Math::exp(-value))/2;
    #endif
  }

  /**
    Returns the hyperbolic cosine.
  */
  static inline double cosh(double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_COSH)
      return isoc::cosh(value);
    #elif defined(_DK_SDU_MIP__BASE__REDIR_ISOC_COSH)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_COSH(value);
    #else
      return (Math::exp(value) + Math::exp(-value))/2;
    #endif
  }

  /**
    Returns the hyperbolic cosine.
  */
  static inline long double cosh(long double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_COSHL)
      return isoc::coshl(value);
    #elif defined(_DK_SDU_MIP__BASE__REDIR_ISOC_COSHL)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_COSHL(value);
    #else
      return (Math::exp(value) + Math::exp(-value))/2;
    #endif
  }

  /**
    Returns the inverse hyperbolic cosine.
  */
  static inline float acosh(float value) throw() {
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_ACOSHF)
      return isoc::acoshf(value);
    #elif defined(_DK_SDU_MIP__BASE__REDIR_ISOC_ACOSHF)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_ACOSHF(value);
    #else
      return Math::ln(value + Math::sqrt(value * value - 1));
    #endif
  }

  /**
    Returns the inverse hyperbolic cosine.
  */
  static inline double acosh(double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_ACOSH)
      return isoc::acosh(value);
    #elif defined(_DK_SDU_MIP__BASE__REDIR_ISOC_ACOSH)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_ACOSH(value);
    #else
      return Math::ln(value + Math::sqrt(value * value - 1));
    #endif
  }

  /**
    Returns the inverse hyperbolic cosine.
  */
  static inline long double acosh(long double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_ACOSHL)
      return isoc::acoshl(value);
    #elif defined(_DK_SDU_MIP__BASE__REDIR_ISOC_ACOSHL)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_ACOSHL(value);
    #else
      return Math::ln(value + Math::sqrt(value * value - 1));
    #endif
  }

  /**
    Returns the hyperbolic tangent.
  */
  static inline float tanh(float value) throw() {
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_TANHF)
      return isoc::tanhf(value);
    #elif defined(_DK_SDU_MIP__BASE__REDIR_ISOC_TANHF)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_TANHF(value);
    #else
      const float temp = Math::exp(2 * value);
      return (temp - 1)/(temp + 1);
    #endif
  }

  /**
    Returns the hyperbolic tangent.
  */
  static inline double tanh(double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_TANH)
      return isoc::tanh(value);
    #elif defined(_DK_SDU_MIP__BASE__REDIR_ISOC_TANH)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_TANH(value);
    #else
      const double temp = Math::exp(2 * value);
      return (temp - 1)/(temp + 1);
    #endif
  }

  /**
    Returns the hyperbolic tangent.
  */
  static inline long double tanh(long double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_TANHL)
      return isoc::tanhl(value);
    #elif defined(_DK_SDU_MIP__BASE__REDIR_ISOC_TANHL)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_TANHL(value);
    #else
      const long double temp = Math::exp(2 * value);
      return (temp - 1)/(temp + 1);
    #endif
  }

  /**
    Returns the inverse hyperbolic tangent.
  */
  static inline float atanh(float value) throw() {
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_ATANHF)
      return isoc::atanhf(value);
    #elif defined(_DK_SDU_MIP__BASE__REDIR_ISOC_ATANHF)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_ATANHF(value);
    #else
      return Math::ln((1 + value)/(1 - value))/2;
    #endif
  }
  
  /**
    Returns the inverse hyperbolic tangent.
  */
  static inline double atanh(double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_ATANH)
      return isoc::atanh(value);
    #elif defined(_DK_SDU_MIP__BASE__REDIR_ISOC_ATANH)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_ATANH(value);
    #else
      return Math::ln((1 + value)/(1 - value))/2;
    #endif
  }

  /**
    Returns the inverse hyperbolic tangent.
  */
  static inline long double atanh(long double value) throw() {
    #if defined(_DK_SDU_MIP__BASE__HAVE_ISOC_ATANHL)
      return isoc::atanhl(value);
    #elif defined(_DK_SDU_MIP__BASE__REDIR_ISOC_ATANHL)
      return _DK_SDU_MIP__BASE__REDIR_ISOC_ATANHL(value);
    #else
      return Math::ln((1 + value)/(1 - value))/2;
    #endif
  }
  
  /**
    Returns the hyperbolic cotangent of the value (do not invoke for 0).
  */
  static inline float cotanh(float value) throw() {
    const float temp = Math::exp(2 * value);
    return (temp + 1)/(temp - 1);
  }

  /**
    Returns the hyperbolic cotangent of the value (do not invoke for 0).
  */
  static inline double cotanh(double value) throw() {
    const double temp = Math::exp(2 * value);
    return (temp + 1)/(temp - 1);
  }
  
  /**
    Returns the hyperbolic cotangent of the value (do not invoke for 0).
  */
  static inline long double cotanh(long double value) throw() {
    const long double temp = Math::exp(2 * value);
    return (temp + 1)/(temp - 1);
  }
  
  /**
    Returns the inverse hyperbolic cotangent of the value.
  */
  static inline float acotanh(float value) throw() {
    return Math::ln((value + 1)/(value - 1))/2;
  }
  
  /**
    Returns the inverse hyperbolic cotangent of the value.
  */
  static inline double acotanh(double value) throw() {
    return Math::ln((value + 1)/(value - 1))/2;
  }
  
  /**
    Returns the inverse hyperbolic cotangent of the value.
  */
  static inline long double acotanh(long double value) throw() {
    return Math::ln((value + 1)/(value - 1))/2;
  }
  
  /**
    Returns the hyperbolic secant of the value.
  */
  static inline float sech(float value) throw() {
    return 2/(Math::exp(value) + Math::exp(-value));
  }
  
  /**
    Returns the hyperbolic secant of the value.
  */
  static inline double sech(double value) throw() {
    return 2/(Math::exp(value) + Math::exp(-value));
  }
  
  /**
    Returns the hyperbolic secant of the value.
  */
  static inline long double sech(long double value) throw() {
    return 2/(Math::exp(value) + Math::exp(-value));
  }
  
  /**
    Returns the inverse hyperbolic secant of the value.
  */
  static inline float asech(float value) throw() {
    return Math::ln((1 + Math::sqrt(1 - value * value))/value);
  }
  
  /**
    Returns the inverse hyperbolic secant of the value.
  */
  static inline double asech(double value) throw() {
    return Math::ln((1 + Math::sqrt(1 - value * value))/value);
  }
  
  /**
    Returns the inverse hyperbolic secant of the value.
  */
  static inline long double asech(long double value) throw() {
    return Math::ln((1 + Math::sqrt(1 - value * value))/value);
  }
  
  /**
    Returns the hyperbolic cosecant of the value (do not invoke for 0).
  */
  static inline float cosech(float value) throw() {
    return 2/(Math::exp(value) - Math::exp(-value));
  }
  
  /**
    Returns the hyperbolic cosecant of the value (do not invoke for 0).
  */
  static inline double cosech(double value) throw() {
    return 2/(Math::exp(value) - Math::exp(-value));
  }
  
  /**
    Returns the hyperbolic cosecant of the value (do not invoke for 0).
  */
  static inline long double cosech(long double value) throw() {
    return 2/(Math::exp(value) - Math::exp(-value));
  }
  
  /**
    Returns the inverse hyperbolic cosecant of the value (do not invoke for 0).
  */
  static inline float acosech(float value) throw() {
    return Math::ln(
      (1 + Math::sqrt(1 + value * value))/((value >= 0) ? value : -value)
    );
  }
  
  /**
    Returns the inverse hyperbolic cosecant of the value (do not invoke for 0).
  */
  static inline double acosech(double value) throw() {
    return Math::ln(
      (1 + Math::sqrt(1 + value * value))/((value >= 0) ? value : -value)
    );
  }
  
  /**
    Returns the inverse hyperbolic cosecant of the value (do not invoke for 0).
  */
  static inline long double acosech(long double value) throw() {
    return Math::ln(
      (1 + Math::sqrt(1 + value * value))/((value >= 0) ? value : -value)
    );
  }



  /**
    Returns the sinc (with normalization PI) of the value.
  */
  static inline float sinc(float value) throw() {
    return (value == 0) ? 1 : Math::sin(value)/value;
  }
  
  /**
    Returns the sinc (with normalization PI) of the value.
  */
  static inline double sinc(double value) throw() {
    return (value == 0) ? 1 : Math::sin(value)/value;
  }
  
  /**
    Returns the sinc (with normalization PI) of the value.
  */
  static inline long double sinc(long double value) throw() {
    return (value == 0) ? 1 : Math::sin(value)/value;
  }

  /**
    Returns the sinc (with normalization 1) of the value.
  */
  static inline float sincPi(float value) throw() {
    return (value == 0) ? 1 :
      Math::sin(constant::PI * value)/(constant::PI * value);
  }
  
  /**
    Returns the sinc (with normalization 1) of the value.
  */
  static inline double sincPi(double value) throw() {
    return (value == 0) ? 1 :
      Math::sin(constant::PI * value)/(constant::PI * value);
  }
  
  /**
    Returns the sinc (with normalization 1) of the value.
  */
  static inline long double sincPi(long double value) throw() {
    return (value == 0) ? 1 :
      Math::sin(constant::PI * value)/(constant::PI * value);
  }
  
  /**
    Returns the tanc of the value.
  */
  static inline float tanc(float value) throw() {
    return (value == 0) ? 1 : Math::tan(value)/value;
  }
  
  /**
    Returns the tanc of the value.
  */
  static inline double tanc(double value) throw() {
    return (value == 0) ? 1 : Math::tan(value)/value;
  }
  
  /**
    Returns the tanc of the value.
  */
  static inline long double tanc(long double value) throw() {
    return (value == 0) ? 1 : Math::tan(value)/value;
  }

  /**
    Returns the natural logarithm of the gamma of the specified value (i.e.
    ln(gamma(x))) where gamma is defined as (x - 1)!.
  */
  static double lngamma(double value) throw();
  
  
  
  /**
    Returns the square root of the specified integer. The result is round down.
  */
  static inline unsigned int iSqrt(unsigned int value) throw() {
    unsigned int accumulator = 0;
    unsigned int remainder = 0;
    for (unsigned int i = sizeof(unsigned int)*8/2; i > 0; --i) {
      remainder = (remainder << 2) | (value >> (sizeof(unsigned int) * 8 - 2));
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
  static inline unsigned int iSqrt8(unsigned int value) throw() {
    unsigned int accumulator = 0;
    unsigned int remainder = 0;
    for (unsigned int i = sizeof(unsigned int)*8/2 + 8; i > 0; --i) {
      remainder = (remainder << 2) | (value >> (sizeof(unsigned int) * 8 - 2));
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
  static inline unsigned int iSqrt16(unsigned int value) throw() {
    unsigned int accumulator = 0;
    unsigned int remainder = 0;
    for (unsigned int i = sizeof(unsigned int)*8/2 + 16; i > 0; --i) {
      remainder = (remainder << 2) | (value >> (sizeof(unsigned int) * 8 - 2));
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
    result is rounded to the nearest 256th of an integer.
  */
  static inline unsigned int iSqrt8Round(unsigned int value) throw() {
    unsigned int accumulator = 0;
    unsigned int remainder = 0;
    for (unsigned int i = sizeof(unsigned int)*8/2 + 8; i > 0; --i) {
      remainder = (remainder << 2) | (value >> (sizeof(unsigned int) * 8 - 2));
      value <<= 2;
      accumulator <<= 1;
      unsigned int e = accumulator << 1;
      if (remainder > e) {
        remainder -= e + 1;
        ++accumulator;
      }
    }
    remainder = (remainder << 2) | (value >> (sizeof(unsigned int) * 8 - 2));
    if (remainder > (accumulator << 2)) {
      ++accumulator; // round up
    }
    return accumulator;
  }

  /**
    Returns true if the specified integer is a power of 2.
  */
  static inline bool isPowerOf2(unsigned char value) throw() {
    // powers of 2 for 4 bit nibble
    // 1, 2, 4, and 8 => 0b0000000100010110 => 0x0116
    const unsigned int IS_POWER_OF_2 = 0x0116;
    return (((value & 0xf0) == 0) && (IS_POWER_OF_2 & (1 << value))) ||
      (((value & 0x0f) == 0) && (IS_POWER_OF_2 & (1 << (value >> 4))));
  }
  
  /**
    Returns true if the specified integer is a power of 2.
  */
  static inline bool isPowerOf2(unsigned int value) throw() {
    if (value != 0) {
      while ((value & 0xff) == 0) {
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
      while ((value & 0xff) == 0) {
        value >>= 8;
      }
      if (value <= 0xff) {
        return isPowerOf2(static_cast<unsigned char>(value));
      }
    }
    return false;
  }

  /**
    Returns the base-2 logarithm of the specified integer. The result is
    rounded down.

    @return 0 if value is 0.
  */
  static inline unsigned int iLog2(unsigned int value) throw() {
    unsigned int result = sizeof(unsigned int) * 8;
    for (unsigned int i = sizeof(unsigned int) * 8/2; i > 0; i >>= 1) {
      if ((value >> (sizeof(unsigned int) * 8 - i)) == 0) {
        result -= i;
        value <<= i;
      }
    }
    return result ? result - 1 : result;
  }
  
  /**
    Returns the base-2 logarithm of the specified integer. The result is
    rounded down.
    
    @return 0 if value is 0.
  */
  static inline unsigned int iLog2(unsigned long value) throw() {
    unsigned int result = sizeof(unsigned long) * 8;
    for (unsigned int i = sizeof(unsigned long) * 8/2; i > 0; i >>= 1) {
      if ((value >> (sizeof(unsigned long) * 8 - i)) == 0) {
        result -= i;
        value <<= i;
      }
    }
    return result ? result - 1 : result;
  }

  /**
    Returns the power of 2 greater than or equal to the specified value.
    
    @return 0 if power of 2 exceeds limit.
  */
  static inline unsigned int getPowerOf2(unsigned int value) throw() {
    unsigned int result = 1 << iLog2(value);
    return (result >= value) ? result : (result << 1);
  }

  /**
    Returns the power of 2 greater than or equal to the specified value.

    @return 0 if power of 2 exceeds limit.
  */
  static inline unsigned long getPowerOf2(unsigned long value) throw() {
    unsigned long result = 1 << iLog2(value);
    return (result >= value) ? result : (result << 1);
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
    return merge(
      getBitReversal(getHighWordOf16(value)),
      getBitReversal(getLowWordOf16(value))
    );
  }
  
  /**
    Returns the bit reversal of the specified 32 bit unsigned integer.
  */
  static inline uint32 getBitReversal(uint32 value) throw() {
    return merge(
      getBitReversal(getHighWordOf32(value)),
      getBitReversal(getLowWordOf32(value))
    );
  }

  /**
    Returns the bit reversal of the specified 64 bit unsigned integer.
  */
  static inline uint64 getBitReversal(uint64 value) throw() {
    return merge(
      getBitReversal(getHighWordOf64(value)),
      getBitReversal(getLowWordOf64(value))
    );
  }

  /**
    Rotates the bits of the specified value to the right.
    
    @param value The value to be rotated.
    @param count The number of bit to rotate. The default is 1.
  */
  static inline unsigned int ror(
    unsigned int value, unsigned int count = 1) throw() {
    count &= sizeof(unsigned int) * 8 - 1;
    return (value >> count) | (value << (sizeof(unsigned int) * 8 - count));
  }

  /**
    Rotates the bits of the specified value to the left.
    
    @param value The value to be rotated.
    @param count The number of bit to rotate. The default is 1.
  */
  static inline unsigned int rol(
    unsigned int value, unsigned int count = 1) throw() {
    count &= sizeof(unsigned int) * 8 - 1;
    return (value << count) | (value >> (sizeof(unsigned int) * 8 - count));
  }

  /**
    Returns the greatest common divisor.
  */
  static int gcd(int m, int n) throw();
  
  /**
    Returns the greatest common divisor.
  */
  static unsigned int gcd(unsigned int n, unsigned int m) throw();

  /**
    Returns the least common multiple.
  */
  static inline int lcm(int m, int n) throw() {
    return m * (n/gcd(m, n));
  }
  
  /**
    Returns the least common multiple.
  */
  static inline unsigned int lcm(unsigned int n, unsigned int m) throw() {
    return m * (n/gcd(m, n));
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
