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

#include <base/math/Math.h>
#include <base/string/FormatOutputStream.h>

#if 0 && (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include___ <complex.h> // avoid - really bad as it defines I as macro
#  define _COM_AZURE_DEV__BASE__COMPLEX_ISOC
#endif

#if defined(I)
#  error I defined
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
namespace isoc {

  extern "C" float crealf(_Fcomplex);
  extern "C" double creal(_Dcomplex);
  extern "C" long double creall(_Lcomplex);

  extern "C" float cimagf(_Fcomplex);
  extern "C" double cimag(_Dcomplex);
  extern "C" long double cimagl(_Lcomplex);

#if 0
  extern "C" _Fcomplex cabsf(_Fcomplex);
  extern "C" _Dcomplex cabs(_Dcomplex);
  extern "C" _Lcomplex cabsl(_Lcomplex);
#endif

  extern "C" _Fcomplex csqrtf(_Fcomplex);
  extern "C" _Dcomplex csqrt(_Dcomplex);
  extern "C" _Lcomplex csqrtl(_Lcomplex);

  extern "C" _Fcomplex cexpf(_Fcomplex);
  extern "C" _Dcomplex cexp(_Dcomplex);
  extern "C" _Lcomplex cexpl(_Lcomplex);

  extern "C" _Fcomplex clogf(_Fcomplex);
  extern "C" _Dcomplex clog(_Dcomplex);
  extern "C" _Lcomplex clogl(_Lcomplex);

  extern "C" _Fcomplex cpowf(float, _Fcomplex);
  extern "C" _Dcomplex cpow(double, _Dcomplex);
  extern "C" _Lcomplex cpowl(_Lcomplex, _Lcomplex);


  extern "C" _Fcomplex csinf(_Fcomplex);
  extern "C" _Dcomplex csin(_Dcomplex);
  extern "C" _Lcomplex csinl(_Lcomplex);

  extern "C" _Fcomplex ccosf(_Fcomplex);
  extern "C" _Dcomplex ccos(_Dcomplex);
  extern "C" _Lcomplex ccosl(_Lcomplex);

  extern "C" _Fcomplex casinf(_Fcomplex);
  extern "C" _Dcomplex casin(_Dcomplex);
  extern "C" _Lcomplex casinl(_Lcomplex);

  extern "C" _Fcomplex cacosf(_Fcomplex);
  extern "C" _Dcomplex cacos(_Dcomplex);
  extern "C" _Lcomplex cacosl(_Lcomplex);

  extern "C" _Fcomplex ctanf(_Fcomplex);
  extern "C" _Dcomplex ctan(_Dcomplex);
  extern "C" _Lcomplex ctanl(_Lcomplex);

  extern "C" _Fcomplex catanf(_Fcomplex);
  extern "C" _Dcomplex catan(_Dcomplex);
  extern "C" _Lcomplex catanl(_Lcomplex);


  extern "C" _Fcomplex csinhf(_Fcomplex);
  extern "C" _Dcomplex csinh(_Dcomplex);
  extern "C" _Lcomplex csinhl(_Lcomplex);

  extern "C" _Fcomplex ccoshf(_Fcomplex);
  extern "C" _Dcomplex ccosh(_Dcomplex);
  extern "C" _Lcomplex ccoshl(_Lcomplex);

  extern "C" _Fcomplex casinhf(_Fcomplex);
  extern "C" _Dcomplex casinh(_Dcomplex);
  extern "C" _Lcomplex casinhl(_Lcomplex);

  extern "C" _Fcomplex cacoshf(_Fcomplex);
  extern "C" _Dcomplex cacosh(_Dcomplex);
  extern "C" _Lcomplex cacoshl(_Lcomplex);

  extern "C" _Fcomplex ctanhf(_Fcomplex);
  extern "C" _Dcomplex ctanh(_Dcomplex);
  extern "C" _Lcomplex ctanhl(_Lcomplex);

  extern "C" _Fcomplex catanhf(_Fcomplex);
  extern "C" _Dcomplex catanh(_Dcomplex);
  extern "C" _Lcomplex catanhl(_Lcomplex);
};
#endif

/**
  Complex number represented by real and imaginary parts.

  @short Complex number.
  @ingroup math
*/

template<typename TYPE>
class _COM_AZURE_DEV__BASE__API Complex {
public:

  typedef TYPE Type;

  /** Imaginary tag. */
  class Imaginary {
  public:

    inline Imaginary() noexcept
    {
    }
  };
  /** Imaginary symbol. Use for faster math operations. */
  static const Imaginary I;

