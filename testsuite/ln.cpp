/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/string/FormatOutputStream.h>
#include <base/filesystem/FileSystem.h>

using namespace base;

class LinkApplication : public Application {
private:
  
  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 1;  
public:
  
  LinkApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw()
    : Application(MESSAGE("ln"), numberOfArguments, arguments, environment) {
  }
  
  void main() throw() {
    fout << getFormalName() << MESSAGE(" version ") << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;
    
    String target;
    String name;
    bool nameSpecified = false;
    
    Array<String> arguments = getArguments();
    switch (arguments.getSize()) {
    case 1:
      target = arguments[0]; // the target path
      break;
    case 2:
      target = arguments[0]; // the target path
      name = arguments[1]; // the link name
      nameSpecified = true;
      break;
    default:
      fout << MESSAGE("ln target [name]") << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }
    
    if (!FileSystem::supportsLinks()) {
      ferr << MESSAGE("Error: ") << MESSAGE("Symbolic links not supported.") << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }
    
    if (!nameSpecified) {
      name = FileSystem::getComponent(target, FileSystem::FILENAME);
    }
    
    if (FileSystem::entryExists(name)) {
      ferr << MESSAGE("Error: ") << MESSAGE("Name already exists.") << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }    
    
    try {
      FileSystem::makeLink(target, name);
    } catch (Exception& e) {
      ferr << MESSAGE("Error: ") << e.getMessage() << ENDL;
      setExitCode(EXIT_CODE_ERROR);
    }
  }
  
  ~LinkApplication() throw() {
  }
};

STUB(LinkApplication);
