/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/string/FormatOutputStream.h>

using namespace dk::sdu::mip::base;

class DebugApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:
  
  DebugApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application("debug", numberOfArguments, arguments, environment) {
  }

  void level3() throw() {
    volatile long double floating = 1.0;
    if (floating > 100.0) {
      return;
    }
    unsigned int* invalid = 0;
    unsigned int temp = *invalid; // read
    *invalid = 0; // write
  }
  
  void level2() throw() {
    level3();
  }
  
  void level1() throw() {
    level2();
  }
  
  void main() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "http://www.mip.sdu.dk/~fonseca/base" << EOL
         << "Copyright (C) 2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>" << EOL
         << ENDL;
    
    level1();
  }

  ~DebugApplication() throw() {
  }
};

STUB(DebugApplication);