  /** Complex number 0. */
  static const Complex ZERO;
  /** Complex number 1. */
  static const Complex ONE;
  /** Complex number -1. */
  static const Complex MINUS_ONE;
  /** Complex number 1i. */
  static const Complex II;

  static constexpr Type _ONE = 1;
public:
  
  /** The real part of the complex number. */
  Type real = 0;
  /** The imaginary part the complex number. */
  TYPE imaginary = 0;

#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
  inline _Dcomplex getNative() const noexcept
  {
    return _DCOMPLEX_(real, imaginary);
    // return real + imaginary * 1i;
  }

  static inline Complex getComplex(const _Dcomplex value) noexcept
  {
    return Complex(isoc::creal(value), isoc::cimag(value));
  }
#endif
public:

  /** Polar representation. Complex number equals to r * (cos(a) + i * sin(a)). */
  class Polar {
  public:
    
    Type r = 0;
    Type a = 0;
  };

  /**
    Initializes complex number as (0, 0).
  */
  inline Complex() noexcept
  {
  }

  /**
    Initializes complex number with the specified real part. The imaginary part
    is initialized to zero.

    @param real The real part.
  */
  inline Complex(Type real) noexcept;

#if 0 // C++: does it make sense to disambiguate functions by setting priority e.g. [[prefer]]
  /** Initialize as imaginary. */
  Complex(const Imaginary) noexcept
    : imaginary(1)
  {
  }
#endif
  
  /**
    Initializes complex number of the specified real and imaginary parts.

    @param real The desired real part.
    @param imaginary The desired imaginary part.
  */
  inline Complex(Type real, Type imaginary) noexcept;

  inline Complex(const Polar& polar) noexcept
  {
    real = polar.r * Math::cos(polar.a);
    imaginary = polar.r * Math::sin(polar.a);
  }

  /**
    Initializes complex number of other complex number.
  */
  inline Complex(const Complex& copy) noexcept
    : real(copy.real), imaginary(copy.imaginary)
  {
  }

  /**
    Assignment of complex number by complex number.
  */
  inline Complex& operator=(const Complex& assign) noexcept
  {
    real = assign.real; // no need to protect against self-assignment
    imaginary = assign.imaginary;
    return *this;
  }

  /**
    Assignment of complex number by real number.
  */
  inline Complex& operator=(Type real) noexcept
  {
    this->real = real; // no need to protect against self-assignment
    imaginary = 0;
    return *this;
  }
  
  static inline Complex sqrt(const Complex& x) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::csqrt(x.getNative()));
#else
    Type r = x.getModulus();
    return Math::sqrt(r) * (x + r)/(x + r).getModulus(); // principal root
#endif
  }

  static inline Complex exp(const Complex& x) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::cexp(x.getNative()));
#else
    Type r = Math::exp(x.real);
    return Complex(r * Math::cos(x.imaginary), r * Math::sin(x.imaginary));
#endif
  }

  static inline Complex ln(const Complex& x) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::clog(x.getNative()));
#else
    auto p = x.getPolar();
    return Complex(Math::ln(p.r), p.a);
#endif
  }

  static inline Complex pow(const Type x, const Complex& y) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::cpow(x, y.getNative()));
#else
    return Complex();
#endif
  }


  static inline Complex sin(const Complex& x) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::csin(x.getNative()));
#else
    // handle inf/nan
    return (_ONE / 2) * (exp(Imaginary() * x) - exp(-x * Imaginary()))/Imaginary();
#endif
  }

  static inline Complex asin(const Complex& x) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::casin(x.getNative()));
#else
    // handle inf/nan
    return -(Imaginary() * ln(Imaginary() * x + sqrt(_ONE - x * x)));
#endif
  }

  static inline Complex cos(const Complex& x) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::ccos(x.getNative()));
#else
    // handle inf/nan
    return (_ONE / 2) * (exp(Imaginary() * x) + exp(-(Imaginary() * x)));
#endif
  }

  static inline Complex acos(const Complex& x) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::cacos(x.getNative()));
#else
    // handle inf/nan
    return -(Imaginary() * ln(x + Imaginary() * sqrt(_ONE - x * x)));
#endif
  }

  static inline Complex tan(const Complex& x) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::ctan(x.getNative()));
#else
    // handle inf/nan
    auto a = exp(Imaginary() * x);
    auto b = exp(-(Imaginary() * x));
    return (a - b)/(Imaginary() * (a + b));
#endif
  }

  static inline Complex atan(const Complex& x) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::catan(x.getNative()));
#else
    // handle inf/nan
    return Complex(0, _ONE / 2) + ln((Imaginary() + x)/(Imaginary() - x));
