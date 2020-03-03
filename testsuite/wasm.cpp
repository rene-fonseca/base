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
#include <base/UnsignedInteger.h>
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
    COMMAND_FORMAT,
    COMMAND_RUN,
    COMMAND_CONVERT
  };
  
  bool colorize = false;
  Command command = COMMAND_RUN;
  String path;
  String destPath;
  String id;
  String pattern = "*";
  Array<AnyValue> callArguments;
  Map<String, String> environment;
  Map<String, String> folders;
  bool time = false;
  bool getJSON = false;
  bool wasi = false;
  bool fake = false;
  bool useLog = false;
  MemorySize maximumMemory = 0; // in Mb
  MemorySize maximumStack = 0; // in Mb
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
    fout << "Usage: " << getFormalName() << " [OPTIONS] PATH [DEST] [ID] [ARGS]" << EOL
         << EOL
         << "Options:" << EOL
         << indent(2) << "--help      This message" << EOL
         << indent(2) << "--fake      Register fake imports" << EOL
         << indent(2) << "--json      Get WASM module info as JSON" << EOL
         << indent(2) << "--wasi      Use WASI mode" << EOL
         << indent(2) << "--dump      Dumps info about the WASM module" << EOL
         << indent(2) << "--log       Enabled log" << EOL
         << indent(2) << "--version   Show the version" << EOL
         << indent(2) << "--run       Run WASM module" << EOL
         << indent(2) << "--convert   Convert WAT to WASM" << EOL
         << indent(2) << "--env       Add environment variable" << EOL
         << indent(2) << "--mount     Mount folder at path" << EOL
         << ENDL;
  }

  R<ObjectModel::Array> getSymbolTable(ObjectModel& o, const Array<WebAssembly::Symbol>& symbols)
  {
    auto result = o.createArray();
    for (const auto& s : symbols) {
      auto i = o.createObject();
      if (s.moduleName) {
        i->setValue("module", s.moduleName);
      }
      if (s.name) {
        i->setValue("name", s.name);
      }
      switch (s.externType) {
      case WebAssembly::EXTERN_FUNCTION:
        {
          i->setValue(o.createString("type"), "FUNCTION");
          auto t = o.createObject();
          auto a = o.createArray();
          for (const auto type : s.functionType.arguments) {
            a->append(o.createString(WebAssembly::toString(type)));
          }
          t->setValue(o.createString("arguments"), a);
          a = o.createArray();
          for (const auto type : s.functionType.results) {
            a->append(o.createString(WebAssembly::toString(type)));
          }
          t->setValue(o.createString("results"), a);
          i->setValue(o.createString("functionType"), t);
        }
        break;
      case WebAssembly::EXTERN_GLOBAL:
        i->setValue("type", "GLOBAL");
        break;
      case WebAssembly::EXTERN_TABLE:
        i->setValue("type", "TABLE");
        break;
      case WebAssembly::EXTERN_MEMORY:
        i->setValue("type", "MEMORY");
        i->setValue(o.createString("memorySize"), o.createInteger(s.memorySize));
        break;
      }
      result->append(i);
    }
    return result;
  }
  
  /** Convert WAT to WASM. */
  void convertWAT2WASM(const String& src, const String& dest)
  {
    if (!src) {
      ferr << "Error: No WAT module provided." << ENDL;
      setExitCode(1);
      return;
    }
    if (!dest) {
      ferr << "Error: No WASM path provided." << ENDL;
      setExitCode(1);
      return;
    }
    if (!FileSystem::fileExists(src)) {
      setExitCode(1);
      ferr << "Error: Module '%1' does not exist." % Subst(src) << ENDL;
      return;
    }
    String wat;
    try {
      wat = File::readFile(src);
    } catch (...) {
      setExitCode(1);
      ferr << "Error: Failed read WAT." << ENDL;
      return;
    }
    String wasm;
    try {
      wasm = WebAssembly::convertWATToWASM(wat);
    } catch (...) {
      setExitCode(1);
      ferr << "Error: Failed to convert WAT to WASM." << ENDL;
      return;
    }
    try {
      File::createFile(dest, wasm);
    } catch (...) {
      setExitCode(1);
      ferr << "Error: Failed write WASM." << ENDL;
      return;
    }
  }
  
  /** Dump format for file. */
  void dumpFormat(const String& path)
  {
    if (!path) {
      ferr << "Error: No module provided." << ENDL;
      setExitCode(1);
      return;
    }
    if (!FileSystem::fileExists(path)) {
      setExitCode(1);
      ferr << "Error: Module '%1' does not exist." % Subst(path) << ENDL;
      return;
    }
    
    String bytes;
    try {
      bytes = File::readFile(path, File::ENCODING_RAW);
    } catch (...) {
      setExitCode(1);
      ferr << "Error: Failed to load module '%1'." % Subst(path) << ENDL;
      return;
    }
    
    switch (WebAssembly::getFormat(bytes)) {
    case WebAssembly::FORMAT_WASM:
      fout << "application/wasm" << ENDL;
      break;
    case WebAssembly::FORMAT_WAT:
      fout << "text/webassembly" << ENDL;
      break;
    default:
      fout << "application/octet-stream" << ENDL;
      setExitCode(1);
      break;
    }
  }
  
  /** Dump all exported symbols in module. */
  void dump(const String& path, const String& pattern)
  {
    if (!path) {
      ferr << "Error: No module provided." << ENDL;
      setExitCode(1);
      return;
    }

    if (!FileSystem::fileExists(path)) {
      setExitCode(1);
      ferr << "Error: Failed to load module '%1'." % Subst(path) << ENDL;
      return;
    }

    String bytes;
    try {
      bytes = File::readFile(path, File::ENCODING_RAW);
    } catch (...) {
      setExitCode(1);
      ferr << "Error: Failed to load module '%1'." % Subst(path) << ENDL;
      return;
    }

    WebAssembly wasm;
    if (!wasm.loadAny(bytes)) {
      setExitCode(1);
      ferr << "Error: Failed to load and compile module." << ENDL;
      return;
    }

    const auto imports = wasm.getImports();

    // TAG: how can we bind 2 modules together - imports from 1 use exports from the other

    ObjectModel o;
    auto root = o.createObject();

    if (getJSON) {
      // root->setValue("digest", "sha256:...");
      root->setValue("type", (wasm.getWASMABI() == WebAssembly::WASM64) ? "WASM64" : "WASM32");
      root->setValue(o.createString("imports"), getSymbolTable(o, imports));
    } else {
      fout << "IMPORTS:" << EOL;
      for (const auto& s : imports) {
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
    }
    
    try {
      if (wasi) {
        if (!wasm.makeWASIInstance(nullptr, nullptr, nullptr)) {
          setExitCode(1);
          ferr << "Error: Failed to create instance." << ENDL;
          return;
        }
      } else {
        if (!wasm.makeInstance(true || fake)) {
          setExitCode(1);
          ferr << "Error: Failed to create instance." << ENDL;
          return;
        }
      }
    } catch (Exception& e) {
      setExitCode(1);
      ferr << "Error: Failed to create instance '%1'." % Subst(e.getMessage()) << ENDL;
      return;
    }
    
    const auto exports = wasm.getExports();

    if (getJSON) {
      root->setValue(o.createString("exports"), getSymbolTable(o, exports));
      if (fout.isANSITerminal()) {
        fout << JSON::getJSON(root, ObjectModel::DEFAULT_FORMATTING | ObjectModel::FLAG_COLOR) << ENDL;
      } else {
        fout << JSON::getJSONNoFormatting(root) << ENDL;
      }
    } else {
      fout << "EXPORTS:" << EOL;
      for (const auto& s : exports) {
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
  }

  /** Call symbol in module. */
  void run(const String& path, const String& id, const Array<AnyValue>& arguments, bool time)
  {
    if (!path) {
      ferr << "Error: No module provided." << ENDL;
      setExitCode(1);
      return;
    }
    
    if (!FileSystem::fileExists(path)) {
      ferr << "Error: Module '%1' does not exist." % Subst(path) << ENDL;
      setExitCode(1);
      return;
    }

    String bytes;
    try {
      bytes = File::readFile(path, File::ENCODING_RAW);
    } catch (...) {
      setExitCode(1);
      ferr << "Error: Failed to load module '%1'." % Subst(path) << ENDL;
      return;
    }

    WebAssembly wasm;
    wasm.setUseLog(useLog);
    if (maximumMemory > 0) {
      wasm.setMaximumMemoryUsage(maximumMemory * 1024 * 1024); // TAG: handle overflow
    }
    // TAG: add support for arguments wasm.setArguments(arguments);
    wasm.setEnvironment(environment);
    wasm.setMountedFolders(folders);
    
    // TAG: set stack limit
    
    Timer timer;
    if (!wasm.loadAny(bytes)) {
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
      // wasm.registerFunction(hello);
      // wasm.registerFunction(throwit);
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

  /** Returns name and value for string with = separator. */
  static StringPair getNameValue(const String& text, char separator = '=')
  {
    auto index = text.indexOf(separator);
    if (index >= 0) {
      return StringPair(text.substring(index), text.substring(index + 1));
    } else {
      return StringPair(text);
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
      } else if (argument == "--wasi") {
        wasi = true;
      } else if (argument == "--log") {
        useLog = true;
      } else if (argument == "--dump") {
        getJSON = false;
        command = COMMAND_DUMP;
      } else if (argument == "--format") {
        command = COMMAND_FORMAT;
      } else if (argument == "--run") {
        command = COMMAND_RUN;
      } else if (argument == "--convert") {
        command = COMMAND_CONVERT;
      } else if (argument == "--json") {
        getJSON = true;
        command = COMMAND_DUMP;
      } else if (argument == "--env") {
        if (!enu.hasNext()) {
          ferr << "Error: " << "Expecting environment variable NAME=VALUE." << ENDL;
          return false;
        }
        String nv = enu.next();
        StringPair p = getNameValue(nv);
        environment.add(p.getFirst(), p.getSecond());
      } else if (argument == "--mount") {
        if (!enu.hasNext()) {
          ferr << "Error: " << "Expecting FOLDER=MOUNT_POINT." << ENDL;
          return false;
        }
        String nv = enu.next();
        StringPair p = getNameValue(nv);
        folders.add(p.getFirst(), p.getSecond());
      } else if (argument == "--version") {
        command = COMMAND_VERSION;
      } else if (argument == "--time") {
        time = true;
      } else if (argument == "--memory LIMIT") {
        if (!enu.hasNext()) {
          ferr << "Error: " << "Expecting memory limit." << ENDL;
          return false;
        }
        try {
          maximumMemory = UnsignedInteger::parse(enu.next());
        } catch (...) {
          ferr << "Error: " << "Invalid memory limit." << ENDL;
          return false;
        }
      //} else if (argument == "--stack LIMIT") {
      //  maximumStack = 0;
      } else {
        if (!path) {
          path = argument;
        } else if ((command == COMMAND_CONVERT) && !destPath) {
          destPath = argument;
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
    case COMMAND_FORMAT:
      dumpFormat(path);
      break;
    case COMMAND_CONVERT:
      convertWAT2WASM(path, destPath);
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
