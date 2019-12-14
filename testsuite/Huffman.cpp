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
#include <base/string/StringOutputStream.h>
#include <base/io/FileOutputStream.h>
#include <base/compression/Huffman.h>
#include <base/Random.h>
#include <base/Cast.h>

using namespace com::azure::dev::base;

class HuffmanApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  enum Command {
    COMMAND_ERROR,
    COMMAND_HELP,
    COMMAND_SIMPLE,
    COMMAND_COMPRESS,
    COMMAND_UNCOMPRESS,
    COMMAND_RANDOM
  };
  
  HuffmanApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[])
    : Application("Huffman", numberOfArguments, arguments, environment) {
  }

  void compress(
    const String& source, const String& destination) {
    File sourceFile(source, File::READ, 0);
    
    unsigned long long size = sourceFile.getSize();
    bassert(
      size <= PrimitiveTraits<unsigned int>::MAXIMUM,
      IOException("File is too big for me.", this)
    );

    Allocator<uint8> buffer(size);
    sourceFile.read(buffer.getElements(), buffer.getSize());
    sourceFile.close();

    FileOutputStream fos(destination, File::CREATE|File::TRUNCATE);
    Huffman huffman;
    huffman.encode(
      fos,
      buffer.getElements(),
      buffer.getSize()
    );
  }
  
  void uncompress(
    const String& source,
    const String& destination) {
    File sourceFile(source, File::READ, 0);
    
    unsigned long long size = sourceFile.getSize();
    bassert(
      size <= PrimitiveTraits<unsigned int>::MAXIMUM,
      IOException("File is too big for me.", this)
    );
    
    Allocator<uint8> buffer(size);
    sourceFile.read(buffer.getElements(), buffer.getSize());
    sourceFile.close();
    
    FileOutputStream fos(destination, File::CREATE|File::TRUNCATE);
    Huffman huffman;
    huffman.decode(fos, buffer.getElements(), buffer.getSize());
  }

  void random(const String& destination)
  {
    unsigned int size = Random::random<unsigned int>() % (64 * 1024);

    Allocator<uint8> buffer(size);
    for (unsigned int i = 0; i < buffer.getSize(); ++i) {
      unsigned int r = Random::random<unsigned int>();
      buffer.getElements()[i] = r; // slow
    }
    
    File file(destination, File::WRITE, File::CREATE|File::TRUNCATE);
    file.write(buffer.getElements(), buffer.getSize());
  }

  void simple(const String& destination)
  {
    StringOutputStream stream;
    stream << getFormalName() << " version "
           << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
           << "The Base Framework (Test Suite)" << EOL
           << FLUSH;
    String message = stream.getString();
    FileOutputStream fos(destination, File::CREATE|File::TRUNCATE);
    Huffman huffman;
    huffman.encode(
      fos,
      Cast::pointer<const uint8*>(message.getElements()),
      message.getLength()
    );
  }
  
  void main()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << ENDL;

    Command command = COMMAND_ERROR;
    String sourceFile;
    String destinationFile;
    
    const Array<String> arguments = getArguments();

    switch (arguments.getSize()) {
    case 1:
      if (arguments[0] == "--help") {
        command = COMMAND_HELP;
      }
      break;
    case 2:
      if (arguments[0] == "--simple") {
        command = COMMAND_SIMPLE;
      } else if (arguments[0] == "--random") {
        command = COMMAND_RANDOM;
      }
      destinationFile = arguments[1];
      break;
    case 3:
      if (arguments[0] == "--compress") {
        command = COMMAND_COMPRESS;
      } else if (arguments[0] == "--uncompress") {
        command = COMMAND_UNCOMPRESS;
      }
      sourceFile = arguments[1];
      destinationFile = arguments[2];
      break;
    }
    
    try {
      switch (command) {
      case COMMAND_ERROR:
        ferr << "Error: Invalid argument(s)." << EOL
             << "For help run: " << getFormalName() << " --help" << EOL
             << ENDL;
        setExitCode(EXIT_CODE_ERROR);
        break;
      case COMMAND_HELP:
        fout << "Options" << EOL
             << "  --compress source destination" << EOL
             << "  --uncompress source destination" << EOL
             << "  --simple destination" << EOL
             << "  --random destination" << EOL
             << "  --help" << EOL
             << ENDL;
        break;
      case COMMAND_SIMPLE:
        simple(destinationFile);
        break;
      case COMMAND_COMPRESS:
        compress(sourceFile, destinationFile);
        break;
      case COMMAND_UNCOMPRESS:
        uncompress(sourceFile, destinationFile);
        break;
      case COMMAND_RANDOM:
        random(destinationFile);
        break;
      }
    } catch (Exception& e) {
      exceptionHandler(e);
    }
  }
  
};

APPLICATION_STUB(HuffmanApplication);
