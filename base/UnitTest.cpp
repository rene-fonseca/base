/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/UnitTest.h>
#include <base/string/Format.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

UnitTestManager::RegisterEntry::EntryNode* UnitTestManager::RegisterEntry::nodes = nullptr;

UnitTest::UnitTest()
{
}

void UnitTest::setName(const String& _name)
{
  name = _name;
}

void UnitTest::setDescription(const String& _description)
{
  description = _description;
}

void UnitTest::setSource(const String& _source)
{
  source = _source;
}

void UnitTest::setType(const Type& _type)
{
  type = _type;
}

void UnitTest::Run::onPassed(const String& what)
{
  ++passed;
  TestResult result;
  result.passed = true;
  result.what = what;
  fout << "TEST: PASSED: " << what << ENDL;
  results.append(result);
}

void UnitTest::Run::onFailed(const String& what)
{
  ++failed;
  TestResult result;
  result.passed = false;
  result.what = what;
  fout << "TEST: FAILED: " << what << ENDL;
  results.append(result);
}

void UnitTest::Run::registerHere(Here here, bool reach, const char* description)
{
  HereMeta meta;
  meta.description = description;
  meta.reach = reach;
  heres[here] = meta;
}

void UnitTest::Run::onHere(Here here)
{
  if (!here || (*here != HERE_MARKER)) {
    onFailed("Test failed due to bad here marker.");
    return;
  }
  if (!heres.isKey(here)) {
    onFailed("Test failed due to undeclared key.");
    return;
  }

  HereMeta meta = heres[here];
  ++meta.count;
  heres[here] = meta; // TAG: improve map so we can use heres[here] directly
}

void UnitTest::Run::onNotHere(Here here)
{
  if (!here || (*here != HERE_MARKER)) {
    onFailed("Test failed due to bad here marker.");
    return;
  }
  if (!heres.isKey(here)) {
    onFailed("Test failed due to undeclared key.");
    return;
  }

  HereMeta meta = heres[here];
  ++meta.count;
  heres[here] = meta; // TAG: improve map so we can use heres[here] directly
}

bool UnitTest::Run::compare(const Run& a, const Run& b)
{
  // TAG: get report
  return false;
}

void UnitTest::run()
{
  onFailed("Test not implemented");
}

#if 0 // TAG: convert enum to iterator
inline Pointer begin() throw() {
  return hasNext();
}

inline Pointer end() throw() {
  return nullptr;
}

inline Pointer operator++() throw(EndOfEnumeration) {
  return next();
}

inline Pointer operator*() throw(EndOfEnumeration) {
  return next();
}
#endif

