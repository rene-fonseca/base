/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/objectmodel/JSON.h>
#include <base/string/Format.h>
#include <base/string/ANSIEscapeSequence.h>
#include <base/UnitTest.h>
#include <base/io/FileOutputStream.h>
#include <base/io/FileDescriptor.h>
#include <base/Profiler.h>
#include <algorithm>

using namespace com::azure::dev::base;

class TestApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;

  enum Command {
    COMMAND_HELP,
    COMMAND_VERSION,
    COMMAND_LIST,
    COMMAND_RUN
  };

  enum Verbosity {
    SILENT,
    FINAL,
    COMPACT,
    NORMAL,
    VERBOSE,
    VERY_VERBOSE
  };

  Command command = COMMAND_RUN;
  Verbosity verbosity = NORMAL;
  bool useANSIColors = false;
  bool useUrlAsSource = false;
  bool randomize = false;
  bool stopOnFailure = false;
  bool runWithExternals = false;
  bool showStackTrace = false;
  bool progressMode = false;
  bool reportJSON = false;
  bool reportJUnit = false;
  String junitPath;
  String testsuiteUUID;
  String pattern = "*";
  bool runDevel = false;
  bool traceExceptions = false;
  bool profile = false;
  Map<String, String> symbols;
public:

  TestApplication()
    : Application("unittesting")
  {
    useANSIColors = FileDescriptor::getStandardOutput().isANSITerminal();
    
    // TAG: also enable by default for macOS terminal and other terminals
    const Map<String, String>& environment = getEnvironment();
    if (const auto found = environment.find(MESSAGE("AGENT_BUILDDIRECTORY"))) { // detect Azure pipeline agent
      useUrlAsSource = true;
    }
  }
  
  bool parseArguments()
  {
    Array<String> arguments = getArguments();
    Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();
    while (enu.hasNext()) {
      String argument = *enu.next();
      if (argument == "--help") {
        command = COMMAND_HELP;
      } else if (argument == "--version") {
        command = COMMAND_VERSION;
      } else if (argument == "--list") {
        command = COMMAND_LIST;
      } else if (argument == "--run") {
        command = COMMAND_RUN;
      } else if (argument == "--silent") {
        verbosity = SILENT;
      } else if (argument == "--final") {
        verbosity = FINAL;
      } else if (argument == "--compact") {
        verbosity = COMPACT;
      } else if (argument == "--verbose") {
        verbosity = VERBOSE;
      } else if (argument == "--veryVerbose") {
        verbosity = VERY_VERBOSE;
      } else if (argument == "--color") {
        useANSIColors = true;
      } else if (argument == "--nocolor") {
        useANSIColors = false;
      } else if (argument == "--urlAsSource") {
        useUrlAsSource = true;
      } else if (argument == "--json") {
        reportJSON = true;
      } else if (argument == "--devel") {
        runDevel = true;
      } else if (argument == "--exceptions") {
        traceExceptions = true;
      } else if (argument == "--profile") {
        profile = true;
      } else if (argument == "--define") {
        if (!enu.hasNext()) {
          ferr << "Error: Expected variable." << ENDL;
          return false;
        }
        String argument = *enu.next();
        MemoryDiff index = argument.indexOf("=");
        if (index >= 0) {
          symbols.add(argument.substring(0, index), argument.substring(index + 1));
        } else {
          symbols.add(argument);
        }
      } else if (argument == "--junit") {
        reportJUnit = true;
        if (!enu.hasNext()) {
          ferr << "Error: Expected JUnit path." << ENDL;
          return false;
        }
        junitPath = *enu.next();
      } else if (argument == "--uuid") {
        if (!enu.hasNext()) {
          ferr << "Error: Expected testsuite UUID." << ENDL;
          return false;
        }
        testsuiteUUID = *enu.next();
      } else if (argument == "--randomize") {
        randomize = true;
      } else if (argument == "--stopOnFailure") {
        stopOnFailure = true;
      } else if (argument == "--externals") {
        runWithExternals = true;
      } else if (argument == "--stackTrace") {
        showStackTrace = true;
      } else if (argument == "--progress") {
        progressMode = true;
      } else {
        if (argument.startsWith("-")) {
          ferr << "Error: Unsupported argument." << ENDL;
          return false;
        }
        pattern = argument;
      }
    }
    return true;
  }
  
  void version()
  {
    fout << getFormalName() << " version "
      << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
      << "The Base Framework (Test Suite)" << EOL
      << ENDL;
  }

  void help()
  {
    version();
    fout << "Usage: " << getFormalName() << " [options] [pattern]" << EOL
      << EOL
      << "--help           This message" << EOL
      << "--version        Dump the version" << EOL
      << EOL
      << "--list           List all tests." << EOL
      << "--run            Run tests." << EOL
      << "--quiet          Quiet mode." << EOL
      << "--final          Final mode." << EOL
      << "--compact        Compact mode." << EOL
      << "--verbose        Verbose mode." << EOL
      << "--progress       Progress mode." << EOL
      << "--color          Use ANSI colors." << EOL
      << "--nocolor        Don't use ANSI colors." << EOL
      << "--json           Output results as JSON." << EOL
      << "--junit          Output results as JUnit. JUnit path must follow." << EOL
      << "--uuid           UUID for testsuite." << EOL
      << "--randomize      Run tests in random order." << EOL
      << "--stopOnFailure  Stop on first failure." << EOL
      << "--externals      Run tests with external dependencies." << EOL
      << "--stackTrace     Show stack trace on assert." << EOL
      << ENDL;
  }

  /** Returns the id for presentation. */
  String presentId(const String& id)
  {
    if (useANSIColors) {
      StringOutputStream stream;
      auto i = id.lastIndexOf('/');
      if (i >= 0) {
        stream << bold() << id.substring(0, i + 1)
               << setForeground(ANSIEscapeSequence::BLUE) << id.substring(i + 1) << normal() << FLUSH;
      } else {
        stream << bold() << setForeground(ANSIEscapeSequence::BLUE) << id << normal() << FLUSH;
      }
      return stream.getString();
    }
    return id;
  }

  void main()
  {
    if (!parseArguments()) {
      return;
    }
    
    if (command == COMMAND_VERSION) {
      version();
    } else if (command == COMMAND_HELP) {
      help();
    } else if (command == COMMAND_LIST) {
      auto& manager = UnitTestManager::getManager();
      manager.loadTests();
      auto tests = manager.getTests();
      tests.sort(UnitTestManager::SortTests());

      for (auto test : tests) {
        const String id = test->getId();
        if (!Parser::doesMatchPattern(pattern, id)) {
          continue;
        }

        fout << "TEST " << presentId(id);
        if (verbosity <= COMPACT) {
          fout << EOL << FLUSH;
          continue;
        }

        fout << ":" << EOL;
        if (String description = test->getDescription()) {
          fout << "  DESCRIPTION=" << description << EOL;
        }
        if (String source = test->getSource()) {
          fout << "  SOURCE=" << source << EOL;
        }
        if (String owner = test->getOwner()) {
          fout << "  OWNER=" << owner << EOL;
        }
        fout << "  PRIORITY=" << test->getPriority() << EOL;
        if (test->hasExternalDependency()) {
          fout << "  EXTERNAL=" << "YES" << EOL;
        }
        if (test->isInDevelopment()) {
          fout << "  DEVELOPMENT=" << test->isInDevelopment() << EOL;
        }
        
        // TAG: can we use PrimitiveArray template to automatically check of overrun - avoid copy of data
        // template<typename TYPE> class FixedArray {};
        static const char* IMPACTS[] = { "PRIVACY", "SECURITY", "CRITICAL", "IMPORTANT", "NORMAL", "LOW", "IGNORE" };
        BASSERT(static_cast<MemorySize>(test->getImpact()) < getArraySize(IMPACTS));
        fout << "  IMPACT=" << IMPACTS[test->getImpact()] << EOL;
        if (test->getAllowConcurrentRun()) {
          fout << "  CONCURRENT=" << test->getAllowConcurrentRun() << EOL;
        }
        fout << "  TIMEOUT=" << test->getTimeout() << " ms" << EOL;
        if (test->getRepeats() > 1) {
          fout << "  REPEATS=" << test->getRepeats() << EOL;
        }
        if (test->getLimitIO() > 0) {
          fout << "  Limit IO=" << test->getLimitIO() << " kb" << EOL;
        }
        if (test->getLimitProcessingTime() > 0) {
          fout << "  Processing time=" << test->getLimitProcessingTime() << " ms" << EOL;
        }
        if (test->getLimitMemory() > 0) {
          fout << "  Limit memory=" << test->getLimitMemory() << " kb" << EOL;
        }

        for (auto dependency : test->getDependencies()) {
          auto tests = manager.getTestByPattern(dependency);
          if (tests) {
            for (auto dependent : tests) {
              if (dependent->getId() == test->getId()) {
                continue; // ignore self
              }
              fout << "  DEPENDENCY=" << presentId(dependent->getId()) << EOL;
            }
          } else {
            fout << "  DEPENDENCY=" << presentId(dependency) << " (NO MATCH FOUND)" << EOL;
          }
        }
        fout << FLUSH;
      }
    } else if (command == COMMAND_RUN) {
      StackFrame::setUseStandardOut(true); // avoid stream sync issues
      Debug::setUseBreakpoint(false); // make sure we do not call breakpoint // TAG: make option?

      auto& manager = UnitTestManager::getManager();

      switch (verbosity) {
      case SILENT:
        manager.setVerbosity(UnitTestManager::SILENT);
        break;
      case FINAL:
        manager.setVerbosity(UnitTestManager::FINAL);
        break;
      case COMPACT:
        manager.setVerbosity(UnitTestManager::COMPACT);
        break;
      case NORMAL:
        manager.setVerbosity(UnitTestManager::NORMAL);
        break;
      case VERBOSE:
        manager.setVerbosity(UnitTestManager::VERBOSE);
        break;
      case VERY_VERBOSE:
        manager.setVerbosity(UnitTestManager::VERBOSE);
        Exception::setDumpExceptions(true);
        break;
      }
      
      manager.setUseANSIColors(useANSIColors);
      manager.setUseUrlAsSource(useUrlAsSource);
      manager.setRandomize(randomize);
      manager.setStopOnFailure(stopOnFailure);
      manager.setRunWithExternalDependencies(runWithExternals);
      manager.setShowStackTrace(showStackTrace);
      manager.setProgressMode(progressMode);
      manager.setTraceExceptions(traceExceptions);
      for (auto symbol : symbols) {
        manager.addSymbol(symbol.getKey(), symbol.getValue());
      }

      if (reportJSON) {
        manager.setUseJSON(true);
      }

      if (profile) {
        Profiler::open("profiler.json");
        Profiler::setUseStackFrames(true);
        Profiler::setCaptureIO(64);
        Profiler::start();
      }

      Profiler::CaptureIO captureIO(64); // enable for main thread

      if (!manager.runTests(pattern, runDevel)) {
        if (profile) {
          Profiler::close();
        }
        setExitCode(1);
      }
      
      if (profile) {
        Profiler::close();
      }

      if (reportJUnit) {
        String xml = manager.getJUnit(testsuiteUUID);
        if (junitPath == "stdout") {
          fout << xml << ENDL;
        } else if (junitPath == "stderr") {
          ferr << xml << ENDL;
        } else {
          try {
            FileOutputStream fos(junitPath);
            fos.write(reinterpret_cast<const uint8*>(xml.getElements()), xml.getLength());
          } catch (...) {
            ferr << "Error: Failed to write JUnit file." << ENDL;
            setExitCode(1);
          }
        }
      }

      // TAG: generate list of tests giving different results
      // TAG: add support for loading baseline for comparison
    } else {
      help();
    }
  }
};

APPLICATION_STUB(TestApplication);
