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
#include <base/string/Format.h>
#include <base/objectmodel/JSON.h>
#include <base/Guid.h>
#include <base/Random.h>
#include <base/TypeInfo.h>
#include <base/Date.h>
#include <algorithm>
#include <memory>

// TAG: record timestamps for all events and show in output

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

UnitTestManager::RegisterEntry::EntryNode* UnitTestManager::RegisterEntry::nodes = nullptr;
UnitTestManager::DependencyEntry::DependencyNode* UnitTestManager::DependencyEntry::nodes = nullptr;

UnitTest::UnitTest()
{
}

String UnitTest::getId() const
{
  String id;
  if (String project = getProject()) {
    id = project + "/" + name;
  } else {
    id = name;
  }
  return id;
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
  result.line = line;
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
  result.line = line;

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
  result.line = line;
  
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
  heres.add(static_cast<const void*>(here), meta);
}

void UnitTest::Run::registerNotHere(const NotHere* here, const char* description)
{
  ExclusiveSynchronize<MutualExclusion> _guard(UnitTestManager::getManager().getLock());

  HereMeta meta;
  meta.description = description;
  meta.reach = false;
  heres.add(static_cast<const void*>(here), meta);
}

void UnitTest::Run::onHere(const Here* _here)
{
  ExclusiveSynchronize<MutualExclusion> _guard(UnitTestManager::getManager().getLock());

  const void* here = static_cast<const void*>(_here);

  if (!here) {
    onFailed("Test failed due to bad here marker");
    return;
  }
  auto found = heres.find(here);
  if (!found) {
    onFailed("Test failed due to undeclared key");
    return;
  }
  ++(found->count);
}

void UnitTest::Run::onNotHere(const NotHere* _here)
{
  ExclusiveSynchronize<MutualExclusion> _guard(UnitTestManager::getManager().getLock());

  const void* here = static_cast<const void*>(_here);

  if (!here) {
    onFailed("Test failed due to bad here marker");
    return;
  }
  auto found = heres.find(here);
  if (!found) {
    onFailed("Test failed due to undeclared key");
    return;
  }
  ++(found->count);
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
  return false;
}

void UnitTest::run()
{
  onFailed("Test not implemented");
}

const char* UnitTestManager::getStdExceptionName(const std::exception& e) noexcept
{
  if (dynamic_cast<const std::runtime_error*>(&e)) {
    if (dynamic_cast<const std::range_error*>(&e)) {
      return "std::range_error";
    } else if (dynamic_cast<const std::overflow_error*>(&e)) {
      return "std::overflow_error";
    } else if (dynamic_cast<const std::underflow_error*>(&e)) {
      return "std::underflow_error";
    } else if (dynamic_cast<const std::out_of_range*>(&e)) {
      return "std::out_of_range";
#if 0
    } else if (dynamic_cast<const std::regex_error*>(&e)) { // C++11
      return "std::regex_error";
#endif
    } else if (dynamic_cast<const std::system_error*>(&e)) { // C++11
      return "std::system_error";
    }
    return "std::runtime_error";
  } else if (dynamic_cast<const std::logic_error*>(&e)) {
    if (dynamic_cast<const std::invalid_argument*>(&e)) {
      return "std::invalid_argument";
    } else if (dynamic_cast<const std::domain_error*>(&e)) {
      return "std::domain_error";
    } else if (dynamic_cast<const std::length_error*>(&e)) {
      return "std::length_error";
    } else if (dynamic_cast<const std::out_of_range*>(&e)) {
      return "std::out_of_range";
#if 0
    } else if (dynamic_cast<const std::future_error*>(&e)) { // C++11
      return "std::future_error";
#endif
    }
    return "std::logic_error";
  } else if (dynamic_cast<const std::bad_typeid*>(&e)) {
    return "std::bad_typeid";
  } else if (dynamic_cast<const std::bad_cast*>(&e)) {
    return "std::bad_cast";
  } else if (dynamic_cast<const std::bad_weak_ptr*>(&e)) { // C++11
    return "std::bad_weak_ptr";
#if 0
  } else if (dynamic_cast<const std::bad_function_call*>(&e)) { // C++11
    return "std::bad_function_call";
#endif
  } else if (dynamic_cast<const std::bad_alloc*>(&e)) {
    return "std::bad_alloc";
  } else if (dynamic_cast<const std::bad_array_new_length*>(&e)) { // C++11
    return "std::bad_array_new_length";
  } else if (dynamic_cast<const std::bad_exception*>(&e)) {
    return "std::bad_exception";
  }
  return "std::exception";
}

