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
#include <base/io/File.h>
#include <base/xml/Document.h>
#include <base/collection/Stack.h>
#include <base/collection/Array.h>

using namespace dk::sdu::mip::base;

class DOMApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  DOMApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application(MESSAGE("dom"),
      numberOfArguments,
      arguments,
      environment) {
  }

  void main() throw() {
    fout << getFormalName() << MESSAGE(" version ")
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;
    
    Array<String> arguments = getArguments();
  
    if (arguments.getSize() != 2) {
      fout << getFormalName() << MESSAGE(" source destination") << ENDL;
      return; // stop
    }
    
    const String filename = arguments[0];
    
    try {
      fout << MESSAGE("Reading document...") << ENDL;
      Document document(filename);

      ProcessingInstruction pi = document.createProcessingInstruction("", "");
      Node node = pi;
      pi = node;
      
      fout << MESSAGE("Saving result document...") << ENDL;
      document.save(arguments[1]);
    } catch(DOMException& e) {
      ferr << MESSAGE("Internal error: ") << e << ENDL;
      setExitCode(EXIT_CODE_ERROR);
    }
  }
};

STUB(DOMApplication);