#endif
  }


  static inline Complex sinh(const Complex& x) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::csinh(x.getNative()));
#else
    // handle inf/nan
    return (_ONE / 2) * (exp(x) - exp(-x));
#endif
  }

  static inline Complex asinh(const Complex& x) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::casinh(x.getNative()));
#else
    // handle inf/nan
    return ln(x + sqrt(x * x + _ONE));
#endif
  }

  static inline Complex cosh(const Complex& x) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::ccosh(x.getNative()));
#else
    // handle inf/nan
    return (_ONE / 2) * (exp(x) + exp(-x));
#endif
  }

  static inline Complex acosh(const Complex& x) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::cacosh(getNative()));
#else
    // handle inf/nan
    return ln(x + sqrt(x * x - _ONE));
#endif
  }

  static inline Complex tanh(const Complex& x) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::ctanh(x.getNative()));
#else
    // handle inf/nan
    auto temp = exp(2 * x);
    return (temp - _ONE)/(temp + _ONE);
#endif
  }

  static inline Complex atanh(const Complex& x) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::catanh(x.getNative()));
#else
    // handle inf/nan
    return ln((_ONE + x)/(_ONE - x)) * (_ONE/2);
#endif
  }

  

  /**
    Returns the real part of the complex number.
  */
  inline Type getReal() const noexcept
  {
    return real;
  }

  /**
    Returns the imaginary part of the complex number.
  */
  inline Type getImaginary() const noexcept
  {
    return imaginary;
  }

  /**
    Sets the real part of the complex number.
  */
  inline void setReal(const Type value) noexcept
  {
    real = value;
  }

  /**
    Sets the imaginary part of the complex number.
  */
  inline void setImaginary(const Type value) noexcept
  {
    imaginary = value;
  }

  /**
    Returns the square of the modulus of the complex number.
  */
  inline Type getSqrModulus() const noexcept
  {
    return real * real + imaginary * imaginary;
  }
  
  inline Polar getPolar() const noexcept
  {
    Polar result;
    result.r = Math::hypot(real, imaginary);
    result.a = Math::atan2(imaginary, real);
    return result;
  }

  /**
    Returns the modulus of the complex number.
  */
  inline Type getModulus() const noexcept
  {
    return Math::hypot(real, imaginary);
  }

  /**
    Returns the angle of the complex number.
  */
  inline Type getAngle() const noexcept
  {
    return Math::atan2(imaginary, real);
  }

  /**
    Unary plus.
  */
  inline Complex plus() const noexcept
  {
    return Complex(real, imaginary);
  }

  /**
    Unary minus.
  */
  inline Complex minus() const noexcept
  {
    return Complex(-real, -imaginary);
  }

  /**
    Negates this number.
  */
  inline Complex negate() const noexcept
  {
    return Complex(-real, -imaginary);
  }

  /**
    Conjugates this number.
  */
  inline Complex conjugate() const noexcept
  {
    return Complex(real, -imaginary);
  }

  /**
    Inverts this number.
  */
  inline Complex invert() const noexcept
  {
    // 1/complex <=> conjugate/(complex * conjugate)
    Type r = real * real + imaginary * imaginary; // -1 * -1 = 1
    return Complex(real/r, -imaginary/r);
  }



  /**
    Returns true if the numbers are equal.

    @param value Complex number to be compared.
  */
  inline bool operator==(const Complex& value) const noexcept
  {
    return (real == value.real) && (imaginary == value.imaginary);
  }

  inline bool operator!=(const Complex& value) const noexcept
  {
    return (real != value.real) || (imaginary != value.imaginary);
  }
  
  /**
    Adds the specified number to this number.

    @param value The value to be added.
  */
  inline Complex& operator+=(const Complex& value) noexcept
  {
    real += value.real;
    imaginary += value.imaginary;
    return *this;
  }

  /**
    Adds the specified number to this number.

    @param value The value to be added.
  */
  inline Complex& operator+=(const Imaginary) noexcept
  {
    real += 0;
    imaginary += 1;
    return *this;
  }

  /**
    Subtracts the specified complex from this number.

    @param value The value to be subtracted.
  */
  inline Complex& operator-=(const Complex& value) noexcept
  {
    real -= value.real;
    imaginary -= value.imaginary;
    return *this;
  }

  /**
    Subtracts the specified complex from this number.

    @param value The value to be subtracted.
  */
  inline Complex& operator-=(const Imaginary) noexcept
  {
    real -= 0;
    imaginary -= 1;
    return *this;
  }

  /**
    Multiplies this number with the specified value.

    @param value The multiplicator.
  */
  inline Complex& operator*=(const Complex& value) noexcept
  {
    Type temp = (real * value.real - imaginary * value.imaginary);
    imaginary = (real * value.imaginary + imaginary * value.real);
    real = temp;
    return *this;
  }

  /**
    Multiplies this number with the specified value.

    @param value The multiplicator.
  */
  inline Complex& operator*=(Type value) noexcept
  {
    real *= value;
    imaginary *= value;
    return *this;
  }

  /**
    Multiplies this number with the specified value.

    @param value The multiplicator.
  */
  inline Complex& operator*=(const Imaginary) noexcept
  {
    Type temp = (real * 0 - imaginary * 1);
    imaginary = (real * 1 + imaginary * 0);
    real = temp;
    return *this;
  }

  /**
    Divides this number with the specified value.

    @param value The divisor.
  */
  inline Complex& operator/=(Type divisor) noexcept
  {
    real /= divisor;
    imaginary /= divisor;
    return *this;
  }
  
  /**
    Divides this number with the specified value.

    @param value The divisor.
  */
  inline Complex& operator/=(const Complex& divisor) noexcept
  {
    const Complex c = divisor.conjugate();
    *this *= c;
    Type r = divisor.real * divisor.real + divisor.imaginary * divisor.imaginary; // -1 * -1 = 1
    real /= r;
    imaginary /= r;
    return *this;
  }

  inline Complex& operator/=(const Imaginary) noexcept
  {
    Type temp = (/*real * 0*/ - imaginary * -1);
    imaginary = (real * -1 /*+ imaginary * 0*/);
    real = temp;
    return *this;
  }

  /**
    Unary plus.
  */
  inline Complex operator+() const noexcept
  {
    return Complex(real, imaginary);
  }

  /**
    Unary minus.
  */
  inline Complex operator-() const noexcept
  {
    return Complex(-real, -imaginary);
  }

