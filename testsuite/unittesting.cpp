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
    COMPACT,
    NORMAL,
    VERBOSE
  };

  Command command = COMMAND_RUN;
  Verbosity verbosity = NORMAL;
  bool useANSIColor = false;
  bool randomize = false;
  bool stopOnFailure = false;
  bool progressMode = false;
  bool reportJSON = false;
  bool reportJUnit = false;
  String junitPath;
  String testsuiteUUID;
  String pattern = "*";
public:

  TestApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[])
    : Application("unittesting", numberOfArguments, arguments, environment)
  {
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
      } else if (argument == "--compact") {
        verbosity = COMPACT;
      } else if (argument == "--verbose") {
        verbosity = VERBOSE;
      } else if (argument == "--color") {
        useANSIColor = true;
      } else if (argument == "--json") {
        reportJSON = true;
      } else if (argument == "--junit") {
        reportJUnit = true;
        if (!enu.hasNext()) {
          ferr << "Expected JUnit path." << ENDL;
          return false;
        }
        junitPath = *enu.next();
      } else if (argument == "--uuid") {
        if (!enu.hasNext()) {
          ferr << "Expected testsuite UUID." << ENDL;
          return false;
        }
        testsuiteUUID = *enu.next();
      } else if (argument == "--randomize") {
        randomize = true;
      } else if (argument == "--stopOnFailure") {
        stopOnFailure = true;
      } else if (argument == "--progress") {
        progressMode = true;
      } else {
        if (argument.startsWith("-")) {
          ferr << "Unsupported argument." << ENDL;
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
      << "Copyright (C) 2019 by Rene Moeller Fonseca" << EOL
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
      << "--list           List all tests" << EOL
      << "--run            Run tests" << EOL
      << "--compact        Compact mode" << EOL
      << "--verbose        Verbose mode" << EOL
      << "--progress       Progress mode" << EOL
      << "--color          Use ANSI colors" << EOL
      << "--json           Output results as JSON" << EOL
      << "--junit          Output results as JUnit. JUnit path must follow." << EOL
      << "--uuid           UUID for testsuite." << EOL
      << "--randomize      Run tests in random order" << EOL
      << "--stopOnFailure  Stop on first failure" << EOL
      << ENDL;
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
      std::sort(tests.begin(), tests.end(), UnitTestManager::SortTests());

      for (auto test : tests) {
        const String id = test->getId();
        if (!Parser::doesMatchPattern(pattern, id)) {
          continue;
        }

        if (useANSIColor) {
          if (String project = test->getProject()) {
            fout << "TEST " << bold() << project << '/' << setForeground(ANSIEscapeSequence::BLUE) << test->getName() << normal();
          } else {
            fout << "TEST " << bold() << id << normal();
          }
        } else {
          fout << "TEST " << id;
        }
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
        static const char* IMPACTS[] = { "PRIVACY", "SECURITY", "CRITICAL", "IMPORTANT", "NORMAL", "LOW", "IGNORE" };
        ASSERT(test->getImpact() < getArraySize(IMPACTS));
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
        fout << FLUSH;
      }
    } else if (command == COMMAND_RUN) {
      auto& manager = UnitTestManager::getManager();

      switch (verbosity) {
      case COMPACT:
        manager.setVerbosity(UnitTestManager::COMPACT);
        break;
      case NORMAL:
        manager.setVerbosity(UnitTestManager::NORMAL);
        break;
      case VERBOSE:
        manager.setVerbosity(UnitTestManager::VERBOSE);
        break;
      }
      
      manager.setUseANSIColors(useANSIColor);
      manager.setRandomize(randomize);
      manager.setStopOnFailure(stopOnFailure);
      manager.setProgressMode(progressMode);

      if (reportJSON) {
        manager.setUseJSON(true);
      }
      
      if (!manager.runTests(pattern)) {
        setExitCode(1);
      }

      if (reportJUnit) {
        String xml = manager.getJUnit(testsuiteUUID);
        try {
          FileOutputStream fos(junitPath);
          fos.write(reinterpret_cast<const uint8*>(xml.getElements()), xml.getLength());
        } catch (...) {
          ferr << "Error: Failed to write JUnit file." << ENDL;
          setExitCode(1);
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
