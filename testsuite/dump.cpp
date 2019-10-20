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
#include <base/io/FileReader.h>
#include <base/mem/MemoryDump.h>

using namespace com::azure::dev::base;

class DumpApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:
  
  DumpApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[])
    : Application("dump", numberOfArguments, arguments, environment) {
  }

  void main() {
    const Array<String> arguments = getArguments();
    
    if (arguments.getSize() == 1) {
      if (arguments[0] == "--help") {
        fout << getFormalName() << " version "
             << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
             << "Base Framework (Test Suite)" << EOL
             << "Copyright (C) 2002-2019 by Rene Moeller Fonseca" << EOL
             << EOL
             << "Usage: " << getFormalName() << " file" << EOL
             << EOL
             << "Options" << EOL
             << "  --help       Shows this message" << EOL
             << "  --version    Displays version information" << EOL
             << ENDL;
      } else if (arguments[0] == "--version") {
        fout << getFormalName() << " version " << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
             << "Base Framework (Test Suite)" << EOL
             << "Copyright (C) 2002-2019 by Rene Moeller Fonseca" << EOL
             << ENDL;
      } else {
        try {
          Allocator<uint8> buffer(4096 * 4);
          
          String filename = arguments[0];
          File file(filename, File::READ, 0);
          unsigned long long size = file.getSize();
          FileReader reader(file, 0);
          
          unsigned long long offset = 0;
          while (offset < size) {
            unsigned int blockSize =
              minimum<unsigned long long>(buffer.getSize(), size - offset);
            file.read(buffer.getElements(), blockSize);
            MemoryDump dump(buffer.getElements(), blockSize);
            dump.setWordSize(1);
            dump.setGlobalOffset(offset);
            fout << dump << ENDL;
            offset = file.getPosition();
          }
        } catch (Exception& e) {
          exceptionHandler(e);
        }
      }
    } else {
      fout << getFormalName() << " version "
           << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
           << "Base Framework (Test Suite)" << EOL
           << "Copyright (C) 2002-2019 by Rene Moeller Fonseca" << EOL
           << EOL
           << "Error: Invalid argument(s)." << EOL
           << "For help: " << getFormalName() << " --help" << EOL
           << ENDL;
      setExitCode(EXIT_CODE_ERROR);
    }
    
  }

};

APPLICATION_STUB(DumpApplication);
