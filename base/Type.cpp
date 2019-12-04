/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Type.h>
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace {

  class _COM_AZURE_DEV__BASE__API Uninitialized {
  };
}

const std::type_info* Type::getUninitialized() noexcept
{
  return &typeid(Uninitialized);
}

const char* Type::getLocalName() const noexcept
{
  return type->name();
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Type) : public UnitTest {
public:

  TEST_PRIORITY(10);
  TEST_PROJECT("base");

  class MyClass {
  };

  class MyOtherClass {
  };

  void run() override
  {
    Type type = Type::getType<MyClass>();
    Type otherType = Type::getType<MyOtherClass>();
    TEST_ASSERT(type != otherType);

    MyClass myObject;
    auto type2 = Type::getType(myObject);
    TEST_ASSERT(type == type2);

    const char* localName = type.getLocalName();
    TEST_ASSERT(localName);
    
    auto type3 = Type::makeType(type2);
    TEST_ASSERT(type == type3);
  }
};

TEST_REGISTER(Type);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
