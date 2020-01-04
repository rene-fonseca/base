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

  /** Potential impact of test. Lower is worse. */
  enum Impact {
    PRIVACY, // loss of privacy
    SECURITY, // loss of trust
    CRITICAL, // corruption
    IMPORTANT, // common issue
    NORMAL,
    LOW, // rare less important problem
    IGNORE // nothing major - not recommended to be used
  };
  
  /** A single result event. */
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
    unsigned int line = 0;
  };

  /** Info on here point. */
  class HereMeta {
  public:

    const char* description = nullptr; // description of point
    bool reach = true; // point must be reached
    unsigned int count = 0; // number of times point was reached
  };

  /** Adds given dependency to test. */
  class AddDependency {
  public:

    AddDependency(UnitTest* test, const char* id);
  };

  /* Recording of test run. */
  class Run : public ReferenceCountedObject {
  public:

    uint64 startTime = 0;
    uint64 endTime = 0;
    unsigned int passed = 0;
    unsigned int failed = 0;
    unsigned int exceptions = 0;
    // unsigned int asserts = 0;
    Map<const void*, HereMeta> heres;
    Array<TestResult> results; // all results for a run
    String exceptionFailure;
    String exceptionType;

    /** Called on exception. */
    void onException(const Exception* exception);

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
  /** Source file line. */
  unsigned int line = 0;
  /** Class type. */
  Type type;

  /** All recorded runs. */
  Array<Reference<Run> > runs;
  /** The current run. */
  Reference<Run> currentRun;
  /** Dependencies. */
  Array<String> dependencies;
public:

  /** Called on exception. */
  void onException(const Exception* exception);
protected:

  /** Called on explicit print request. */
  void onPrint(const String& what, unsigned int line = 0);
  
  /** Called when subtest passed. */
  void onPassed(const String& what, unsigned int line = 0);
  
  /** Called when subtest failed. */
  void onFailed(const String& what, unsigned int line = 0);
  
  /** Called when subtest failed. */
  void onAssert(bool passed, const String& what, unsigned int line = 0);

  /** Called when here point is declared. */
  void registerHere(const Here* here, const char* description);
  
  /** Called when not-here point is declared. */
  void registerNotHere(const NotHere* here, const char* description);
  
  /** Called when here point is reached. */
  void onHere(const Here* here);
  
  /** Called when nothere point is reached but shouldn't be reached. */
  void onNotHere(const NotHere* here);
public:

  /** Initializes the test. */
  UnitTest();

  /** Sets the name/id of the test. */
  void setName(const String& name);
  
  /** Sets the description of the test. */
  void setDescription(const String& description);
  
  /** Sets the source file of the test. */
  void setSource(const String& source, unsigned int line = 0);

  /** Sets the type (from class) of the test. */
  void setType(const Type& type);

  /** Returns the ID of the test (project/name). */
  String getId() const;

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

  /** Returns the source line of the test. */
  inline unsigned int getLine() const noexcept
  {
    return line;
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

  /** Use TEST_IN_DEVELOPMENT to tag test as being in development. */
  virtual bool isInDevelopment() const noexcept
  {
    return false;
  }

  /** Use TEST_IMPACT to set priority for test. */
  virtual Impact getImpact() const noexcept
  {
    return NORMAL;
  }
  
  /** Use TEST_OWNER to set owner for test. */
  virtual String getOwner() const noexcept
  {
    return String();
  }

  /** Use TEST_PROJECT to set project for test. */
  virtual String getProject() const noexcept
  {
    return String();
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
  
  /** Returns the last run. */
  Reference<Run> getLastRun()
  {
    return !runs.isEmpty() ? runs.getAt(runs.getSize() - 1) : nullptr;
  }

  /** Returns dependencies. */
  inline const Array<String>& getDependencies() const noexcept
  {
    return dependencies;
  }

  /** Adds dependency. */
  void addDependency(const String& id)
  {
    dependencies.append(id);
  }

  /** Runs the test. */
  virtual void run();

  /** Returns the global state. Allows tests to share state. But keep tests deterministic. */
  AnyValue getState(const String& id);

  /** Sets the global state. Allows tests to share state. But keep tests deterministic. */
  void setState(const String& id, const AnyValue& value);

  /** Internal run. */
  Reference<Run> runImpl();

  /** Returns the JUnit fragment. */
  String getJUnit() const;

  ~UnitTest();
};

/** Unit test manager. */
class _COM_AZURE_DEV__BASE__API UnitTestManager {
  friend class UnitTest;
public:

  enum Verbosity {
    SILENT,
    FINAL,
    COMPACT,
    NORMAL,
    VERBOSE
  };

  /** Sort tests. */
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
  bool showStackTrace = false;
  bool traceExceptions = false;
  bool useUrlAsSource = false;
  unsigned int passed = 0;
  unsigned int failed = 0;
  Timer timer;
  MutualExclusion lock;
  /** All tests. */
  Array<Reference<UnitTest> > tests;
  /** States. */
  Map<String, AnyValue> states;

  static UnitTestManager unitTestManager;
public:

  UnitTestManager();
  
  static UnitTestManager& getManager();

  /** Adds the given test. */
  void addTest(Reference<UnitTest> test);
  
  /** Only returns the filename after the last / or \. */
  static const char* trimPath(const char*) noexcept;

  /**
    Returns the JUnit document.

    @param uuid UUID to use for testsuite. If empty, a random UUID will be used.
    @param name The testsuite name ("BASE" by default).
  */
  String getJUnit(const String& uuid = String(), const String& name = "BASE") const;

  /** Create test instance and adds it. */
  template<class TYPE>
  inline void registerTest(const String& name, const String& source = String(), unsigned int line = 0, const String& description = String())
  {
    auto test = new TYPE();
    test->setName(name);
    test->setType(Type::getType<TYPE>()); // we cannot get type of original type since it may be a template
    test->setDescription(description);
    test->setSource(source, line);
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

  /** Returns color mode. */
  inline bool getUseANSIColors() const noexcept
  {
    return useANSIColors;
  }

  /** Sets color mode. */
  inline void setUseANSIColors(bool _useANSIColors) noexcept
  {
    useANSIColors = _useANSIColors;
  }
  
  /** Returns true if source shold be shown as GIT repo url. */
  inline bool getUseUrlAsSource() const noexcept
  {
    return useUrlAsSource;
  }

  /** Sets if source shold be shown as GIT repo url. */
  inline void setUseUrlAsSource(bool _useUrlAsSource) noexcept
  {
    useUrlAsSource = _useUrlAsSource;
  }

  /** Sets randomization mode. */
  inline void setRandomize(bool _randomize) noexcept
  {
    randomize = _randomize;
  }

  /** Sets stop on failure. */
  inline void setStopOnFailure(bool _stopOnFailure) noexcept
  {
    stopOnFailure = _stopOnFailure;
  }

  /** Returns true if stack traces are enabled. */
  inline bool getShowStackTrace() const noexcept
  {
    return showStackTrace;
  }

  /** Enables stack traces. */
  inline void setShowStackTrace(bool _showStackTrace) noexcept
  {
    showStackTrace = _showStackTrace;
  }

  /** Returns progress mode. */
  inline bool getProgressMode() const noexcept
  {
    return progressMode;
  }
  
  /** Sets progress mode. */
  inline void setProgressMode(bool _progressMode) noexcept
  {
    progressMode = _progressMode;
  }

  /** Sets exception trace mode. */
  inline void setTraceExceptions(bool _traceExceptions) noexcept
  {
    traceExceptions = _traceExceptions;
  }

  /** Runs the test. A test must be able to run multiple times. */
  bool runTest(Reference<UnitTest> test);

  /** Loads the registered tests. */
  void loadTests();

  /** Runs the tests matching the given pattern. */
  bool runTests(const String& pattern = "*", bool runDevel = false);

  /** Returns all the registered tests. */
  inline const Array<Reference<UnitTest> >& getTests() const noexcept
  {
    return tests;
  }

  /** Returns the test for the given ID. */
  Reference<UnitTest> getTest(const String& id) const noexcept;

  /** Returns the test(s) matching the given pattern. */
  Array<Reference<UnitTest> > getTestByPattern(const String& id) const noexcept;

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

  /** Internal test dependency helper. */
  class _COM_AZURE_DEV__BASE__API DependencyEntry {
    friend class UnitTestManager;
  public:

    struct DependencyNode {
      const char* key;
      const char* dependency;
      DependencyNode* next; // next node
    };

    DependencyNode* node = nullptr;
  private:

    /** The last test dependency. This is a pointer so it is always ready for use during global initialization. */
    static DependencyNode* nodes;
  public:

    /** Records test dependency. */
    DependencyEntry(DependencyNode* entry);

    ~DependencyEntry();
  };
};



#define TEST_REGISTER_IMPL(ID, TYPE) \
  namespace { namespace ID { \
    void _entry() { base::UnitTestManager::getManager().registerTest<TEST_CLASS(TYPE)>(#TYPE, UnitTestManager::trimPath(_COM_AZURE_DEV__BASE__SOURCE_FILE), __LINE__, String()); } \
    base::UnitTestManager::RegisterEntry::EntryNode _storage = {#TYPE, _entry, nullptr, false}; \
    base::UnitTestManager::RegisterEntry _register(&_storage); \
  } }

/** Fast registration of test for later run. Test will be put into unique subnamespace. */
#define TEST_REGISTER(TYPE) \
  TEST_REGISTER_IMPL(_COM_AZURE_DEV__BASE__MAKE_IDENTIFIER(tests), TYPE)

/**
  Adds dependency for test. If required test failed this test is skipped.
  DEPENDENCY may be a pattern. TEST_DEPENDENCY is recommended over
  TEST_DEPENDENCY_ON.
*/
#define TEST_DEPENDENCY(DEPENDENCY) \
  base::UnitTest::AddDependency _COM_AZURE_DEV__BASE__MAKE_IDENTIFIER(_dependency) = base::UnitTest::AddDependency(this, DEPENDENCY);

/** Sets dependency on other test. If required test failed this test is skipped. DEPENDENCY may be a pattern. */
#define TEST_DEPENDENCY_ON(TYPE, DEPENDENCY) \
  namespace { namespace _COM_AZURE_DEV__BASE__MAKE_IDENTIFIER(tests) { \
    base::UnitTestManager::DependencyEntry::DependencyNode _storage = {#TYPE, "" DEPENDENCY, nullptr}; \
    base::UnitTestManager::DependencyEntry _register(&_storage); \
  } }

/** Tag all references to the Test class using this define. */
#define TEST_CLASS(CLASS) _TEST_ ## CLASS

/** An assert/subtest within the test. */
#define TEST_ASSERT(EXPRESSION) \
  base::UnitTest::onAssert((EXPRESSION), _COM_AZURE_DEV__BASE__STRINGIFY(EXPRESSION), __LINE__)

/** Require expression to be equal to the given comperand. operator==() must be supported. Expression and comperand must have output rule to FormatOutputStream. */
#define TEST_EQUAL(EXPRESSION, COMPERAND) \
  { \
    const auto result = (EXPRESSION); \
    const bool equal = result == COMPERAND; \
    base::UnitTest::onAssert(equal, _COM_AZURE_DEV__BASE__STRINGIFY((EXPRESSION) == COMPERAND), __LINE__); \
    if (!equal) { \
      StringOutputStream stream; \
      stream << "Expecting value '" << COMPERAND << "' for '" << _COM_AZURE_DEV__BASE__STRINGIFY(EXPRESSION) << "'. But got '" << result << "'." << FLUSH; \
      base::UnitTest::onPrint(stream.getString(), __LINE__); \
    } \
  }

/** Require expression to throw given exception. */
#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__WASI)
#define TEST_EXCEPTION(EXPRESSION, EXCEPTION) \
  try { \
    EXPRESSION; \
    base::UnitTest::onAssert(false, "Expecting exception '" #EXCEPTION "' for expression '" #EXPRESSION "'", __LINE__); \
  } catch (EXCEPTION&) { \
  } catch (Exception& e) { \
    StringOutputStream stream; \
    stream << "Expecting exception '" << Type::getType<EXCEPTION>() << "' for '" << _COM_AZURE_DEV__BASE__STRINGIFY(EXPRESSION) \
           << "' but got " << e.getThisType() << " with message '" << e.getMessage() << "'." << FLUSH; \
    base::UnitTest::onPrint(stream.getString(), __LINE__); \
    _rethrow; \
  } catch (std::exception& e) { \
    StringOutputStream stream; \
    stream << "Expecting exception '" << Type::getType<EXCEPTION>() << "' for '" << _COM_AZURE_DEV__BASE__STRINGIFY(EXPRESSION) \
           << "' but got " << Exception::getStdExceptionName(e) << " with message '" << e.what() << "'." << FLUSH; \
    base::UnitTest::onPrint(stream.getString(), __LINE__); \
    _rethrow; \
  } catch (...) { \
    StringOutputStream stream; \
    stream << "Expecting exception '" << Type::getType<EXCEPTION>() << "' for '" << _COM_AZURE_DEV__BASE__STRINGIFY(EXPRESSION) << "' but got unknown exception." << FLUSH; \
    base::UnitTest::onPrint(stream.getString(), __LINE__); \
    _rethrow; \
  }
#else
#define TEST_EXCEPTION(EXPRESSION, EXCEPTION) while (false)
#endif

/** Assert within an expression. */
#define TEST_INLINE_ASSERT(EXPRESSION) \
  (base::UnitTest::onAssert(EXPRESSION, "" #EXPRESSION, __LINE__), (EXPRESSION))

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

/** Sets the priority for the test. Lower priorty is "higher" and gets run first. */
#define TEST_PRIORITY(priority) int getPriority() const noexcept override {return static_cast<int>(priority);}

/** Tags the test as being in development which means test is ignored by default. */
#define TEST_IN_DEVELOPMENT() bool isInDevelopment() const noexcept override {return true;}

/** Sets the impact for the test. Lower impact is "higher" and gets run first for the same priority group. */
#define TEST_IMPACT(impact) Impact getImpact() const noexcept override {return static_cast<Impact>(impact);}

/** Sets the owner for the test. */
#define TEST_OWNER(owner) String getOwner() const noexcept override {return static_cast<const char*>(owner);}

/** Sets the project for the test. */
#define TEST_PROJECT(project) String getProject() const noexcept override {return static_cast<const char*>(project);}

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
