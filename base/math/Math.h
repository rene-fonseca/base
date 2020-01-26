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
#include <base/math/Constants.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  define _COM_AZURE_DEV__BASE__ISOC_API __declspec(dllimport)
#else
#  define _COM_AZURE_DEV__BASE__ISOC_API
#endif

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
#  define _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT throw()
#else
#  define _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT
#endif

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__EMCC)

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
namespace internal { // do NOT use namespace directly

  extern "C" float _COM_AZURE_DEV__BASE__ISOC_API _hypotf(float, float);
}
#  define _COM_AZURE_DEV__BASE__REDIR_ISOC_HYPOTF(x, y) internal::_hypotf(x, y)
#endif

#  define _COM_AZURE_DEV__BASE__REDIR_ISOC_FABSF(x) static_cast<float>(isoc::fabs(static_cast<double>(x)))
#  define _COM_AZURE_DEV__BASE__REDIR_ISOC_FABSL(x) static_cast<long double>(isoc::fabs(static_cast<double>(x)))
#  define _COM_AZURE_DEV__BASE__REDIR_ISOC_CEILL(x) static_cast<long double>(isoc::ceil(static_cast<double>(x)))
#  define _COM_AZURE_DEV__BASE__REDIR_ISOC_FLOORL(x) static_cast<long double>(isoc::floor(static_cast<double>(x)))
#if (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__X86)
#  define _COM_AZURE_DEV__BASE__REDIR_ISOC_CEILF(x) static_cast<float>(isoc::ceil(static_cast<double>(x)))
#  define _COM_AZURE_DEV__BASE__REDIR_ISOC_FLOORF(x) static_cast<float>(isoc::floor(static_cast<double>(x)))
#  define _COM_AZURE_DEV__BASE__REDIR_ISOC_SQRTF(x) static_cast<float>(isoc::sqrt(static_cast<double>(x)))
#  define _COM_AZURE_DEV__BASE__REDIR_ISOC_EXPF(x) static_cast<float>(isoc::exp(static_cast<double>(x)))
#  define _COM_AZURE_DEV__BASE__REDIR_ISOC_LOGF(x) static_cast<float>(isoc::log(static_cast<double>(x)))
#  define _COM_AZURE_DEV__BASE__REDIR_ISOC_SINF(x) static_cast<float>(isoc::sin(static_cast<double>(x)))
#  define _COM_AZURE_DEV__BASE__REDIR_ISOC_COSF(x) static_cast<float>(isoc::cos(static_cast<double>(x)))
#  define _COM_AZURE_DEV__BASE__REDIR_ISOC_TANF(x) static_cast<float>(isoc::tan(static_cast<double>(x)))
#  define _COM_AZURE_DEV__BASE__REDIR_ISOC_ASINF(x) static_cast<float>(isoc::asin(static_cast<double>(x)))
#  define _COM_AZURE_DEV__BASE__REDIR_ISOC_ACOSF(x) static_cast<float>(isoc::acos(static_cast<double>(x)))
#  define _COM_AZURE_DEV__BASE__REDIR_ISOC_ATANF(x) static_cast<float>(isoc::atan(static_cast<double>(x)))
#  define _COM_AZURE_DEV__BASE__REDIR_ISOC_ATAN2F(y, x) static_cast<float>(isoc::atan2(static_cast<double>(y), static_cast<double>(x)))
#endif
#endif

/**
  @defgroup math Math
*/

namespace isoc { // do NOT use namespace directly

