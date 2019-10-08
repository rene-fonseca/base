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

#include <base/string/FormatOutputStream.h>
#include <base/mathematics/Math.h>
#include <complex.h>

// see std::complex instead

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)

#define _DK_SDU_MIP__BASE__COMPLEX_ISOC

// TAG: need typedef for complex types
// typedef float complex _Fcomplex;
// typedef double complex _Dcomplex;
// typedef long double complex _Lcomplex;

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if defined(_DK_SDU_MIP__BASE__COMPLEX_ISOC)
namespace isoc {

  extern "C" float crealf(_Fcomplex);
  extern "C" double creal(_Dcomplex);
  extern "C" long double creall(_Lcomplex);

  extern "C" float cimagf(_Fcomplex);
  extern "C" double cimag(_Dcomplex);
  extern "C" long double cimagl(_Lcomplex);

  extern "C" _Fcomplex cabsf(_Fcomplex);
  extern "C" _Dcomplex cabs(_Dcomplex);
  extern "C" _Lcomplex  cabsl(_Lcomplex);

  extern "C" _Fcomplex csqrtf(_Fcomplex);
  extern "C" _Dcomplex csqrt(_Dcomplex);
  extern "C" _Lcomplex  csqrtl(_Lcomplex);

  extern "C" _Fcomplex cexpf(_Fcomplex);
  extern "C" _Dcomplex cexp(_Dcomplex);
  extern "C" _Lcomplex  cexpl(_Lcomplex);

  extern "C" _Fcomplex clogf(_Fcomplex);
  extern "C" _Dcomplex clog(_Dcomplex);
  extern "C" _Lcomplex  clogl(_Lcomplex);

  extern "C" _Fcomplex cpowf(float, _Fcomplex);
  extern "C" _Dcomplex cpow(double, _Dcomplex);
  extern "C" _Lcomplex  cpowl(_Lcomplex, _Lcomplex);


  extern "C" _Fcomplex csinf(_Fcomplex);
  extern "C" _Dcomplex csin(_Dcomplex);
  extern "C" _Lcomplex  csinl(_Lcomplex);

  extern "C" _Fcomplex ccosf(_Fcomplex);
  extern "C" _Dcomplex ccos(_Dcomplex);
  extern "C" _Lcomplex  ccosl(_Lcomplex);

  extern "C" _Fcomplex casinf(_Fcomplex);
  extern "C" _Dcomplex casin(_Dcomplex);
  extern "C" _Lcomplex  casinl(_Lcomplex);

  extern "C" _Fcomplex cacosf(_Fcomplex);
  extern "C" _Dcomplex cacos(_Dcomplex);
  extern "C" _Lcomplex  cacosl(_Lcomplex);

  extern "C" _Fcomplex ctanf(_Fcomplex);
  extern "C" _Dcomplex ctan(_Dcomplex);
  extern "C" _Lcomplex  ctanl(_Lcomplex);

  extern "C" _Fcomplex catanf(_Fcomplex);
  extern "C" _Dcomplex catan(_Dcomplex);
  extern "C" _Lcomplex  catanl(_Lcomplex);


  extern "C" _Fcomplex csinhf(_Fcomplex);
  extern "C" _Dcomplex csinh(_Dcomplex);
  extern "C" _Lcomplex  csinhl(_Lcomplex);

  extern "C" _Fcomplex ccoshf(_Fcomplex);
  extern "C" _Dcomplex ccosh(_Dcomplex);
  extern "C" _Lcomplex  ccoshl(_Lcomplex);

  extern "C" _Fcomplex casinhf(_Fcomplex);
  extern "C" _Dcomplex casinh(_Dcomplex);
  extern "C" _Lcomplex  casinhl(_Lcomplex);

  extern "C" _Fcomplex cacoshf(_Fcomplex);
  extern "C" _Dcomplex cacosh(_Dcomplex);
  extern "C" _Lcomplex  cacoshl(_Lcomplex);

  extern "C" _Fcomplex ctanhf(_Fcomplex);
  extern "C" _Dcomplex ctanh(_Dcomplex);
  extern "C" _Lcomplex  ctanhl(_Lcomplex);

  extern "C" _Fcomplex catanhf(_Fcomplex);
  extern "C" _Dcomplex catanh(_Dcomplex);
  extern "C" _Lcomplex  catanhl(_Lcomplex);
};
#endif

/**
  Complex number represented by real and imaginary parts.

  @short Complex number.
  @ingroup mathematics
  @version 1.0
*/

class Complex {
public:

  typedef double Type;

  /** The imaginary unit. */
  static const Complex II;
private:

  /** The real part of the complex number. */
  double real = 0;
  /** The imaginary part the complex number. */
  double imaginary = 0;

  inline _Dcomplex getNative() const throw() {
    return _DCOMPLEX_(real, imaginary);
    // return real + imaginary * 1i;
  }

