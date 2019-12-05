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
  Complex number represented by real and imaginary parts. double used. template class is not available.

  @short Complex number.
  @ingroup math
*/

class _COM_AZURE_DEV__BASE__API Complex {
public:

  typedef double Type;

  /** Imaginary tag. */
  class Imaginary {};
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
private:

  /** The real part of the complex number. */
  double real = 0;
  /** The imaginary part the complex number. */
  double imaginary = 0;

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
    
    double r = 0;
    double a = 0;
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
  Complex(double real) noexcept;

#if 0 // C++: does it make sense to disambiguate functions by setting priority e.g. [[prefer]]
  /** Initialize as imaginary. */
  Complex(const Imaginary) noexcept
    : imaginary(1) {
  }
#endif
  
  /**
    Initializes complex number of the specified real and imaginary parts.

    @param real The desired real part.
    @param imaginary The desired imaginary part.
  */
  Complex(double real, double imaginary) noexcept;

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
  inline Complex& operator=(double real) noexcept
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
    double r = x.getModulus();
    return Math::sqrt(r) * (x + r)/(x + r).getModulus(); // principal root
#endif
  }

  static inline Complex exp(const Complex& x) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::cexp(x.getNative()));
#else
    double r = Math::exp(x.real);
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

  static inline Complex pow(const double x, const Complex& y) noexcept
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
    return 0.5 * (exp(I * x) - exp(-x * I))/I;
#endif
  }

  static inline Complex asin(const Complex& x) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::casin(x.getNative()));
#else
    // handle inf/nan
    return -(I * ln(I * x + sqrt(1 - x * x)));
#endif
  }

  static inline Complex cos(const Complex& x) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::ccos(x.getNative()));
#else
    // handle inf/nan
    return 0.5 * (exp(I * x) + exp(-(I * x)));
#endif
  }

  static inline Complex acos(const Complex& x) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::cacos(x.getNative()));
#else
    // handle inf/nan
    return -(I * ln(x + I * sqrt(1 - x * x)));
#endif
  }

  static inline Complex tan(const Complex& x) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::ctan(x.getNative()));
#else
    // handle inf/nan
    auto a = exp(I * x);
    auto b = exp(-(I * x));
    return (a - b)/(I * (a + b));
#endif
  }

  static inline Complex atan(const Complex& x) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::catan(x.getNative()));
#else
    // handle inf/nan
    return I * 0.5 + ln((I + x)/(I - x));
#endif
  }


  static inline Complex sinh(const Complex& x) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::csinh(x.getNative()));
#else
    // handle inf/nan
    return 0.5 * (exp(x) - exp(-x));
#endif
  }

  static inline Complex asinh(const Complex& x) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::casinh(x.getNative()));
#else
    // handle inf/nan
    return ln(x + sqrt(x * x + 1));
#endif
  }

  static inline Complex cosh(const Complex& x) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::ccosh(x.getNative()));
#else
    // handle inf/nan
    return 0.5 * (exp(x) + exp(-x));
#endif
  }

  //
  static inline Complex acosh(const Complex& x) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::cacosh(getNative()));
#else
    // handle inf/nan
    return ln(x + sqrt(x * x - 1));
#endif
  }

  static inline Complex tanh(const Complex& x) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::ctanh(x.getNative()));
#else
    // handle inf/nan
    auto temp = exp(2 * x);
    return (temp - 1)/(temp + 1);
#endif
  }

  static inline Complex atanh(const Complex& x) noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::catanh(x.getNative()));
#else
    // handle inf/nan
    return ln((1 + x)/(1 - x)) * 0.5;
