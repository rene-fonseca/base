/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/mem/ReferenceCounter.h>
#include <string>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

template class ReferenceCounter<std::string>;
template class ReferenceCounter<std::wstring>;

#if defined(TESTS)

namespace tests {

class MyObject {
public:
  
  MyObject()
  {
  }
  
  void doit()
  {
  }

  virtual ~MyObject()
  {
  }
};

class MyOtherObject : public MyObject {
public:
};

// TAG: add new for ref class? Reference<MyObject>::make();
void test()
{
  ReferenceCounter<MyOtherObject> myOtherObject = new MyOtherObject();
  // myOtherObject2 refs == 1;
  ReferenceCounter<MyObject> myObject = new MyObject();
  myObject.isValid();
  myObject.getNumberOfReferences();
  myObject.isMultiReferenced();
  myObject->doit();
  {
    auto& _COM_AZURE_DEV__BASE__MAKE_IDENTIFIER(id) = *myObject;
  }
  ReferenceCounter<MyOtherObject> myOtherObject2 = myObject.cast<MyOtherObject>();
  if (myOtherObject2 == nullptr) {};
  ReferenceCounter<MyObject> myObject3 = myOtherObject;
  myObject3 = nullptr;

  ReferenceCounter<MyOtherObject> myOtherObject3 = myObject3.cast<MyOtherObject>();
  myOtherObject3.copyOnWrite();
  myOtherObject3.invalidate();
  // myOtherObject3.cast<>()
  // myObject.invalidate();
  if (myOtherObject3 != nullptr) {};
  {
    auto& _COM_AZURE_DEV__BASE__MAKE_IDENTIFIER(id) = *myOtherObject3;
  }
  myObject = nullptr;
}

// REGISTER_TEST(test);

} // namespace tests

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
