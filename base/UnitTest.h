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
#include <base/concurrency/MutualExclusion.h>
#include <base/objectmodel/ObjectModel.h>

#if defined(TEST_REGISTER)
#  error TEST_REGISTER already defined
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
    DEFAULT_PRIORITY = 0, // key feature priority
    DEFAULT_TIMEOUT = 15 * 60 * 1000
  };

  enum Impact {
    PRIVACY, // loss of privacy
    SECURITY, // loss of trust
    CRITICAL, // corruption
    IMPORTANT, // common issue
    NORMAL,
    LOW, // rare less important problem
    IGNORE // nothing major - not recommended to be used
  };
  
  enum ResultEvent {
    FAILED,
    PASSED,
    PRINT
  };
  
  /** A single subtest result. */
  class TestResult {
  public:

    ResultEvent event = FAILED;
    String what;
  };

  /** Info on here point. */
  class HereMeta {
  public:

    const char* description = nullptr; // description of point
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

    /** Returns the report. */
    Reference<ObjectModel::Object> getReport() const;

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
  
  /** Called when subtest failed. */
  inline void onAssert(bool passed, const String& what, unsigned int line = 0)
  {
    if (passed) {
      currentRun->onPassed(what, line);
    } else {
      currentRun->onFailed(what, line);
    }
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

  /** Use TEST_IMPACT to set priority for test. */
  virtual Impact getImpact() const noexcept
  {
    return NORMAL;
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
  
  /** Use TEST_LIMIT_IO to set IO limit. */
  virtual uint64 getLimitIO() const noexcept
  {
    return 0;
  }

  /** Use TEST_LIMIT_TIME_MS to set processing time limit. */
  virtual uint64 getLimitProcessingTime() const noexcept
  {
    return 0;
  }

  /** Use TEST_LIMIT_MEMORY to set processing time limit. */
  virtual uint64 getLimitMemory() const noexcept
  {
    return 0;
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

  class SortTests {
  public:

    bool operator()(const Reference<UnitTest>& a, const Reference<UnitTest>& b)
    {
      if (a->getPriority() < b->getPriority()) {
        return true;
      }
      if (a->getPriority() == b->getPriority()) {
        if (a->getImpact() < b->getImpact()) {
          return true;
        }
        if (a->getImpact() <= b->getImpact()) {
          if (a->getName() < b->getName()) {
            return true;
          }
        }
      }
      return false;
    }
  };
private:
  
  Verbosity verbosity = VERBOSE;
  bool useJSON = false;
  bool useANSIColors = false;
  bool randomize = false;
  bool stopOnFailure = false;
  bool progressMode = false;
  unsigned int passed = 0;
  unsigned int failed = 0;
  Timer timer;
  MutualExclusion lock;

  Array<Reference<UnitTest> > tests;

  static UnitTestManager unitTestManager;
public:
  
  UnitTestManager();
  
  static UnitTestManager& getManager();

  /** Adds the given test. */
  void addTest(Reference<UnitTest> test);
  
  /** Only returns the filename after the last / or \. */
  static const char* trimPath(const char*) noexcept;

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

  inline decltype(lock)& getLock() noexcept
  {
    return lock;
  }
  
  inline Verbosity getVerbosity() const noexcept
  {
    return verbosity;
  }

  inline void setVerbosity(Verbosity _verbosity) noexcept
  {
    verbosity = _verbosity;
  }

  inline void setUseJSON(bool _useJSON) noexcept
  {
    useJSON = _useJSON;
  }

  inline bool getUseANSIColors() const noexcept
  {
    return useANSIColors;
  }

  inline void setUseANSIColors(bool _useANSIColors) noexcept
  {
    useANSIColors = _useANSIColors;
  }

  inline void setRandomize(bool _randomize) noexcept
  {
    randomize = _randomize;
  }

  inline void setStopOnFailure(bool _stopOnFailure) noexcept
  {
    stopOnFailure = _stopOnFailure;
  }

  inline bool getProgressMode() const noexcept
  {
    return progressMode;
  }
  
  inline void setProgressMode(bool _progressMode) noexcept
  {
    progressMode = _progressMode;
  }

  /** Runs the test. A test must be able to run multiple times. */
  bool runTest(Reference<UnitTest> test);

  /** Loads the registered tests. */
  void loadTests();

  /** Runs the tests matching the given pattern. */
  bool runTests(const String& pattern = "*");

  /** Returns all the registered tests. */
  inline const Array<Reference<UnitTest> >& getTests() const noexcept
  {
    return tests;
  }

  /** Internal test registration helper. */
  class _COM_AZURE_DEV__BASE__API RegisterEntry {
    friend class UnitTestManager;
  public:

    typedef void (*Entry)();

    struct EntryNode {
      const char* key; // used to make it easier to debug
      Entry entry; // entry function for test registration
      EntryNode* next; // next entry function node
      bool loaded; // indicates that test has been registered
    };

    EntryNode* node = nullptr;
  private:

    /** The last test registered. This is a pointer so it is always ready for use during global initialization. */
    static EntryNode* nodes;
  public:

    /** Records test for delayed registration. */
    RegisterEntry(EntryNode* entry);

    ~RegisterEntry();
  };
};



#define TEST_REGISTER_IMPL(ID, TYPE) \
  namespace { namespace ID { \
    void _entry() { base::UnitTestManager::getManager().registerTest<TEST_CLASS(TYPE)>(#TYPE, UnitTestManager::trimPath(_COM_AZURE_DEV__BASE__SOURCE_FILE), String()); } \
    base::UnitTestManager::RegisterEntry::EntryNode _storage = {#TYPE, _entry, nullptr, false}; \
    base::UnitTestManager::RegisterEntry _register(&_storage); \
  } }

/** Fast registration of test for later run. Test will be put into unique subnamespace. */
#define TEST_REGISTER(TYPE) \
  TEST_REGISTER_IMPL(_COM_AZURE_DEV__BASE__MAKE_IDENTIFIER(tests), TYPE)

/** Sets the priority the test. Lower priorty gets run first. */
#define TEST_DEPENDENCY(CLASS, DEPENDENCY) // TAG: how can we handle this - register combo of names

/** Tag all references to the Test class using this define. */
#define TEST_CLASS(CLASS) _TEST_ ## CLASS

/** An assert/subtest within the test. */
#define TEST_ASSERT(EXPRESSION) \
  base::UnitTest::onAssert(EXPRESSION, #EXPRESSION, __LINE__)

/** Assert within an expression. */
#define TEST_INLINE_ASSERT(EXPRESSION) \
  (base::UnitTest::onAssert(EXPRESSION, #EXPRESSION, __LINE__), (EXPRESSION))

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

/** Sets the priority the test. Lower priorty is "higher" and gets run first. */
#define TEST_PRIORITY(priority) int getPriority() const noexcept override {return static_cast<int>(priority);}

/** Sets the priority the test. Lower impact is "higher" and gets run first for the same priority group. */
#define TEST_IMPACT(impact) Impact getImpact() const noexcept override {return static_cast<Impact>(impact);}

/** Sets the timeout for the test in milliseconds. */
#define TEST_TIMEOUT_MS(timeout) unsigned int getTimeout() const noexcept override {return static_cast<unsigned int>(timeout);}

/** Sets the number of repeats for the test. */
#define TEST_REPEATS(count) unsigned int getRepeats() const noexcept override {return static_cast<unsigned int>(repeats);}

/** Sets the timeout for the test in milliseconds. */
#define TEST_LIMIT_IO(kbytes) uint64 getLimitIO() const noexcept override {return static_cast<uint64>(kbytes);}

/** Sets the timeout for the test in milliseconds. */
#define TEST_LIMIT_TIME_MS(time) uint64 getLimitProcessingTime() const noexcept override {return static_cast<uint64>(time);}

/** Sets the timeout for the test in milliseconds. */
#define TEST_LIMIT_MEMORY(kbytes) uint64 getLimitMemory() const noexcept override {return static_cast<uint64>(kbytes);}

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

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