Reference<UnitTest::Run> UnitTest::runImpl()
{
  // TAG: how should we handle access to required login/accounts/resources - get environment variable?

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

    const char* _type = e.getThisType().getLocalName();
#if (_COM_AZURE_DEV__BASE__COMPILER != _COM_AZURE_DEV__BASE__COMPILER_MSC)
    String type = TypeInfo::demangleName(_type);
#else
    String type(_type);
#endif

    currentRun->exceptionFailure = "Failed with exception.";
    currentRun->exceptionType = e.getThisType().getLocalName(); // "base::Exception" - use simple format - skip class prefix
    if (e.getMessage()) {
      currentRun->exceptionFailure = e.getMessage();
      onFailed(Format::subst("Test failed with exception: '%1' / '%2'", type, e.getMessage()));
    } else {
      onFailed(Format::subst("Test failed with exception: '%1'", type));
    }
  } catch (std::exception& e) {
    const String w = e.what();
    currentRun->exceptionFailure = "Failed with exception.";
    currentRun->exceptionType = UnitTestManager::getStdExceptionName(e);
    if (w.isEmpty()) {
      onFailed("Test failed with std::exception");
    } else {
      currentRun->exceptionFailure = w;
      onFailed(Format::subst("Test failed with exception '%1': '%2'", currentRun->exceptionType, w));
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

String UnitTest::getJUnit() const
{
  String xml;
  
  Reference<Run> run;
  if (!runs.isEmpty()) {
    run = runs.getAt(runs.getSize() - 1);
  }
  uint64 time = 0;
  if (run) {
    time = run->endTime - run->startTime;
  }

  String id = getId();
  if (id.isEmpty()) {
    id = Guid::createGuidAsString();
  }

  // alternatively use getType() for classname
  const String owner = getOwner(); // responsible entity
  xml += Format::subst(
    "<testcase id=\"%1\" name=\"%2\" classname=\"%3\" time=\"%4\" owner=\"%5\" priority=\"%6\">",
    id, getName(), getName(), time/1000000.0, owner, getPriority()
  );

#if 0
  if (String description = getDescription()) {
    description.replaceAll("]]>", "]]&gt;");
    xml += "<base:description><![CDATA[" + description + "]]></base:description>";
  }
#endif

  String output;
  String error;
  if (!run) {
    xml += "<skipped/>";
  } else {
    if (run && !run->exceptionFailure.isEmpty()) {
      xml += Format::subst("<error message=\"%1\" type=\"%2\"/>", run->exceptionFailure, run->exceptionType);
    }

    // additional info
    // getImpact();
    // stats: getIO(), getMemory(), getProcessingTime()

    // TAG: escape data - switch to XML API when ready
    if (getDescription()) {
      output += Format::subst("DESCRIPTION: %1", getDescription()) + "\n";
    }
    if (getOwner()) {
      output += Format::subst("OWNER: %1", getOwner()) + "\n";
    }
    output += Format::subst("PRIORITY: %1", getPriority()) + "\n";
    static const char* IMPACTS[] = { "PRIVACY", "SECURITY", "CRITICAL", "IMPORTANT", "NORMAL", "LOW", "IGNORE" };
    ASSERT(getImpact() < getArraySize(IMPACTS));
    output += Format::subst("IMPACT: %1", String(IMPACTS[getImpact()])) + "\n";
    output += Format::subst("TIMEOUT: %1", getTimeout()) + "\n";

    for (auto dependency : getDependencies()) {
      auto tests = UnitTestManager::getManager().getTestByPattern(dependency);
      if (tests) {
        for (auto dependent : tests) {
          if (dependent->getId() == getId()) {
            continue; // ignore self
          }
          output += Format::subst("DEPENDENCY: %1", dependent->getId()) + "\n";
        }
      } else {
        output += Format::subst("DEPENDENCY: %1 (NO MATCH FOUND)", dependency) + "\n";
      }
    }

    for (auto result : run->results) {
      if (result.event == FAILED) {
        // type: WARNING
        xml += Format::subst("<failure message=\"%1\" type=\"%2\">", Format::subst("%1:%2 ", getSource(), result.line) + result.what, "ERROR");
        xml += "<![CDATA[";
        xml += result.what + "\n";
        xml += Format::subst("File: %1\n", getSource());
        xml += Format::subst("Line: %1\n", result.line);
        xml += "]]>";
        xml += "</failure>";
      }
      switch (result.event) {
      case FAILED:
        output += Format::subst("FAILED [%1:%2]: ", getSource(), result.line) + result.what + "\n";
        break;
      case PASSED:
        output += Format::subst("PASSED [%1:%2]: ", getSource(), result.line) + result.what + "\n";
        break;
      case PRINT:
        output += Format::subst("PRINT [%1:%2]: ", getSource(), result.line) + result.what + "\n";
        break;
      }
    }
  }
  if (!output.isEmpty()) {
    xml += "<system-out><![CDATA[";
    output.replaceAll("]]>", "]]&gt;");
    xml += output;
    xml += "]]></system-out>";
  }
  if (!error.isEmpty()) {
    xml += "<system-err><![CDATA[";
    error.replaceAll("]]>", "]]&gt;");
    xml += error;
    xml += "]]></system-err>";
  }
  xml += "</testcase>";
  return xml;
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
      entry->loaded = true; // we can also remove from linked list
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
  RegisterEntry::nodes = nullptr; // we have data in heap now - allow unload of module

  // add the dependencies set by TEST_DEPENDENCY_ON - TEST_DEPENDENCY is recommended since it doesnt have global id issue
  Reference<UnitTest> previousTest;
  auto dependency = DependencyEntry::nodes;
  while (dependency) {
    const String key = dependency->key; // not global id
    const String id = dependency->dependency;
    if (previousTest && (previousTest->getName() == key)) { // avoid search for test
      previousTest->addDependency(id);
    } else {
      for (auto test : tests) {
        if (test->getName() == key) {
          test->addDependency(id); // we do not resolve dependencies until we run the test
          previousTest = test;
          break;
        }
      }
    }
    dependency = dependency->next;
  }
  DependencyEntry::nodes = nullptr; // we have data in heap now - allow unload of module
}

class TestingThread : public Thread {
private:
  
  Reference<UnitTest> test;
public:
  
  bool success = false;
  Event doneEvent;
  Thread::Times times;
  
  TestingThread(Reference<UnitTest> _test) : test(_test)
  {
  }
  
  void run()
  {
    auto& manager = UnitTestManager::getManager();
    try {
      success = manager.runTest(test);
    } catch (...) {
    }
    doneEvent.signal();
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

namespace {

  /** Returns the id for presentation. */
  String presentId(const String& id)
  {
    if (UnitTestManager::getManager().getUseANSIColors()) {
      StringOutputStream stream;
      auto i = id.lastIndexOf('/');
      if (i >= 0) {
        stream << bold() << id.substring(0, i + 1) << setForeground(ANSIEscapeSequence::BLUE) << id.substring(i + 1) << normal() << FLUSH;
      } else {
        stream << bold() << setForeground(ANSIEscapeSequence::BLUE) << id << normal() << FLUSH;
      }
      return stream.getString();
    }
    return id;
  }
}

bool UnitTestManager::runTests(const String& pattern)
{
  // TAG: allow tests to run concurrently

  loadTests();

  Array<Reference<UnitTest> > tests;
  tests.ensureCapacity(this->tests.getSize());
  for (auto test : this->tests) {

    String id;
    if (String project = test->getProject()) {
      id = test->getProject() + "/" + test->getName();
    } else {
      id = test->getName();
    }

    if (Parser::doesMatchPattern(pattern, id)) {
      tests.append(test);
    }
  }

  if (randomize) {
    tests.shuffle();
  } else {
    std::sort(tests.begin(), tests.end(), SortTests());
    
    // TAG: handle dependencies - reorder - handle cyclic dependencies
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
      fout << "TEST " << presentId(test->getId())
           << " [" << count << "/" << tests.getSize() << "] (" << static_cast<int>(count * 1000.0 / tests.getSize()) / 10.0 << "%)" << ENDL;
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

    // TAG: find by ID
    // TAG: skip if dependent test failed
    
    TestingThread thread(test);
    thread.start();
    bool timedOut = false;
    if (progressMode) {
      do {
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

        if ((elapsed / 1000) > test->getTimeout()) {
          timedOut = true;
          break;
        }

      } while (!thread.doneEvent.wait(500 * 1000));
    }

    auto elapsed = timer.getLiveMicroseconds();
    if (timedOut || ((elapsed / 1000) > test->getTimeout())) {
      timedOut = true;
      if (progressMode) {
        fout << "                                                                               \r";
      }
      if (UnitTestManager::getManager().getUseANSIColors()) {
        fout << setForeground(ANSIEscapeSequence::MAGENTA);
      }
      fout << "  TEST: TIMED OUT / " << test->getName() << ENDL;
      if (UnitTestManager::getManager().getUseANSIColors()) {
        fout << normal();
      }
      // thread.terminate(); // terminate on exit of loop instead
    }

    if (!timedOut) {
      thread.join();
    }
    
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
    
    if (!timedOut && thread.success) {
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

Reference<UnitTest> UnitTestManager::getTest(const String& id) const noexcept
{
  // use lookup if this gets used later
  for (auto test : tests) {
    if (test->getId() == id) {
      return test;
    }
  }
  return nullptr;
}

Array<Reference<UnitTest> > UnitTestManager::getTestByPattern(const String& pattern) const noexcept
{
  Array<Reference<UnitTest> > result;
  for (auto test : tests) {
    if (Parser::doesMatchPattern(pattern, test->getId())) {
      result.append(test);
    }
  }
  return result;
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
    // TAG: we need to pull out from linked list because entire storage is no longer available
#if 0
    auto previous = node->previous;
    auto next = node->next;
    previous->next = next;
    next->previous = previous;
#endif
    node->entry = nullptr; // prevent access of unloaded memory
  }
}

UnitTestManager::DependencyEntry::DependencyEntry(DependencyNode* _node)
  : node(_node)
{
  if (node) {
    auto previous = nodes;
    node->next = previous;
    nodes = node;
  }
}

UnitTestManager::DependencyEntry::~DependencyEntry()
{
  if (node) {
    // TAG: we need to pull out from linked list because entire storage if no longer available
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

String UnitTestManager::getJUnit(const String& uuid, const String& name) const
{
  // https://www.ibm.com/support/knowledgecenter/en/SSQ2R2_14.2.0/com.ibm.rsar.analysis.codereview.cobol.doc/topics/cac_useresults_junit.html
#if 0
  <?xml version="1.0" encoding="UTF-8" ?>
  <!-- from https://svn.jenkins-ci.org/trunk/hudson/dtkit/dtkit-format/dtkit-junit-model/src/main/resources/com/thalesgroup/dtkit/junit/model/xsd/junit-4.xsd -->
  <xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">

      <xs:element name="failure">
          <xs:complexType mixed="true">
              <xs:attribute name="type" type="xs:string" use="optional"/>
              <xs:attribute name="message" type="xs:string" use="optional"/>
          </xs:complexType>
      </xs:element>

      <xs:element name="error">
          <xs:complexType mixed="true">
              <xs:attribute name="type" type="xs:string" use="optional"/>
              <xs:attribute name="message" type="xs:string" use="optional"/>
          </xs:complexType>
      </xs:element>

      <xs:element name="properties">
          <xs:complexType>
              <xs:sequence>
                  <xs:element ref="property" maxOccurs="unbounded"/>
              </xs:sequence>
          </xs:complexType>
      </xs:element>

      <xs:element name="property">
          <xs:complexType>
              <xs:attribute name="name" type="xs:string" use="required"/>
              <xs:attribute name="value" type="xs:string" use="required"/>
          </xs:complexType>
      </xs:element>

      <xs:element name="skipped" type="xs:string"/>
      <xs:element name="system-err" type="xs:string"/>
      <xs:element name="system-out" type="xs:string"/>

      <xs:element name="testcase">
          <xs:complexType>
              <xs:sequence>
                  <xs:element ref="skipped" minOccurs="0" maxOccurs="1"/>
                  <xs:element ref="error" minOccurs="0" maxOccurs="unbounded"/>
                  <xs:element ref="failure" minOccurs="0" maxOccurs="unbounded"/>
                  <xs:element ref="system-out" minOccurs="0" maxOccurs="unbounded"/>
                  <xs:element ref="system-err" minOccurs="0" maxOccurs="unbounded"/>
              </xs:sequence>
              <xs:attribute name="name" type="xs:string" use="required"/>
              <xs:attribute name="assertions" type="xs:string" use="optional"/>
              <xs:attribute name="time" type="xs:string" use="optional"/>
              <xs:attribute name="classname" type="xs:string" use="optional"/>
              <xs:attribute name="status" type="xs:string" use="optional"/>
          </xs:complexType>
      </xs:element>

      <xs:element name="testsuite">
          <xs:complexType>
              <xs:sequence>
                  <xs:element ref="properties" minOccurs="0" maxOccurs="1"/>
                  <xs:element ref="testcase" minOccurs="0" maxOccurs="unbounded"/>
                  <xs:element ref="system-out" minOccurs="0" maxOccurs="1"/>
                  <xs:element ref="system-err" minOccurs="0" maxOccurs="1"/>
              </xs:sequence>
              <xs:attribute name="name" type="xs:string" use="required"/>
              <xs:attribute name="tests" type="xs:string" use="required"/>
              <xs:attribute name="failures" type="xs:string" use="optional"/>
              <xs:attribute name="errors" type="xs:string" use="optional"/>
              <xs:attribute name="time" type="xs:string" use="optional"/>
              <xs:attribute name="disabled" type="xs:string" use="optional"/>
              <xs:attribute name="skipped" type="xs:string" use="optional"/>
              <xs:attribute name="timestamp" type="xs:string" use="optional"/>
              <xs:attribute name="hostname" type="xs:string" use="optional"/>
              <xs:attribute name="id" type="xs:string" use="optional"/>
              <xs:attribute name="package" type="xs:string" use="optional"/>
          </xs:complexType>
      </xs:element>

      <xs:element name="testsuites">
          <xs:complexType>
              <xs:sequence>
                  <xs:element ref="testsuite" minOccurs="0" maxOccurs="unbounded"/>
              </xs:sequence>
              <xs:attribute name="name" type="xs:string" use="optional"/>
              <xs:attribute name="time" type="xs:string" use="optional"/>
              <xs:attribute name="tests" type="xs:string" use="optional"/>
              <xs:attribute name="failures" type="xs:string" use="optional"/>
              <xs:attribute name="disabled" type="xs:string" use="optional"/>
              <xs:attribute name="errors" type="xs:string" use="optional"/>
          </xs:complexType>
      </xs:element>

  </xs:schema>
#endif
  
  String xml;
  xml += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

  uint64 totalTime = 0;
  for (auto test : tests) {
    if (auto run = test->getLastRun()) {
      uint64 time = run->endTime - run->startTime;
      totalTime += time;
    }
  }
  
  // https://docs.microsoft.com/en-us/azure/devops/pipelines/tasks/test/publish-test-results?view=azure-devops&tabs=yaml
  // See results mapping

  // xmlns:base="https://dev.azure.com/renefonseca/base"
  // attributes: disabled, errors
  xml += Format::subst(
    "<testsuites id=\"%1\" name=\"%2\">\n", // tests = \"%3\" failures=\"%4\" time=\"%5\"
    Guid::createGuidAsString(), name ? name : "BASE" /*, 1, failed ? 1 : 0, totalTime/1000000.0*/
  );
  
  String hostname = "localhost";
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  hostname = "windows";
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
  hostname = "gnulinux";
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  hostname = "macos";
#endif

#if (_COM_AZURE_DEV__BASE__DEBUG)
  const String configuration("Debug");
#else
  const String configuration("Release");
#endif

  // timestamp - ISO 8601 format (2014-01-21T16:17:18) without timezone!
  String timestamp = Date::getNow().format(String("%FT%T"), false);
  // other attributes: errors, disabled, skipped, package
  xml += Format::subst(
    "<testsuite id=\"%1\" name=\"%2\" hostname=\"%3\" tests=\"%4\" failures=\"%5\" time=\"%6\" timestamp=\"%7\">\n",
    uuid ? uuid : Guid::createGuidAsString(), name ? name : "BASE", hostname, tests.getSize(), failed, totalTime/1000000.0, timestamp
  );

  xml += "<properties>\n";
  xml += Format::subst("<property name=\"%1\" value=\"%2\"/>\n", "configuration", configuration);
  xml += "</properties>\n";

  for (auto test : tests) {
    xml += test->getJUnit();
  }
  xml += "</testsuite>\n";
  xml += "</testsuites>\n";
  return xml;
}

UnitTest::AddDependency::AddDependency(UnitTest* test, const char* id)
{
  if (test) {
    test->addDependency(id);
  }
}

AnyValue UnitTest::getState(const String& id)
{
  const AnyValue* value = UnitTestManager::getManager().states.find(id);
  if (!value) {
    return AnyValue();
  }
  onPrint(Format::subst("Getting state '%1' = '%2'", id, *value));
  return *value;
}

void UnitTest::setState(const String& id, const AnyValue& value)
{
  onPrint(Format::subst("Setting state '%1' = '%2'", id, value));
  UnitTestManager::getManager().states.add(name, value);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
