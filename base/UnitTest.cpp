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
#include <base/concurrency/Process.h>
#include <base/concurrency/Thread.h>
#include <base/string/ANSIEscapeSequence.h>
#include <base/objectmodel/JSON.h>
#include <base/Random.h>
#include <algorithm>

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

void UnitTest::Run::onPrint(const String& what, unsigned int line)
{
  ExclusiveSynchronize<MutualExclusion> _guard(UnitTestManager::getManager().getLock());

  /*
  TestResult result;
  result.passed = xxx;
  result.what = what;
  */

  if (UnitTestManager::getManager().getVerbosity() > UnitTestManager::NORMAL) {
    if (line > 0) {
      fout << "  PRINT: '" << what << "' at line " << line << ENDL;
    }
    else {
      fout << "  PRINT: '" << what << "'" << ENDL;
    }
  }
  // results.append(result);
}

void UnitTest::Run::onPassed(const String& what, unsigned int line)
{
  ExclusiveSynchronize<MutualExclusion> _guard(UnitTestManager::getManager().getLock());

  ++passed;
  TestResult result;
  result.event = PASSED;
  result.what = what;

  if (UnitTestManager::getManager().getVerbosity() > UnitTestManager::NORMAL) {
    if (UnitTestManager::getManager().getUseANSIColors()) {
      fout << setForeground(ANSIEscapeSequence::GREEN);
    }
    if (line > 0) {
      fout << "  PASSED: '" << what << "' at line " << line << ENDL;
    } else {
      fout << "  PASSED: '" << what << "'" << ENDL;
    }
    if (UnitTestManager::getManager().getUseANSIColors()) {
      fout << normal();
    }
  }
  results.append(result);
}

void UnitTest::Run::onFailed(const String& what, unsigned int line)
{
  ExclusiveSynchronize<MutualExclusion> _guard(UnitTestManager::getManager().getLock());

  ++failed;
  TestResult result;
  result.event = FAILED;
  result.what = what;
  if (UnitTestManager::getManager().getVerbosity() >= UnitTestManager::NORMAL) {
    if (UnitTestManager::getManager().getUseANSIColors()) {
      fout << setForeground(ANSIEscapeSequence::RED);
    }
    if (line > 0) {
      fout << "  FAILED: '" << what << "' at line " << line << ENDL;
    } else {
      fout << "  FAILED: '" << what << "'" << ENDL;
    }
    if (UnitTestManager::getManager().getUseANSIColors()) {
      fout << normal();
    }
  }
  results.append(result);
}

void UnitTest::Run::registerHere(const Here* here, const char* description)
{
  ExclusiveSynchronize<MutualExclusion> _guard(UnitTestManager::getManager().getLock());

  HereMeta meta;
  meta.description = description;
  meta.reach = true;
  heres[static_cast<const void*>(here)] = meta;
}

void UnitTest::Run::registerNotHere(const NotHere* here, const char* description)
{
  ExclusiveSynchronize<MutualExclusion> _guard(UnitTestManager::getManager().getLock());

  HereMeta meta;
  meta.description = description;
  meta.reach = false;
  heres[static_cast<const void*>(here)] = meta;
}

void UnitTest::Run::onHere(const Here* _here)
{
  ExclusiveSynchronize<MutualExclusion> _guard(UnitTestManager::getManager().getLock());

  const void* here = static_cast<const void*>(_here);

  if (!here) {
    onFailed("Test failed due to bad here marker");
    return;
  }
  if (!heres.isKey(here)) {
    onFailed("Test failed due to undeclared key");
    return;
  }

  HereMeta meta = heres[here];
  ++meta.count;
  heres[here] = meta; // TAG: improve map so we can use heres[here] directly
}

void UnitTest::Run::onNotHere(const NotHere* _here)
{
  ExclusiveSynchronize<MutualExclusion> _guard(UnitTestManager::getManager().getLock());

  const void* here = static_cast<const void*>(_here);

  if (!here) {
    onFailed("Test failed due to bad here marker");
    return;
  }
  if (!heres.isKey(here)) {
    onFailed("Test failed due to undeclared key");
    return;
  }

  HereMeta meta = heres[here];
  ++meta.count;
  heres[here] = meta; // TAG: improve map so we can use heres[here] directly
}

