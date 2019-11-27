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
#include <base/Version.h>
#include <base/rmi/Orb.h>
#include <base/rmi/LisaEncoding.h>
#include <base/rmi/TCPIPConnectionFactory.h>
#include <base/rmi/TCPIPProtocol.h>

using namespace com::azure::dev::base;

class VersionApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  VersionApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application("Version", numberOfArguments, arguments, environment)
  {
  }

  void main() throw()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << ENDL;
    
    Orb orb;
    orb.registerEncoding(new LisaEncoding(), Orb::OUTGOING|Orb::INCOMING);

    orb.registerProtocol(new TCPIPProtocol());
    
    InetAddress address;
    InetEndPoint endPoint(address, TCPIPProtocol::DEFAULT_PORT);
    orb.registerFactory(new TCPIPConnectionFactory(endPoint, 16));
    
    // orb.openFactory("TCPIP://[::]:1234");
    
    fout << "ORB:" << EOL
         << indent(2) << "number of encoding schemes: " << orb.getNumberOfEncodings() << EOL
         << indent(2) << "encoding schemes: " << orb.getEncodings() << EOL
         << EOL
         << indent(2) << "number of schemes: " << orb.getNumberOfSchemes() << EOL
         << indent(2) << "schemes: " << orb.getSchemes() << EOL
         << EOL
         << indent(2) << "number of factories: " << orb.getNumberOfFactories() << EOL
         << EOL
         << indent(2) << "skeletons: " << orb.getNumberOfSkeletons() << EOL
         << EOL
         << indent(2) << "stubs: " << orb.getNumberOfStubs() << EOL
         << EOL
         << indent(2) << "connections: " << orb.getNumberOfConnections() << EOL
         << ENDL;
    
    HashSet<String> factories = orb.getFactories();
    HashSet<String>::ReadEnumerator enu = factories.getReadEnumerator();
    fout << "Active factories:" << EOL;
    while (enu.hasNext()) {
      fout << indent(2) << *enu.next() << EOL;
    }
    fout << ENDL;
    
    orb.run();
  }
};

STUB(VersionApplication);
