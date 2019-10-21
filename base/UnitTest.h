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

#include <base/string/Format.h>
#include <base/collection/Array.h>
#include <base/collection/Map.h>
#include <base/Type.h>
#include <base/Timer.h>

#if defined(REGISTER_TEST)
#  error REGISTER_TEST already defined
#endif
#if defined(TEST_CLASS)
#  error TEST_CLASS already defined
#endif
#if defined(TEST_ASSERT)
#  error TEST_ASSERT already defined
#endif
#if defined(TEST_HERE)
#  error TEST_HERE already defined
#endif
#if defined(TEST_NOT_HERE)
#  error TEST_NOT_HERE already defined
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Unit test.
*/
class _COM_AZURE_DEV__BASE__API UnitTest : public ReferenceCountedObject {
public:

  class Marker {};
  class _Here : public Marker {};
  class _NotHere : public Marker {};

  typedef const _Here* Here;
  typedef const _NotHere* NotHere;

  enum {
    HERE_MARKER = 0x21564836,
    DEFAULT_PRIORITY = 0,
    DEFAULT_TIMEOUT = 15 * 60 * 1000
  };

  /** A single subtest result. */
  class TestResult {
  public:

    bool passed = false;
    String what;
  };

  /** Info on here point. */
  class HereMeta {
  public:

    const char* description; // description of point
    bool reach = true; // point must be reached
    unsigned int count = 0; // number of times point was reached
  };

  /* Recording of test run. */
  class Run : public ReferenceCountedObject {
  public:

    uint64 startTime = 0;
    uint64 endTime = 0;
    unsigned int passed = 0;
    unsigned int failed = 0;
    // unsigned int asserts = 0;
    Map<const void*, HereMeta> heres;
    Array<TestResult> results;

    /** Called on explicit print request. */
    void onPrint(const String& what, unsigned int line = 0);

    /** Called when subtest passed. */
    void onPassed(const String& what, unsigned int line = 0);

    /** Called when subtest failed. */
    void onFailed(const String& what, unsigned int line = 0);

    /** Called when here point is declared. */
    void registerHere(const Here* here, const char* description);

    /** Called when here point is declared. */
    void registerNotHere(const NotHere* here, const char* description);

    /** Called when here point is reached. */
    void onHere(const Here* here);

    /** Called when here point is reached but shouldn't be reached. */
    void onNotHere(const NotHere* here);

    /** Returns the JSON report. */
    String getReport() const;

    /** Compares 2 runs. */
    static bool compare(const Run& a, const Run& b);
  };
private:

  /** Name/id. */
  String name;
  /** Description. */
  String description;
  /** Source file. */
  String source;
  /** Class type. */
  Type type;

  // TAG: record timing and allow comparison of runs
  // TAG: add timing constraints
  // TAG: add IO constraints

  /** All recorded runs. */
  Array<Reference<Run> > runs;
  /** The current run. */
  Reference<Run> currentRun;
protected:

  /** Called on explicit print request. */
  inline void onPrint(const String& what, unsigned int line = 0)
  {
    currentRun->onPrint(what, line);
  }

  /** Called when subtest passed. */
  inline void onPassed(const String& what, unsigned int line = 0)
  {
    currentRun->onPassed(what, line);
  }

  /** Called when subtest failed. */
  inline void onFailed(const String& what, unsigned int line = 0)
  {
    currentRun->onFailed(what, line);
  }

  /** Called when here point is declared. */
  inline void registerHere(const Here* here, const char* description)
  {
    currentRun->registerHere(here, description);
  }

  /** Called when not-here point is declared. */
  inline void registerNotHere(const NotHere* here, const char* description)
  {
    currentRun->registerNotHere(here, description);
  }

  /** Called when here point is reached. */
  inline void onHere(const Here* here)
  {
    currentRun->onHere(here);
  }

  /** Called when nothere point is reached but shouldn't be reached. */
  inline void onNotHere(const NotHere* here)
  {
    currentRun->onNotHere(here);
  }
public:

  /** Initializes the test. */
  UnitTest();

  /** Sets the name/id of the test. */
  void setName(const String& name);
  
  /** Sets the description of the test. */
  void setDescription(const String& description);
  
  /** Sets the source file of the test. */
  void setSource(const String& source);

  /** Sets the type (from class) of the test. */
  void setType(const Type& type);

