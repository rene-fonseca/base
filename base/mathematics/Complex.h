/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MATHEMATICS__COMPLEX_H
#define _DK_SDU_MIP__BASE_MATHEMATICS__COMPLEX_H

#include <base/string/FormatOutputStream.h>
#include <base/mathematics/Math.h>

#define _DK_SDU_MIP__BASE__COMPLEX_ISOC

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if defined(_DK_SDU_MIP__BASE__COMPLEX_ISOC)
namespace isoc {

  extern "C" float crealf(_Complex float);
  extern "C" double creal(_Complex double);
  extern "C" long double creall(_Complex long double);

  extern "C" float cimagf(_Complex float);
  extern "C" double cimag(_Complex double);
  extern "C" long double cimagl(_Complex long double);


  extern "C" _Complex float cabsf(_Complex float);
  extern "C" _Complex double cabs(_Complex double);
  extern "C" _Complex long double cabsl(_Complex long double);

  extern "C" _Complex float csqrtf(_Complex float);
  extern "C" _Complex double csqrt(_Complex double);
  extern "C" _Complex long double csqrtl(_Complex long double);

  extern "C" _Complex float cexpf(_Complex float);
  extern "C" _Complex double cexp(_Complex double);
  extern "C" _Complex long double cexpl(_Complex long double);

  extern "C" _Complex float clogf(_Complex float);
  extern "C" _Complex double clog(_Complex double);
  extern "C" _Complex long double clogl(_Complex long double);

  extern "C" _Complex float cpowf(_Complex float, _Complex float);
  extern "C" _Complex double cpow(_Complex double, _Complex double);
  extern "C" _Complex long double cpowl(_Complex long double, _Complex long double);


  extern "C" _Complex float csinf(_Complex float);
  extern "C" _Complex double csin(_Complex double);
  extern "C" _Complex long double csinl(_Complex long double);

  extern "C" _Complex float ccosf(_Complex float);
  extern "C" _Complex double ccos(_Complex double);
  extern "C" _Complex long double ccosl(_Complex long double);

  extern "C" _Complex float casinf(_Complex float);
  extern "C" _Complex double casin(_Complex double);
  extern "C" _Complex long double casinl(_Complex long double);

  extern "C" _Complex float cacosf(_Complex float);
  extern "C" _Complex double cacos(_Complex double);
  extern "C" _Complex long double cacosl(_Complex long double);

  extern "C" _Complex float ctanf(_Complex float);
  extern "C" _Complex double ctan(_Complex double);
  extern "C" _Complex long double ctanl(_Complex long double);

  extern "C" _Complex float catanf(_Complex float);
  extern "C" _Complex double catan(_Complex double);
  extern "C" _Complex long double catanl(_Complex long double);


  extern "C" _Complex float csinhf(_Complex float);
  extern "C" _Complex double csinh(_Complex double);
  extern "C" _Complex long double csinhl(_Complex long double);

  extern "C" _Complex float ccoshf(_Complex float);
  extern "C" _Complex double ccosh(_Complex double);
  extern "C" _Complex long double ccoshl(_Complex long double);

  extern "C" _Complex float casinhf(_Complex float);
  extern "C" _Complex double casinh(_Complex double);
  extern "C" _Complex long double casinhl(_Complex long double);

  extern "C" _Complex float cacoshf(_Complex float);
  extern "C" _Complex double cacosh(_Complex double);
  extern "C" _Complex long double cacoshl(_Complex long double);

  extern "C" _Complex float ctanhf(_Complex float);
  extern "C" _Complex double ctanh(_Complex double);
  extern "C" _Complex long double ctanhl(_Complex long double);

  extern "C" _Complex float catanhf(_Complex float);
  extern "C" _Complex double catanh(_Complex double);
  extern "C" _Complex long double catanhl(_Complex long double);
};
#endif

/**
  Complex number represented by real and imaginary parts.

  @short Complex number.
  @ingroup mathematics
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Complex {
public:

  typedef long double Type;

  /** The imaginary unit. */
  static const Complex I;
private:

  /** The real part of the complex number. */
  long double real;
  /** The imaginary part the complex number. */
  long double imaginary;

  inline _Complex long double getNative() const throw() {
    return real + imaginary * 1i;
  }

  static inline Complex getComplex(_Complex long double value) throw() {
    return Complex(isoc::creall(value), isoc::cimagl(value));
  }