#if 0
  friend Complex operator+(const Type left, const Complex& right) noexcept;
  friend Complex operator+(const Imaginary, const Complex& right) noexcept;
  friend Complex operator+(const Complex& left, const Imaginary) noexcept;
  friend Complex operator+(const Complex& left, const Type right) noexcept;
  friend Complex operator+(const Complex& left, const Complex& right) noexcept;
  friend Complex operator-(const Type left, const Complex& right) noexcept;
  friend Complex operator-(const Imaginary, const Complex& right) noexcept;
  friend Complex operator-(const Complex& left, const Imaginary) noexcept;
  friend Complex operator-(const Complex& left, const Type right) noexcept;
  friend Complex operator-(const Complex& left, const Complex& right) noexcept;
  friend Complex operator*(const Complex& left, const Complex& right) noexcept;
  friend Complex operator*(const Imaginary, const Complex& right) noexcept;
  friend Complex operator*(const Complex& left, const Imaginary) noexcept;
  friend Complex operator*(const Complex& left, Type right) noexcept;
  friend Complex operator*(Type left, const Complex& right) noexcept;
  friend Complex operator/(const Complex& left, Type right) noexcept;
  friend Complex operator/(const Complex& left, const Complex& right) noexcept;
  friend Complex operator/(const Complex& left, const Imaginary) noexcept;
#endif
};

template<typename TYPE>
inline Complex<TYPE>::Complex(TYPE _real) noexcept
  : real(_real)
{
}

template<typename TYPE>
inline Complex<TYPE>::Complex(TYPE _real, TYPE _imaginary) noexcept
  : real(_real), imaginary(_imaginary)
{
}

template<typename TYPE>
inline Complex<TYPE> operator+(const TYPE left, const Complex<TYPE>& right) noexcept
{
  return Complex<TYPE>(left + right.real, right.imaginary);
}

template<typename TYPE>
inline Complex<TYPE> operator+(const int left, const Complex<TYPE>& right) noexcept
{
  return Complex<TYPE>(left + right.real, right.imaginary);
}

template<typename TYPE>
inline Complex<TYPE> operator+(const Complex<TYPE>& left, const TYPE right) noexcept
{
  return Complex<TYPE>(left.real + right, left.imaginary);
}

template<typename TYPE>
inline Complex<TYPE> operator+(const Complex<TYPE>& left, const int right) noexcept
{
  return Complex<TYPE>(left.real + right, left.imaginary);
}

template<typename TYPE>
inline Complex<TYPE> operator+(const typename Complex<TYPE>::Imaginary, const Complex<TYPE>& right) noexcept
{
  return Complex<TYPE>(right.real, 1 + right.imaginary);
}

template<typename TYPE>
inline Complex<TYPE> operator+(const Complex<TYPE>& left, const typename Complex<TYPE>::Imaginary) noexcept
{
  return Complex<TYPE>(left.real, left.imaginary + 1);
}

