/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/string/FormatOutputStream.h>
#include <base/dl/LinkerManager.h>
#include "MyLinkerModule.h"

using namespace dk::sdu::mip::base;

class DynamicLinkerApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  DynamicLinkerApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application("dlapp", numberOfArguments, arguments, environment) {
  }

  void load(const String& modulePath) {
    LinkerManager manager;
    fout << "Loading module..." << ENDL;
    LinkerModule* temp = manager.load(modulePath);
    if (temp) {
      MyLinkerModule* module = dynamic_cast<MyLinkerModule*>(temp);
      if (module) {
        fout << "Module has been loaded" << EOL
             << "  identity: " << module->getIdentity() << EOL
             << "  name: " << module->getName() << EOL
             << "  version: " << module->getVersion() << ENDL;
        module->myFunction();
      } else {
        ferr << "Module is not a MyLinkerModule" << ENDL;
      }
    } else {
      ferr << "Error during module loading" << ENDL;
    }
    manager.unload(temp);
  }
  
  void main() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "http://www.mip.sdu.dk/~fonseca/base" << EOL
         << "Copyright (C) 2001-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>" << EOL
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
      fout << "Usage: " << getFormalName() << " [module]" << ENDL;
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
