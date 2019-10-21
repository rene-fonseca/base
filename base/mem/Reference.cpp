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
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

template class Reference<ReferenceCountedObject>;

// TAG: add define for getting tests in release also

#if defined(_COM_AZURE_DEV__BASE__ANY_DEBUG)

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



// TAG: record source file also
class TEST_CLASS(Reference) : public UnitTest {
public:
  
  ReferenceTest(const String& name) : UnitTest(name) {
    // setSource(__FILE__);
  }
  
  void run()
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
};

REGISTER_TEST(Reference);

#endif

// TAG: add new for ref class? Reference<MyObject>::make();

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