#endif
  }

  

  /**
    Returns the real part of the complex number.
  */
  inline double getReal() const noexcept
  {
    return real;
  }

  /**
    Returns the imaginary part of the complex number.
  */
  inline double getImaginary() const noexcept
  {
    return imaginary;
  }

  /**
    Sets the real part of the complex number.
  */
  inline void setReal(const double value) noexcept
  {
    real = value;
  }

  /**
    Sets the imaginary part of the complex number.
  */
  inline void setImaginary(const double value) noexcept
  {
    imaginary = value;
  }

  /**
    Returns the square of the modulus of the complex number.
  */
  inline double getSqrModulus() const noexcept
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
  inline double getModulus() const noexcept
  {
    return Math::hypot(real, imaginary);
  }

  /**
    Returns the angle of the complex number.
  */
  inline double getAngle() const noexcept
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
    double r = real * real + imaginary * imaginary; // -1 * -1 = 1
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
    double temp = (real * value.real - imaginary * value.imaginary);
    imaginary = (real * value.imaginary + imaginary * value.real);
    real = temp;
    return *this;
  }

  /**
    Multiplies this number with the specified value.

    @param value The multiplicator.
  */
  inline Complex& operator*=(double value) noexcept
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
    double temp = (real * 0 - imaginary * 1);
    imaginary = (real * 1 + imaginary * 0);
    real = temp;
    return *this;
  }

  /**
    Divides this number with the specified value.

    @param value The divisor.
  */
  inline Complex& operator/=(double divisor) noexcept
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
    double r = divisor.real * divisor.real + divisor.imaginary * divisor.imaginary; // -1 * -1 = 1
    real /= r;
    imaginary /= r;
    return *this;
  }

  inline Complex& operator/=(const Imaginary) noexcept
  {
    double temp = (/*real * 0*/ - imaginary * -1);
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

  friend Complex operator+(const double left, const Complex& right) noexcept;
  friend Complex operator+(const Imaginary, const Complex& right) noexcept;
  friend Complex operator+(const Complex& left, const Imaginary) noexcept;
  friend Complex operator+(const Complex& left, const double right) noexcept;
  friend Complex operator+(const Complex& left, const Complex& right) noexcept;
  friend Complex operator-(const double left, const Complex& right) noexcept;
  friend Complex operator-(const Imaginary, const Complex& right) noexcept;
  friend Complex operator-(const Complex& left, const Imaginary) noexcept;
  friend Complex operator-(const Complex& left, const double right) noexcept;
  friend Complex operator-(const Complex& left, const Complex& right) noexcept;
  friend Complex operator*(const Complex& left, const Complex& right) noexcept;
  friend Complex operator*(const Imaginary, const Complex& right) noexcept;
  friend Complex operator*(const Complex& left, const Imaginary) noexcept;
  friend Complex operator*(const Complex& left, double right) noexcept;
  friend Complex operator*(double left, const Complex& right) noexcept;
  friend Complex operator/(const Complex& left, double right) noexcept;
  friend Complex operator/(const Complex& left, const Complex& right) noexcept;
  friend Complex operator/(const Complex& left, const Imaginary) noexcept;
};

inline Complex::Complex(double _real) noexcept
  : real(_real)
{
}

inline Complex::Complex(double _real, double _imaginary) noexcept
  : real(_real), imaginary(_imaginary)
{
}

inline Complex operator+(const double left, const Complex& right) noexcept
{
  return Complex(left + right.real, right.imaginary);
}

inline Complex operator+(const Complex& left, const double right) noexcept
{
  return Complex(left.real + right, left.imaginary);
}

inline Complex operator+(const Complex::Imaginary, const Complex& right) noexcept
{
  return Complex(right.real, 1 + right.imaginary);
}

inline Complex operator+(const Complex& left, const Complex::Imaginary) noexcept
{
  return Complex(left.real, left.imaginary + 1);
}

inline Complex operator+(const Complex& left, const Complex& right) noexcept
{
  return Complex(left.real + right.real, left.imaginary + right.imaginary);
}

inline Complex operator-(const double left, const Complex& right) noexcept
{
  return Complex(left - right.real, -right.imaginary);
}

inline Complex operator-(const Complex& left, const double right) noexcept
{
  return Complex(left.real - right, left.imaginary);
}

inline Complex operator-(const Complex::Imaginary, const Complex& right) noexcept
{
  return Complex(-right.real, 1 - right.imaginary);
}

inline Complex operator-(const Complex& left, const Complex::Imaginary) noexcept
{
  return Complex(left.real, left.imaginary - 1);
}

inline Complex operator-(const Complex& left, const Complex& right) noexcept
{
  return Complex(left.real - right.real, left.imaginary - right.imaginary);
}

inline Complex operator*(const Complex& left, const Complex& right) noexcept
{
  return Complex(
    left.real * right.real - left.imaginary * right.imaginary,
    left.real * right.imaginary + left.imaginary * right.real
  );
}

inline Complex operator*(const Complex::Imaginary, const Complex& right) noexcept
{
  return Complex(
    /*0 * right.real*/ - 1 * right.imaginary,
    /*0 * right.imaginary*/ + 1 * right.real
  );
}

inline Complex operator*(const Complex& left, const Complex::Imaginary) noexcept
{
  return Complex(
    /*left.real * 0*/ - left.imaginary * 1,
    left.real * 1 /*+ left.imaginary * 0*/
  );
}

inline Complex operator*(const Complex& left, double right) noexcept
{
  return Complex(left.real * right, left.imaginary * right);
}

inline Complex operator*(double left, const Complex& right) noexcept
{
  return Complex(right.real * left, right.imaginary * left);
}

inline Complex operator/(const Complex& left, double right) noexcept
{
  return Complex(left.real/right, left.imaginary/right);
}

inline Complex operator/(const Complex& left, const Complex& right) noexcept
{
  Complex result(left);
  result /= right;
  return result;
}

inline Complex operator/(const Complex& left, const Complex::Imaginary) noexcept
{
  Complex result(left);
  result /= Complex::I;
  return result;
}

/**
  Writes a string representation of the complex number to a format stream
  "(real[+-]imaginaryi)".

  @relates Complex
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const Complex& value);

template<>
class IsUninitializeable<Complex> : public IsUninitializeable<double> {
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
