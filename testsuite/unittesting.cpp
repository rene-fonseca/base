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
  bool reportJSON = false;
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
      } else if (argument == "--randomize") {
        randomize = true;
        // TAG: reverse order support
      } else {
        if (argument.startsWith("-")) {
          ferr << "Unsupported argument." << ENDL;
          return false;
        }
        // TAG: handle priority
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
      << "--help      This message" << EOL
      << "--version   Dump the version" << EOL
      << EOL
      << "--list      List all tests" << EOL
      << "--run       Run tests" << EOL
      << ENDL;
  }

  void main()
  {
    // TAG: get pattern and priority from args
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
      const auto& tests = manager.getTests();
// TAG: add ansi color - can we turn on/off colors globally
      for (auto test : tests) {
        if (useANSIColor) {
          fout << "TEST " << bold() << test->getName() << normal() << ":" << EOL;
        } else {
          fout << "TEST " << test->getName() << ":" << EOL;
        }
        if (!test->getDescription().isEmpty()) {
          fout << "  DESCRIPTION=" << test->getDescription() << EOL;
        }
        if (!test->getSource().isEmpty()) {
          fout << "  SOURCE=" << test->getSource() << EOL;
        }
        fout << "  PRIORITY=" << test->getPriority() << EOL;
        fout << "  IMPACT=" << test->getImpact() << EOL;
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

      if (reportJSON) {
        manager.setUseJSON(true);
      }
      
      if (!manager.runTests(pattern)) {
        setExitCode(1);
      }

      // TAG: add randomize order support
      // TAG: generate list of tests giving different results
      // TAG: add support for loading baseline for comparison
      // TAG: allow new run via http
    } else {
      help();
    }
  }
};

APPLICATION_STUB(TestApplication);