template<typename TYPE>
inline Complex<TYPE> operator+(const Complex<TYPE>& left, const Complex<TYPE>& right) noexcept
{
  return Complex<TYPE>(left.real + right.real, left.imaginary + right.imaginary);
}

template<typename TYPE>
inline Complex<TYPE> operator-(const TYPE left, const Complex<TYPE>& right) noexcept
{
  return Complex<TYPE>(left - right.real, -right.imaginary);
}

template<typename TYPE>
inline Complex<TYPE> operator-(const Complex<TYPE>& left, const TYPE right) noexcept
{
  return Complex<TYPE>(left.real - right, left.imaginary);
}

template<typename TYPE>
inline Complex<TYPE> operator-(const Complex<TYPE>& left, const int right) noexcept
{
  return Complex<TYPE>(left.real - right, left.imaginary);
}

template<typename TYPE>
inline Complex<TYPE> operator-(const typename Complex<TYPE>::Imaginary, const Complex<TYPE>& right) noexcept
{
  return Complex<TYPE>(-right.real, 1 - right.imaginary);
}

template<typename TYPE>
inline Complex<TYPE> operator-(const Complex<TYPE>& left, const typename Complex<TYPE>::Imaginary) noexcept
{
  return Complex<TYPE>(left.real, left.imaginary - 1);
}

template<typename TYPE>
inline Complex<TYPE> operator-(const Complex<TYPE>& left, const Complex<TYPE>& right) noexcept
{
  return Complex<TYPE>(left.real - right.real, left.imaginary - right.imaginary);
}

template<typename TYPE>
inline Complex<TYPE> operator*(const Complex<TYPE>& left, const Complex<TYPE>& right) noexcept
{
  return Complex<TYPE>(
    left.real * right.real - left.imaginary * right.imaginary,
    left.real * right.imaginary + left.imaginary * right.real
  );
}

template<typename TYPE>
Complex<TYPE> operator*(const typename Complex<TYPE>::Imaginary, const Complex<TYPE>& right) noexcept
{
  return Complex<TYPE>(
    /*0 * right.real*/ - 1 * right.imaginary,
    /*0 * right.imaginary*/ + 1 * right.real
  );
}

template<typename TYPE>
inline Complex<TYPE> operator*(const Complex<TYPE>& left, const typename Complex<TYPE>::Imaginary) noexcept
{
  return Complex<TYPE>(
    /*left.real * 0*/ - left.imaginary * 1,
    left.real * 1 /*+ left.imaginary * 0*/
  );
}

template<typename TYPE>
inline Complex<TYPE> operator*(const Complex<TYPE>& left, TYPE right) noexcept
{
  return Complex<TYPE>(left.real * right, left.imaginary * right);
}

template<typename TYPE>
inline Complex<TYPE> operator*(const Complex<TYPE>& left, int right) noexcept
{
  return Complex<TYPE>(left.real * right, left.imaginary * right);
}

template<typename TYPE>
inline Complex<TYPE> operator*(TYPE left, const Complex<TYPE>& right) noexcept
{
  return Complex<TYPE>(right.real * left, right.imaginary * left);
}

template<typename TYPE>
inline Complex<TYPE> operator*(int left, const Complex<TYPE>& right) noexcept
{
  return Complex<TYPE>(right.real * left, right.imaginary * left);
}

template<typename TYPE>
inline Complex<TYPE> operator/(const Complex<TYPE>& left, TYPE right) noexcept
{
  return Complex<TYPE>(left.real/right, left.imaginary/right);
}

template<typename TYPE>
inline Complex<TYPE> operator/(const Complex<TYPE>& left, int right) noexcept
{
  return Complex<TYPE>(left.real / right, left.imaginary / right);
}

template<typename TYPE>
inline Complex<TYPE> operator/(const Complex<TYPE>& left, const Complex<TYPE>& right) noexcept
{
  Complex<TYPE> result(left);
  result /= right;
  return result;
}

template<typename TYPE>
inline Complex<TYPE> operator/(const Complex<TYPE>& left, const typename Complex<TYPE>::Imaginary) noexcept
{
  Complex<TYPE> result(left);
  result /= Complex<TYPE>::I;
  return result;
}

/**
  Writes a string representation of the complex number to a format stream
  "(real[+-]imaginaryi)".

  @relates Complex
*/
template<typename TYPE>
_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(FormatOutputStream& stream, const Complex<TYPE>& value);

template<typename TYPE>
class IsUninitializeable<Complex<TYPE> > : public IsUninitializeable<TYPE> {
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
