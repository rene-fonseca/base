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
#include <base/UnitTest.h>

_COM_AZURE_DEV__BASE__DUMMY_SYMBOL

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(ReferenceCounter) : public UnitTest {
public:

  TEST_PRIORITY(20);
  TEST_PROJECT("base/mem");
  
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

  void run() override
  {
    ReferenceCounter<MyOtherObject> myOtherObject = new MyOtherObject();
    TEST_ASSERT(myOtherObject.getNumberOfReferences() == 1);
    ReferenceCounter<MyObject> myObject = new MyObject();
    TEST_ASSERT(myObject.isValid());
    TEST_ASSERT(myObject.getNumberOfReferences() == 1);
    auto myObject2 = myObject;
    TEST_ASSERT(myObject.isMultiReferenced());
    myObject->doit();
    {
      auto& _COM_AZURE_DEV__BASE__MAKE_IDENTIFIER(id) = *myObject;
    }
    ReferenceCounter<MyOtherObject> myOtherObject2 = myObject.cast<MyOtherObject>();
    TEST_ASSERT(!myOtherObject2);
    TEST_ASSERT(myOtherObject2 == nullptr);
    ReferenceCounter<MyObject> myObject3 = myOtherObject;
    TEST_ASSERT(myObject3);

    ReferenceCounter<MyOtherObject> myOtherObject3 = myObject3.cast<MyOtherObject>();
    TEST_ASSERT(myOtherObject3.cast<MyObject>());
    myOtherObject3.copyOnWrite();
    myOtherObject3.invalidate();
    TEST_ASSERT(!myOtherObject3.cast<MyObject>());
    // myObject.invalidate();
    if (myOtherObject3 != nullptr) {
      auto& _COM_AZURE_DEV__BASE__MAKE_IDENTIFIER(id) = *myOtherObject3;
    }
    myObject = nullptr;
  }
};

TEST_REGISTER(ReferenceCounter);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
