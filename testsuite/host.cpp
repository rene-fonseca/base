/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/net/InetAddress.h>
#include <base/string/FormatOutputStream.h>

using namespace dk::sdu::mip::base;

class HostApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
  
  enum Command {
    COMMAND_HELP,
    COMMAND_VERSION,
    COMMAND_HOST
  };
  
  Array<String> hosts;
public:
  
  HostApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application("host", numberOfArguments, arguments, environment) {
  }
  
  void version() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "http://www.mip.sdu.dk/~fonseca/base" << EOL
         << "Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>" << EOL
         << ENDL;
  }

  void help() throw() {
    version();
    fout << "Usage: " << getFormalName()
         << " [--help] [--version] host(s)" << ENDL;
  }
  
  void host() throw() {
    Array<String>::ReadEnumerator enu = hosts.getReadEnumerator();
    
    while (enu.hasNext()) {
      String host = *enu.next();
      try {
        
        InetAddress address;
        try {
          address = InetAddress::getAddressByName(host);
        } catch (HostNotFound& e) {
          ferr << "Error: " << "Unable to resolve host" << ENDL;
          setExitCode(EXIT_CODE_ERROR);
          return;
        }
        
        String name = host;
        try {
          name = address.getHostName(true); // fully qualified
        } catch (...) {
        }
        
        List<InetAddress> addresses = InetAddress::getAddressesByName(host);
        List<InetAddress>::ReadEnumerator enu = addresses.getReadEnumerator();
        
        fout << name << EOL;
        while (enu.hasNext()) {
          fout << indent(2) << *enu.next() << EOL;
        }
        fout << EOL << FLUSH;
      } catch (...) {
        setExitCode(EXIT_CODE_ERROR);
        return;
      }
    }
  }
  
  void main() throw() {
    Command command = COMMAND_HOST;
    
    const Array<String> arguments = getArguments();
    Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();
    while (enu.hasNext()) {
      String argument = *enu.next();
      if (argument == "--help") {
        command = COMMAND_HELP;
      } else if (argument == "--version") {
        command = COMMAND_VERSION;
      } else {
        hosts.append(argument);
      }
    }
    
    switch (command) {
    case COMMAND_VERSION:
      version();
      break;
    case COMMAND_HELP:
      help();
      break;
    case COMMAND_HOST:
      if (hosts.getSize() == 0) {
        ferr << "Error: " << "Host(s) must be specified" << ENDL;
        setExitCode(EXIT_CODE_ERROR);
        return;
      }
      host();
      break;
    }
  }

  ~HostApplication() throw() {
  }    
};

STUB(HostApplication);
