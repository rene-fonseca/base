/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/math/Vector3D.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

template class _COM_AZURE_DEV__BASE__API Vector3D<float>;
template class _COM_AZURE_DEV__BASE__API Vector3D<double>;
template class _COM_AZURE_DEV__BASE__API Vector3D<long double>;

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Vector3D) : public UnitTest {
public:

  TEST_PRIORITY(100);
  TEST_PROJECT("base/math");
  TEST_TIMEOUT_MS(30 * 1000);

  void run() override
  {
    dvector3 a;
    a = dvector3(9,8,6);
    dvector3 b(0,1,2);
    auto c = 3.0 * a + b/9.0;
    auto d = b - a;
    d.getModulus();
    d.getZAngle();
    d.getXYAngle();
    d.negate();
    auto _dot = dot(a, b);
    TEST_ASSERT(_dot != 0);
    auto e = cross(a, b);
    TEST_ASSERT(!a.isZero());
    auto f = -a;
  }
};

TEST_REGISTER(Vector3D);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
