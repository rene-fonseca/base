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
#include <base/string/FormatOutputStream.h>
#include <base/dl/LinkerManager.h>
#include "MyLinkerModule.h"

using namespace com::azure::dev::base;

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
      MyLinkerModule* module = dynamic_cast<MyLinkerModule*>(temp); // requires typeinfo
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
         << "Copyright (C) 2001-2019 by Rene Moeller Fonseca" << EOL
         << ENDL;

    Array<String> arguments = getArguments();

    // TAG: need method to translate into filename of module
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    String module = "MyLinkerModule.dll"; // the module to load
#else
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
    String module = "MyLinkerModule.dylib"; // the module to load
#else
    String module = "MyLinkerModule.so"; // the module to load
#endif
#endif

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
    } catch (Exception&) {
      setExitCode(EXIT_CODE_ERROR);
    }
  }
};

STUB(DynamicLinkerApplication);
