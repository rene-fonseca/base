/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/string/FormatOutputStream.h>
#include <base/communication/SerialPort.h>

using namespace dk::sdu::mip::base;

class SerialPortApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:
  
  SerialPortApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw()
    : Application(MESSAGE("SerialPort"), numberOfArguments, arguments, environment) {
  }
  
  void onTermination() throw() {
    // override default application termination
  }

  void main() throw() {
    fout << getFormalName() << MESSAGE(" version ") << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;

    const List<String> ports = SerialPort::getPorts();
    List<String>::ReadEnumerator enu(ports.getReadEnumerator());

    fout << MESSAGE("Available serial communications ports:") << EOL;
    while (enu.hasNext()) {
      fout << *enu.next() << EOL;
    }
    fout << FLUSH;
  }
  
};

STUB(SerialPortApplication);