public:

  /**
    Initializes complex number as (0, 0).
  */
  inline Complex() throw() : real(0), imaginary(0) {
  }

  /**
    Initializes complex number with the specified real part. The imaginary part
    is initialized to zero.

    @param real The real part.
  */
  inline Complex(long double real) throw();

  /**
    Initializes complex number of the specified real and imaginary parts.

    @param real The desired real part.
    @param imaginary The desired imaginary part.
  */
  inline Complex(long double real, long double imaginary) throw();

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
  inline Complex& operator=(long double real) throw() {
    this->real = real; // no need to protect against self-assignment
    imaginary = 0;
    return *this;
  }

  inline Complex sqrt() const throw() {
    return getComplex(isoc::csqrtl(getNative()));
  }

  inline Complex exp() const throw() {
    return getComplex(isoc::cexpl(getNative()));
  }

  inline Complex log() const throw() {
    return getComplex(isoc::clogl(getNative()));
  }

  inline Complex pow(const Complex& value) const throw() {
    return getComplex(isoc::cpowl(getNative(), value.getNative()));
  }


  inline Complex sin() const throw() {
    return getComplex(isoc::csinl(getNative()));
  }

  inline Complex asin() const throw() {
    return getComplex(isoc::casinl(getNative()));
  }

  inline Complex cos() const throw() {
    return getComplex(isoc::ccosl(getNative()));
  }

  inline Complex acos() const throw() {
    return getComplex(isoc::cacosl(getNative()));
  }

  inline Complex tan() const throw() {
    return getComplex(isoc::ctanl(getNative()));
  }

  inline Complex atan() const throw() {
    return getComplex(isoc::catanl(getNative()));
  }


  inline Complex sinh() const throw() {
    return getComplex(isoc::csinhl(getNative()));
  }

  inline Complex asinh() const throw() {
    return getComplex(isoc::casinhl(getNative()));
  }

  inline Complex cosh() const throw() {
    return getComplex(isoc::ccoshl(getNative()));
  }

  inline Complex acosh() const throw() {
    return getComplex(isoc::cacoshl(getNative()));
  }

  inline Complex tanh() const throw() {
    return getComplex(isoc::ctanhl(getNative()));
  }

  inline Complex atanh() const throw() {
    return getComplex(isoc::catanhl(getNative()));
  }


  /**
    Returns the real part of the complex number.
  */
  inline long double getReal() const throw() {
    return real;
  }

  /**
    Returns the imaginary part of the complex number.
  */
  inline long double getImaginary() const throw() {
    return imaginary;
  }

  /**
    Sets the real part of the complex number.
  */
  inline void setReal(const long double& value) throw() {
    real = value;
  }

  /**
    Sets the imaginary part of the complex number.
  */
  inline void setImaginary(const long double& value) throw() {
    imaginary = value;
  }

  /**
    Returns the square of the modulus of the complex number.
  */
  inline long double getSqrModulus() const throw() {
    return real * real + imaginary * imaginary;
  }

  /**
    Returns the modulus of the complex number.
  */
  inline long double getModulus() const throw() {
    return Math::sqrt(getSqrModulus());
  }

  /**
    Returns the angle of the complex number.
  */
  inline long double getAngle() const throw() {
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
    long double scale = getSqrModulus(); // TAG: possible division by zero
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
  inline Complex& multiply(long double value) throw() {
    real *= value;
    imaginary *= value;
    return *this;
  }

  /**
    Multiplies this number with the specified value.
  */
  inline Complex& multiply(const Complex& value) throw() {
    long double temp = real * value.real - imaginary * value.imaginary;
    imaginary = real * value.imaginary + imaginary * value.real;
    real = temp;
    return *this;
  }

  /**
    Divides this number with the specified value.
  */
  inline Complex& divide(long double value) throw() {
    return multiply(1/value);
  }

  /**
    Divides this number with the specified value.
  */
  inline Complex& divide(const Complex& value) throw() {
    long double scale = value.getSqrModulus(); // TAG: possible division by zero
    //assert(scale != 0, DivisionByZero(this));
    scale = 1/scale;
    long double temp = (real * value.real - imaginary * -value.imaginary) * scale;
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
  inline Complex& operator*=(long double value) throw() {
    return multiply(value);
  }

  /**
    Divides this number with the specified value.

    @param value The divisor.
  */
  inline Complex& operator/=(long double value) throw() {
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
  friend Complex operator*(const Complex& left, long double right) throw();

  /**
    Returns the product of the real value and the complex number.
  */
  friend Complex operator*(long double left, const Complex& right) throw();

  /**
    Returns the result of the number divided by the value.
  */
  friend Complex operator/(const Complex& left, long double right) throw();
};

inline Complex::Complex(long double _real) throw() : real(_real) {
}

inline Complex::Complex(long double _real, long double _imaginary) throw()
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

inline Complex operator*(const Complex& left, long double right) throw() {
  return Complex(left.real * right, left.imaginary * right);
}

inline Complex operator*(long double left, const Complex& right) throw() {
  return Complex(right.real * left, right.imaginary * left);
}

inline Complex operator/(const Complex& left, const Complex& right) throw() {
  return Complex(left).divide(right);
}

inline Complex operator/(const Complex& left, long double right) throw() {
  long double temp = 1/right;
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

  static const bool IS_UNINITIALIZEABLE = Uninitializeable<long double>::IS_UNINITIALIZEABLE;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
