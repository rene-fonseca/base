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

using namespace com::azure::dev::base;

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

  void level3(unsigned int value) throw() {
    volatile long double floating = 1.0;
    if (floating > 100.0) {
      return;
    }
    
    volatile double zero = 0.0;
    double doubleResult = 1.0/zero;
    
    unsigned int* invalid = nullptr;
    unsigned int temp = *invalid; // read
    *invalid = 0; // write
  }
  
  void level2(unsigned int value)  {
    if (value) {
      level2(--value);
    } else {
      level3(3);
    }
  }
  
  void level1(unsigned int value) throw() {
    if (value) {
      level1(--value);
    } else {
      level2(3);
    }
  }
  
  void main() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "Copyright (C) 2003-2019 by Rene Moeller Fonseca" << EOL
         << ENDL;

    volatile unsigned int value = 3;
    level1(value);
  }

  ~DebugApplication() throw() {
  }
};

STUB(DebugApplication);
