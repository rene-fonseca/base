/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/math/Vector2D.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

template class _COM_AZURE_DEV__BASE__API Vector2D<float>;
template class _COM_AZURE_DEV__BASE__API Vector2D<double>;
template class _COM_AZURE_DEV__BASE__API Vector2D<long double>;

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Vector2D) : public UnitTest {
public:

  TEST_PRIORITY(100);
  TEST_PROJECT("base/math");
  TEST_TIMEOUT_MS(30 * 1000);

  void run() override
  {
    dvector2 a;
    a = dvector2(9,8);
    dvector2 b(1,2);
    auto c = 3.0 * a + b/9.0;
    auto d = b - a;
    d.getModulus();
    d.negate();
    auto _dot = dot(a, b);
    TEST_ASSERT(_dot != 0);
    TEST_ASSERT(!a.isZero());
    auto f = -a;
  }
};

TEST_REGISTER(Vector2D);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
