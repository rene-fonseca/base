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
#include <base/Timer.h>
#include <base/UnsignedInteger.h>
#include <base/mathematics/Math.h>
#include <base/net/ClientSocket.h>
#include <base/net/InetInterface.h>
#include <base/net/InetService.h>
#include <base/net/InetEndPoint.h>
#include <base/concurrency/Thread.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/StringOutputStream.h>

using namespace base;

class InetApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
  
  enum Command {
    HELP,
    INET
  };
public:
  
  InetApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw()
    : Application(MESSAGE("inet"), numberOfArguments, arguments, environment) {
  }
  
  void onTermination() throw() {
  }
  
  void inet() throw() {
    try {
      {
        HashTable<String, unsigned int> names = InetInterface::getInterfaceNames();
        HashTable<String, unsigned int>::ReadEnumerator enu = names.getReadEnumerator();
        fout << MESSAGE("Interfaces:") << ENDL;
        while (enu.hasNext()) {
          const HashTable<String, unsigned int>::HashTableAssociation* node = enu.next();
          fout << indent(2) << *node->getKey() << ' ' << *node->getValue() << EOL;
        }
      }
      
      List<InetInterface> interfaces = InetInterface::getInterfaces();
      List<InetInterface>::ReadEnumerator enu = interfaces.getReadEnumerator();
      while (enu.hasNext()) {
        InetInterface interface = *enu.next();
        fout << interface.getName() << ' ' << interface.getIndex() << EOL
             << indent(2) << MESSAGE("flags: ") << interface.getFlags() << EOL
             << indent(2) << MESSAGE("address: ") << interface.getAddress() << EOL
             << indent(2) << MESSAGE("broadcast: ") << interface.getBroadcast() << EOL
             << indent(2) << MESSAGE("destination: ") << interface.getDestination() << EOL
             << indent(2) << MESSAGE("metric: ") << interface.getMetric() << EOL
             << indent(2) << MESSAGE("ethernet: ") << interface.getEthernetAddress() << EOL
             << ENDL;
//     UP
//     LOOPBACK
//     POINT_TO_POINT
//     BROADCAST
//     MULTICAST
      }
    } catch (...) {
      setExitCode(EXIT_CODE_ERROR);
    }
  }
  
  void help() throw() {
    fout << getFormalName() << MESSAGE(" version ")
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;
    fout << getFormalName() << MESSAGE(" [--help]") << ENDL;
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
