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
#include <base/webassembly/WebAssembly.h>
#include <base/filesystem/FileSystem.h>
#include <base/string/Format.h>

using namespace com::azure::dev::base;

class WASMApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;  
  
  enum Command {
    COMMAND_VERSION,
    COMMAND_HELP,
    COMMAND_RUN
  };
public:
  
  WASMApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[])
    : Application("wasm", numberOfArguments, arguments, environment)
  {
  }

  void version()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << ENDL;

    WebAssembly wasm;
    fout << "Engine: " << wasm.getEngine() << ENDL;
  }
  
  void help()
  {
    version();
    fout << "Usage: " << getFormalName() << " [OPTIONS] [ID] [ARGS]" << EOL
         << EOL
         << "Options:" << EOL
         << indent(2) << "--help      This message" << EOL
         << indent(2) << "--version   Dump the version" << EOL
         << indent(2) << "--run       Run WASM module" << EOL
         << ENDL;
  }

  void run(const String& path, const String& id, bool time)
  {
    if (!FileSystem::fileExists(path)) {
      ferr << "Error: Failed to load module." << ENDL;
      setExitCode(1);
      return;
    }
    WebAssembly wasm;
    Timer timer;
    if (!wasm.load(path)) {
      ferr << "Error: Failed to load and compile module." << ENDL;
      setExitCode(1);
      return;
    }
    if (time) {
      fout << "Compile time: " << timer << ENDL;
    }
    timer.start();
    try {
      wasm.call(id, Array<AnyValue>());
    } catch (...) {
      ferr << Format::subst("Error: Failed to call function '%1'.", id) << ENDL;
      setExitCode(1);
    }
    if (time) {
      fout << "Execution time: " << timer << ENDL;
    }
  }
  
  void main()
  {
    Command command = COMMAND_RUN;
    String path;
    String id;
    Array<String> callArguments;
    bool time = false;
    
    const auto arguments = getArguments();
    Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();
    while (enu.hasNext()) {
      const String argument = *enu.next();
      if (argument == "--help") {
        command = COMMAND_HELP;
      } else if (argument == "--version") {
        command = COMMAND_VERSION;
      } else if (argument == "--time") {
        time = true;
      } else {
        if (!path) {
          path = argument;
        } else if (!id) {
          id = argument;
        } else {
          callArguments.append(argument); // TAG: unescape string type
        }
      }
    }
    
    switch (command) {
    case COMMAND_VERSION:
      version();
      break;
    case COMMAND_HELP:
      help();
      break;
    default:
      if (!id) {
        id = "main";
      }
      run(path, id, time);
      break;
    }
  }
};

APPLICATION_STUB(WASMApplication);
