/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/math/Complex.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

template class _COM_AZURE_DEV__BASE__API Complex<float>;
template class _COM_AZURE_DEV__BASE__API Complex<double>;
template class _COM_AZURE_DEV__BASE__API Complex<long double>;
#if 0 && (_COM_AZURE_DEV__BASE__HAS_FLOAT128) // requires Math float128 support
template class _COM_AZURE_DEV__BASE__API Complex<float128>;
#endif

template<> const Complex<float>::Imaginary Complex<float>::I;
template<> const Complex<float> Complex<float>::ZERO(0, 0);
template<> const Complex<float> Complex<float>::ONE(1, 0);
template<> const Complex<float> Complex<float>::MINUS_ONE(-1, 0);
template<> const Complex<float> Complex<float>::II(0, 1);

template<> const Complex<double>::Imaginary Complex<double>::I;
template<> const Complex<double> Complex<double>::ZERO(0, 0);
template<> const Complex<double> Complex<double>::ONE(1, 0);
template<> const Complex<double> Complex<double>::MINUS_ONE(-1, 0);
template<> const Complex<double> Complex<double>::II(0, 1);

template<> const Complex<long double>::Imaginary Complex<long double>::I;
template<> const Complex<long double> Complex<long double>::ZERO(0, 0);
template<> const Complex<long double> Complex<long double>::ONE(1, 0);
template<> const Complex<long double> Complex<long double>::MINUS_ONE(-1, 0);
template<> const Complex<long double> Complex<long double>::II(0, 1);

#if (_COM_AZURE_DEV__BASE__HAS_FLOAT128)
template<> const Complex<float128>::Imaginary Complex<float128>::I;
template<> const Complex<float128> Complex<float128>::ZERO(0, 0);
template<> const Complex<float128> Complex<float128>::ONE(1, 0);
template<> const Complex<float128> Complex<float128>::MINUS_ONE(-1, 0);
template<> const Complex<float128> Complex<float128>::II(0, 1);
#endif

FormatOutputStream& operator<<(FormatOutputStream& stream, const Complex<float>& value)
{
  FormatOutputStream::PushContext push(stream);
  return stream << '(' << value.getReal() << FPLUS << value.getImaginary() << "i)";
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Complex<double>& value)
{
  FormatOutputStream::PushContext push(stream);
  return stream << '(' << value.getReal() << FPLUS << value.getImaginary() << "i)";
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Complex<long double>& value)
{
  FormatOutputStream::PushContext push(stream);
  return stream << '(' << value.getReal() << FPLUS << value.getImaginary() << "i)";
}

#if (_COM_AZURE_DEV__BASE__HAS_FLOAT128)
FormatOutputStream& operator<<(FormatOutputStream& stream, const Complex<float128>& value)
{
  FormatOutputStream::PushContext push(stream);
  return stream << '(' << value.getReal() << FPLUS << value.getImaginary() << "i)";
}
#endif

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Complex) : public UnitTest {
public:

  TEST_PRIORITY(200);
  TEST_PROJECT("base/math");
  TEST_IMPACT(NORMAL)
  TEST_TIMEOUT_MS(30 * 1000);

  void run() override
  {
    Complex<double> a(0, 1);
    Complex<double> b(1, 1);
    auto c = a + b;
    c = a - b;
    c = a * b;
    c = a / b;
    c = a * 10;
    c = 10 * a;
    c = a/10;
    c = -a;
    c = a.conjugate();
    double modulus = a.getModulus();
    TEST_ASSERT(modulus >= 0);
    double angle = a.getAngle();
    TEST_ASSERT(angle < (3 * constant::PI));
    auto p = a.getPolar();
    TEST_ASSERT(!(a == b));
    TEST_ASSERT(a != b);
  }
};

TEST_REGISTER(Complex);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
