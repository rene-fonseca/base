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
#include <base/xml/DOMImplementation.h>

using namespace com::azure::dev::base;

class ValidateApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  ValidateApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) noexcept
    : Application("validate",
      numberOfArguments,
      arguments,
      environment) {
  }

  void main()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << ENDL;
    
    Array<String> arguments = getArguments();
  
    if (arguments.getSize() != 1) {
      fout << getFormalName() << " uri" << ENDL;
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
      fout << "Error: " << e << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }

    if (true) {
      fout << "Document is valid" << ENDL;
    } else {
      fout << "Document is well-formed" << ENDL;
    }
  }
};

APPLICATION_STUB(ValidateApplication);