  /** Returns the name/id of the test. */
  inline const String& getName() const noexcept
  {
    return name;
  }

  /** Returns the description of the test. */
  inline const String& getDescription() const noexcept
  {
    return description;
  }

  /** Returns the source of the test. */
  inline const String& getSource() const noexcept
  {
    return source;
  }

  /** Returns the type (from class) of the test. */
  inline const Type& getType() const noexcept
  {
    return type;
  }

  /** Use TEST_ALLOW_CONCURRENT to allow concurrent test runs. */
  virtual bool getAllowConcurrentRun() const noexcept
  {
    return false;
  }
  
  /** Use TEST_PRIORITY to set priority for test. */
  virtual int getPriority() const noexcept
  {
    return DEFAULT_PRIORITY;
  }

  /** Use TEST_TIMEOUT_MS to set priority for test. */
  virtual unsigned int getTimeout() const noexcept
  {
    return DEFAULT_TIMEOUT;
  }

  /** Use TEST_REPEATS to set number of repeats. */
  virtual unsigned int getRepeats() const noexcept
  {
    return 1;
  }
  
  /** Runs the test. */
  virtual void run();

  /** Internal run. */
  Reference<Run> runImpl();

  // TAG: get result in test format XML/JSON
  
  ~UnitTest();
};

/** Unit test manager. */
class _COM_AZURE_DEV__BASE__API UnitTestManager {
  friend class UnitTest;
public:

  enum Verbosity {
    SILENT,
    COMPACT,
    NORMAL,
    VERBOSE
  };
private:
  
  Verbosity verbosity = VERBOSE;
  bool useANSIColors = false;
  unsigned int passed = 0;
  unsigned int failed = 0;
  Timer timer;

  Array<Reference<UnitTest> > tests;

  static UnitTestManager unitTestManager;
public:
  
  UnitTestManager();
  
  static UnitTestManager& getManager();

  /** Adds the given test. */
  void addTest(Reference<UnitTest> test);
  
  /** Create test instance and adds it. */
  template<class TYPE>
  inline void registerTest(const String& name, const String& source = String(), const String& description = String())
  {
    auto test = new TYPE();
    test->setName(name);
    test->setType(Type::getType<TYPE>()); // we cannot get type of original type since it may be a template
    test->setDescription(description);
    test->setSource(source);
    addTest(test);
  }

  inline Verbosity getVerbosity() const noexcept
  {
    return verbosity;
  }

  inline void setVerbosity(Verbosity _verbosity) noexcept
  {
    verbosity = _verbosity;
  }

  inline bool getUseANSIColors() const noexcept
  {
    return useANSIColors;
  }

  inline void setUseANSIColors(bool _useANSIColors) noexcept
  {
    useANSIColors = _useANSIColors;
  }

  /** Runs the test. A test must be able to run multiple times. */
  bool runTest(Reference<UnitTest> test);

  /** Loads the registered tests. */
  void loadTests();

  /** Runs the tests matching the given pattern. */
  bool runTests(const String& pattern = "*");

  /** Returns all the registered tests. */
  inline const Array<Reference<UnitTest> >& getTests() const
  {
    return tests;
  }

  /** Internal test registration helper. */
  class _COM_AZURE_DEV__BASE__API RegisterEntry {
  public:

    typedef void (*Entry)();

    struct EntryNode {
      const char* key; // used to make it easier to debug
      Entry entry; // entry function for test registration
      EntryNode* next; // next entry function node
      bool loaded; // indicates that test has been registered
    };

    /** The last test registered. This is a pointer so it is always ready for use during global initialization. */
    static EntryNode* nodes;

    /** Records test for delayed registration. */
    RegisterEntry(EntryNode* entry);
  };
};


