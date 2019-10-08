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
#include <base/Timer.h>
#include <base/UnsignedInteger.h>
#include <base/mathematics/Math.h>
#include <base/net/StreamSocket.h>
#include <base/net/InetInterface.h>
#include <base/net/InetService.h>
#include <base/net/InetEndPoint.h>
#include <base/concurrency/Thread.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/StringOutputStream.h>

using namespace dk::sdu::mip::base;

class InetApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
  
  enum Command {
    HELP,
    INET
  };
public:
  
  InetApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application("inet", numberOfArguments, arguments, environment) {
  }
  
  void onTermination() throw() {
  }
  
  void inet() throw() {
    try {
      if (false) {
        HashTable<String, unsigned int> names =
          InetInterface::getInterfaceNames();
        HashTable<String, unsigned int>::ReadEnumerator enu =
          names.getReadEnumerator();
        fout << "Interfaces:" << ENDL;
        while (enu.hasNext()) {
          const HashTable<String, unsigned int>::HashTableAssociation* node = enu.next();
          fout << indent(2) << node->getKey() << ' ' << node->getValue() << EOL;
        }
      }
      
      List<InetInterface> interfaces = InetInterface::getInterfaces();
      List<InetInterface>::ReadEnumerator enu = interfaces.getReadEnumerator();
      while (enu.hasNext()) {
        InetInterface interface = *enu.next();

        unsigned int flags = interface.getFlags();
        String temp;
        if (flags & InetInterface::UP) {
          temp += MESSAGE(" UP");
        }
        if (flags & InetInterface::LOOPBACK) {
          temp += MESSAGE(" LOOPBACK");
        }
        if (flags & InetInterface::POINT_TO_POINT) {
          temp += MESSAGE(" POINT_TO_POINT");
        }
        if (flags & InetInterface::BROADCAST) {
          temp += MESSAGE(" BROADCAST");
        }
        if (flags & InetInterface::MULTICAST) {
          temp += MESSAGE(" MULTICAST");
        }
        if (flags & InetInterface::DYNAMIC) {
          temp += MESSAGE(" DYNAMIC");
        }
        
        fout << interface.getName() << ' ' << interface.getIndex() << EOL
             << indent(2) << "flags:" << temp << EOL
             << indent(2) << "address: " << interface.getAddress() << EOL
             << indent(2) << "netmask: " << interface.getNetmask() << EOL
             << indent(2) << "broadcast: " << interface.getBroadcast() << EOL
             << indent(2) << "destination: " << interface.getDestination() << EOL
             << indent(2) << "metric: " << interface.getMetric() << EOL
             << indent(2) << "ethernet (EUI-64): " << interface.getEthernetAddress() << EOL
             << ENDL;
      }
    } catch (...) {
      setExitCode(EXIT_CODE_ERROR);
    }
  }
  
  void help() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "http://www.mip.sdu.dk/~fonseca/base" << EOL
         << "Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>" << EOL
         << ENDL;
    fout << getFormalName() << " [--help]" << ENDL;
  }
  
  void main() throw() {
    Command command = INET;
    
    const Array<String> arguments = getArguments();
    if (arguments.getSize() > 0) {
      Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();
      while (enu.hasNext()) {
        String argument = *enu.next();
        if (argument == "--help") {
          command = HELP;
        }
      }
    }
    
    switch (command) {
    case HELP:
      help();
      break;
    case INET:
      inet();
      break;
    }
  }
};

STUB(InetApplication);