  extern "C" float /*_COM_AZURE_DEV__BASE__ISOC_API*/ fabsf(float) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
  extern "C" double /*_COM_AZURE_DEV__BASE__ISOC_API*/ fabs(double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
#if !defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_FABSL)
  extern "C" long double _COM_AZURE_DEV__BASE__ISOC_API fabsl(long double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
#endif

#if !defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_CEILF)
  extern "C" float _COM_AZURE_DEV__BASE__ISOC_API ceilf(float) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
#endif
  extern "C" double _COM_AZURE_DEV__BASE__ISOC_API ceil(double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
#if !defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_CEILL)
  extern "C" long double _COM_AZURE_DEV__BASE__ISOC_API ceill(long double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
#endif

#if !defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_FLOORF)
  extern "C" float _COM_AZURE_DEV__BASE__ISOC_API floorf(float) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
#endif
  extern "C" double _COM_AZURE_DEV__BASE__ISOC_API floor(double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
#if !defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_FLOORL)
  extern "C" long double _COM_AZURE_DEV__BASE__ISOC_API floorl(long double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
#endif

  extern "C" float _COM_AZURE_DEV__BASE__ISOC_API roundf(float) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
  extern "C" double _COM_AZURE_DEV__BASE__ISOC_API round(double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
  extern "C" long double _COM_AZURE_DEV__BASE__ISOC_API roundl(long double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;

  extern "C" float _COM_AZURE_DEV__BASE__ISOC_API truncf(float) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
  extern "C" double _COM_AZURE_DEV__BASE__ISOC_API trunc(double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
  extern "C" long double _COM_AZURE_DEV__BASE__ISOC_API truncl(long double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;

#if !defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_SQRTF)
  extern "C" float _COM_AZURE_DEV__BASE__ISOC_API sqrtf(float) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
#endif
  extern "C" double /*_COM_AZURE_DEV__BASE__ISOC_API*/ sqrt(double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;

  extern "C" float _COM_AZURE_DEV__BASE__ISOC_API cbrtf(float) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
  extern "C" double _COM_AZURE_DEV__BASE__ISOC_API cbrt(double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;

#if !defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_HYPOTF)
  extern "C" float _COM_AZURE_DEV__BASE__ISOC_API hypotf(float, float) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
#endif
  extern "C" double _COM_AZURE_DEV__BASE__ISOC_API hypot(double, double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;

#if !defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_EXPF)
  extern "C" float _COM_AZURE_DEV__BASE__ISOC_API expf(float) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
#endif
  extern "C" double /*_COM_AZURE_DEV__BASE__ISOC_API*/ exp(double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;

#if !defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_LOGF)
  extern "C" float _COM_AZURE_DEV__BASE__ISOC_API logf(float) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
#endif
  extern "C" double /*_COM_AZURE_DEV__BASE__ISOC_API*/ log(double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;

  extern "C" float _COM_AZURE_DEV__BASE__ISOC_API log2f(float) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
  extern "C" double _COM_AZURE_DEV__BASE__ISOC_API log2(double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;

  extern "C" float _COM_AZURE_DEV__BASE__ISOC_API log10f(float) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
  extern "C" double /*_COM_AZURE_DEV__BASE__ISOC_API*/ log10(double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;

  extern "C" float _COM_AZURE_DEV__BASE__ISOC_API powf(float, float) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
  extern "C" double /*_COM_AZURE_DEV__BASE__ISOC_API*/ pow(double, double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;

#if !defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_SINF)
  extern "C" float _COM_AZURE_DEV__BASE__ISOC_API sinf(float) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
#endif
  extern "C" double /*_COM_AZURE_DEV__BASE__ISOC_API*/ sin(double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;

#if !defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_COSF)
  extern "C" float _COM_AZURE_DEV__BASE__ISOC_API cosf(float) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
#endif
  extern "C" double /*_COM_AZURE_DEV__BASE__ISOC_API*/ cos(double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;

#if !defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_ASINF)
  extern "C" float _COM_AZURE_DEV__BASE__ISOC_API asinf(float) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
#endif
  extern "C" double /*_COM_AZURE_DEV__BASE__ISOC_API*/ asin(double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;

#if !defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_ACOSF)
  extern "C" float _COM_AZURE_DEV__BASE__ISOC_API acosf(float) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
#endif
  extern "C" double /*_COM_AZURE_DEV__BASE__ISOC_API*/ acos(double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;

  extern "C" float _COM_AZURE_DEV__BASE__ISOC_API sinhf(float) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
  extern "C" double /*_COM_AZURE_DEV__BASE__ISOC_API*/ sinh(double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;

  extern "C" float _COM_AZURE_DEV__BASE__ISOC_API coshf(float) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
  extern "C" double /*_COM_AZURE_DEV__BASE__ISOC_API*/ cosh(double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;

  extern "C" float _COM_AZURE_DEV__BASE__ISOC_API asinhf(float) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
  extern "C" double _COM_AZURE_DEV__BASE__ISOC_API asinh(double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;

  extern "C" float _COM_AZURE_DEV__BASE__ISOC_API acoshf(float) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
  extern "C" double _COM_AZURE_DEV__BASE__ISOC_API acosh(double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;

#if !defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_TANF)
  extern "C" float _COM_AZURE_DEV__BASE__ISOC_API tanf(float) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
#endif
  extern "C" double /*_COM_AZURE_DEV__BASE__ISOC_API*/ tan(double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;

#if !defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_ATANF)
  extern "C" float _COM_AZURE_DEV__BASE__ISOC_API atanf(float) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
#endif
  extern "C" double /*_COM_AZURE_DEV__BASE__ISOC_API*/ atan(double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;

  extern "C" float _COM_AZURE_DEV__BASE__ISOC_API tanhf(float) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
  extern "C" double /*_COM_AZURE_DEV__BASE__ISOC_API*/ tanh(double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;

  extern "C" float _COM_AZURE_DEV__BASE__ISOC_API atanhf(float) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
  extern "C" double _COM_AZURE_DEV__BASE__ISOC_API atanh(double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;

#if !defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_ATAN2F)
  extern "C" float _COM_AZURE_DEV__BASE__ISOC_API atan2f(float, float) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;
#endif
  extern "C" double /*_COM_AZURE_DEV__BASE__ISOC_API*/ atan2(double, double) _COM_AZURE_DEV__BASE__ISOC_NOEXCEPT;

#if 1 // redirect long double functions to double version if not supported
  inline long double sqrtl(long double x) noexcept
  {
    return sqrt(static_cast<double>(x));
  }

  inline long double cbrtl(long double x) noexcept
  {
    return cbrt(static_cast<double>(x));
  }

  inline long double hypotl(long double x, long double y) noexcept
  {
    return hypot(static_cast<double>(x), static_cast<double>(y));
  }

  inline long double expl(long double x) noexcept
  {
    return exp(static_cast<double>(x));
  }

  inline long double logl(long double x) noexcept
  {
    return log(static_cast<double>(x));
  }

  inline long double log2l(long double x) noexcept
  {
    return log2(static_cast<double>(x));
  }

  inline long double log10l(long double x) noexcept
  {
    return log10(static_cast<double>(x));
  }

  inline long double powl(long double x, long double y) noexcept
  {
    return pow(static_cast<double>(x), static_cast<double>(y));
  }

  inline long double sinl(long double x) noexcept
  {
    return sin(static_cast<double>(x));
  }

  inline long double cosl(long double x) noexcept
  {
    return cos(static_cast<double>(x));
  }

  inline long double asinl(long double x) noexcept
  {
    return asin(static_cast<double>(x));
  }

  inline long double acosl(long double x) noexcept
  {
    return acos(static_cast<double>(x));
  }

  inline long double sinhl(long double x) noexcept
  {
    return sinh(static_cast<double>(x));
  }

  inline long double coshl(long double x) noexcept
  {
    return cosh(static_cast<double>(x));
  }

  inline long double asinhl(long double x) noexcept
  {
    return asinh(static_cast<double>(x));
  }

  inline long double acoshl(long double x) noexcept
  {
    return acosh(static_cast<double>(x));
  }

  inline long double tanl(long double x) noexcept
  {
    return tan(static_cast<double>(x));
  }

  inline long double atanl(long double x) noexcept
  {
    return atan(static_cast<double>(x));
  }

  inline long double tanhl(long double x) noexcept
  {
    return tanh(static_cast<double>(x));
  }

  inline long double atanhl(long double x) noexcept
  {
    return atanh(static_cast<double>(x));
  }

  inline long double atan2l(long double x, long double y) noexcept
  {
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
  @ingroup math
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Math {
private:

  /** Bit reversal lookup table (8 bit). */
  static const uint8 BIT_REVERSAL[256];

  static const uint8* getBitReversalData() noexcept;
public:
  
  /** Power of 10 exponents in 64-bit. */
  static const int64 EXPONENTS10_64[19];
  /** Power of 5 exponents in 64-bit. */
  static const int64 EXPONENTS5_64[28];
  /** Power of 2 exponents in 64-bit. */
  static const int64 EXPONENTS2_64[63];

  /** Power of 10 exponents in 32-bit. */
  static const int32 EXPONENTS10_32[10];
  /** Power of 5 exponents in 32-bit. */
  static const int32 EXPONENTS5_32[14];
  /** Power of 2 exponents in 32-bit. */
  static const int32 EXPONENTS2_32[31];
  
  /** Returns true if value is finite. */
  static bool isFinite(float value) noexcept;
  /** Returns true if value is finite. */
  static bool isFinite(double value) noexcept;
  /** Returns true if value is finite. */
  static bool isFinite(long double value) noexcept;
#if (_COM_AZURE_DEV__BASE__HAS_FLOAT128)
  /** Returns true if value is finite. */
  static bool isFinite(float128 value) noexcept;
#endif

  /** Returns true if value is infinity. */
  static bool isInfinity(float value) noexcept;
  /** Returns true if value is infinity. */
  static bool isInfinity(double value) noexcept;
  /** Returns true if value is infinity. */
  static bool isInfinity(long double value) noexcept;
#if (_COM_AZURE_DEV__BASE__HAS_FLOAT128)
  /** Returns true if value is infinity. */
  static bool isInfinity(float128 value) noexcept;
#endif

  /** Returns true if value is NaN. */
  static bool isNaN(float value) noexcept;
  /** Returns true if value is NaN. */
  static bool isNaN(double value) noexcept;
  /** Returns true if value is NaN. */
  static bool isNaN(long double value) noexcept;
#if (_COM_AZURE_DEV__BASE__HAS_FLOAT128)
  /** Returns true if value is NaN. */
  static bool isNaN(float128 value) noexcept;
#endif

  /** Returns signed zero. */
  template<typename TYPE>
  static constexpr TYPE getZero(bool negative) noexcept;

  /** Returns infinity. You get negative infinity by using - operator. */
  template<typename TYPE>
  static constexpr TYPE getInfinity() noexcept;

  /** Returns quiet NaN. Signaling NaN should be avoided due to platform inconsistencies. */
  template<typename TYPE>
  static constexpr TYPE getNaN() noexcept;

  /**
    Returns the absolute value.
  */
  static inline float abs(float value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_FABSF)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_FABSF(value);
    #else
      return isoc::fabsf(value);
    #endif
  }

  /**
    Returns the absolute value.
  */
  static inline double abs(double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_FABS)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_FABS(value);
    #else
      return isoc::fabs(value);
    #endif
  }

  /**
    Returns the absolute value.
  */
  static inline long double abs(long double value) noexcept
  {
    #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__EMCC)
      return (value >= 0) ? value : -value;
    #elif defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_FABSL)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_FABSL(value);
    #else
      return isoc::fabsl(value);
    #endif
  }

  /**
    Rounds (towards +infinity) the value to the nearest integer.
  */
  static inline float ceil(float value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_CEILF)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_CEILF(value);
    #else
      return isoc::ceilf(value);
    #endif
  }

  /**
    Rounds (towards +infinity) the value to the nearest integer.
  */
  static inline double ceil(double value) noexcept
  {
    return isoc::ceil(value);
  }

  /**
    Rounds (towards +infinity) the value to the nearest integer.
  */
  static inline long double ceil(long double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_CEILL)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_CEILL(value);
    #else
      return isoc::ceill(value);
    #endif
  }

  /**
    Rounds (towards -infinity) the value to the nearest integer.
  */
  static inline float floor(float value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_FLOORF)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_FLOORF(value);
    #else
      return isoc::floorf(value);
    #endif
  }

  /**
    Rounds (towards -infinity) the value to the nearest integer.
  */
  static inline double floor(double value) noexcept
  {
    return isoc::floor(value);
  }

  /**
    Rounds (towards -infinity) the value to the nearest integer.
  */
  static inline long double floor(long double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_FLOORL)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_FLOORL(value);
    #else
      return isoc::floorl(value);
    #endif
  }

  /**
    Rounds the value to the nearest integer (away from 0).
  */
  static inline float round(float value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_ROUNDF)
      return isoc::roundf(value);
    #elif defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_ROUNDL)
      return isoc::roundl(value);
    #elif defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_ROUND)
      return isoc::round(value);
    #else
      return (value >= 0) ? Math::floor(value + 0.5f) : Math::ceil(value - 0.5f);
    #endif
  }
  
  /**
    Rounds the value to the nearest integer.
  */
  static inline double round(double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_ROUND)
      return isoc::round(value);
    #elif defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_ROUNDL)
      return isoc::roundl(value);
    #elif defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_ROUNDF)
      return isoc::roundf(value);
    #else
      return (value >= 0) ? Math::floor(value + 0.5) : Math::ceil(value - 0.5);
    #endif
  }
  
  /**
    Rounds the value to the nearest integer.
  */
  static inline long double round(long double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_ROUNDL)
      return isoc::roundl(value);
    #elif defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_ROUND)
      return isoc::round(value);
    #elif defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_ROUNDF)
      return isoc::roundf(value);
    #else
      return (value >= 0) ? Math::floor(value + 0.5L) : Math::ceil(value - 0.5L);
    #endif
  }
  
  /**
    Rounds (towards zero) the value to the nearest integer.
  */
  static inline float trunc(float value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_TRUNCF)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_TRUNCF(value);
    #else
      return isoc::truncf(value);
    #endif
  }

  /**
    Rounds (towards zero) the value to the nearest integer.
  */
  static inline double trunc(double value) noexcept
  {
    return isoc::trunc(value);
  }

  /**
    Rounds (towards zero) the value to the nearest integer.
  */
  static inline long double trunc(long double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_TRUNCL)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_TRUNCL(value);
    #else
      return isoc::truncl(value);
    #endif
  }

  /** Returns the fraction. */
  static float frac(float value) noexcept;

  /** Returns the fraction. */
  static double frac(double value) noexcept;

  /** Returns the fraction. */
  static long double frac(long double value) noexcept;



  /**
    Returns the square root of the value.
  */
  static inline float sqrt(float value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_SQRTF)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_SQRTF(value);
    #else
      return isoc::sqrtf(value);
    #endif
  }

  /**
    Returns the square root of the value.
  */
  static inline double sqrt(double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_SQRT)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_SQRT(value);
    #else
      return isoc::sqrt(value);
    #endif
  }

  /**
    Returns the square root of the value.
  */
  static inline long double sqrt(long double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_SQRTL)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_SQRTL(value);
    #else
      return isoc::sqrtl(value);
    #endif
  }

  /**
    Returns the cube root of the value.
  */
  static inline float cbrt(float value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_CBRTF)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_CBRTF(value);
    #else
      return isoc::cbrtf(value);
    #endif
  }

  /**
    Returns the cube root of the value.
  */
  static inline double cbrt(double value) noexcept
  {
    return isoc::cbrt(value);
  }

  /**
    Returns the cube root of the value.
  */
  static inline long double cbrt(long double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_CBRTL)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_CBRTL(value);
    #else
      return isoc::cbrtl(value);
    #endif
  }

  /**
    Returns the Euclidian distance - sqrt(x^2 + y^2).
  */
  static inline float hypot(float x, float y) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_HYPOTF)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_HYPOTF(x, y);
    #else
      return isoc::hypotf(x, y);
    #endif
  }

  /**
    Returns the Euclidian distance - sqrt(x^2 + y^2).
  */
  static inline double hypot(double x, double y) noexcept
  {
    return isoc::hypot(x, y);
  }

  /**
    Returns the Euclidian distance - sqrt(x^2 + y^2).
  */
  static inline long double hypot(long double x, long double y) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_HYPOTL)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_HYPOTL(x, y);
    #else
      return isoc::hypotl(x, y);
    #endif
  }

  /**
    Returns the exponential of the value.
  */
  static inline float exp(float value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_EXPF)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_EXPF(value);
    #else
      return isoc::expf(value);
    #endif
  }

  /**
    Returns the exponential of the value.
  */
  static inline double exp(double value) noexcept
  {
    return isoc::exp(value);
  }

  /**
    Returns the exponential of the value.
  */
  static inline long double exp(long double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_EXPL)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_EXPL(value);
    #else
      return isoc::expl(value);
    #endif
  }

  /**
    Returns the natural logarithm of the value.
  */
  static inline float ln(float value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_LOGF)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_LOGF(value);
    #else
      return isoc::logf(value);
    #endif
  }

  /**
    Returns the natural logarithm of the value.
  */
  static inline double ln(double value) noexcept
  {
    return isoc::log(value);
  }

  /**
    Returns the natural logarithm of the value.
  */
  static inline long double ln(long double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_LOGL)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_LOGL(value);
    #else
      return isoc::logl(value);
    #endif
  }

  /**
    Returns the logarithm with base 2 of the value.
  */
  static inline float log2(float value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_LOG2F)
      return isoc::log2f(value);
    #elif defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_LOG2L)
      return isoc::log2l(value);
    #elif defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_LOG2)
      return isoc::log2(value);
    #else
      return Math::ln(value) * constant::LOG2E_F;
    #endif
  }

  /**
    Returns the logarithm with base 2 of the value.
  */
  static inline double log2(double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_LOG2)
      return isoc::log2(value);
    #elif defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_LOG2L)
      return isoc::log2l(value);
    #elif defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_LOG2F)
      return isoc::log2f(value);
    #else
      return Math::ln(value) * constant::LOG2E;
    #endif
  }

  /**
    Returns the logarithm with base 2 of the value.
  */
  static inline long double log2(long double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_LOG2L)
      return isoc::log2l(value);
    #elif defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_LOG2)
      return isoc::log2(value);
    #elif defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_LOG2F)
      return isoc::log2f(value);
    #else
      return Math::ln(value) * constant::LOG2E_L;
    #endif
  }
  
  /**
    Returns the logarithm with base 10 of the value.
  */
  static inline float log10(float value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_LOG10F)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_LOG10F(value);
    #else
      #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_LOG10F)
        return isoc::log10f(value);
      #else
        #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_LOGF)
          return _COM_AZURE_DEV__BASE__REDIR_ISOC_LOGF(value) * constant::LOG10E_F;
        #else
          return isoc::logf(value) * constant::LOG10E_F;
        #endif
      #endif
    #endif
  }

  /**
    Returns the logarithm with base 10 of the value.
  */
  static inline double log10(double value) noexcept
  {
    return isoc::log10(value);
  }
  
  /**
    Returns the logarithm with base 10 of the value.
  */
  static inline long double log10(long double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_LOG10L)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_LOG10L(value);
    #else
      #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_LOG10L)
        return isoc::log10l(value);
      #else
        #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_LOGL)
          return _COM_AZURE_DEV__BASE__REDIR_ISOC_LOGL(value) * constant::LOG10E_L;
        #else
          return isoc::logl(value) * constant::LOG10E_L;
        #endif
      #endif
    #endif
  }

  /**
    Returns the logarithm with base n of the value.
  */
  static inline float logn(float value, float base) noexcept
  {
    return Math::ln(value) / base;
  }

  /**
    Returns the logarithm with base n of the value.
  */
  static inline double logn(double value, double base) noexcept
  {
    return Math::ln(value) / base;
  }

  /**
    Returns the logarithm with base n of the value.
  */
  static inline long double logn(long double value, long double base) noexcept
  {
    return Math::ln(value) / base;
  }
  
  /**
    Returns the power a^b.
  */
  static inline float power(float a, float b) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_POWF)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_POWF(a, b);
    #else
      return isoc::powf(a, b);
    #endif
  }

  /**
    Returns the power a^b.
  */
  static inline double power(double a, double b) noexcept
  {
    return isoc::pow(a, b);
  }

  /**
    Returns the power a^b.
  */
  static inline long double power(long double a, long double b) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_POWL)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_POWL(a, b);
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
  static inline float sin(float value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_SINF)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_SINF(value);
    #else
      return isoc::sinf(value);
    #endif
  }

  /**
    Returns the sine.
  */
  static inline double sin(double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_SIN)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_SIN(value);
    #else
      return isoc::sin(value);
    #endif
  }

  /**
    Returns the sine.
  */
  static inline long double sin(long double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_SINL)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_SINL(value);
    #else
      return isoc::sinl(value);
    #endif
  }

  /**
    Returns the inverse sine.
  */
  static inline float asin(float value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_ASINF)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_ASINF(value);
    #else
      return isoc::asinf(value);
    #endif
  }

  /**
    Returns the inverse sine.
  */
  static inline double asin(double value) noexcept
  {
    return isoc::asin(value);
  }

  /**
    Returns the inverse sine.
  */
  static inline long double asin(long double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_ASINL)
      return isoc::asinl(value);
    #elif defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_ASINL)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_ASINL(value);
    #else
      return Math::atan2(value, Math::sqrt(1 - value * value));
      // return Math::atan(value/Math::sqrt((1 - value * value)));
    #endif
  }

  /**
    Returns the cosine.
  */
  static inline float cos(float value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_COSF)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_COSF(value);
    #else
      return isoc::cosf(value);
    #endif
  }

  /**
    Returns the cosine.
  */
  static inline double cos(double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_COS)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_COS(value);
    #else
      return isoc::cos(value);
    #endif
  }

  /**
    Returns the cosine.
  */
  static inline long double cos(long double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_COSL)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_COSL(value);
    #else
      return isoc::cosl(value);
    #endif
  }

  /**
    Returns the inverse cosine.
  */
  static inline float acos(float value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_ACOSF)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_ACOSF(value);
    #else
      return isoc::acosf(value);
    #endif
  }

  /**
    Returns the inverse cosine.
  */
  static inline double acos(double value) noexcept
  {
    return isoc::acos(value);
  }

  /**
    Returns the inverse cosine.
  */
  static inline long double acos(long double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_ACOSL)
      return isoc::acosl(value);
    #elif defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_ACOSL)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_ACOSL(value);
    #else
      return Math::atan2(Math::sqrt(1 - value * value), value);
      // return constant::PI_2_L - Math::atan(value/Math::sqrt((1 - value * value)));
    #endif
  }

  /**
    Returns the tangent.
  */
  static inline float tan(float value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_TANF)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_TANF(value);
    #else
      return isoc::tanf(value);
    #endif
  }

  /**
    Returns the tangent.
  */
  static inline double tan(double value) noexcept
  {
    return isoc::tan(value);
  }

  /**
    Returns the tangent.
  */
  static inline long double tan(long double value) 
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_TANL)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_TANL(value);
    #else
      return isoc::tanl(value);
    #endif
  }
  
  
  /**
    Returns the inverse tangent.
  */
  static inline float atan(float value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_ATANF)
      return isoc::atanf(value);
    #elif defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_ATANF)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_ATANF(value);
    #else
      return isoc::atanf(value);
    #endif
  }

  /**
    Returns the inverse tangent.
  */
  static inline double atan(double value) noexcept
  {
    return isoc::atan(value);
  }
  
  /**
    Returns the inverse tangent.
  */
  static inline long double atan(long double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_ATANL)
      return isoc::atanl(value);
    #elif defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_ATANL)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_ATANL(value);
    #else
      return isoc::atan(value);
    #endif
  }
  
  /**
    Returns the cotangent of the value.
  */
  static inline long double cotan(long double value) noexcept
  {
    return 1/Math::tan(value);
  }
  
  /**
    Returns the inverse cotangent of the value.
  */
  static inline long double acotan(long double value) noexcept
  {
    return Math::atan2(1, value);
  }
  
  /**
    Returns the secant of the value.
  */
  static inline long double sec(long double value) noexcept
  {
    return 1/Math::cos(value);
  }
  
  /**
    Returns the inverse secant of the value.
  */
  static inline long double asec(long double value) noexcept
  {
    return Math::atan2(value, Math::sqrt(1 - value * value)); // acos(1/value)
  }
  
  /**
    Returns the cosecant of the value.
  */
  static inline long double cosec(long double value) noexcept
  {
    return 1/Math::sin(value);
  }
  
  /**
    Returns the inverse cosecant of the value.
  */
  static inline long double acosec(long double value) noexcept
  {
    return Math::atan2(Math::sqrt(1 - value * value), value); // asin(1/value)
  }
  
  /**
    Returns the exsecant of the value.
  */
  static inline long double exsec(long double value) noexcept
  {
    return 1/Math::cos(value) - 1;
  }
  
  /**
    Returns the inverse exsecant of the value.
  */
  static inline long double aexsec(long double value) noexcept
  {
    return Math::atan(Math::sqrt(value * (value + 2)));
  }
  
  /**
    Returns versine of value.
  */
  static inline long double vers(long double value) noexcept
  {
    return 1 - Math::cos(value);
  }
  
  /**
    Returns the inverse versine of the value.
  */
  static inline long double avers(long double value) noexcept
  {
    return Math::atan(Math::sqrt((2 - value) * value/((value - 1)*(value - 1))));
  }
  
  /**
    Returns the haversine of value.
  */
  static inline long double hav(long double value) noexcept
  {
    return (1 - Math::cos(value))/2;
  }
  
  /**
    Returns the inverse hav of the value.
  */
  static inline long double ahav(long double value) noexcept
  {
    return Math::atan(Math::sqrt(1/((1 - 2 * value) * (1 - 2 * value)) - 1));
  }
  
  /**
    Returns the coversine of value.
  */
  static inline long double covers(long double value) noexcept
  {
    return 1 - Math::sin(value);
  }
  
  /**
    Returns the inverse coversine of the value.
  */
  static inline long double acovers(long double value) noexcept
  {
    return Math::atan(Math::sqrt(-1/(value * (value - 2)) - 1));
  }
  
  /**
    Returns the arc tangent of y/x using the signs of both arguments to
    determine the proper quadrant.
  */
  static inline float atan2(float y, float x) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_ATAN2F)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_ATAN2F(y, x);
    #else
      return isoc::atan2f(y, x);
    #endif
  }

  /**
    Returns the arc tangent of y/x using the signs of both arguments to
    determine the proper quadrant.
  */
  static inline double atan2(double y, double x) noexcept
  {
    return isoc::atan2(y, x);
  }

  /**
    Returns the arc tangent of y/x using the signs of both arguments to
    determine the proper quadrant.
  */
  static inline long double atan2(long double y, long double x) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_ATAN2L)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_ATAN2L(y, x);
    #else
      return isoc::atan2l(y, x);
    #endif
  }



  /**
    Returns the hyperbolic sine.
  */
  static inline float sinh(float value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_SINHF)
      return isoc::sinhf(value);
    #elif defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_SINHF)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_SINHF(value);
    #else
      return (Math::exp(value) - Math::exp(-value))/2;
    #endif
  }

  /**
    Returns the hyperbolic sine.
  */
  static inline double sinh(double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_SINH)
      return isoc::sinh(value);
    #elif defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_SINH)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_SINH(value);
    #else
      return (Math::exp(value) - Math::exp(-value))/2;
    #endif
  }

  /**
    Returns the hyperbolic sine.
  */
  static inline long double sinh(long double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_SINH)
      return isoc::sinhl(value);
    #elif defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_SINHL)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_SINHL(value);
    #else
      return (Math::exp(value) - Math::exp(-value))/2;
    #endif
  }

  /**
    Returns the inverse hyperbolic sine.
  */
  static inline float asinh(float value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_ASINHF)
      return isoc::asinhf(value);
    #elif defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_ASINHF)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_ASINHF(value);
    #else
      return Math::ln(value + Math::sqrt(1 + value * value));
    #endif
  }

  /**
    Returns the inverse hyperbolic sine.
  */
  static inline double asinh(double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_ASINH)
      return isoc::asinh(value);
    #elif defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_ASINH)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_ASINH(value);
    #else
      return Math::ln(value + Math::sqrt(1 + value * value));
    #endif
  }

  /**
    Returns the inverse hyperbolic sine.
  */
  static inline long double asinh(long double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_ASINHL)
      return isoc::asinhl(value);
    #elif defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_ASINHL)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_ASINHL(value);
    #else
      return Math::ln(value + Math::sqrt(1 + value * value));
    #endif
  }

  /**
    Returns the hyperbolic cosine.
  */
  static inline float cosh(float value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_COSHF)
      return isoc::coshf(value);
    #elif defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_COSHF)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_COSHF(value);
    #else
      return (Math::exp(value) + Math::exp(-value))/2;
    #endif
  }

  /**
    Returns the hyperbolic cosine.
  */
  static inline double cosh(double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_COSH)
      return isoc::cosh(value);
    #elif defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_COSH)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_COSH(value);
    #else
      return (Math::exp(value) + Math::exp(-value))/2;
    #endif
  }

  /**
    Returns the hyperbolic cosine.
  */
  static inline long double cosh(long double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_COSHL)
      return isoc::coshl(value);
    #elif defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_COSHL)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_COSHL(value);
    #else
      return (Math::exp(value) + Math::exp(-value))/2;
    #endif
  }

  /**
    Returns the inverse hyperbolic cosine.
  */
  static inline float acosh(float value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_ACOSHF)
      return isoc::acoshf(value);
    #elif defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_ACOSHF)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_ACOSHF(value);
    #else
      return Math::ln(value + Math::sqrt(value * value - 1));
    #endif
  }

  /**
    Returns the inverse hyperbolic cosine.
  */
  static inline double acosh(double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_ACOSH)
      return isoc::acosh(value);
    #elif defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_ACOSH)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_ACOSH(value);
    #else
      return Math::ln(value + Math::sqrt(value * value - 1));
    #endif
  }

  /**
    Returns the inverse hyperbolic cosine.
  */
  static inline long double acosh(long double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_ACOSHL)
      return isoc::acoshl(value);
    #elif defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_ACOSHL)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_ACOSHL(value);
    #else
      return Math::ln(value + Math::sqrt(value * value - 1));
    #endif
  }

  /**
    Returns the hyperbolic tangent.
  */
  static inline float tanh(float value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_TANHF)
      return isoc::tanhf(value);
    #elif defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_TANHF)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_TANHF(value);
    #else
      const float temp = Math::exp(2 * value);
      return (temp - 1)/(temp + 1);
    #endif
  }

  /**
    Returns the hyperbolic tangent.
  */
  static inline double tanh(double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_TANH)
      return isoc::tanh(value);
    #elif defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_TANH)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_TANH(value);
    #else
      const double temp = Math::exp(2 * value);
      return (temp - 1)/(temp + 1);
    #endif
  }

  /**
    Returns the hyperbolic tangent.
  */
  static inline long double tanh(long double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_TANHL)
      return isoc::tanhl(value);
    #elif defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_TANHL)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_TANHL(value);
    #else
      const long double temp = Math::exp(2 * value);
      return (temp - 1)/(temp + 1);
    #endif
  }

  /**
    Returns the inverse hyperbolic tangent.
  */
  static inline float atanh(float value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_ATANHF)
      return isoc::atanhf(value);
    #elif defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_ATANHF)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_ATANHF(value);
    #else
      return Math::ln((1 + value)/(1 - value))/2;
    #endif
  }
  
  /**
    Returns the inverse hyperbolic tangent.
  */
  static inline double atanh(double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_ATANH)
      return isoc::atanh(value);
    #elif defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_ATANH)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_ATANH(value);
    #else
      return Math::ln((1 + value)/(1 - value))/2;
    #endif
  }

  /**
    Returns the inverse hyperbolic tangent.
  */
  static inline long double atanh(long double value) noexcept
  {
    #if defined(_COM_AZURE_DEV__BASE__HAVE_ISOC_ATANHL)
      return isoc::atanhl(value);
    #elif defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_ATANHL)
      return _COM_AZURE_DEV__BASE__REDIR_ISOC_ATANHL(value);
    #else
      return Math::ln((1 + value)/(1 - value))/2;
    #endif
  }
  
  /**
    Returns the hyperbolic cotangent of the value (do not invoke for 0).
  */
  static inline float cotanh(float value) noexcept
  {
    const float temp = Math::exp(2 * value);
    return (temp + 1)/(temp - 1);
  }

  /**
    Returns the hyperbolic cotangent of the value (do not invoke for 0).
  */
  static inline double cotanh(double value) noexcept
  {
    const double temp = Math::exp(2 * value);
    return (temp + 1)/(temp - 1);
  }
  
  /**
    Returns the hyperbolic cotangent of the value (do not invoke for 0).
  */
  static inline long double cotanh(long double value) noexcept
  {
    const long double temp = Math::exp(2 * value);
    return (temp + 1)/(temp - 1);
  }
  
  /**
    Returns the inverse hyperbolic cotangent of the value.
  */
  static inline float acotanh(float value) noexcept
  {
    return Math::ln((value + 1)/(value - 1))/2;
  }
  
  /**
    Returns the inverse hyperbolic cotangent of the value.
  */
  static inline double acotanh(double value) noexcept
  {
    return Math::ln((value + 1)/(value - 1))/2;
  }
  
  /**
    Returns the inverse hyperbolic cotangent of the value.
  */
  static inline long double acotanh(long double value) noexcept
  {
    return Math::ln((value + 1)/(value - 1))/2;
  }
  
  /**
    Returns the hyperbolic secant of the value.
  */
  static inline float sech(float value) noexcept
  {
    return 2/(Math::exp(value) + Math::exp(-value));
  }
  
  /**
    Returns the hyperbolic secant of the value.
  */
  static inline double sech(double value) noexcept
  {
    return 2/(Math::exp(value) + Math::exp(-value));
  }
  
  /**
    Returns the hyperbolic secant of the value.
  */
  static inline long double sech(long double value) noexcept
  {
    return 2/(Math::exp(value) + Math::exp(-value));
  }
  
  /**
    Returns the inverse hyperbolic secant of the value.
  */
  static inline float asech(float value) noexcept
  {
    return Math::ln((1 + Math::sqrt(1 - value * value))/value);
  }
  
  /**
    Returns the inverse hyperbolic secant of the value.
  */
  static inline double asech(double value) noexcept
  {
    return Math::ln((1 + Math::sqrt(1 - value * value))/value);
  }
  
  /**
    Returns the inverse hyperbolic secant of the value.
  */
  static inline long double asech(long double value) noexcept
  {
    return Math::ln((1 + Math::sqrt(1 - value * value))/value);
  }
  
  /**
    Returns the hyperbolic cosecant of the value (do not invoke for 0).
  */
  static inline float cosech(float value) noexcept
  {
    return 2/(Math::exp(value) - Math::exp(-value));
  }
  
  /**
    Returns the hyperbolic cosecant of the value (do not invoke for 0).
  */
  static inline double cosech(double value) noexcept
  {
    return 2/(Math::exp(value) - Math::exp(-value));
  }
  
  /**
    Returns the hyperbolic cosecant of the value (do not invoke for 0).
  */
  static inline long double cosech(long double value) noexcept
  {
    return 2/(Math::exp(value) - Math::exp(-value));
  }
  
  /**
    Returns the inverse hyperbolic cosecant of the value (do not invoke for 0).
  */
  static inline float acosech(float value) noexcept
  {
    return Math::ln(
      (1 + Math::sqrt(1 + value * value))/((value >= 0) ? value : -value)
    );
  }
  
  /**
    Returns the inverse hyperbolic cosecant of the value (do not invoke for 0).
  */
  static inline double acosech(double value) noexcept
  {
    return Math::ln(
      (1 + Math::sqrt(1 + value * value))/((value >= 0) ? value : -value)
    );
  }
  
  /**
    Returns the inverse hyperbolic cosecant of the value (do not invoke for 0).
  */
  static inline long double acosech(long double value) noexcept
  {
    return Math::ln(
      (1 + Math::sqrt(1 + value * value))/((value >= 0) ? value : -value)
    );
  }



  /**
    Returns the sinc (with normalization PI) of the value.
  */
  static inline float sinc(float value) noexcept
  {
    return (value == 0) ? 1 : Math::sin(value)/value;
  }
  
  /**
    Returns the sinc (with normalization PI) of the value.
  */
  static inline double sinc(double value) noexcept
  {
    return (value == 0) ? 1 : Math::sin(value)/value;
  }
  
  /**
    Returns the sinc (with normalization PI) of the value.
  */
  static inline long double sinc(long double value) noexcept
  {
    return (value == 0) ? 1 : Math::sin(value)/value;
  }

  /**
    Returns the sinc (with normalization 1) of the value.
  */
  static inline float sincPi(float value) noexcept
  {
    return (value == 0) ? 1 :
      Math::sin(constant::PI_F * value)/(constant::PI_F * value);
  }
  
  /**
    Returns the sinc (with normalization 1) of the value.
  */
  static inline double sincPi(double value) noexcept
  {
    return (value == 0) ? 1 :
      Math::sin(constant::PI * value)/(constant::PI * value);
  }
  
  /**
    Returns the sinc (with normalization 1) of the value.
  */
  static inline long double sincPi(long double value) noexcept
  {
    return (value == 0) ? 1 :
      Math::sin(constant::PI_L * value)/(constant::PI_L * value);
  }
  
  /**
    Returns the tanc of the value.
  */
  static inline float tanc(float value) noexcept
  {
    return (value == 0) ? 1 : Math::tan(value)/value;
  }
  
  /**
    Returns the tanc of the value.
  */
  static inline double tanc(double value) noexcept
  {
    return (value == 0) ? 1 : Math::tan(value)/value;
  }
  
  /**
    Returns the tanc of the value.
  */
  static inline long double tanc(long double value) noexcept
  {
    return (value == 0) ? 1 : Math::tan(value)/value;
  }

  /**
    Returns the natural logarithm of the gamma of the specified value (i.e.
    ln(gamma(x))) where gamma is defined as (x - 1)!.
  */
  static double lngamma(double value) noexcept;
  
  
  
  /**
    Returns the square root of the specified integer. The result is round down.
  */
  static inline unsigned int iSqrt(unsigned int value) noexcept
  {
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
  static inline unsigned int iSqrt8(unsigned int value) noexcept
  {
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
  static inline unsigned int iSqrt16(unsigned int value) noexcept
  {
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
  static inline unsigned int iSqrt8Round(unsigned int value) noexcept
  {
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
  static inline bool isPowerOf2(uint8 value) noexcept
  {
    // powers of 2 for 4 bit nibble
    // 1, 2, 4, and 8 => 0b0000000100010110 => 0x0116
    const unsigned int IS_POWER_OF_2 = 0x0116;
    return (((value & 0xf0) == 0) && (IS_POWER_OF_2 & (1 << value))) ||
      (((value & 0x0f) == 0) && (IS_POWER_OF_2 & (1 << (value >> 4))));
  }
  
  /**
    Returns true if the specified integer is a power of 2.
  */
  static inline bool isPowerOf2(uint32 value) noexcept
  {
    if (value != 0) {
      while ((value & 0xff) == 0) {
        value >>= 8;
      }
      if (value <= 0xff) {
        return isPowerOf2(static_cast<uint8>(value));
      }
    }
    return false;
  }

  /**
    Returns true if the specified integer is a power of 2.
  */
  static inline bool isPowerOf2(unsigned long long value) noexcept
  {
    if (value != 0) {
      while ((value & 0xff) == 0) {
        value >>= 8;
      }
      if (value <= 0xff) {
        return isPowerOf2(static_cast<uint8>(value));
      }
    }
    return false;
  }

  /**
    Returns the base-2 logarithm of the specified integer. The result is
    rounded down.

    @return 0 if value is 0.
  */
  static inline unsigned int iLog2(unsigned int value) noexcept
  {
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
  static inline unsigned int iLog2(unsigned long value) noexcept
  {
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
    Returns the base-2 logarithm of the specified integer. The result is
    rounded down.

    @return 0 if value is 0.
  */
  static inline unsigned int iLog2(unsigned long long value) noexcept
  {
    unsigned int result = sizeof(unsigned long long) * 8;
    for (unsigned int i = sizeof(unsigned long long) * 8 / 2; i > 0; i >>= 1) {
      if ((value >> (sizeof(unsigned long long) * 8 - i)) == 0) {
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
  static inline unsigned int getPowerOf2(unsigned int value) noexcept
  {
    unsigned int result = static_cast<unsigned int>(1) << iLog2(value);
    return (result >= value) ? result : (result << 1);
  }

  /**
    Returns the power of 2 greater than or equal to the specified value.

    @return 0 if power of 2 exceeds limit.
  */
  static inline unsigned long getPowerOf2(unsigned long value) noexcept
  {
    unsigned long result = static_cast<unsigned long>(1) << iLog2(value);
    return (result >= value) ? result : (result << 1);
  }

  /**
    Returns the power of 2 greater than or equal to the specified value.

    @return 0 if power of 2 exceeds limit.
  */
  static inline unsigned long long getPowerOf2(unsigned long long value) noexcept
  {
    unsigned long long result = static_cast<unsigned long long>(1) << iLog2(value);
    return (result >= value) ? result : (result << 1);
  }

  /**
    Returns the bit index of the highest set bit.
  */
  static inline unsigned int getHighestBit(uint8 value) noexcept
  {
    static const uint8 BITS[16] = {0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4};
    if (auto bits = BITS[value >> 4]) {
      return bits + 4;
    }
    return BITS[value];
  }

  /**
    Returns the bit index of the highest set bit.
  */
  static inline unsigned int getHighestBit(uint16 value) noexcept
  {
    if (auto bits = getHighestBit(static_cast<uint8>(value >> 8))) {
      return bits + 8;
    }
    return getHighestBit(static_cast<uint8>(value));
  }

  /**
    Returns the bit index of the highest set bit.
  */
  static inline unsigned int getHighestBit(uint32 value) noexcept
  {
    if (auto bits = getHighestBit(static_cast<uint16>(value >> 16))) {
      return bits + 16;
    }
    return getHighestBit(static_cast<uint16>(value));
  }

  /**
    Returns the bit index of the highest set bit.
  */
  static inline unsigned int getHighestBit(uint64 value) noexcept
  {
    if (auto bits = getHighestBit(static_cast<uint32>(value >> 32))) {
      return bits + 32;
    }
    return getHighestBit(static_cast<uint32>(value));
  }

  /**
    Returns the bit index of the highest set bit.
  */
  static inline unsigned int getHighestBit(const uint128& value) noexcept
  {
    if (auto bits = getHighestBit(value.high)) {
      return bits + 64;
    }
    return getHighestBit(value.low);
  }

  /**
    Returns the bit reversal of the specified 8 bit unsigned integer.
  */
  static inline uint8 getBitReversal(uint8 value) noexcept
  {
    return getBitReversalData()[value];
  }

  /**
    Returns the bit reversal of the specified 16 bit unsigned integer.
  */
  static inline uint16 getBitReversal(uint16 value) noexcept
  {
    return merge(
      getBitReversal(getHighWordOf16(value)),
      getBitReversal(getLowWordOf16(value))
    );
  }
  
  /**
    Returns the bit reversal of the specified 32 bit unsigned integer.
  */
  static inline uint32 getBitReversal(uint32 value) noexcept
  {
    return merge(
      getBitReversal(getHighWordOf32(value)),
      getBitReversal(getLowWordOf32(value))
    );
  }

  /**
    Returns the bit reversal of the specified 64 bit unsigned integer.
  */
  static inline uint64 getBitReversal(uint64 value) noexcept
  {
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
    unsigned int value, unsigned int count = 1) noexcept
  {
    count &= sizeof(unsigned int) * 8 - 1;
    return (value >> count) | (value << (sizeof(unsigned int) * 8 - count));
  }

  /**
    Rotates the bits of the specified value to the left.
    
    @param value The value to be rotated.
    @param count The number of bit to rotate. The default is 1.
  */
  static inline unsigned int rol(
    unsigned int value, unsigned int count = 1) noexcept
  {
    count &= sizeof(unsigned int) * 8 - 1;
    return (value << count) | (value >> (sizeof(unsigned int) * 8 - count));
  }

  /**
    Returns the greatest common divisor.
  */
  static int gcd(int m, int n) noexcept;
  
  /**
    Returns the greatest common divisor.
  */
  static unsigned int gcd(unsigned int n, unsigned int m) noexcept;

  /**
    Returns the least common multiple.
  */
  static inline int lcm(int m, int n) noexcept
  {
    return m * (n/gcd(m, n));
  }
  
  /**
    Returns the least common multiple.
  */
  static inline unsigned int lcm(unsigned int n, unsigned int m) noexcept
  {
    return m * (n/gcd(m, n));
  }
  
  /**
    Returns value * mul/div while handling overflow.
  */
  static uint64 muldiv(uint64 value, uint64 mul, uint64 div);

  /** Returns the carry for the addition. */
  static inline uint8 addCarry(uint8& value, const uint8 addend) noexcept
  {
    // use intrinsic
    constexpr uint8 MAXIMUM = 0xffU;
    if (value > (MAXIMUM - addend)) {
      value += addend;
      return 1;
    }
    value += addend;
    return 0;
  }

  /** Returns the carry for the addition. */
  static inline uint16 addCarry(uint16& value, const uint16 addend) noexcept
  {
    // use intrinsic
    constexpr uint16 MAXIMUM = 0xffffU;
    if (value > (MAXIMUM - addend)) {
      value += addend;
      return 1;
    }
    value += addend;
    return 0;
  }

  /** Returns the carry for the addition. */
  static inline uint32 addCarry(uint32& value, const uint32 addend) noexcept
  {
    // use intrinsic
    constexpr uint32 MAXIMUM = 0xffffffffU;
    if (value > (MAXIMUM - addend)) {
      value += addend;
      return 1;
    }
    value += addend;
    return 0;
  }

  /** Returns the carry for the addition. */
  static inline uint64 addCarry(uint64& value, const uint64 addend) noexcept
  {
    // use intrinsic
    constexpr uint64 MAXIMUM = (static_cast<uint64>(0) - 1);
    if (value > (MAXIMUM - addend)) {
      value += addend;
      return 1;
    }
    value += addend;
    return 0;
  }

  /** Returns 1 on borrow for subtraction. */
  static inline uint8 subtractBorrow(uint8& value, const uint8 subtrahend) noexcept
  {
    // use intrinsic
    const uint8 borrow = (subtrahend > value) ? 1 : 0;
    value -= subtrahend;
    return borrow;
  }

  /** Returns 1 on borrow for subtraction. */
  static inline uint16 subtractBorrow(uint16& value, const uint16 subtrahend) noexcept
  {
    // use intrinsic
    const uint16 borrow = (subtrahend > value) ? 1 : 0;
    value -= subtrahend;
    return borrow;
  }

  /** Returns 1 on borrow for subtraction. */
  static inline uint32 subtractBorrow(uint32& value, const uint32 subtrahend) noexcept
  {
    // use intrinsic
    const uint32 borrow = (subtrahend > value) ? 1 : 0;
    value -= subtrahend;
    return borrow;
  }

  /** Returns 1 on borrow for subtraction. */
  static inline uint64 subtractBorrow(uint64& value, const uint64 subtrahend) noexcept
  {
    // use intrinsic
    const uint64 borrow = (subtrahend > value) ? 1 : 0;
    value -= subtrahend;
    return borrow;
  }
};

/** Returns +/- 0 for float. */
template<>
inline constexpr float Math::getZero<float>(bool negative) noexcept
{
  return negative ? -0.0f : 0.0f;
}

/** Returns +/- 0 for double. */
template<>
inline constexpr double Math::getZero<double>(bool negative) noexcept
{
  return negative ? -0.0 : 0.0;
}

/** Returns +/- 0 for long double. */
template<>
inline constexpr long double Math::getZero<long double>(bool negative) noexcept
{
  return negative ? -0.0l : 0.0l;
}

#if (_COM_AZURE_DEV__BASE__HAS_FLOAT128)
/** Returns +/- 0 for float128. */
template<>
inline constexpr float128 Math::getZero<float128>(bool negative) noexcept
{
  return negative ? _COM_AZURE_DEV__BASE__MAKE_FLOAT128(-0.0) : _COM_AZURE_DEV__BASE__MAKE_FLOAT128(0.0);
}
#endif

/** Returns +infinity for float. */
template<>
inline /*constexpr*/ float Math::getInfinity<float>() noexcept
{
#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC)
  return __builtin_huge_valf();
#else
  return 1/0.f;
#endif
}

/** Returns +infinity for double. */
template<>
inline /*constexpr*/ double Math::getInfinity<double>() noexcept
{
#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC)
  return __builtin_huge_val();
#else
  return 1/0.;
#endif
}

/** Returns +infinity for long double. */
template<>
inline /*constexpr*/ long double Math::getInfinity<long double>() noexcept
{
#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC)
  return __builtin_huge_val(); // double same as long double
#else
  return 1/0.l;
#endif
}

#if (_COM_AZURE_DEV__BASE__HAS_FLOAT128)
/** Returns +infinity for float128. */
template<>
inline /*constexpr*/ float128 Math::getInfinity<float128>() noexcept
{
#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC)
  return __builtin_huge_val(); // double same as long double
#else
  return 1 / _COM_AZURE_DEV__BASE__MAKE_FLOAT128(0.);
#endif
}
#endif

/** Returns quiet NaN for float. */
template<>
inline /*constexpr*/ float Math::getNaN<float>() noexcept
{
#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC)
  return __builtin_nanf("0");
#else
  return 0/0.f;
#endif
}

/** Returns quiet NaN for double. */
template<>
inline /*constexpr*/ double Math::getNaN<double>() noexcept
{
#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC)
  return __builtin_nan("0");
#else
  return 0/0.;
#endif
}

/** Returns quiet NaN for long double. */
template<>
inline /*constexpr*/ long double Math::getNaN<long double>() noexcept
{
#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC)
  return __builtin_nan("0"); // double same as long double
#else
  return 0/0.l;
#endif
}

#if (_COM_AZURE_DEV__BASE__HAS_FLOAT128)
/** Returns quiet NaN for float128. */
template<>
inline /*constexpr*/ float128 Math::getNaN<float128>() noexcept
{
#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC)
  return __builtin_nan("0"); // double same as long double
#else
  return 0 / _COM_AZURE_DEV__BASE__MAKE_FLOAT128(0.);
#endif
}
#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