Reference<ObjectModel::Object> UnitTest::Run::getReport() const
{
  ObjectModel o;
  auto report = o.createObject();
  report->setValue(o.createString("elapsedTime"), o.createFloat(endTime - startTime));
  report->setValue(o.createString("passed"), o.createInteger(passed));
  report->setValue(o.createString("failed"), o.createInteger(failed));
  auto a = o.createArray();
  report->setValue(o.createString("results"), a);

  for (auto result : results) {
    auto item = o.createObject();
    item->setValue(o.createString("what"), o.createString(result.what));
    switch (result.event) {
    case FAILED:
      item->setValue(o.createString("event"), o.createString("failed"));
      break;
    case PASSED:
      item->setValue(o.createString("event"), o.createString("passed"));
      break;
    case PRINT:
      item->setValue(o.createString("event"), o.createString("print"));
      break;
    }
    a->append(item);
  }
  
  return report;
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
  
  Reference<Run> r = new Run();
  currentRun = r;
  runs.append(r);

  Process process = Process::getProcess();
  const Process::Times processTimes = process.getTimes();
  r->startTime = manager.timer.getLiveMicroseconds();
  /*
  if (manager.getVerbosity() >= UnitTestManager::COMPACT) {
    fout << Format::subst("  START at %1s", r->startTime / 1000000.0) << ENDL;
  }
  */
  try {
    run();
  } catch (Exception& e) {
    if (e.getMessage()) {
      onFailed(Format::subst("Test failed with exception: '%1'", e.getType())); // TAG: dump all info for exception
    } else {
      onFailed(Format::subst("Test failed with exception: '%1'", e.getType(), e.getMessage())); // TAG: dump all info for exception
    }
  } catch (std::exception& e) {
    // TAG: dump all info for exception
    String w = e.what();
    if (w.isEmpty()) {
      onFailed("Test failed with std::exception");
    } else {
      onFailed(Format::subst("Test failed with std::exception: '%1'", w));
    }
  } catch (...) {
    onFailed("Test failed with unknown exception");
  }
  r->endTime = manager.timer.getLiveMicroseconds();
  currentRun = nullptr;
  const Process::Times processTimes2 = process.getTimes();

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

  r->failed += (pointsReach - pointsReached);
  r->failed += (pointsNotReach - pointsNotReached);
  
  if ((manager.getVerbosity() > UnitTestManager::SILENT) && !manager.getProgressMode()) {
    if (UnitTestManager::getManager().getUseANSIColors()) {
      fout << setForeground(
        (r->failed || (pointsReached != pointsReach) || (pointsNotReached != pointsNotReach)) ? ANSIEscapeSequence::RED : ANSIEscapeSequence::GREEN
      );
    }
    
    String text = Format::subst("  PASSED:%1 FAILED:%2 HERE:%3/%4 NOT_HERE:%5/%6", r->passed, r->failed, pointsReached, pointsReach, pointsNotReached, pointsNotReach);
    if (manager.getVerbosity() >= UnitTestManager::COMPACT) {
      text += Format::subst(" ELAPSED:%1ms", (r->endTime - r->startTime) / 1000.0); // r->endTime/1000.0
    }
    if (manager.getVerbosity() >= UnitTestManager::COMPACT) {
      text += Format::subst(" RUN:#%1", runs.getSize());
    }
    if (manager.getVerbosity() >= UnitTestManager::VERBOSE) {
      const auto delta = processTimes2 - processTimes;
      text += Format::subst(" USER:%1ms SYSTEM:%2ms", delta.user / 1000.0, delta.system / 1000.0);
    }

    fout << text << ENDL;
    if (UnitTestManager::getManager().getUseANSIColors()) {
      fout << normal();
    }
  }

  if (((pointsReached < pointsReach) || (pointsNotReached < pointsNotReach)) &&
      (manager.getVerbosity() >= UnitTestManager::COMPACT)) {
    auto e = r->heres.getReadEnumerator();
    while (e.hasNext()) {
      auto n = e.next();
      auto meta = n->getValue();
      if (meta.reach) {
        if (meta.count == 0) {
          fout << Format::subst("  UNREACHED HERE '%1'", String(meta.description)) << ENDL;
        }
      } else {
        if (meta.count != 0) {
          fout << Format::subst("  REACHED NOT_HERE '%1' %2 times", String(meta.description), meta.count) << ENDL;
        }
      }
    }
  }

  if (manager.useJSON) {
    // TAG: add heres
    ObjectModel o;
    auto report = r->getReport();
    report->setValue(o.createString("test"), o.createString(getName()));
    report->setValue(o.createString("description"), o.createString(getDescription()));
    report->setValue(o.createString("source"), o.createString(getSource()));
    
    const auto delta = processTimes2 - processTimes;
    const double time = delta.getTotal()/1000.0;
    report->setValue(o.createString("processingTime"), o.createFloat(time));

    report->setValue(o.createString("pointsReached"), o.createInteger(pointsReached));
    report->setValue(o.createString("pointsReach"), o.createInteger(pointsReach));
    report->setValue(o.createString("pointsNotReached"), o.createInteger(pointsNotReached));
    report->setValue(o.createString("pointsNotReach"), o.createInteger(pointsNotReach));

    String json = JSON::getJSON(report, false);
    ferr << report << ENDL;
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
  tests.append(test);
}

bool UnitTestManager::runTest(Reference<UnitTest> test)
{
  auto run = test->runImpl();
  if (!run->failed) {
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
          Trace::message(Format::subst(MESSAGE("Failed to register test '%1'"), String(entry->key)).native());
          ASSERT(!"Failed to register test");
        }
      }
    }
    entry = entry->next;
  }
}

