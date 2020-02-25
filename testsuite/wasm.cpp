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
  
  bool colorize = false;
  Command command = COMMAND_RUN;
  String path;
  String id;
  String pattern = "*";
  Array<AnyValue> callArguments;
  bool time = false;
  bool getJSON = false;
  bool wasi = false;
  bool fake = false;
public:
  
  WASMApplication()
    : Application("wasm")
  {
    colorize = fout.isANSITerminal();
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
         << indent(2) << "--fake      Register fake imports" << EOL
         << indent(2) << "--json      Get WASM module info as JSON" << EOL
         << indent(2) << "--wasi      Use WASI mode" << EOL
         << indent(2) << "--dump      Dumps info about the WASM module" << EOL
         << indent(2) << "--version   Show the version" << EOL
         << indent(2) << "--run       Run WASM module" << EOL
         << ENDL;
  }

  /** Dump all exported symbols in module. */
  void dump(const String& path, const String& pattern)
  {
    if (!FileSystem::fileExists(path)) {
      setExitCode(1);
      ferr << "Error: Failed to load module." << ENDL;
      return;
    }
    WebAssembly wasm;
    if (!wasm.load(path)) {
      setExitCode(1);
      ferr << "Error: Failed to load and compile module." << ENDL;
      return;
    }

    // TAG: get imports and exports as JSON
    // TAG: global info, imports, exports
    // TAG: how can we bind 2 modules together - imports from 1 use exports from the other
    
    fout << "IMPORTS:" << EOL;
    for (const auto& s : wasm.getImports()) {
      if (Parser::doesMatchPattern(pattern, s.name)) {
        fout << "  " << "[" << s.index << "] ";
        switch (s.externType) {
        case WebAssembly::EXTERN_FUNCTION:
          fout << "FUNCTION " << WebAssembly::toString(s, colorize) << ENDL;
          break;
        case WebAssembly::EXTERN_GLOBAL:
          fout << "GLOBAL " << s.name << ENDL;
          break;
        case WebAssembly::EXTERN_TABLE:
          fout << "TABLE " << s.name << ENDL;
          break;
        case WebAssembly::EXTERN_MEMORY:
          fout << "MEMORY " << s.name << ENDL;
          break;
        default:
          fout << "?" << " " << s.name << ENDL;
        }
      }
    }
    
    try {
      if (!wasm.makeInstance(true || fake)) {
        setExitCode(1);
        ferr << "Error: Failed to create instance." << ENDL;
        return;
      }
    } catch (Exception& e) {
      setExitCode(1);
      ferr << "Error: Failed to create instance '%1'." % Subst(e.getMessage()) << ENDL;
      return;
    }
    
    fout << "EXPORTS:" << EOL;
    for (const auto& s : wasm.getExports()) {
      if (Parser::doesMatchPattern(pattern, s.name)) {
        fout << "  " << "[" << s.index << "] ";
        switch (s.externType) {
        case WebAssembly::EXTERN_FUNCTION:
          fout << "FUNCTION " << WebAssembly::toString(s, colorize) << ENDL;
          break;
        case WebAssembly::EXTERN_GLOBAL:
          fout << "GLOBAL " << s.name << ENDL;
          break;
        case WebAssembly::EXTERN_TABLE:
          fout << "TABLE " << s.name << ENDL;
          break;
        case WebAssembly::EXTERN_MEMORY:
          if (s.name) {
            fout << "MEMORY " << s.name << " SIZE=" << s.memorySize/1024 << " kB" << ENDL;
          } else {
            fout << "MEMORY " << "SIZE=" << s.memorySize/1024 << " kB" << ENDL;
          }
          break;
        default:
          fout << "?" << " " << s.name << ENDL;
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
        
    // TAG: add option to control memory - max limit

    Timer timer;
    if (!wasm.load(path)) {
      ferr << "Error: Failed to load and compile module." << ENDL;
      setExitCode(1);
      return;
    }
    if (time) {
      fout << "Load time: " << timer << ENDL;
    }
    timer.start();
    if (wasi) {
      if (!wasm.makeWASIInstance(nullptr, nullptr, nullptr)) {
        ferr << "Error: Failed to create instance." << ENDL;
        setExitCode(1);
        return;
      }
    } else {
      wasm.registerFunction(hello);
      wasm.registerFunction(throwit);
      // TAG: show imports registered
      if (!wasm.makeInstance(fake)) {
        ferr << "Error: Failed to create instance." << ENDL;
        setExitCode(1);
        return;
      }
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
    bool gotFunction = false;
    const auto arguments = getArguments();
    Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();
    while (enu.hasNext()) {
      const String argument = enu.next();
      if (argument == "--help") {
        command = COMMAND_HELP;
      } else if (argument == "--fake") {
        fake = true;
      } else if (argument == "--dump") {
        getJSON = false;
        command = COMMAND_DUMP;
      } else if (argument == "--run") {
        command = COMMAND_RUN;
      } else if (argument == "--json") {
        getJSON = true;
        command = COMMAND_DUMP;
      } else if (argument == "--version") {
        command = COMMAND_VERSION;
      } else if (argument == "--time") {
        time = true;
      } else {
        if (!path) {
          path = argument;
        } else if (!gotFunction) {
          gotFunction = true;
          id = argument;
          pattern = argument;
        } else {
          Reference<ObjectModel::Value> v;
          try {
            v = JSON::parse(argument);
          } catch (...) {
            fout << "Error: Invalid argument." << ENDL;
            setExitCode(1);
            return false;
          }
          if (auto _v = v.cast<ObjectModel::Integer>()) {
            callArguments.append(_v->value);
          } else if (auto _v = v.cast<ObjectModel::Boolean>()) {
            callArguments.append(_v->value);
          } else if (auto _v = v.cast<ObjectModel::Float>()) {
            callArguments.append(_v->value);
          } else if (auto _v = v.cast<ObjectModel::String>()) {
            callArguments.append(_v->value);
          } else {
            fout << "Error: Invalid argument." << ENDL;
            setExitCode(1);
            return false;
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