  static inline Complex getComplex(const _Dcomplex value) throw() {
    return Complex(isoc::creal(value), isoc::cimag(value));
  }
public:

  /**
    Initializes complex number as (0, 0).
  */
  inline Complex() throw() {
  }

  /**
    Initializes complex number with the specified real part. The imaginary part
    is initialized to zero.

    @param real The real part.
  */
  inline Complex(double real) throw();

  /**
    Initializes complex number of the specified real and imaginary parts.

    @param real The desired real part.
    @param imaginary The desired imaginary part.
  */
  inline Complex(double real, double imaginary) throw();

  /**
    Initializes complex number of other complex number.
  */
  inline Complex(const Complex& copy) throw()
    : real(copy.real), imaginary(copy.imaginary) {
  }

  /**
    Assignment of complex number by complex number.
  */
  inline Complex& operator=(const Complex& eq) throw() {
    real = eq.real; // no need to protect against self-assignment
    imaginary = eq.imaginary;
    return *this;
  }

  /**
    Assignment of complex number by real number.
  */
  inline Complex& operator=(double real) throw() {
    this->real = real; // no need to protect against self-assignment
    imaginary = 0;
    return *this;
  }

  inline Complex sqrt() const throw() {
    return getComplex(isoc::csqrt(getNative()));
  }

  inline Complex exp() const throw() {
    return getComplex(isoc::cexp(getNative()));
  }

  inline Complex log() const throw() {
    return getComplex(isoc::clog(getNative()));
  }

  inline Complex pow(const double x, const Complex& y) const throw() {
    return getComplex(isoc::cpow(x, y.getNative()));
  }


  inline Complex sin() const throw() {
    return getComplex(isoc::csin(getNative()));
  }

  inline Complex asin() const throw() {
    return getComplex(isoc::casin(getNative()));
  }

  inline Complex cos() const throw() {
    return getComplex(isoc::ccos(getNative()));
  }

  inline Complex acos() const throw() {
    return getComplex(isoc::cacos(getNative()));
  }

  inline Complex tan() const throw() {
    return getComplex(isoc::ctan(getNative()));
  }

  inline Complex atan() const throw() {
    return getComplex(isoc::catan(getNative()));
  }


  inline Complex sinh() const throw() {
    return getComplex(isoc::csinh(getNative()));
  }

  inline Complex asinh() const throw() {
    return getComplex(isoc::casinh(getNative()));
  }

  inline Complex cosh() const throw() {
    return getComplex(isoc::ccosh(getNative()));
  }

  inline Complex acosh() const throw() {
    return getComplex(isoc::cacosh(getNative()));
  }

  inline Complex tanh() const throw() {
    return getComplex(isoc::ctanh(getNative()));
  }

  inline Complex atanh() const throw() {
    return getComplex(isoc::catanh(getNative()));
  }


  /**
    Returns the real part of the complex number.
  */
  inline double getReal() const throw() {
    return real;
  }

  /**
    Returns the imaginary part of the complex number.
  */
  inline double getImaginary() const throw() {
    return imaginary;
  }

  /**
    Sets the real part of the complex number.
  */
  inline void setReal(const double value) throw() {
    real = value;
  }

  /**
    Sets the imaginary part of the complex number.
  */
  inline void setImaginary(const double value) throw() {
    imaginary = value;
  }

  /**
    Returns the square of the modulus of the complex number.
  */
  inline double getSqrModulus() const throw() {
    return real * real + imaginary * imaginary;
  }

  /**
    Returns the modulus of the complex number.
  */
  inline double getModulus() const throw() {
    return Math::sqrt(getSqrModulus());
  }

  /**
    Returns the angle of the complex number.
  */
  inline double getAngle() const throw() {
    return Math::atan2(imaginary, real);
  }

  /**
    Unary plus.
  */
  inline Complex plus() const throw() {
    return Complex(*this);
  }

  /**
    Unary minus.
  */
  inline Complex minus() const throw() {
    return Complex(*this).negate();
  }

  /**
    Negates this number.
  */
  inline Complex& negate() throw() {
    real = -real;
    imaginary = -imaginary;
    return *this;
  }

  /**
    Conjugates this number.
  */
  inline Complex& conjugate() throw() {
    imaginary = -imaginary;
    return *this;
  }

  /**
    Inverts this number.
  */
  inline Complex& invert() throw() {
    double scale = getSqrModulus(); // TAG: possible division by zero
    //assert(scale != 0, DivisionByZero());
    scale = 1/scale;
    real *= scale;
    imaginary *= -scale;
    return *this;
  }

  /**
    Adds the specified number to this number.
  */
  inline Complex& add(const Complex& value) throw() {
    real += value.real;
    imaginary += value.imaginary;
    return *this;
  }