class TestingThread : public Thread {
private:
  
  Reference<UnitTest> test;
public:
  
  bool success = false;
  Thread::Times times;
  
  TestingThread(Reference<UnitTest> _test) : test(_test)
  {
  }
  
  void run()
  {
    auto& manager = UnitTestManager::getManager();
    success = manager.runTest(test);
    times = Thread::getTimes();
  }
};

String makeProgress(double progress, unsigned int width = 20)
{
  // improve
  String result;
  int count = static_cast<int>(progress * width);
  for (int i = 0; i < (count - 1); ++i) {
    result.append('=');
  }
  result.append('>');
  for (int i = 0; i < static_cast<int>(width - count); ++i) {
    result.append(' ');
  }
  return result;
}

bool UnitTestManager::runTests(const String& pattern)
{
  // TAG: allow test to be terminated timeout
  // TAG: allow tests to run concurrently

  loadTests();

#if 0 // do a lot of tests by duplicating tests
  // TAG: need to fix corruption issue
  auto tests = this->tests;
  for (unsigned int i = 0; i < 100; ++i) {
    for (auto& test : tests) {
      tests.append(new UnitTest(*test));
    }
  }
#endif
  
  Array<Reference<UnitTest> > tests;
  // TAG: tests.ensureCapacity();
  for (auto test : this->tests) {
    if (Parser::doesMatchPattern(pattern, test->getName())) {
      tests.append(test);
    }
  }

  if (randomize) {
    tests.shuffle();
  } else {
    std::sort(tests.begin(), tests.end(), SortTests());
  }

  unsigned int passed = 0;
  unsigned int failed = 0;
  unsigned int count = 0;
  Thread::Times totalTimes;

  for (auto test : tests) {
    // TAG: show progress - show time since last output - include processing time
    Timer timer;

    ++count;
    if (!progressMode) {
      fout << "===============================================================================" << EOL;
      if (UnitTestManager::getManager().getUseANSIColors()) {
        fout << "TEST " << bold() << test->getName() << normal();
      } else {
        fout << "TEST " << test->getName();
      }
      fout << " [" << count << "/" << tests.getSize() << "] (" << static_cast<int>(count * 1000.0 / tests.getSize()) / 10.0 << "%)" << ENDL;
      if (UnitTestManager::getManager().getUseANSIColors()) {
        fout << normal();
      }
      if (!test->getSource().isEmpty()) {
        fout << "  Source: " << test->getSource() << ENDL;
      }
      if (!test->getDescription().isEmpty()) {
        fout << "  Description: " << test->getDescription() << ENDL;
      }
    }
    
    TestingThread thread(test);
    thread.start();
    // TAG: show progress here so we can see it live
    // TAG: show subpasses/fails - elapsed time
    // TAG: check thread event
    if (progressMode) {
      // TAG: get memory used
      // TAG: use color GREEN / RED
      for (unsigned int i = 0; i < 100; ++i) {
        auto elapsed = timer.getLiveMicroseconds();
        // TAG: get line width from console and trim output to fit
        /*
        fout << "                                                                               \r"
           << Format::subst("TEST: [%1] PASSED:%2 FAILED:%3 / %4/%5 / %6 / %7 s\r", makeProgress(count*1.0/tests.getSize(), 20), passed, failed, count, tests.getSize(), test->getName(), elapsed/1000/1000.0) << FLUSH;
        */
        String percent = format() << ZEROPAD << setWidth(4) << static_cast<int>(count*1000.0/tests.getSize())/10.0;
        if (UnitTestManager::getManager().getUseANSIColors()) {
          fout << setForeground(!failed ? ANSIEscapeSequence::GREEN : ANSIEscapeSequence::RED) << bold();
        }
        fout << "                                                                               \r"
             << Format::subst("TEST: [%1%%] PASSED:%2 FAILED:%3 / %4/%5 / %6 / %7 ms\r", percent, passed, failed, count, tests.getSize(), test->getName(), elapsed/1000.0) << FLUSH;
        if (UnitTestManager::getManager().getUseANSIColors()) {
          fout << normal();
        }
        Thread::microsleep(1*1000);
        // TAG: clear when done
      }
    }
    thread.join();
    
    #if 0
        if (!progressMode) {
          auto time = thread.times.getTotal();
          if (time < 10000) {
            fout << "  PROCESSING TIME: " << thread.times.getTotal() / 1.0 << " ns" << ENDL;
          } else {
            fout << "  PROCESSING TIME: " << thread.times.getTotal() / 1000000.0 << " ms" << ENDL;
          }
        }
    #endif
    
    if (thread.success) {
      ++passed;
    } else {
      ++failed;
      if (stopOnFailure) {
        break;
      }
    }

    totalTimes = totalTimes + thread.times;
  }
  
  if (progressMode) {
    fout << "                                                                               \r";
  }
  
  fout << EOL << "===============================================================================" << EOL;
  if (UnitTestManager::getManager().getUseANSIColors()) {
    fout << setForeground((passed == count) ? ANSIEscapeSequence::GREEN : ANSIEscapeSequence::RED) << Format::subst("TOTAL PASSED: %1/%2", passed, count) << normal() << ENDL;
  } else {
    fout << Format::subst("TOTAL PASSED: %1/%2", passed, count) << ENDL;
  }
  fout << "TOTAL PROCESSING TIME: " << totalTimes.getTotal()/1000000.0 << " ms" << ENDL;

  return !failed;
}

UnitTestManager::RegisterEntry::RegisterEntry(EntryNode* _node)
  : node(_node)
{
  if (node) {
    static unsigned int numberOfTests = 0;
    auto previous = nodes;
    node->next = previous;
    nodes = node;
    ++numberOfTests;
  }
}

UnitTestManager::RegisterEntry::~RegisterEntry()
{
  if (node) {
    // TAG: we need to pull out from linked list also!
    node->entry = nullptr; // prevent access of unloaded memory
    // TAG: if already loaded we will have a problem
    // ASSERT(!node->loaded); // Unloading module which has registered test
    // TAG: find module and disable
  }
}

const char* UnitTestManager::trimPath(const char* src) noexcept
{
  // const char* PATH = _COM_AZURE_DEV__BASE__SOURCE_FILE; // trim UnitTest.cpp - this will give the base path - but we need to support any source
  
  if (!src) {
    return nullptr;
  }
  const char* last = src;
  while (*src) {
    if ((*src == '/') || (*src == '\\')) {
      ++src;
      last = src;
      continue;
    }
    ++src;
  }
  return last;
}

// TAG: add support for http server to view results live

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
