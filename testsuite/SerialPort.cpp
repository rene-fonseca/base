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
#include <base/communication/SerialPort.h>

using namespace base;

class SerialPortApplication : public Application {
public:
  
  SerialPortApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw() :
    Application(MESSAGE("SerialPort"), numberOfArguments, arguments, environment) {
  }
  
  void onTermination() throw() {
    // override default application termination
  }

  void main() throw() {
    fout << Application::getFormalName() << MESSAGE(" version 1.0") << EOL
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

int main(int argc, const char* argv[], const char* env[]) {
  SerialPortApplication application(argc, argv, env);
  try {
    application.main();
  } catch(Exception& e) {
    return Application::getApplication()->exceptionHandler(e);
  } catch(...) {
    return Application::getApplication()->exceptionHandler();
  }
  return Application::getApplication()->getExitCode();
}
