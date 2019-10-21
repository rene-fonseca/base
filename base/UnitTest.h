/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/string/String.h>
#include <base/collection/Array.h>
#include <base/collection/Map.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class TestResult {
public:

  // record passed / failed
  unsigned int passed = 0;
  unsigned int failed = 0;
};

/**
  Unit test.
*/

class _COM_AZURE_DEV__BASE__API UnitTest : public ReferenceCountedObject {
private:

  String name;
  String description;
  String source;

  unsigned int passed = 0;
  unsigned int failed = 0;
  unsigned int asserts = 0;
  Map<String, bool> heres;
  
  void onPass(const String& d)
  {
    ++passed;
  }

  void onFailure(const String& d)
  {
    ++failed;
  }
  
  void here(const String& id)
  {
    if (!heres.isKey(id)) {
      // here not mentioned
    }
    heres[id] = true;
  }
public:

  UnitTest();

  UnitTest(const String& name);

  void setName(const String& name);
  
  void setDescription(const String& description);
  
  void setSource(const String& source);
  
  inline const String& getName() const noexcept
  {
    return name;
  }
  
  inline const String& getDescription() const noexcept
  {
    return description;
  }
  
  virtual void run();

  // TAG: get result in test format XML/JSON
  
  ~UnitTest();
};

// class TestResult {};

class _COM_AZURE_DEV__BASE__API UnitTestManager {
private:
  
  unsigned int passed = 0;
  unsigned int failed = 0;
  
  Array<Reference<UnitTest> > tests;

  static UnitTestManager unitTestManager;
public:
  
  UnitTestManager();
  
  static UnitTestManager& getManager();
  
  void addTest(Reference<UnitTest> test);
  
  // TAG: use this and then call method to set test meta info like source - get data from entry struct
  template<class TYPE>
  void registerTest()
  {
    auto test = new TYPE();
    addTest(test);
    // test->setSource(entry->source);
  }
  
  template<class TYPE>
  void registerTest(const String& name)
  {
    auto test = new TYPE(name);
    addTest(test);
  }
  
  bool runTest(Reference<UnitTest> test);

  // void loadTests();
  
  bool runTests(const String& pattern = String());

  inline const Array<Reference<UnitTest> >& getTests() const
  {
    return tests;
  }
  
  // Array<String> getTestNames();
};

// TAG: make test class from test method also

class _COM_AZURE_DEV__BASE__API RegisterTestInit {
public:

  typedef void (*Entry)();

  struct EntryNode {
    const char* name;
    const char* source;
    Entry entry;
    EntryNode* next;
  };
  
  /** The last test registered. This is a pointer so it is always ready for use during global initialization. */
  static EntryNode* nodes;

  RegisterTestInit(EntryNode* entry);
};

// TAG: record source file also __FILE__

#define REGISTER_TEST_IMPL(ID, TYPE) \
  namespace { \
    void _COM_AZURE_DEV__BASE__CONCATENATE(ID, _entry)() { UnitTestManager::getManager().registerTest<TYPE ## Test>(#TYPE); } \
    RegisterTestInit::EntryNode _COM_AZURE_DEV__BASE__CONCATENATE(ID, _storage) = {#TYPE, __FILE__, _COM_AZURE_DEV__BASE__CONCATENATE(ID, _entry), nullptr}; \
    RegisterTestInit _COM_AZURE_DEV__BASE__CONCATENATE(ID, _register)(&_COM_AZURE_DEV__BASE__CONCATENATE(ID, _storage)); \
  }

// TAG: add support for custom name of test

#define REGISTER_TEST(TYPE) \
  REGISTER_TEST_IMPL(_COM_AZURE_DEV__BASE__MAKE_IDENTIFIER(test), TYPE)

#define TEST_CLASS(CLASS) CLASS ## Test

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