  /**
    Subtracts the specified number from this number.
  */
  inline Complex& subtract(const Complex& value) throw() {
    real -= value.real;
    imaginary -= value.imaginary;
    return *this;
  }

  /**
    Multiplies this number with the specified value.
  */
  inline Complex& multiply(double value) throw() {
    real *= value;
    imaginary *= value;
    return *this;
  }

  /**
    Multiplies this number with the specified value.
  */
  inline Complex& multiply(const Complex& value) throw() {
    double temp = real * value.real - imaginary * value.imaginary;
    imaginary = real * value.imaginary + imaginary * value.real;
    real = temp;
    return *this;
  }

  /**
    Divides this number with the specified value.
  */
  inline Complex& divide(double value) throw() {
    return multiply(1/value);
  }

  /**
    Divides this number with the specified value.
  */
  inline Complex& divide(const Complex& value) throw() {
    double scale = value.getSqrModulus(); // TAG: possible division by zero
    //assert(scale != 0, DivisionByZero(this));
    scale = 1/scale;
    double temp = (real * value.real - imaginary * -value.imaginary) * scale;
    imaginary = (real * -value.imaginary + imaginary * value.real) * scale;
    real = temp;
    // return multiply(Complex(value).invert());
    return *this;
  }



  /**
    Returns true if the numbers are equal.

    @param value Complex number to be compared.
  */
  inline bool operator==(const Complex& value) const throw() {
    return (real == value.real) && (imaginary == value.imaginary);
  }

  /**
    Adds the specified number to this number.

    @param value The value to be added.
  */
  inline Complex& operator+=(const Complex& value) throw() {
    return add(value);
  }

  /**
    Subtracts the specified complex from this number.

    @param value The value to be subtracted.
  */
  inline Complex& operator-=(const Complex& value) throw() {
    return subtract(value);
  }

  /**
    Multiplies this number with the specified value.

    @param value The multiplicator.
  */
  inline Complex& operator*=(const Complex& value) throw() {
    return multiply(value);
  }

  /**
    Multiplies this number with the specified value.

    @param value The multiplicator.
  */
  inline Complex& operator*=(double value) throw() {
    return multiply(value);
  }

  /**
    Divides this number with the specified value.

    @param value The divisor.
  */
  inline Complex& operator/=(double value) throw() {
    return divide(value);
  }

  /**
    Unary plus.
  */
  inline Complex operator+() const throw() {
    return plus();
  }

  /**
    Unary minus.
  */
  inline Complex operator-() const throw() {
    return minus();
  }



  /**
    Returns the sum of the complex numbers.
  */
  friend Complex operator+(const Complex& left, const Complex& right) throw();

  /**
    Returns the difference of the complex numbers.
  */
  friend Complex operator-(const Complex& left, const Complex& right) throw();

  /**
    Returns the product of the complex numbers.
  */
  friend Complex operator*(const Complex& left, const Complex& right) throw();

  /**
    Returns the product of the complex number and the real value.
  */
  friend Complex operator*(const Complex& left, double right) throw();

  /**
    Returns the product of the real value and the complex number.
  */
  friend Complex operator*(double left, const Complex& right) throw();

  /**
    Returns the result of the number divided by the value.
  */
  friend Complex operator/(const Complex& left, double right) throw();
};

inline Complex::Complex(double _real) throw() : real(_real) {
}

inline Complex::Complex(double _real, double _imaginary) throw()
  : real(_real), imaginary(_imaginary) {
}

inline Complex operator+(const Complex& left, const Complex& right) throw() {
  return Complex(left.real + right.real, left.imaginary + right.imaginary);
}

inline Complex operator-(const Complex& left, const Complex& right) throw() {
  return Complex(left.real - right.real, left.imaginary - right.imaginary);
}

inline Complex operator*(const Complex& left, const Complex& right) throw() {
  return Complex(
    left.real * right.real - left.imaginary * right.imaginary,
    left.real * right.imaginary + left.imaginary * right.real
  );
}

inline Complex operator*(const Complex& left, double right) throw() {
  return Complex(left.real * right, left.imaginary * right);
}

inline Complex operator*(double left, const Complex& right) throw() {
  return Complex(right.real * left, right.imaginary * left);
}

inline Complex operator/(const Complex& left, const Complex& right) throw() {
  return Complex(left).divide(right);
}

inline Complex operator/(const Complex& left, double right) throw() {
  double temp = 1/right;
  return Complex(left.real * temp, left.imaginary * temp);
}

/**
  Writes a string representation of the complex number to a format stream
  "(real; imaginary)".

  @relates Complex
*/
FormatOutputStream& operator<<(
  FormatOutputStream& stream, const Complex& value) throw(IOException);

template<>
class Uninitializeable<Complex> {
public:

  static const bool IS_UNINITIALIZEABLE = Uninitializeable<double>::IS_UNINITIALIZEABLE;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
