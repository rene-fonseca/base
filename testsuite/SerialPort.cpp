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
#include <base/communication/SerialPort.h>

using namespace com::azure::dev::base;

class SerialPortApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:
  
  SerialPortApplication()
    : Application("SerialPort")
  {
  }
  
  void onTermination() noexcept
  {
    // override default application termination
  }

  void main()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << ENDL;

    const List<String> ports = SerialPort::getPorts();
    List<String>::ReadEnumerator enu(ports.getReadEnumerator());

    fout << "Available serial communications ports:" << EOL;
    while (enu.hasNext()) {
      fout << enu.next() << EOL;
    }
    fout << FLUSH;
  }
  
};

APPLICATION_STUB(SerialPortApplication);