Reference<UnitTest::Run> UnitTest::runImpl()
{
  auto& manager = UnitTestManager::getManager();

  fout << "TEST " << name << " / type " << type.getLocalName() << ENDL;
  if (!source.isEmpty()) {
    fout << "  source " << source << ENDL;
  }
  if (!description.isEmpty()) {
    fout << "  description " << description << ENDL;
  }

  Reference<Run> r = new Run();
  currentRun = r;
  r->startTime = manager.timer.getLiveMicroseconds();
  fout << Format::subst("START TEST '%1' at %2.", getName(), r->startTime/1000000.0) << ENDL;
  try {
    run();
  } catch (Exception& e) {
    if (e.getMessage()) {
      onFailed(Format::subst("Test failed with exception: '%1'.", e.getType())); // TAG: dump all info for exception
    } else {
      onFailed(Format::subst("Test failed with exception: '%1'.", e.getType(), e.getMessage())); // TAG: dump all info for exception
    }
  } catch (std::exception& e) {
    // TAG: dump all info for exception
    String w = e.what();
    if (w.isEmpty()) {
      onFailed("Test failed with std::exception.");
    } else {
      onFailed(Format::subst("Test failed with std::exception: '%1'.", w));
    }
  } catch (...) {
    onFailed("Test failed with unknown exception.");
  }
  r->endTime = manager.timer.getLiveMicroseconds();
  fout << Format::subst("END TEST '%1' at %2 (%3).", getName(), r->endTime / 1000000.0, (r->endTime - r->startTime)/1000000.0) << ENDL;

  unsigned int pointsReached = 0;
  unsigned int pointsReach = 0;
  unsigned int pointsNotReached = 0;
  unsigned int pointsNotReach = 0;
  {
    auto e = r->heres.getReadEnumerator();
    while (e.hasNext()) {
      auto n = e.next();
      auto meta = n->getValue();
      if (meta.reach) {
        ++pointsReach;
        if (meta.count != 0) {
          ++pointsReached;
        }
      } else {
        ++pointsNotReach;
        if (meta.count == 0) {
          ++pointsNotReached;
        }
      }
    }
  }

  fout << Format::subst("TEST '%1' PASSED:%2 FAILED:%3 HERE:%4/%5 NOTHERE:%6/%7.", getName(), r->passed, r->failed, pointsReached, pointsReach, pointsNotReach, pointsNotReached) << ENDL;

  if ((pointsReached < pointsReach) || (pointsNotReached < pointsNotReach)) {
    auto e = r->heres.getReadEnumerator();
    while (e.hasNext()) {
      auto n = e.next();
      auto meta = n->getValue();
      if (meta.reach) {
        if (meta.count == 0) {
          fout << Format::subst("TEST UNREACHED HERE '%1'.", String(meta.description)) << ENDL;
        }
      } else {
        if (meta.count != 0) {
          fout << Format::subst("TEST REACHED NOTHERE '%1' %2 times.", String(meta.description), meta.count) << ENDL;
        }
      }
    }
  }

  return r;
}

UnitTest::~UnitTest()
{
}

UnitTestManager::UnitTestManager()
{
}

UnitTestManager& UnitTestManager::getManager()
{
  return unitTestManager;
}

void UnitTestManager::addTest(Reference<UnitTest> test)
{
  // fout << Format::subst("TESTS: Adding test '%1' from '%2'.", test->getName(), test->getSource()) << ENDL;
  tests.append(test);
}

bool UnitTestManager::runTest(Reference<UnitTest> test)
{
  if (test->runImpl()) {
    ++passed;
    return true;
  } else {
    ++failed;
    return false;
  }
}

void UnitTestManager::loadTests()
{
  auto entry = RegisterEntry::nodes;
  while (entry) {
    if (!entry->loaded) {
      entry->loaded = true; // TAG: we can also remove from linked list
      if (INLINE_ASSERT(entry->entry)) {
        try {
          entry->entry();
        } catch (...) {
          Trace::message(Format::subst(MESSAGE("Failed to register test '%1'."), String(entry->key)).native());
          ASSERT(!"Failed to register test.");
        }
      }
    }
    entry = entry->next;
  }
}

bool UnitTestManager::runTests(const String& pattern)
{
  // TAG: add support for pattern
  // TAG: continue on failure - can also get critical from test and continue is non-critical
  // TAG: order tests by priority, limited time
  // TAG: allow to run by priority - p < some value
  // TAG: allow test to be terminated
  // TAG: allow tests to run concurrently

  loadTests();
  // TAG: allow progress to be written - n out of m (p%) - and show name/type

  bool result = true;
  unsigned int count = 0;
  for (auto test : tests) {
    // TAG: show time
    Timer timer;
    timer.getLiveMicroseconds();
    fout << "                                                                               \r"
         << Format::subst("TEST: %1 - %2/%3 (%4%%)\r", test->getName(), count, tests.getSize(), static_cast<int>(count*1*00.0/tests.getSize())/10.0);

    // TAG: record processing time for thread
    // TAG: we could sample processing time also
    // TAG: need viewer for test results
    // Thread thread;
    result &= runTest(test);
  }
  return result;
}

UnitTestManager::RegisterEntry::RegisterEntry(EntryNode* node)
{
  if (node) {
    static unsigned int numberOfTests = 0;
    auto previous = nodes;
    node->next = previous;
    nodes = node;
    ++numberOfTests;
  }
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
