/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/DynamicObject.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

bool DynamicObject::isValidObject() const noexcept
{
  const void* _this = this;
#if defined(_COM_AZURE_DEV__BASE__DEBUG)
  return (_this != nullptr) && (valid == STATE_VALID);
#else
  return (_this != nullptr);
#endif
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(DynamicObject) : public UnitTest {
public:

  TEST_PRIORITY(1);
  TEST_PROJECT("base");

  void run() override
  {
    DynamicObject o1;
    TEST_ASSERT(o1.isValidObject());
#if defined(_COM_AZURE_DEV__BASE__DEBUG)
    uint8 buffer[sizeof(DynamicObject)];
    DynamicObject* o2 = new(buffer) DynamicObject();
    o2->~DynamicObject();
    TEST_ASSERT(!o2->isValidObject());
    // skip inplace delete - no heap is leaked
#endif
  }
};

TEST_REGISTER(DynamicObject);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
