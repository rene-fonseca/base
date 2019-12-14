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
#include <base/Timer.h>
#include <base/UnsignedInteger.h>
#include <base/math/Math.h>
#include <base/net/StreamSocket.h>
#include <base/net/InetInterface.h>
#include <base/net/InetService.h>
#include <base/net/InetEndPoint.h>
#include <base/concurrency/Thread.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/StringOutputStream.h>

using namespace com::azure::dev::base;

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
    const char* environment[])
    : Application("inet", numberOfArguments, arguments, environment)
  {
  }
  
  void onTermination() noexcept
  {
  }
  
  void inet()
  {
    try {
      if (false) {
        auto names = InetInterface::getInterfaceNames();
        // bubbleSort(names.begin(), names.end());

#if 0
        Map<String, unsigned int> names;
        auto names2 = InetInterface::getInterfaceNames();
        auto enu = names2.getReadEnumerator();
        while (enu.hasNext()) {
          names.add(*enu.next());
        }
#endif

        fout << "Interfaces:" << ENDL;
        for (auto node : names) {
          fout << indent(2) << node.getKey() << " index=" << node.getValue() << EOL;
          try {
            auto addr = InetInterface::getAddress(node.getValue());
            fout << indent(4) << "inet " << addr << ENDL;
          } catch (...) {
          }
        }
      }
      
      List<InetInterface> interfaces = InetInterface::getInterfaces();
      List<InetInterface>::ReadEnumerator enu = interfaces.getReadEnumerator();
      while (enu.hasNext()) {
        InetInterface interface = *enu.next();
        try {

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
        
        fout << interface.getName() << " index=" << interface.getIndex() << EOL
             << indent(2) << "flags:" << temp << EOL
             << indent(2) << "address: " << interface.getAddress() << EOL
             << indent(2) << "netmask: " << interface.getNetmask() << EOL
             << indent(2) << "broadcast: " << interface.getBroadcast() << EOL
             << indent(2) << "destination: " << interface.getDestination() << EOL
             << indent(2) << "metric: " << interface.getMetric() << EOL
             << indent(2) << "ethernet (EUI-64): " << interface.getEthernetAddress() << EOL
             << ENDL;
        } catch (...) {
        }
      }
    } catch (...) {
      setExitCode(EXIT_CODE_ERROR);
    }
  }
  
  void help()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << ENDL;
    fout << getFormalName() << " [--help]" << ENDL;
  }
  
  void main()
  {
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

APPLICATION_STUB(InetApplication);
