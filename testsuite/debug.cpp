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
  
  DebugApplication()
    : Application("debug")
  {
  }

  void level3(unsigned int value)
  {
    volatile long double floating = 1.0;
    if (floating > 100.0) {
      return;
    }
    
    volatile double zero = 0.0;
    double doubleResult = 1.0/zero;
    if (doubleResult) {
    }
    
    unsigned int* invalid = reinterpret_cast<unsigned int*>(Runtime::getNullPointer());
    unsigned int temp = *invalid; // read
    if (temp) {
    }
    *invalid = 0; // write
  }
  
  void level2(unsigned int value)
  {
    if (value) {
      level2(--value);
    } else {
      level3(3);
    }
  }
  
  void level1(unsigned int value) {
    if (value) {
      level1(--value);
    } else {
      level2(3);
    }
  }
  
  void main()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << ENDL;

    volatile unsigned int value = 3;
    level1(value);
  }

  ~DebugApplication() {
  }
};

APPLICATION_STUB(DebugApplication);
