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
#include <base/string/InvalidFormat.h>
#include <base/communication/IEEE1394.h>
#include <base/concurrency/Thread.h>
#include <base/Timer.h>
#include <base/Cast.h>
#include <base/UnsignedInteger.h>
#include <base/UnsignedLongInteger.h>
#include <base/mem/MemoryDump.h>

using namespace base;

class IEEE1394Application : public Application,
                            public IEEE1394::IsochronousChannelListener,
                            public IEEE1394::FunctionControlProtocolListener {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:
  
  enum Command {
    COMMAND_DUMP_ADAPTERS,
    COMMAND_DUMP_TOPOLOGY,
    COMMAND_DUMP_SPEEDS,
    COMMAND_DUMP_NODES,
    COMMAND_FCP,
    COMMAND_ISOCHRONOUS,
    COMMAND_RESET,
    COMMAND_REGISTER_SPACE,
    COMMAND_HELP,
    COMMAND_VERSION,
    COMMAND_USAGE,
    COMMAND_ERROR
  };
  
  IEEE1394Application(int numberOfArguments, const char* arguments[], const char* environment[]) throw()
    : Application(MESSAGE("IEEE1394"), numberOfArguments, arguments, environment) {
  }

  void dumpAdapters() throw() {
    try {
      IEEE1394 ieee1394;
      Array<EUI64> adapters = ieee1394.getAdapters();
      if (adapters.getSize() == 0) {
        fout << MESSAGE("No IEEE 1394 adapters available") << ENDL;
      } else {
        Array<EUI64>::ReadEnumerator enu = adapters.getReadEnumerator();
        fout << MESSAGE("IEEE 1394 adapters:") << EOL;
        while (enu.hasNext()) {
          fout << MESSAGE("  ") << *enu.next() << EOL;
        }
        fout << FLUSH;
      }
    } catch (Exception& e) {
      fout << ENDL;
      ferr << MESSAGE("Unable to get available adapters") << EOL
           << e << ENDL;
      Application::getApplication()->setExitCode(EXIT_CODE_ERROR);
      return;
    }
  }

  void openAdapter(IEEE1394& ieee1394, const EUI64& id) throw() {
    fout << MESSAGE("Opening IEEE 1394 adapter (") << id << ')' << ENDL;
    ieee1394.open(id);
  }

  void dumpAdapter() throw() {
  }
  
  void dumpNodes(const EUI64& guid) throw() {   
    try {
      IEEE1394 ieee1394;
      
      EUI64 id = guid;
      if (guid.isInvalid()) {
        Array<EUI64> adapters = ieee1394.getAdapters();
        if (adapters.getSize() == 0) {
          ferr << MESSAGE("No adapters available") << ENDL;
          setExitCode(Application::EXIT_CODE_ERROR);
          return;
        }
        id = adapters[0];
      }
      
      fout << MESSAGE("Opening IEEE 1394 adapter (") << id << ')' << ENDL;
      ieee1394.open(id);
      ieee1394.checkResetGeneration();

      static const StringLiteral STANDARDS[] = {
        MESSAGE("Unspecified"),
        MESSAGE("IEEE 1394"),
        MESSAGE("IEEE 1394A"),
        MESSAGE("IEEE 1394B")
      };
      
      static const StringLiteral SPEEDS[] = {
        MESSAGE("S100"),
        MESSAGE("S200"),
        MESSAGE("S400"),
        MESSAGE("S800"),
        MESSAGE("S1600"),
        MESSAGE("S3200")
      };

      fout << MESSAGE("Number of local nodes: ") << ieee1394.getNumberOfNodes() << EOL
           << MESSAGE("Local id: ") << ieee1394.getLocalId() << EOL
           << MESSAGE("Root node: ") << ieee1394.getRootNode() << EOL;

      
      if (ieee1394.getBusManager() != IEEE1394::BROADCAST) {
        fout << MESSAGE("Bus manager: ") << ieee1394.getBusManager() << EOL;
      } else {
        fout << MESSAGE("Bus manager: ") << MESSAGE("(not present)") << EOL;
      }

      if (ieee1394.getCycleMaster() != IEEE1394::BROADCAST) {
        fout << MESSAGE("Cycle master: ") << ieee1394.getCycleMaster() << EOL;
      } else {
        fout << MESSAGE("Cycle master: ") << MESSAGE("(not present)") << EOL;
      }
      
      if (ieee1394.getIsochronousResourceManager() != IEEE1394::BROADCAST) {
        fout << MESSAGE("Isochronous resource manager: ")
             << ieee1394.getIsochronousResourceManager() << EOL
             << MESSAGE("  Available bandwidth: ")
             << ieee1394.getAvailableBandwidth() << EOL
             << MESSAGE("  Available channels: ")
             << BIN << setWidth(64) << ZEROPAD << NOPREFIX
             << ieee1394.getAvailableIsochronousChannels() << EOL;
      } else {
        fout << MESSAGE("Isochronous resource manager: ") << MESSAGE("(not present)") << EOL;
      }
      fout << MESSAGE("Maximum broadcast speed: ") << SPEEDS[ieee1394.getBroadcastSpeed()] << EOL;
      
      for (unsigned int id = 0; id < ieee1394.getNumberOfNodes(); ++id) {
        const unsigned short node = IEEE1394::makeNodeId(id);
        const unsigned int capabilities = ieee1394.getCapabilities(node);
        fout << MESSAGE("Node: ") << id << EOL;

        if (!ieee1394.isLinkLayerActive(id)) {
          fout << MESSAGE("  link layer is not active") << EOL;
          continue;
        }
        
        try {
          IEEE1394::Standard standard = ieee1394.getCompliance(node);
          fout << MESSAGE("  standard: ") << STANDARDS[standard] << EOL;
        } catch (IEEE1394Exception& e) {
        }

        if (!ieee1394.getLocalIdentifier(id).isInvalid()) {
          fout << MESSAGE("  guid: ") << ieee1394.getLocalIdentifier(id) << EOL;
        }

        try {
          unsigned int vendor = ieee1394.getVendorId(node);
          fout << MESSAGE("  vendor: ")
               << HEX << setWidth(2) << ZEROPAD << NOPREFIX << ((vendor >> 16) & 0xff) << ':'
               << HEX << setWidth(2) << ZEROPAD << NOPREFIX << ((vendor >> 8) & 0xff) << ':'
               << HEX << setWidth(2) << ZEROPAD << NOPREFIX << (vendor & 0xff) << EOL;        
        } catch (IEEE1394Exception& e) {
        }
        
        try {
          String description = ieee1394.getDescription(node);
          if (description.isProper()) {
            fout << MESSAGE("  description: ") << description << EOL;
          }
          
          String keywords = ieee1394.getKeywords(node);
          if (keywords.isProper()) {
            fout << MESSAGE("  keywords: ") << keywords << EOL;
          }
        } catch (IEEE1394Exception& e) {
        }
        
        try {
          unsigned int max = ieee1394.getMaximumPayload(node);
          fout << MESSAGE("  maximum asynchronous payload: ") << max << EOL;
        } catch (IEEE1394Exception& e) {
        }

        fout << MESSAGE("  is contender: ") << ieee1394.isContender(id) << EOL
             << MESSAGE("  maximum physical speed: ")
             << SPEEDS[ieee1394.getMaximumSpeed(id)] << EOL
             << MESSAGE("  maximum link speed: ")
             << SPEEDS[ieee1394.getMaximumLinkSpeed(id)] << EOL;

        if (capabilities) {
          fout << MESSAGE("  capabilities: ") << EOL;
          if (capabilities & IEEE1394::ISOCHRONOUS_RESOURCE_MANAGER_CAPABLE) {
            fout << MESSAGE("    isochronous resource manager") << EOL;
          }
          if (capabilities & IEEE1394::CYCLE_MASTER_CAPABLE) {
            fout << MESSAGE("    cycle master") << EOL;
          }
          if (capabilities & IEEE1394::ISOCHRONOUS_TRANSACTION_CAPABLE) {
            fout << MESSAGE("    isochronous transaction") << EOL;
          }
          if (capabilities & IEEE1394::BUS_MASTER_CAPABLE) {
            fout << MESSAGE("    bus master") << EOL;
          }
          if (capabilities & IEEE1394::POWER_MANAGER_CAPABLE) {
            fout << MESSAGE("    power manager") << EOL;
          }
        }
        
        if ((ieee1394.getIsochronousResourceManager() == id) ||
            (ieee1394.getBusManager() == id)) {
          fout << MESSAGE("  roles: ") << EOL;
          if (ieee1394.getCycleMaster() == id) {
            fout << MESSAGE("    cycle master") << EOL;
          }
//          if (ieee1394.getPowerManager() == id) {
//            fout << MESSAGE("    power manager") << EOL;
//          }
          if (ieee1394.getIsochronousResourceManager() == id) {
            fout << MESSAGE("    isochronous resource manager") << EOL;
          }
          if (ieee1394.getBusManager() == id) {
            fout << MESSAGE("    bus manager") << EOL;
          }
        }
        
        if (capabilities & IEEE1394::CYCLE_MASTER_CAPABLE) {
          try {
            unsigned int busTime = ieee1394.getBusTime(node);
            fout << MESSAGE("  bus time: ") << busTime << EOL;            
          } catch (IEEE1394Exception& e) {
          }
        }
        if (capabilities & IEEE1394::ISOCHRONOUS_TRANSACTION_CAPABLE) {
          try {
            unsigned int cycleTime = ieee1394.getCycleTime(node);
            fout << MESSAGE("  cycle time: ") << cycleTime << EOL;
          } catch (IEEE1394Exception& e) {
          }
        }
      }
      fout << ENDL;
    } catch (IEEE1394Exception& e) {
      fout << ENDL;
      ferr << MESSAGE("Exception: ") << e << ENDL;
      setExitCode(Application::EXIT_CODE_ERROR);
    }
  }

  void dumpRegisterSpace(uint64 firstAddress, uint64 lastAddress, const EUI64& guid, int node) throw() {
    try {
      IEEE1394 ieee1394;
      
      EUI64 id = guid;
      if (guid.isInvalid()) {
        Array<EUI64> adapters = ieee1394.getAdapters();
        if (adapters.getSize() == 0) {
          ferr << MESSAGE("No adapters available") << ENDL;
          setExitCode(Application::EXIT_CODE_ERROR);
          return;
        }
        id = adapters[0];
      }
      
      fout << MESSAGE("Opening IEEE 1394 adapter (") << id << ')' << ENDL;
      ieee1394.open(id);

      if (node < 0) {
        node = IEEE1394::makeNodeId(ieee1394.getLocalId());
      }
      
      const uint32 DEFAULT_VALUE = 0xdccd2332;
      const uint64 endAddress = lastAddress + sizeof(uint32);
      
      uint32 buffer[4096];
      
      while (firstAddress < endAddress) {
        uint32 bytesToRead = endAddress - firstAddress;
        if (bytesToRead > getArraySize(buffer) * sizeof(uint32)) {
          bytesToRead = getArraySize(buffer) * sizeof(uint32);
        }
        
        ieee1394.read(node, firstAddress, buffer, bytesToRead/sizeof(uint32), DEFAULT_VALUE);
        // TAG: could read twice with difference default values

//         MemoryDump dump(Cast::pointer<const uint8*>(buffer), bytesToRead);
//         dump.setWordSize(4);
//         dump.setGlobalOffset(firstAddress);
//         fout << dump << ENDL;
            
        for (unsigned int i = 0; i < bytesToRead/sizeof(uint32); ++i) {
          uint32 value = buffer[i];
          if (value != DEFAULT_VALUE) {
            char characters[4];
            characters[0] = ASCIITraits::isPrintable((value >> 24) & 0xff) ? ((value >> 24) & 0xff) : '.';
            characters[1] = ASCIITraits::isPrintable((value >> 16) & 0xff) ? ((value >> 16) & 0xff) : '.';
            characters[2] = ASCIITraits::isPrintable((value >> 8) & 0xff) ? ((value >> 8) & 0xff) : '.';
            characters[3] = ASCIITraits::isPrintable(value & 0xff) ? (value & 0xff) : '.';
            
            fout << HEX << setWidth(16) << ZEROPAD << NOPREFIX << (firstAddress + i * sizeof(uint32)) << ' ' << ' '
                 << HEX << setWidth(8) << ZEROPAD << NOPREFIX << value << ' ' << ' '
                 << characters[0] << characters[1] << characters[2] << characters[3] << EOL;
          } else {
            fout << HEX << setWidth(16) << ZEROPAD << NOPREFIX << (firstAddress + i * sizeof(uint32)) << ' ' << ' '
                 << MESSAGE("........") << EOL;
          }
        }
        
        firstAddress += bytesToRead;
      }
    } catch (Exception& e) {
      fout << ENDL;
      ferr << MESSAGE("Exception: ") << e << ENDL;
      setExitCode(Application::EXIT_CODE_ERROR);
    }
  }

  void onFCPRequest(unsigned short nodeId, const uint8* buffer, unsigned int size) throw() {
    fout << MESSAGE("FCP request: ") << EOL
         << MESSAGE("  source node: ") << IEEE1394::getAsString(nodeId) << EOL
         << ENDL;
    MemoryDump dump(buffer, size);
    fout << dump << ENDL;
  }

  void onFCPResponse(unsigned short nodeId, const uint8* buffer, unsigned int size) throw() {
    fout << MESSAGE("FCP response: ") << EOL
         << MESSAGE("  source node: ") << IEEE1394::getAsString(nodeId) << EOL
         << ENDL;
    MemoryDump dump(buffer, size);
    fout << dump << ENDL;
  }
  
  bool onIsochronousPacket(const uint8* buffer, unsigned int size) throw() {
    MemoryDump dump(buffer, size);
    fout << dump << ENDL;
    return false;
  }

  void fcp(const EUI64& guid) throw() {
    try {
      IEEE1394 ieee1394;
      
      EUI64 id = guid;
      if (guid.isInvalid()) {
        Array<EUI64> adapters = ieee1394.getAdapters();
        if (adapters.getSize() == 0) {
          ferr << MESSAGE("No adapters available") << ENDL;
          setExitCode(Application::EXIT_CODE_ERROR);
          return;
        }
        id = adapters[0];
      }
      
      fout << MESSAGE("Opening IEEE 1394 adapter (") << id << ')' << ENDL;
      ieee1394.open(id);

      ieee1394.registerFCPListener(this);

      static const char CMD[] = {1,2,3,4,5,6,7,8,9};
      
      ieee1394.write(
        IEEE1394::makeNodeId(ieee1394.getLocalId()),
        IEEE1394::CSR_BASE_ADDRESS + IEEE1394::FCP_COMMAND_FRAME,
        CMD,
        sizeof(CMD)
      );

      ieee1394.write(
        IEEE1394::makeNodeId(ieee1394.getLocalId()),
        IEEE1394::CSR_BASE_ADDRESS + IEEE1394::FCP_RESPONSE_FRAME,
        CMD,
        sizeof(CMD)
      );

      while (ieee1394.wait(1000)) {
        ieee1394.dequeue();
      }
      ieee1394.unregisterFCPListener();
    } catch (Exception& e) {
      fout << ENDL;
      ferr << MESSAGE("Exception: ") << e << ENDL;
      setExitCode(Application::EXIT_CODE_ERROR);
    }
  }
  
  void isochronousTransfer(const EUI64& guid, unsigned int channel) throw() {
    try {
      IEEE1394 ieee1394;
      
      EUI64 id = guid;
      if (guid.isInvalid()) {
        Array<EUI64> adapters = ieee1394.getAdapters();
        if (adapters.getSize() == 0) {
          ferr << MESSAGE("No adapters available") << ENDL;
          setExitCode(Application::EXIT_CODE_ERROR);
          return;
        }
        id = adapters[0];
      }
      
      fout << MESSAGE("Opening IEEE 1394 adapter (") << id << ')' << ENDL;
      ieee1394.open(id);

      //char buffer[(255 + 1) * sizeof(IEEE1394::Quadlet)];
      fout << MESSAGE("Listening for isochronous packets on channel ") << channel << MESSAGE("...") << ENDL;
      ieee1394.readIsochronous(channel, this);
    } catch (Exception& e) {
      fout << ENDL;
      ferr << MESSAGE("Exception: ") << e << ENDL;
      setExitCode(Application::EXIT_CODE_ERROR);
    }
  }

  void resetBus(const EUI64& guid) throw() {
  }

  unsigned short getNodeId(const String& string) const throw(InvalidFormat) {
    unsigned int busId = IEEE1394::LOCAL_BUS;
    unsigned int physicalId;
    int index = string.indexOf(':');
    if (index >= 0) { // format "bus:physical"
      String bus = string.substring(0, index);
      physicalId = UnsignedInteger::parse(
        string.substring(index + 1),
        UnsignedLongInteger::ANY
      );
      if (bus != "local") {
        busId = UnsignedInteger::parse(
          string.substring(0, index),
          UnsignedLongInteger::ANY
        );
        assert(busId <= IEEE1394::LOCAL_BUS, InvalidFormat("Invalid bus id", this));
      }
    } else { // physical
      physicalId = UnsignedInteger::parse(string, UnsignedLongInteger::ANY);
    }
    assert(physicalId <= IEEE1394::BROADCAST, InvalidFormat("Invalid physical id", this));
    return IEEE1394::makeNodeId(physicalId, busId);
  }
  
  void main() throw() {
    fout << getFormalName() << MESSAGE(" version ") << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;
    
    Command command = COMMAND_ERROR;
    String errorMessage;
    String guid;
    uint64 firstAddress;
    uint64 lastAddress;
    int nodeId = -1; // -1 indicates default
    int channel = 0;
    
    const Array<String> arguments = getArguments();
    
    try {
      if (arguments.getSize() == 0) {
        command = COMMAND_USAGE;
      } else if (arguments.getSize() == 1) {
        if (arguments[0] == "--help") {
          command = COMMAND_HELP;
        } else if (arguments[0] == "--version") {
          command = COMMAND_VERSION;
        } else if (arguments[0] == "--adapters") {
          command = COMMAND_DUMP_ADAPTERS;
        } else if (arguments[0] == "--topology") {
          command = COMMAND_DUMP_TOPOLOGY;
        } else if (arguments[0] == "--speed") {
          command = COMMAND_DUMP_SPEEDS;
        } else if (arguments[0] == "--nodes") {
          command = COMMAND_DUMP_NODES;
        }
      } else if (arguments.getSize() == 2) {
        if (arguments[0] == "--nodes") {
          command = COMMAND_DUMP_NODES;
          guid = arguments[1];
        } else if (arguments[0] == "--reset") {
          command = COMMAND_RESET;
          guid = arguments[1];
        }
      } else if (arguments.getSize() >= 3) {
        if (arguments[0] == "--fcp") { // --fcp nodeid data adapter
          command = COMMAND_FCP;
        } else if (arguments[0] == "--iso") {
          command = COMMAND_ISOCHRONOUS;
          guid = arguments[1];
          channel = UnsignedInteger::parse(arguments[2]);
          assert(channel <= 63, Exception("Invalid isochronous channel", this));
        } else if (arguments[0] == "--registers") {
          command = COMMAND_REGISTER_SPACE;
        
          String registers = arguments[1];
          const int plus = registers.indexOf('+');
          if (plus >= 0) { // offset and size
            firstAddress = UnsignedLongInteger::parse(
              registers.substring(0, plus),
              UnsignedLongInteger::PREFIX | UnsignedLongInteger::HEX
            );
            lastAddress = UnsignedLongInteger::parse(
              registers.substring(plus + 1),
              UnsignedLongInteger::PREFIX | UnsignedLongInteger::HEX
            ) + firstAddress;
          } else {
            const int minus = registers.indexOf('-');
            if (minus >= 0) { // range begin - end
              firstAddress = UnsignedLongInteger::parse(
                registers.substring(0, minus),
                UnsignedLongInteger::PREFIX | UnsignedLongInteger::HEX
              );
              lastAddress = UnsignedLongInteger::parse(
                registers.substring(minus + 1),
                UnsignedLongInteger::PREFIX | UnsignedLongInteger::HEX
              );
            } else { // single register (quadlet)
              if (registers == "topology") {
                firstAddress = IEEE1394::CSR_BASE_ADDRESS + IEEE1394::TOPOLOGY_MAP;
                lastAddress = firstAddress + 0x3fc;
              } else if (registers == "speed") {
                firstAddress = IEEE1394::CSR_BASE_ADDRESS + IEEE1394::SPEED_MAP;
                lastAddress = firstAddress + 0xffc;
              } else if (registers == "csr") {
                firstAddress = IEEE1394::CSR_BASE_ADDRESS;
                lastAddress = firstAddress + 0x1fc;
              } else if (registers == "bus") {
                firstAddress = IEEE1394::CSR_BASE_ADDRESS + IEEE1394::BUS_DEPENDENT;
                lastAddress = firstAddress + 0x1fc;
              } else if (registers == "rom") {
                firstAddress = IEEE1394::CSR_BASE_ADDRESS + IEEE1394::CONFIGURATION_ROM;
                lastAddress = firstAddress + 0x3fc;
              } else if (registers == "cycletime") {
                firstAddress = IEEE1394::CSR_BASE_ADDRESS + IEEE1394::CYCLE_TIME;
                lastAddress = firstAddress;
              } else {
                firstAddress = UnsignedLongInteger::parse(
                  registers,
                  UnsignedLongInteger::PREFIX | UnsignedLongInteger::HEX
                );
                lastAddress = firstAddress;
              }
            }
          }
          if ((firstAddress % sizeof(IEEE1394::Quadlet) != 0) ||
              (lastAddress % sizeof(IEEE1394::Quadlet) != 0) ||
              (firstAddress > lastAddress)) {
            errorMessage = MESSAGE("Misaligned register(s)");
            command = COMMAND_ERROR;
          }
          guid = arguments[2]; // TAG: remove when ready

          switch (arguments.getSize()) {
          case 2:
            break;
          case 3:
            guid = arguments[2];
            break;
          case 4:
            nodeId = getNodeId(arguments[3]);
            break;
          default:
            command = COMMAND_ERROR;
          }
        }
      }
    } catch (Exception& e) {
      errorMessage = e.getMessage();
      command = COMMAND_ERROR;
    }
    
    EUI64 id;
    if ((command != COMMAND_ERROR) && guid.isProper()) {
      try {
        id = EUI64(guid);
      } catch (InvalidFormat& e) {
        command = COMMAND_ERROR;
      }
      if (id.isInvalid()) {
        command = COMMAND_ERROR;
      }
      if (command == COMMAND_ERROR) {
        errorMessage = MESSAGE("Invalid GUID of adapter");
      }
    }

    if ((command != COMMAND_ERROR) && (nodeId >= 0)) {
      if (IEEE1394Common::getPhysicalId(nodeId) == IEEE1394::BROADCAST) {
        errorMessage = MESSAGE("Broadcast id not premitted");
        command = COMMAND_ERROR;
      } else if (nodeId > PrimitiveTraits<uint16>::MAXIMUM) {
        errorMessage = MESSAGE("Invalid node id");
        command = COMMAND_ERROR;
      }
    }
    
    switch (command) {
    case COMMAND_DUMP_ADAPTERS:
      dumpAdapters();
      break;
    case COMMAND_DUMP_NODES:
      dumpNodes(id);
      break;
    case COMMAND_RESET:
      resetBus(id);
      break;
    case COMMAND_REGISTER_SPACE:
      dumpRegisterSpace(firstAddress, lastAddress, id, nodeId);
      break;
    case COMMAND_ISOCHRONOUS:
      isochronousTransfer(id, 1);
      break;
    case COMMAND_FCP:
      fcp(id);
      break;
    case COMMAND_HELP:
      // TAG: more help
      fout << MESSAGE("Usage: ") << getFormalName()
           << MESSAGE(" [options] [adapter EUI-64] [node EUI-64 or physical id]") << ENDL;
      break;
    case COMMAND_VERSION:
      break;
    case COMMAND_USAGE:
      fout << MESSAGE("Usage: ") << getFormalName()
           << MESSAGE(" [options] [adapter EUI-64] [node EUI-64 or physical id]") << ENDL;
      break;
    case COMMAND_ERROR:
      // fout has been flushed
      if (errorMessage.isProper()) {
        ferr << MESSAGE("ERROR: Invalid arguments(s): ") << errorMessage << EOL;
      } else {
        ferr << MESSAGE("ERROR: Invalid arguments(s)") << EOL;
      }
      ferr << MESSAGE("For help: ") << getFormalName() << MESSAGE(" --help") << ENDL;
      setExitCode(Application::EXIT_CODE_ERROR);
      break;
    }
  }
};

STUB(IEEE1394Application);
