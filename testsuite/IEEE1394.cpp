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

class IEEE1394Application : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:
  
  enum Command {
    COMMAND_DUMP_ADAPTERS,
    COMMAND_DUMP_TOPOLOGY,
    COMMAND_DUMP_SPEEDS,
    COMMAND_DUMP_NODES,
    COMMAND_RESET,
    COMMAND_REGISTER_SPACE,
    COMMAND_HELP,
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
    
    static const StringLiteral STANDARD[] = {
      MESSAGE("unspecified"),
      MESSAGE("IEEE 1394"),
      MESSAGE("IEEE 1394A"),
      MESSAGE("IEEE 1394B")
    };
    
    IEEE1394::Standard standard = ieee1394.getCompliance();
    if (standard > IEEE1394::STANDARD_IEEE_1394B) {
      standard = IEEE1394::STANDARD_UNSPECIFIED;
    }
    fout << MESSAGE("  Standard: ") << STANDARD[standard] << EOL
         << MESSAGE("  Status: ") << ieee1394.getStatus() << EOL
         << EOL << ENDL;
    
    // TAG: topology and speed map
  }

  void dumpAdapter() throw() {
  }
  
  void dumpNodes(IEEE1394& ieee1394) throw() {
    uint64 present = ieee1394.getPresentNodes();
    
    fout << MESSAGE("Available nodes: ") << EOL;
    
    for (unsigned int id = 0; id < IEEE1394::BROADCAST; ++id) {
      if (present & (1ULL << id)) {
        try {
          const unsigned short node = IEEE1394::makeNodeId(id);
          fout << MESSAGE("Node: ") << ieee1394.getIdentifier(node) << EOL
               << MESSAGE("  Physical id: ") << id << ENDL;
          
          unsigned int vendor = ieee1394.getVendorId(node);
          fout << MESSAGE("  Vendor id: ")
               << HEX << setWidth(2) << ZEROPAD << NOPREFIX << ((vendor >> 16) & 0xff) << ':'
               << HEX << setWidth(2) << ZEROPAD << NOPREFIX << ((vendor >> 8) & 0xff) << ':'
               << HEX << setWidth(2) << ZEROPAD << NOPREFIX << (vendor & 0xff) << ENDL;
          
          unsigned int capabilities = ieee1394.getCapabilities(node);
          if (capabilities) {
            fout << MESSAGE("  Capabilities:") << EOL;
            if (capabilities & IEEE1394::ISOCHRONOUS_RESOURCE_MANAGER) {
              fout << ' ' << MESSAGE("    Isochronous resource manager") << EOL;
            }
            if (capabilities & IEEE1394::CYCLE_MASTER) {
              fout << ' ' << MESSAGE("    Cycle master") << EOL;
            }
            if (capabilities & IEEE1394::ISOCHRONOUS_TRANSMISSION) {
              fout << ' ' << MESSAGE("    Isochronous transmission") << EOL;
            }
            if (capabilities & IEEE1394::BUS_MASTER) {
              fout << ' ' << MESSAGE("    Bus master") << EOL;
            }
            if (capabilities & IEEE1394::POWER_MANAGEMENT) {
              fout << ' ' << MESSAGE("    Power management") << EOL;
            }
          }
          
          fout << MESSAGE("  Maximum asynchronous payload: ") << ieee1394.getMaximumPayload(node) << ENDL;

          String description = ieee1394.getDescription(node);
          if (description.isProper()) {
            fout << MESSAGE("  Description: ") << description << ENDL;
          }
        } catch (IEEE1394Exception& e) {
          fout << MESSAGE("Exception: ") << e << ENDL;
          // check cause
        }
      }
    }
    fout << EOL << ENDL;
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
      
      // TAG: get bus id's, get nodes for specific bus, is bus present methods?
      dumpNodes(ieee1394);
      
      fout << MESSAGE("Status: ") << ieee1394.getStatus() << ENDL;
      
      fout << MESSAGE("Closing IEEE 1394 adapter") << ENDL;
      ieee1394.close();
    } catch (Exception& e) {
      fout << ENDL;
      ferr << MESSAGE("Unable to get device information: ") << e << ENDL;
      Application::getApplication()->setExitCode(EXIT_CODE_ERROR);
      return;
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
        node = IEEE1394::makeNodeId(ieee1394.getPhysicalId());
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
  
  void resetBus() {
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
    
    const Array<String> arguments = getArguments();
    
    try {
      if (arguments.getSize() == 0) {
        command = COMMAND_USAGE;
      } else if (arguments.getSize() == 1) {
        if (arguments[0] == "--help") {
          command = COMMAND_HELP;
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
        if (arguments[0] == "--registers") {
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

//     STATE_CLEAR = 0x000,
//     STATE_SET = 0x004,
//     NODE_IDS = 0x008,
//     RESET_START = 0x00c,
//     SPLIT_TIMEOUT_HI = 0x018,
//     SPLIT_TIMEOUT_LO = 0x01c,
//     ARGUMENT_HI = 0x0020,
//     ARGUMENT_LO = 0x0024,
//     TEST_START = 0x028,
//     TEST_STATUS = 0x02c,
//     BUS_TIME = 0x204,
//     POWER_FAIL_IMMINENT = 0x208,
//     POWER_SOURCE = 0x20c,
//     BUSY_TIMEOUT = 0x210,
//     BANDWIDTH_AVAILABLE = 0x200,
//     CHANNELS_AVAILABLE_HI = 0x224,
//     CHANNELS_AVAILABLE_LO = 0x228,
//     FCP_COMMAND = 0xb00,
//     FCP_RESPONSE = 0xd00,

              if (registers == "topology") {
                firstAddress = IEEE1394::CSR_BASE_ADDRESS + IEEE1394::TOPOLOGY_MAP;
                lastAddress = firstAddress + 0x3fc;
              } else if (registers == "speed") {
                firstAddress = IEEE1394::CSR_BASE_ADDRESS + IEEE1394::SPEED_MAP;
                lastAddress = firstAddress + 0xffc;
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
            nodeId = UnsignedInteger::parse(
              arguments[3],
              UnsignedLongInteger::PREFIX | UnsignedLongInteger::HEX
            );
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
      if (nodeId % 64 == 63) {
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
      resetBus();
      break;
    case COMMAND_REGISTER_SPACE:
      dumpRegisterSpace(firstAddress, lastAddress, id, nodeId);
      break;
    case COMMAND_HELP:
      // TAG: more help
      fout << MESSAGE("Usage: ") << getFormalName() << MESSAGE(" [options] [adapter EUI-64] [node EUI-64 or physical id]") << ENDL;
      break;
    case COMMAND_USAGE:
      fout << MESSAGE("Usage: ") << getFormalName() << MESSAGE(" [options] [adapter EUI-64] [node EUI-64 or physical id]") << ENDL;
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
