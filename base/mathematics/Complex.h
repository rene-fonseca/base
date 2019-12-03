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

#include <base/mathematics/Math.h>
#include <base/string/FormatOutputStream.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <complex.h> // avoid - really bad as it defines I as macro
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

  static const Complex ZERO;
  static const Complex ONE;
  static const Complex MINUS_ONE;
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
#endif

#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
  static inline Complex getComplex(const _Dcomplex value) noexcept
  {
    return Complex(isoc::creal(value), isoc::cimag(value));
  }
#endif
public:

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

  /**
    Initializes complex number of the specified real and imaginary parts.

    @param real The desired real part.
    @param imaginary The desired imaginary part.
  */
  Complex(double real, double imaginary) noexcept;

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

  inline Complex sqrt() const noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::csqrt(getNative()));
#else
    return Complex();
#endif
  }

  inline Complex exp() const noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::cexp(getNative()));
#else
    return Complex();
#endif
  }

  inline Complex log() const noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::clog(getNative()));
#else
    return Complex();
#endif
  }

  inline Complex pow(const double x, const Complex& y) const noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::cpow(x, y.getNative()));
#else
    return Complex();
#endif
  }


  inline Complex sin() const noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::csin(getNative()));
#else
    return Complex();
#endif
  }

  inline Complex asin() const noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::casin(getNative()));
#else
    return Complex();
#endif
  }

  inline Complex cos() const noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::ccos(getNative()));
#else
    return Complex();
#endif
  }

  inline Complex acos() const noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::cacos(getNative()));
#else
    return Complex();
#endif
  }

  inline Complex tan() const noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::ctan(getNative()));
#else
    return Complex();
#endif
  }

  inline Complex atan() const noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::catan(getNative()));
#else
    return Complex();
#endif
  }


  inline Complex sinh() const noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::csinh(getNative()));
#else
    return Complex();
#endif
  }

  inline Complex asinh() const noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::casinh(getNative()));
#else
    return Complex();
#endif
  }

  inline Complex cosh() const noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::ccosh(getNative()));
#else
    return Complex();
#endif
  }

  inline Complex acosh() const noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::cacosh(getNative()));
#else
    return Complex();
#endif
  }

  inline Complex tanh() const noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::ctanh(getNative()));
#else
    return Complex();
#endif
  }

  inline Complex atanh() const noexcept
  {
#if defined(_COM_AZURE_DEV__BASE__COMPLEX_ISOC)
    return getComplex(isoc::catanh(getNative()));
#else
    return Complex();
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

  /**
    Returns the modulus of the complex number.
  */
  inline double getModulus() const noexcept
  {
    return Math::sqrt(getSqrModulus());
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
    double scale = getSqrModulus(); // TAG: possible division by zero
    //assert(scale != 0, DivisionByZero());
    scale = 1/scale;
    return Complex(real * scale, -imaginary * scale);
    return *this;
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
    auto c = divisor.conjugate();
    *this *= c;
    double r = divisor.real * divisor.real + divisor.imaginary * divisor.imaginary; // -1 * -1 = 1
    real /= r;
    imaginary /= r;
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



  /**
    Returns the sum of the complex numbers.
  */
  friend Complex operator+(const Complex& left, const Complex& right) noexcept;

  /**
    Returns the difference of the complex numbers.
  */
  friend Complex operator-(const Complex& left, const Complex& right) noexcept;

  /**
    Returns the product of the complex numbers.
  */
  friend Complex operator*(const Complex& left, const Complex& right) noexcept;

  /**
    Returns the product of the complex number and the real value.
  */
  friend Complex operator*(const Complex& left, double right) noexcept;

  /**
    Returns the product of the real value and the complex number.
  */
  friend Complex operator*(double left, const Complex& right) noexcept;

  /**
    Returns the result of the number divided by the value.
  */
  friend Complex operator/(const Complex& left, double right) noexcept;
};

inline Complex::Complex(double _real) noexcept
  : real(_real)
{
}

inline Complex::Complex(double _real, double _imaginary) noexcept
  : real(_real), imaginary(_imaginary)
{
}

inline Complex operator+(const Complex& left, const Complex& right) noexcept
{
  return Complex(left.real + right.real, left.imaginary + right.imaginary);
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

inline Complex operator*(const Complex& left, double right) noexcept
{
  return Complex(left.real * right, left.imaginary * right);
}

inline Complex operator*(double left, const Complex& right) noexcept
{
  return Complex(right.real * left, right.imaginary * left);
}

inline Complex operator/(const Complex& left, const Complex& right) noexcept
{
  Complex result(left);
  result /= right;
  return result;
}

inline Complex operator/(const Complex& left, double right) noexcept
{
  return Complex(left.real/right, left.imaginary/right);
}

/**
  Writes a string representation of the complex number to a format stream
  "(real[+-]imaginaryi)".

  @relates Complex
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const Complex& value) throw(IOException);

template<>
class IsUninitializeable<Complex> : public IsUninitializeable<double> {
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
