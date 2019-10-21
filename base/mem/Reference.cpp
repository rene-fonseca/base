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

#if defined(_COM_AZURE_DEV__BASE__TESTS)

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

class TEST_CLASS(Reference) : public UnitTest {
public:

  TEST_PRIORITY(0);
  TEST_TIMEOUT_MS(30 * 1000);
  // TAG: TEST_LIMIT_PROCESSING_TIME(1000);
  // TAG: TEST_LIMIT_MEMORY(1024);
  // TAG: TEST_LIMIT_LEAK_MEMORY(1024);

  void run()
  {
    Reference<MyOtherObject> myOtherObject = new MyOtherObject();
    myOtherObject = myOtherObject; // self assignment
    TEST_ASSERT(myOtherObject.getNumberOfReferences() == 1);
    Reference<MyObject> myObject = myOtherObject;
    TEST_ASSERT(myOtherObject.getNumberOfReferences() == 2);

    myObject = new MyObject();
    TEST_ASSERT(myOtherObject.getNumberOfReferences() == 1);
    TEST_ASSERT(myObject.isValid());
    TEST_ASSERT(myObject.getNumberOfReferences() == 1);
    TEST_ASSERT(!myObject.isMultiReferenced());
    myObject->doit();
    {
      auto& TEST_UNIQUE_ID(id) = *myObject;
    }

    Reference<MyOtherObject> myOtherObject2 = myObject.cast<MyOtherObject>();
    TEST_ASSERT(myOtherObject2 == nullptr);
    
    Reference<MyObject> myObject3 = myOtherObject;
    TEST_ASSERT(myObject3 != nullptr);
    myObject3 = nullptr;

    Reference<MyOtherObject> myOtherObject3 = myObject3.cast<MyOtherObject>();
    Reference<MyOtherObject> myOtherObject4 = myOtherObject3;
    TEST_ASSERT(myOtherObject3.getNumberOfReferences() > 1);
    myOtherObject3.copyOnWrite();
    TEST_ASSERT(myOtherObject3.getNumberOfReferences() == 1);
    myOtherObject3.invalidate();
    TEST_ASSERT(myOtherObject3 == nullptr);

    {
      auto& TEST_UNIQUE_ID(id) = *myOtherObject3;
    }
    myObject = nullptr;
  }
};

REGISTER_TEST(Reference);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
