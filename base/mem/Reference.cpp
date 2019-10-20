/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/mem/Reference.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

template class Reference<ReferenceCountedObject>;

// TAG: need a general way to add test and run them
// TAG: use namespace for tests
// TAG: register tests to run at any time

#if defined(TESTS)

namespace tests {

class MyObject;

void doitImpl(Reference<MyObject> myObject)
{
  if (myObject) {
  }
}

class MyObject : public ReferenceCountedObject {
public:
  
  MyObject() {
  }
  
  void doit() {
    Reference<MyObject> myRef = this; // safe
    doitImpl(myRef);
  }
};

class MyOtherObject : public MyObject {
public:
};

// TAG: add new for ref class? Reference<MyObject>::make();
void test()
{
  Reference<MyOtherObject> myOtherObject = new MyOtherObject();
  // myOtherObject2 refs == 1;
  Reference<MyObject> myObject = new MyObject();
  myObject.isValid();
  myObject.getNumberOfReferences();
  myObject.isMultiReferenced();
  myObject->doit();
  {
    auto& _COM_AZURE_DEV__BASE__MAKE_IDENTIFIER(id) = *myObject;
  }
  Reference<MyOtherObject> myOtherObject2 = myObject.cast<MyOtherObject>();
  if (myOtherObject2 == nullptr) {};
  Reference<MyObject> myObject3 = myOtherObject;
  myObject3 = nullptr;

  Reference<MyOtherObject> myOtherObject3 = myObject3.cast<MyOtherObject>();
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
