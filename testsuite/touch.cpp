/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/string/FormatOutputStream.h>
#include <base/io/File.h>

using namespace dk::sdu::mip::base;

class TouchApplication : public Application {
public:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
  
  TouchApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application("touch", numberOfArguments, arguments, environment) {
  }

  void main() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "http://www.mip.sdu.dk/~fonseca/base" << EOL
         << "Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>" << EOL
         << ENDL;

    const Array<String> arguments = getArguments();

    if (arguments.getSize() == 0) {
      fout << "Usage: " << getFormalName() << " [OPTIONS] [FILE]" << ENDL;
      return;
    }
    
    Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();

    String module = *enu.next();
    // -a --access
    // -m --modification
    // -c --no-create
    // -r --reference=FILE
    // -t --time=format?
    // --help
    // --version
    
    DateTime time;

    if no creation then => options
            
    file.setCreationTime();
    file.setModificationTime;
    
    
    try {
    } catch (Exception& e) {
      exceptionHandler(e);
    }
  }
};

STUB(TouchApplication);