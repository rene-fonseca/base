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
#include <base/io/FileInputStream.h>
#include <base/UnsignedInteger.h>
#include <base/Random.h>

using namespace com::azure::dev::base;

class CPPApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;

  String identifier = "_IDENTIFIER";
  unsigned int randomize = 0;
  String srcPath;
  String destPath;
public:

  CPPApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) noexcept
    : Application("cpp", numberOfArguments, arguments, environment)
  {
  }

  bool parseArguments()
  {
    Array<String> arguments = getArguments();
    Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();
    while (enu.hasNext()) {
      String argument = *enu.next();
      if (argument == "--identifier") {
        if (!enu.hasNext()) {
          ferr << "Error: Expected identifier of struct." << ENDL;
          return false;
        }
        identifier = *enu.next();
      } else if (argument == "--random") {
        srcPath.clear();
        if (!enu.hasNext()) {
          ferr << "Error: Expected random size." << ENDL;
          return false;
        }
        try {
          randomize = UnsignedInteger::parse(*enu.next());
        } catch (...) {
          ferr << "Error: Expected random size." << ENDL;
          return false;
        }
      } else {
        if (!srcPath) {
          srcPath = argument;
          randomize = 0;
        } else if (!destPath) {
          destPath = argument;
        } else {
          ferr << "Error: Unexpected argument." << ENDL;
          return false;
        }
      }
    }
    return true;
  }

  void main()
  {
    if (!parseArguments()) {
      fout << "cpp [--identifier ID] [--random SIZE]|SRC [DEST]" << ENDL;
      setExitCode(1);
      return;
    }

    if (!(srcPath || (randomize > 0))) {
      ferr << "Error: No input given." << ENDL;
      setExitCode(1);
      return;
    }

    try {

      Allocator<uint8> buffer;
      if (randomize > 0) {
        buffer.setSize(randomize);
        Random::fill(buffer.getElements(), buffer.getElements() + buffer.getSize()); // no need for buffer
      } else {
        File file(srcPath, File::READ, 0);
        auto size = file.getSize();
        if (size == 0) {
          ferr << "Error: File is empty." << ENDL;
          setExitCode(1);
          return;
        }
        buffer.setSize(size);
        file.read(buffer.getElements(), size, false); // TAG: do not read everything
      }

      const MemorySize wordsPerLine = 16;

      if (destPath) {
        // TAG: add support for dest path
        return;
      }

      auto src = buffer.getBeginReadIterator();
      auto end = buffer.getEndReadIterator();
      fout << "/* Generated file - do NOT edit. */" << EOL << EOL;
      fout << "static unsigned char " << identifier << "[" << buffer.getSize() << "] = {" << EOL;
      while (src != end) {
        const MemoryDiff left = minimum<MemorySize>(wordsPerLine, end - src);
        fout << indent(2);
        if ((end - src) > left) {
          for (MemoryDiff i = 0; i < left; ++i) {
            fout << HEX << ZEROPAD << setWidth(4) << src[i] << ", ";
          }
        } else {
          for (MemoryDiff i = 0; i < left; ++i) {
            if (i != 0) {
              fout << ", ";
            }
            fout << HEX << ZEROPAD << setWidth(4) << src[i];
          }
        }
        fout << EOL;
        src += left;
      }
      fout << "};" << EOL;
      fout << FLUSH;

    } catch (...) {
      ferr << "Error: Failed to generate code." << ENDL;
      setExitCode(1);
      return;
    }
  }
};

APPLICATION_STUB(CPPApplication);