#define REGISTER_TEST_IMPL(ID, TYPE) \
  namespace tests { \
    void _COM_AZURE_DEV__BASE__CONCATENATE(ID, _entry)() { base::UnitTestManager::getManager().registerTest<TEST_CLASS(TYPE)>(#TYPE, __FILE__, String()); } \
    base::UnitTestManager::RegisterEntry::EntryNode _COM_AZURE_DEV__BASE__CONCATENATE(ID, _storage) = {#TYPE, _COM_AZURE_DEV__BASE__CONCATENATE(ID, _entry), nullptr, false}; \
    base::UnitTestManager::RegisterEntry _COM_AZURE_DEV__BASE__CONCATENATE(ID, _register)(&_COM_AZURE_DEV__BASE__CONCATENATE(ID, _storage)); \
  }

/** Fast registration of test for later run. Test will be put in the "tests" subnamespace. */
#define REGISTER_TEST(TYPE) \
  REGISTER_TEST_IMPL(_COM_AZURE_DEV__BASE__MAKE_IDENTIFIER(test), TYPE)

/** Tag all references to the Test class using this define. */
#define TEST_CLASS(CLASS) _TEST_ ## CLASS

/** An assert/subtest within the test. */
#define TEST_ASSERT(EXPRESSION) if (EXPRESSION) { base::UnitTest::onPassed(#EXPRESSION, __LINE__); } else { base::UnitTest::onFailed(#EXPRESSION, __LINE__); }

/** Print info. */
#define TEST_PRINT(TEXT) base::UnitTest::onPrint(TEXT, __LINE__)

#define TEST_DECLARE_HERE_IMPL(IDENTIFIER, DESCRIPTION) \
  static const base::UnitTest::Here IDENTIFIER = nullptr; \
  base::UnitTest::registerHere(&IDENTIFIER, DESCRIPTION)

/** Declare here point that must be reached during execution. */
#define TEST_DECLARE_HERE(IDENTIFIER) \
  TEST_DECLARE_HERE_IMPL(_COM_AZURE_DEV__BASE__CONCATENATE(_TEST_HERE__, IDENTIFIER), #IDENTIFIER)

#define TEST_DECLARE_NOT_HERE_IMPL(IDENTIFIER, DESCRIPTION) \
  static const base::UnitTest::NotHere IDENTIFIER = nullptr; \
  base::UnitTest::registerNotHere(&IDENTIFIER, DESCRIPTION)

/** Declare here point that must NOT be reached during execution. */
#define TEST_DECLARE_NOT_HERE(IDENTIFIER) \
  TEST_DECLARE_NOT_HERE_IMPL(_COM_AZURE_DEV__BASE__CONCATENATE(_TEST_HERE__, IDENTIFIER), #IDENTIFIER)

/** Make a unique identifier. Must be on a separate source line. */
#define TEST_UNIQUE_ID(PREFIX) _COM_AZURE_DEV__BASE__MAKE_IDENTIFIER(PREFIX)

/** Here point. */
#define TEST_HERE(IDENTIFIER) base::UnitTest::onHere(&_COM_AZURE_DEV__BASE__CONCATENATE(_TEST_HERE__, IDENTIFIER));

/** Here point which must not be reached. */
#define TEST_NOT_HERE(IDENTIFIER) base::UnitTest::onNotHere(&_COM_AZURE_DEV__BASE__CONCATENATE(_TEST_HERE__, IDENTIFIER));

/** Tells system that test may be run concurrent with other tests. */
#define TEST_ALLOW_CONCURRENT() bool getAllowConcurrentRun() const noexcept override {return true;}

/** Sets the priority the test. Lower priorty gets run first. */
#define TEST_PRIORITY(priority) int getPriority() const noexcept override {return static_cast<int>(priority);}

/** Sets the timeout for the test in milliseconds. */
#define TEST_TIMEOUT_MS(timeout) unsigned int getTimeout() const noexcept override {return static_cast<unsigned int>(timeout);}

/** Sets the number of repeats for the test. */
#define TEST_REPEATS(count) unsigned int getRepeats() const noexcept override {return static_cast<unsigned int>(repeats);}

/**
  Declares a test for a given type.

  TEST_DECLARE(LinkerException);

  void TEST_CLASS(LinkerException)::run()
  {
    // my test
  }
*/
#define TEST_DECLARE(TYPE) \
  class TEST_CLASS(Reference) : public UnitTest { \
  public: \
    \
    void run() override; \
  }

/** Prototype for run method for test of type. */
#define TEST_RUN_IMPL(TYPE) TEST_CLASS(TYPE)::run

// TAG: include namespace in test id - so we can filter easily
// TAG: add TEST_INLINE_ASSERT() - good for TEST_PRINT
// TAG: add quick strigify of buffers and similar info
// TAG: should we indicate severity/security impact TEST_IMPACT(xxx) enum Impact {CRITICAL, SECURITY, PRIVACY, ..., NORMAL, IGNORE}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
