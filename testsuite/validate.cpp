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
#include <base/xml/DOMImplementation.h>

using namespace dk::sdu::mip::base;

class ValidateApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  ValidateApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application(MESSAGE("validate"),
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
  
    if (arguments.getSize() != 1) {
      fout << getFormalName() << MESSAGE(" uri") << ENDL;
      return; // stop
    }
    
    // TAG: need option --wellformed
    
    const String sourceURI = arguments[0];
    
    DOMImplementation dom;
    DOMImplementation::Mode mode =
      true ? DOMImplementation::VALIDATING : DOMImplementation::PARSING;
    
    try {
      Document document = dom.createFromURI(sourceURI, mode);
    } catch (DOMException& e) {
      fout << MESSAGE("Error: ") << e << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }

    if (true) {
      fout << MESSAGE("Document is valid") << ENDL;
    } else {
      fout << MESSAGE("Document is well-formed") << ENDL;
    }
  }
};

STUB(ValidateApplication);
