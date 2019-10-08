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

class PingApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
  /** The echo service port. */
  static const unsigned short ECHO_SERVICE_PORT = 7;
  
  /** The server port. */
  unsigned short port;
  /** The bytes to send. */
  unsigned int dataSize;
  unsigned int timeout;
  unsigned int packetsToTransmit;
  
  enum Command {
    COMMAND_HELP,
    COMMAND_VERSION,
    COMMAND_PING
  };
public:
  
  PingApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application("ping", numberOfArguments, arguments, environment) {
    port = ECHO_SERVICE_PORT;
    dataSize = 32;
    timeout = 1000000;
    packetsToTransmit = 0;
  }
  
  void onTermination() throw() {
  }
  
  String getTimeAsString2(uint64 microseconds) throw() {
    return Literal("");
  }
  
  String getTimeAsString(uint64 microseconds) throw() {
    StringOutputStream stream;
    if (microseconds < 1000) {
      stream << microseconds << "us";
    } else if (microseconds < 1000000) {
      stream << FIXED
             << setPrecision(3) << microseconds/1000.0 << "ms";
    } else {
      stream << FIXED
             << setPrecision(3) << microseconds/1000000.0 << "s";
    }
    stream << FLUSH;
    return stream.getString();
  }
  
  void ping(const String& host) throw() {
    bool byName = true;
    InetAddress address;
    
//     if (InetAddress.isIPAddress(host)) {
//       address = InetAddress(host);
//       byName = false;
//     }
    
    try {
      address = InetAddress::getAddressByName(host); // the address of the remote host
    } catch (HostNotFound&) {
      ferr << "Error: " << "Unable to resolve host" << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }

    String name = host;
    try {
      name = address.getHostName(true);
    } catch (...) {
    }
    
    if (byName) {
      fout << "Pinging " << name << ' ' << '(' << address << ')' << " with "
           << dataSize << " bytes of data" << EOL
           << ENDL;
    } else {
      fout << "Pinging " << ' ' << '(' << address << ')' << " with "
           << dataSize << " bytes of data" << EOL
           << ENDL;
    }
    
    InetEndPoint endPoint(address, port);
    StreamSocket socket;
    try {
      socket.connect(endPoint.getAddress(), endPoint.getPort());
    } catch (IOException& e) {
      ferr << "Error: " << "Unable to connect" << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }
    
    socket.setReceiveBufferSize(dataSize);
    socket.setSendBufferSize(dataSize);
    socket.setTcpNoDelay(true);    
    
    Timer timeoutTimer;
    Timer timer;
    
    unsigned int packetsTransmitted = 0;
    unsigned int packetsReceived = 0;
    uint64 minimumTime = PrimitiveTraits<uint64>::MAXIMUM;
    uint64 maximumTime = 0;
    uint64 totalTime = 0;
    
    Allocator<uint8> outgoing(dataSize);
    Allocator<uint8> incoming(dataSize);
    fill<uint8>(outgoing.getElements(), outgoing.getSize(), 0);
    
    while (!isTerminated() &&
           ((packetsToTransmit == 0) ||
            (packetsTransmitted < packetsToTransmit))) {
      uint8* dest = outgoing.getElements();
      *dest++ = packetsTransmitted;
      timeoutTimer.start();
      timer.start();
      socket.write(outgoing.getElements(), outgoing.getSize());
      ++packetsTransmitted;
      socket.wait(timeout); // minimum(250, timeout)
      // TAG: use timer to check for timeout
      unsigned int bytesAvailable = socket.available();
      if (bytesAvailable < incoming.getSize()) {
        fout << name << ' ' << '(' << address << ')'
             << ':' << " request timed out" << ENDL;
      } else {
        timer.stop();
        minimumTime = minimum(minimumTime, timer.getMicroseconds());
        maximumTime = maximum(maximumTime, timer.getMicroseconds());
        totalTime += timer.getMicroseconds();
        while (bytesAvailable >= incoming.getSize()) {
          socket.read(incoming.getElements(), incoming.getSize());
          if (compare(incoming.getElements(), outgoing.getElements(), dataSize) == 0) {
            ++packetsReceived;
            fout << incoming.getSize()
                 << " bytes from " << name << ' '
                 << '(' << address << ')' << ':' << ' '
                 << "n=" << packetsReceived << ' '
                 << "time=" << getTimeAsString(timer.getMicroseconds())
                 << ENDL;
            break;
          }
        }
      }
      timeoutTimer.stop();
      if (timeoutTimer.getMicroseconds() < timeout) {
        Thread::microsleep(timeout - timeoutTimer.getMicroseconds());
      }
    }
    
    socket.close();
    
    unsigned int packetsLost = packetsTransmitted - packetsReceived;
    double meanTime = totalTime/packetsReceived;
    
    fout << EOL
         << "--- statistics for " << name << " ---" << EOL
         << "Packets transmitted: " << packetsTransmitted << EOL
         << "Packets received: " << packetsReceived << EOL
         << "Packets lost: " << packetsLost << ' '
         << '(' << static_cast<int>(100*packetsLost/packetsTransmitted) << '%' << ')' << EOL
         << "Time minimum/maximum/mean: "
         << getTimeAsString(minimumTime) << '/'
         << getTimeAsString(maximumTime) << '/'
         << getTimeAsString(static_cast<uint64>(meanTime)) << EOL
         << ENDL;
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
    fout << getFormalName()
         << " [--help] [--port PORT] [--data SIZE] [--time MS] host" << ENDL;
  }
  
  void main() throw() {
    Command command = COMMAND_PING;
    String host;
    
    const Array<String> arguments = getArguments();
    if (arguments.getSize() > 0) {
      Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();
      while (enu.hasNext()) {
        String argument = *enu.next();
        if (argument == "--help") {
          command = COMMAND_HELP;
          break;
        } else if (argument == "--version") {
          command = COMMAND_VERSION;
          break;
        } else if (argument == "--port") {
          String temp = *enu.next();
          try {
            UnsignedInteger value(temp);
            if (value > 0xffff) {
              ferr << "Error: " << "Invalid port" << ENDL;
              setExitCode(EXIT_CODE_ERROR);
              return;
            }
            port = value;
          } catch (InvalidFormat&) {
            try {
              InetService service(temp);
              port = service.getPort();
            } catch (ServiceNotFound& e) {
              ferr << "Error: " << e.getMessage() << ENDL;
              setExitCode(EXIT_CODE_ERROR);
              return;
            }
          }
        } else {
          host = argument;
        }
      }
    }

    if (host.isEmpty()) {
      ferr << "Error: " << "Host not specified" << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }
    
    switch (command) {
    case COMMAND_HELP:
      help();
      break;
    case COMMAND_VERSION:
      version();
      break;
    case COMMAND_PING:
      ping(host);
      break;
    }
  }
};

STUB(PingApplication);
