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
#include <base/string/Parser.h>
#include <base/LongInteger.h>
#include <base/objectmodel/JSON.h>
#include <base/Timer.h>

using namespace com::azure::dev::base;

class WASMApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;  
  
  enum Command {
    COMMAND_VERSION,
    COMMAND_HELP,
    COMMAND_DUMP,
    COMMAND_RUN
  };
  
  Command command = COMMAND_RUN;
  String path;
  String id;
  String pattern = "*";
  Array<AnyValue> callArguments;
  bool time = false;
public:
  
  WASMApplication()
    : Application("wasm")
  {
  }

  static void hello()
  {
    fout << "Hello, World!" << ENDL;
  }

  static void throwit()
  {
    _throw Exception("This is an exception.");
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
    fout << "Usage: " << getFormalName() << " [OPTIONS] PATH [ID] [ARGS]" << EOL
         << EOL
         << "Options:" << EOL
         << indent(2) << "--help      This message" << EOL
         << indent(2) << "--dump      Dumps info about the WASM module" << EOL
         << indent(2) << "--version   Show the version" << EOL
         << indent(2) << "--run       Run WASM module" << EOL
         << ENDL;
  }

  /** Dump all exported symbols in module. */
  void dump(const String& path, const String& pattern)
  {
    if (!FileSystem::fileExists(path)) {
      ferr << "Error: Failed to load module." << ENDL;
      setExitCode(1);
      return;
    }
    WebAssembly wasm;
    if (!wasm.load(path)) {
      ferr << "Error: Failed to load and compile module." << ENDL;
      setExitCode(1);
      return;
    }
    for (const auto& s : wasm.getExports()) {
      if (Parser::doesMatchPattern(pattern, s.name)) {
        if (s.type == WebAssembly::TYPE_FUNCTION) {
          fout << "[";
          bool first = true;
          for (auto a : s.results) {
            if (first) {
              first = false;
            } else {
              fout << ", ";
            }
            fout << WebAssembly::toString(a);
          }
          fout << "]";
          fout << s.name << "(";
          first = true;
          for (auto a : s.arguments) {
            if (first) {
              fout << ", ";
            }
            first = false;
            fout << WebAssembly::toString(a);
          }
          fout << ")" << ENDL;
        } else {
          for (auto a : s.results) {
            fout << WebAssembly::toString(a) << " " << s.name << ENDL;
          }
        }
      }
    }
  }

  /** Call symbol in module. */
  void run(const String& path, const String& id, const Array<AnyValue>& arguments, bool time)
  {
    if (!FileSystem::fileExists(path)) {
      ferr << "Error: Failed to load module." << ENDL;
      setExitCode(1);
      return;
    }
    WebAssembly wasm;
    
    wasm.registerFunction(hello);
    wasm.registerFunction(throwit);

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
      auto result = wasm.call(id, arguments);
      fout << result << ENDL;
    } catch (...) {
      ferr << Format::subst("Error: Failed to call function '%1'.", id) << ENDL;
      setExitCode(1);
    }
    if (time) {
      fout << "Execution time: " << timer << ENDL;
    }
  }
  
  bool parseArguments()
  {
    const auto arguments = getArguments();
    Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();
    while (enu.hasNext()) {
      const String argument = enu.next();
      if (argument == "--help") {
        command = COMMAND_HELP;
      } else if (argument == "--dump") {
        command = COMMAND_DUMP;
      } else if (argument == "--version") {
        command = COMMAND_VERSION;
      } else if (argument == "--time") {
        time = true;
      } else {
        if (!path) {
          path = argument;
        } else if (!id) {
          id = argument;
          pattern = argument;
        } else {
          // TAG: parse as JSON value?
          try {
            callArguments.append(static_cast<int64>(LongInteger::parse(argument)));
          } catch (...) {
            double value = 0;
            if (Posix::toDouble(argument, value)) {
              callArguments.append(value);
              continue;
            }
            try {
              callArguments.append(JSON::parseString(argument));
            } catch (...) {
              fout << "Error: Invalid argument." << ENDL;
              setExitCode(1);
              return false;
            }
          }
        }
      }
    }
    return true;
  }
  
  void main()
  {
    if (!parseArguments()) {
      return;
    }
    
    switch (command) {
    case COMMAND_VERSION:
      version();
      break;
    case COMMAND_HELP:
      help();
      break;
    case COMMAND_DUMP:
      dump(path, pattern);
      break;
    default:
      if (!id) {
        id = ""; // call first exported
      }
      run(path, id, callArguments, time);
    }
  }
};

APPLICATION_STUB(WASMApplication);
