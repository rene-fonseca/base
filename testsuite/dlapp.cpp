/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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

void testLinker(const String& modulePath) {
  LinkerManager manager;
  fout << MESSAGE("Loading module...") << ENDL;
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
      ferr << MESSAGE("Module is not a MyLinkerModule") << ENDL;
    }
  } else {
    ferr << MESSAGE("Error during module loading") << ENDL;
  }
  manager.unload(temp);
}

int main(int argc, const char* argv[], const char* env[]) {
  Application app("dlapp", argc, argv, env);

  Array<String> arguments = Application::getApplication()->getArguments();

  String module = "/home/fonseca/src/base/base/test/MyLinkerModule.so.0.1"; // the module to load

  switch (arguments.getSize()) {
  case 0: // use default
    break;
  case 1:
    module = arguments[0]; // the module path
    break;
  default:
    fout << "usage: " << Application::getApplication()->getName() << " [module]" << ENDL;
    return Application::EXIT_CODE_NORMAL; // stop
  }

  try {
    fout << MESSAGE("Testing DynamicLinker...") << ENDL;
    testLinker(module);
  } catch(Exception& e) {
    return Application::getApplication()->exceptionHandler(e);
  } catch(...) {
    return Application::getApplication()->exceptionHandler();
  }
  return Application::getApplication()->getExitCode();
}
