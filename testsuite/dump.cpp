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
#include <base/io/FileReader.h>
#include <base/mem/MemoryDump.h>

using namespace base;

class DumpApplication : public Application {
public:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
  
  DumpApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw() :
    Application(MESSAGE("dump"), numberOfArguments, arguments, environment) {
  }

  void main() throw() {
    const Array<String> arguments = getArguments();
    
    if (arguments.getSize() == 1) {
      if (arguments[0] == "--help") {
        fout << getFormalName() << MESSAGE(" version ") << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
             << MESSAGE("Base Framework (Test Suite)") << EOL
             << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
             << MESSAGE("Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
             << EOL
             << MESSAGE("Usage: ") << getFormalName() << MESSAGE(" file") << EOL
             << EOL
             << MESSAGE("Options") << EOL
             << MESSAGE("  --help       Shows this message") << EOL
             << MESSAGE("  --version    Displays version information") << EOL
             << ENDL;
      } else if (arguments[0] == "--version") {
        fout << getFormalName() << MESSAGE(" version ") << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
             << MESSAGE("Base Framework (Test Suite)") << EOL
             << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
             << MESSAGE("Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
             << ENDL;
      } else {
        try {
          Allocator<char> buffer(4096 * 4);
          
          String filename = arguments[0];
          File file(filename, File::READ, 0);
          unsigned long long size = file.getSize();
          FileReader reader(file, 0);
          
          unsigned long long offset = 0;
          while (offset < size) {
            unsigned int blockSize = minimum<unsigned long long>(buffer.getSize(), size - offset);
            file.read(buffer.getElements(), blockSize);
            MemoryDump dump(Cast::pointer<const uint8*>(buffer.getElements()), blockSize);
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
      fout << getFormalName() << MESSAGE(" version ") << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
           << MESSAGE("Base Framework (Test Suite)") << EOL
           << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
           << MESSAGE("Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
           << EOL
           << MESSAGE("Error: Invalid argument(s).") << EOL
           << MESSAGE("For help: ") << getFormalName() << MESSAGE(" --help") << EOL
           << ENDL;
      setExitCode(EXIT_CODE_ERROR);
    }
    
  }

};

int main(int argc, const char* argv[], const char* env[]) {
  DumpApplication application(argc, argv, env);
  try {
    application.main();
  } catch(Exception& e) {
    return Application::getApplication()->exceptionHandler(e);
  } catch(...) {
    return Application::getApplication()->exceptionHandler();
  }
  return Application::getApplication()->getExitCode();
}
