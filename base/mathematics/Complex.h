/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MATH__COMPLEX_H
#define _DK_SDU_MIP__BASE_MATH__COMPLEX_H

#include <base/Object.h>
#include <base/string/FormatOutputStream.h>
#include <math.h> // not nice; but what can I do

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Complex number represented by real and imaginary parts.

  @short Complex number.
  @author René Møller Fonseca
*/

class Complex {
private:

  /** The real part of the complex number. */
  double real;
  /** The imaginary part the complex number. */
  double imaginary;
public:

  /**
    Initializes complex number as (0, 0).
  */
  inline Complex() throw() : real(0), imaginary(0) {}

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
  inline Complex(const Complex& copy) throw() : real(copy.real), imaginary(copy.imaginary) {}

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

  /**
    Returns the real part of the complex number.
  */
  inline double getReal() const throw() {return real;}

  /**
    Returns the imaginary part of the complex number.
  */
  inline double getImaginary() const throw() {return imaginary;}

  /**
    Sets the real part of the complex number.
  */
  inline void setReal(const double& value) throw() {real = value;}

  /**
    Sets the imaginary part of the complex number.
  */
  inline void setImaginary(const double& value) throw() {imaginary = value;}

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
    return sqrt(getSqrModulus());
  }

  /**
    Returns the angle of the complex number.
  */
  inline double getAngle() const throw() {
    return atan2(imaginary, real);
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
    Inverts this number.
  */
  inline Complex& invert() throw() {
    double modulus = 1/getModulus(); // possible division by zero
    double angle = -getAngle();
    real = modulus * cos(angle);
    imaginary = modulus * sin(angle);
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
    return multiply(Complex(value).invert());
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
  inline Complex& operator+=(const Complex& value) throw() {return add(value);}

  /**
    Subtracts the specified complex from this number.

    @param value The value to be subtracted.
  */
  inline Complex& operator-=(const Complex& value) throw() {return subtract(value);}

  /**
    Multiplies this number with the specified value.

    @param value The multiplicator.
  */
  inline Complex& operator*=(const Complex& value) throw() {return multiply(value);}

  /**
    Multiplies this number with the specified value.

    @param value The multiplicator.
  */
  inline Complex& operator*=(double value) throw() {return multiply(value);}

  /**
    Divides this number with the specified value.

    @param value The divisor.
  */
  inline Complex& operator/=(double value) throw() {return divide(value);}

  /**
    Unary plus.
  */
  inline Complex operator+() const throw() {return plus();}

  /**
    Unary minus.
  */
  inline Complex operator-() const throw() {return minus();}



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

inline Complex::Complex(double r) throw() : real(r) {}

inline Complex::Complex(double r, double i) throw() : real(r), imaginary(i) {}

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
  return left * Complex(right).invert();
}

inline Complex operator/(const Complex& left, double right) throw() {
  double temp = 1/right;
  return Complex(left.real * temp, left.imaginary * temp);
}

/**
  Writes a string representation of the complex number to a format stream "(real; imaginary)".
*/
FormatOutputStream& operator<<(FormatOutputStream& stream, const Complex& value);

template<>
inline bool isRelocateable<Complex>() throw() {return isRelocateable<Object>();}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
