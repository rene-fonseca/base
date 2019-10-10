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
#include <base/filesystem/FileSystem.h>

using namespace com::azure::dev::base;

class LinkApplication : public Application {
private:
  
  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 1;  
public:
  
  LinkApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application("ln", numberOfArguments, arguments, environment) {
  }
  
  void main() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "Copyright (C) 2002-2019 by Rene Moeller Fonseca" << EOL
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
      fout << getFormalName() << " target [name]" << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }
    
    if (!FileSystem::supportsLinks()) {
      ferr << "Error: " << "Symbolic links not supported." << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }
    
    if (!nameSpecified) {
      name = FileSystem::getComponent(target, FileSystem::FILENAME);
    }
    
    if (FileSystem::entryExists(name)) {
      ferr << "Error: " << "Name already exists." << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }
    
    try {
      FileSystem::makeLink(target, name);
    } catch (FileSystemException& e) {
      ferr << "Error: " << "Unable to create link: " << e.getMessage() << ENDL;
      setExitCode(EXIT_CODE_ERROR);
    }
  }
  
  ~LinkApplication() throw() {
  }
};

STUB(LinkApplication);
