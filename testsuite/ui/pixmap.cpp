/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/string/FormatOutputStream.h>
#include <base/ui/Pixmap.h>

using namespace com::azure::dev::base;

class PixmapApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  PixmapApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application("Pixmap", numberOfArguments, arguments, environment) {
  }

  class MyPixmap : public Pixmap {
  public:

    MyPixmap(const Dimension& dimension, unsigned int flags) throw(UserInterfaceException)
      : Pixmap(dimension, flags) {
    }    
  };
  
  void main() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "Copyright (C) 2002-2019 by Rene Moeller Fonseca" << EOL
         << ENDL;

    MyPixmap myPixmap(Dimension(256, 256), 0);
    myPixmap.clear();
    myPixmap.line(Position(10, 10), Position(100, 100));
  }
};

STUB(PixmapApplication);
