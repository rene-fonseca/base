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
#include <base/string/StringOutputStream.h>
#include <base/io/FileOutputStream.h>
#include <base/compression/Huffman.h>
#include <base/mathematics/Random.h>
#include <base/Cast.h>

using namespace base;

class HuffmanApplication : public Application {
public:

  enum Command {
    COMMAND_ERROR,
    COMMAND_HELP,
    COMMAND_SIMPLE,
    COMMAND_COMPRESS,
    COMMAND_UNCOMPRESS,
    COMMAND_RANDOM
  };
  
  HuffmanApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw()
    : Application(MESSAGE("Huffman"), numberOfArguments, arguments, environment) {
  }

  void compress(const String& source, const String& destination) throw(IOException) {
    File sourceFile(source, File::READ, 0);
    
    unsigned long long size = sourceFile.getSize();
    assert(size <= PrimitiveTraits<unsigned int>::MAXIMUM, IOException("File is too big for me", this));

    Allocator<char> buffer(size);
    sourceFile.read(buffer.getElements(), buffer.getSize());
    sourceFile.close();

    FileOutputStream fos(destination, File::CREATE|File::TRUNCATE);
    Huffman huffman;
    huffman.encode(fos, Cast::pointer<const uint8*>(buffer.getElements()), buffer.getSize());
  }
  
  void uncompress(const String& source, const String& destination) throw(InvalidFormat, IOException) {
    File sourceFile(source, File::READ, 0);
    
    unsigned long long size = sourceFile.getSize();
    assert(size <= PrimitiveTraits<unsigned int>::MAXIMUM, IOException("File is too big for me", this));
    
    Allocator<char> buffer(size);
    sourceFile.read(buffer.getElements(), buffer.getSize());
    sourceFile.close();
    
    FileOutputStream fos(destination, File::CREATE|File::TRUNCATE);
    Huffman huffman;
    huffman.decode(fos, Cast::pointer<const uint8*>(buffer.getElements()), buffer.getSize());
  }

  void random(const String& destination) throw(IOException) {
    Random random;
    unsigned int size = random.getInteger() % (64 * 1024);
    
    Allocator<char> buffer(size);
    for (unsigned int i = 0; i < buffer.getSize(); ++i) {
      buffer.getElements()[i] = random.getInteger(); // slow
    }
    
    File file(destination, File::WRITE, File::CREATE|File::TRUNCATE);
    file.write(buffer.getElements(), buffer.getSize());
  }

  void simple(const String& destination) throw(IOException) {
    StringOutputStream stream;
    stream << Application::getFormalName() << MESSAGE(" version 1.0") << EOL
           << MESSAGE("The Base Framework (Test Suite)") << EOL
           << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
           << MESSAGE("Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
           << FLUSH;
    String message = stream.getString();
    FileOutputStream fos(destination, File::CREATE|File::TRUNCATE);
    Huffman huffman;
    huffman.encode(fos, Cast::pointer<const uint8*>(message.getElements()), message.getLength());
  }
  
  void main() throw() {
    fout << Application::getFormalName() << MESSAGE(" version 1.0") << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
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
        ferr << MESSAGE("Error: Invalid argument(s).") << EOL
             << MESSAGE("For help run: ") << getFormalName() << MESSAGE(" --help") << EOL
             << ENDL;
        setExitCode(EXIT_CODE_ERROR);
        break;
      case COMMAND_HELP:
        fout << MESSAGE("Options") << EOL
             << MESSAGE("  --compress source destination") << EOL
             << MESSAGE("  --uncompress source destination") << EOL
             << MESSAGE("  --simple destination") << EOL
             << MESSAGE("  --random destination") << EOL
             << MESSAGE("  --help") << EOL
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

int main(int argc, const char* argv[], const char* env[]) {
  HuffmanApplication application(argc, argv, env);
  try {
    application.main();
  } catch(Exception& e) {
    return Application::getApplication()->exceptionHandler(e);
  } catch(...) {
    return Application::getApplication()->exceptionHandler();
  }
  return Application::getApplication()->getExitCode();
}
