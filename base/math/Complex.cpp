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

const Complex::Imaginary Complex::I;
const Complex Complex::ZERO(0, 0);
const Complex Complex::ONE(1, 0);
const Complex Complex::MINUS_ONE(-1, 0);
const Complex Complex::II(0, 1);

FormatOutputStream& operator<<(FormatOutputStream& stream, const Complex& value)
{
  FormatOutputStream::PushContext push(stream);
  return stream << '(' << value.getReal() << FPLUS << value.getImaginary() << "i)";
}


#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Complex) : public UnitTest {
public:

  TEST_PRIORITY(200);
  TEST_PROJECT("base/math");
  TEST_IMPACT(NORMAL)
  TEST_TIMEOUT_MS(30 * 1000);

  void run() override
  {
    Complex a(0, 1);
    Complex b(1, 1);
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
    double angle = a.getAngle();
    auto p = a.getPolar();
    TEST_ASSERT(!(a == b));
    TEST_ASSERT(a != b);
  }
};

TEST_REGISTER(Complex);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
