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

  Command command = COMMAND_RUN;
  String pattern = "*";
public:

  TestApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[])
    : Application("unittesting", numberOfArguments, arguments, environment) {
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

      for (auto test : tests) {
        fout << Format::subst("TEST %1:", test->getName()) << EOL;
        if (!test->getDescription().isEmpty()) {
          fout << "  DESCRIPTION=" << test->getDescription() << EOL;
        }
        if (!test->getSource().isEmpty()) {
          fout << "  SOURCE=" << test->getSource() << EOL;
        }
        fout << "  PRIORITY=" << test->getPriority() << EOL;
        if (test->getAllowConcurrentRun()) {
          fout << "  CONCURRENT=" << test->getAllowConcurrentRun() << EOL;
        }
        fout << "  TIMEOUT=" << test->getTimeout() << EOL;
        if (test->getRepeats() > 1) {
          fout << "  REPEATS=" << test->getRepeats() << EOL;
        }
        fout << FLUSH;
        // TAG: show all limits
      }
    } else if (command == COMMAND_RUN) {
      auto& manager = UnitTestManager::getManager();
      manager.runTests(pattern);

      // TAG: add support for loading baseline for comparison
      // TAG: dump to JSON
    } else {
      help();
    }
  }
};

APPLICATION_STUB(TestApplication);
