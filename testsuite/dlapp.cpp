/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/string/FormatOutputStream.h>
#include <base/dl/LinkerManager.h>
#include "MyLinkerModule.h"

using namespace base;

class DynamicLinkerApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  DynamicLinkerApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw()
    : Application(MESSAGE("dlapp"), numberOfArguments, arguments, environment) {
  }

  void load(const String& modulePath) {
    LinkerManager manager;
    fout << MESSAGE("Loading module...") << ENDL;
    LinkerModule* temp = manager.load(modulePath);
    if (temp) {
      MyLinkerModule* module = dynamic_cast<MyLinkerModule*>(temp);
      if (module) {
        fout << MESSAGE("Module has been loaded") << EOL
             << MESSAGE("  identity: ") << module->getIdentity() << EOL
             << MESSAGE("  name: ") << module->getName() << EOL
             << MESSAGE("  version: ") << module->getVersion() << ENDL;
        module->myFunction();
      } else {
        ferr << MESSAGE("Module is not a MyLinkerModule") << ENDL;
      }
    } else {
      ferr << MESSAGE("Error during module loading") << ENDL;
    }
    manager.unload(temp);
  }
  
  void main() throw() {
    fout << getFormalName() << MESSAGE(" version ") << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;

    Array<String> arguments = getArguments();

    // TAG: need method to translate into filename of module
    String module = "/home/fonseca/src/base/base/test/MyLinkerModule.so.0.1"; // the module to load
    
    switch (arguments.getSize()) {
    case 0: // use default
      break;
    case 1:
      module = arguments[0]; // the module path
      break;
    default:
      fout << MESSAGE("Usage: ") << getFormalName() << MESSAGE(" [module]") << ENDL;
      return;
    }

    try {
      load(module);
    } catch (Exception& e) {
      setExitCode(EXIT_CODE_ERROR);
    }
  }
};

STUB(DynamicLinkerApplication);
