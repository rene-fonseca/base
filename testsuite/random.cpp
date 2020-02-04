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
#include <base/UnsignedInteger.h>
#include <base/Random.h>

using namespace com::azure::dev::base;

class RandomApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;  
  
  enum Command {
    COMMAND_VERSION,
    COMMAND_HELP,
    COMMAND_RANDOM
  };
public:
  
  RandomApplication()
    : Application("random")
  {
  }

  void version()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << ENDL;
  }
  
  void help()
  {
    version();
    fout << "Usage: " << getFormalName() << " [OPTIONS] expression" << EOL
         << EOL
         << "Options:" << EOL
         << indent(2) << "--help      this message" << EOL
         << indent(2) << "--version   dump the version" << EOL
         << ENDL;
  }
  
  void main() {
    Command command = COMMAND_RANDOM;
    unsigned int generateCount = 0;
    
    const auto arguments = getArguments();
    Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();
    while (enu.hasNext()) {
      String argument = enu.next();
      if (argument == "--help") {
        command = COMMAND_HELP;
      } else if (argument == "--version") {
        command = COMMAND_VERSION;
      } else {
        try {
          generateCount = UnsignedInteger::parse(argument);
        } catch (...) {
          setExitCode(1);
          ferr << "Error: Invalid number of random chars." << ENDL;
          return;
        }
      }
    }    
    
    switch (command) {
    case COMMAND_VERSION:
      version();
      break;
    case COMMAND_HELP:
      help();
      break;
    default:
      // TAG: add option to control output type, int, uint, base64, bytes, ...
      // TAG: add file dest
      RandomInputStream& ris = Random::getRandomInputStream();
      PrimitiveStackArray<uint8> buffer(4096);
      while (true) {
        ris.read(buffer, buffer.size(), false);
        auto bytesToWrite = buffer.size();
        if (generateCount > 0) {
          bytesToWrite = minimum<MemorySize>(generateCount, buffer.size());
        }
        fout.write(buffer, bytesToWrite, false);
        if (generateCount > 0) {
          generateCount -= bytesToWrite;
          if (generateCount == 0) {
            break;
          }
        }
      }
      fout << FLUSH;
      break;
    }
  }
};

APPLICATION_STUB(RandomApplication);
