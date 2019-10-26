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

#if 0 // for testing natvis
#include <base/collection/Pair.h>
#include <base/collection/Array.h>
#include <base/collection/List.h>
#include <base/AnyValue.h>
#endif

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
  TEST_IMPACT(CRITICAL);
  TEST_TIMEOUT_MS(30 * 1000);
  // TAG: TEST_LIMIT_PROCESSING_TIME(1000);
  // TAG: TEST_LIMIT_MEMORY(1024);
  // TAG: TEST_LIMIT_LEAK_MEMORY(1024);

  void run() override
  {
    TEST_DECLARE_HERE(A);

#if 0 // for testing natvis
    String s("Hi there!");
    MemorySpan span(s.native());

    Pair<String, const char*> pair("First", "Last");

    Array<String> values;
    values.append("hello");
    values.append("world");
    values.append("from");
    values.append("us");

    Array<AnyValue> values2;
    values2.append('C');
    values2.append(L"Hello");
    values2.append(54ULL);
    values2.append(true);
    values2.append(-9);
    // values2.append(Reference<MyObject>());
    values2.append(String("My item"));
    values2.append((void*)nullptr);
    values2.append(-5.9);

    List<String> list;
    list.append("Hello");
    list.append("World!");

    Map<int, String> map;
    map[4] = "Testing";
    map[77] = "More testing";

    Reference<MyOtherObject> nr;
#endif

    Reference<MyOtherObject> myOtherObject = new MyOtherObject();
    // myOtherObject = myOtherObject; // self assignment
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

    Reference<MyOtherObject> myOtherObject3 = new MyOtherObject();
    Reference<MyOtherObject> myOtherObject4 = myOtherObject3;
    if (!TEST_INLINE_ASSERT(myOtherObject3.getNumberOfReferences() > 1)) {
      TEST_PRINT(Format::subst("REFERENCES=%1", myOtherObject3.getNumberOfReferences()));
    }
    // TEST_PRINT_FORMAT("REFERENCES=%1", myOtherObject3.getNumberOfReferences());
    myOtherObject3.copyOnWrite();
    TEST_ASSERT(myOtherObject3.getNumberOfReferences() == 1);
    myOtherObject3.invalidate();
    TEST_ASSERT(myOtherObject3 == nullptr);

    try {
      auto& TEST_UNIQUE_ID(id) = *myOtherObject3;
    } catch (NullPointer) {
      TEST_HERE(A);
    }

    myObject = nullptr;
  }
};

TEST_REGISTER(Reference);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
