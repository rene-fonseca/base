/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/StringOutputStream.h>
#include <base/io/FileOutputStream.h>
#include <base/compression/BZip2Deflater.h>
#include <base/compression/BZip2Inflater.h>
#include <base/compression/ZLibDeflater.h>
#include <base/compression/ZLibInflater.h>
#include <base/filesystem/FileSystem.h>
#include <base/Timer.h>
#include <base/UnsignedInteger.h>
#include <base/mem/OwnershipPointer.h>

using namespace dk::sdu::mip::base;

class CompressApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
  /** The input and output buffer size. */
  static const unsigned int BUFFER_SIZE = 4096 * 4;
  /** The update time use when progress is enabled. */
  static const unsigned int UPDATE_TIME = 500000;
  
  enum Command {
    COMMAND_ERROR,
    COMMAND_HELP,
    COMMAND_VERSION,
    COMMAND_COMPRESS,
    COMMAND_DECOMPRESS
  };

  enum Method {
    METHOD_BZIP2,
    METHOD_ZLIB
  };

  bool debug;
  Command command;
  Method method;
  unsigned int level;
  bool force;
  bool progress;
  String sourceFile;
  String destinationFile;
public:
  
  CompressApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application("compress", numberOfArguments, arguments, environment) {
    debug = false;
    command = COMMAND_ERROR;
    method = METHOD_BZIP2;
    level = 9;
    force = false;
    progress = false;
  }

  void onTermination() throw() {
    fout << ENDL;
  }
  
  void error(const String& message) throw() {
    ferr << "Error: " << message << ENDL;
    setExitCode(EXIT_CODE_ERROR);
    command = COMMAND_ERROR;
  }
  
  void parseArguments() throw() {
    bool sourceSpecified = false;
    bool destinationSpecified = false;
    
    Array<String> arguments = getArguments();
    Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();
    while (enu.hasNext()) {
      String argument = *enu.next();
      if (argument == "--help") {
        command = COMMAND_HELP;
        return;
      } else if (argument == "--compress") {
        command = COMMAND_COMPRESS;
      } else if (argument == "--decompress") {
        command = COMMAND_DECOMPRESS;
      } else if (argument == "--force") {
        force = true;
      } else if (argument == "--progress") {
        progress = true;
      } else if (argument == "--verbose") {
        debug = true;
      } else if (argument == "--version") {
        command = COMMAND_VERSION;
        return;
      } else if (argument == "--level") {
        if (!enu.hasNext()) {
          error("Compression level is missing");
          return;
        }
        try {
          level = UnsignedInteger::parse(*enu.next(), UnsignedInteger::DEC);
        } catch (InvalidFormat& e) {
          level = 0; // force error
        }
        if ((level < 1) || (level > 9)) {
          error("Invalid compression level");
          return;
        }
      } else if (argument == "--method") {
        if (!enu.hasNext()) {
          error("Compression method is missing");
          return;
        }
        String argument = *enu.next();
        if (argument == "BZIP2") {
          method = METHOD_BZIP2;
        } else if (argument == "ZLIB") {
          method = METHOD_ZLIB;
        } else {
          error("Invalid compression method (BZIP2 or ZLIB)");
          return;
        }        
      } else {
        if (!sourceSpecified) {
          sourceFile = argument;
          sourceSpecified = true;
        } else if (!destinationSpecified) {
          destinationFile = argument;
          destinationSpecified = true;
        } else {
          error("Invalid argument");
          return;
        }
      }
    }
    
    if ((!sourceSpecified) && (!destinationSpecified)) {
      error("Source and destination must be specified");
      return;
    }
    
    if (!sourceSpecified) {
      error("Source must be specified");
      return;
    }
    
    if (!destinationSpecified) {
      error("Destination must be specified");
      return;
    }
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
    fout << "Usage: " << getFormalName() << " [options] source destination" << EOL
         << EOL
         << "Options:" << EOL
         << indent(2) << "--help          this message" << EOL
         << indent(2) << "--version       shows the version" << EOL
         << EOL
         << indent(2) << "--compress      source destination" << EOL
         << indent(2) << "--decompress    source destination" << EOL
         << indent(2) << "--method        selects between BZIP2 and ZLIB" << EOL
         << indent(2) << "--progress      show progress" << EOL
         << indent(2) << "--level [1-9]   selects the compression level [1-9]" << EOL
         << ENDL;
  }

  String getProgress(long long position, long long size, long long compressedSize, long long elapsedTime) throw() {
    unsigned int percent = position * 100/size;
    long long rate = (position * 1000000)/elapsedTime;
    long long remainingBytes = size - position;
    long long remainingTime = elapsedTime * remainingBytes/(position + 1)/1000000;
    double ratio = position ? (compressedSize * 1.0)/position : 1;
    long long expectedSize = (position < size) ? static_cast<long long>(size * ratio) : compressedSize;
    
    StringOutputStream stream;
    stream << position << '/' << size << SP
           << '(' << percent << '%' << ')' << SP
           << "> " << compressedSize << '/' << expectedSize << SP
           << '(' << setPrecision(1) << 100*ratio << '%' << ')' << SP
           << "ra:" << rate/1024 << "kb/s" << SP;
    
    stream << "re:";
    if (remainingTime < 60) {
      stream << remainingTime << 's';
    } else if (remainingTime < 60*60) {
      stream << remainingTime/60 << 'm' << ':' << remainingTime%60 << 's';
    } else {
      stream << remainingTime/(60*60) << 'h' << ':' << remainingTime/60%60 << 'm';
    }
    stream << ' ';
    
    stream << "el:";
    elapsedTime /= 1000000;
    if (elapsedTime < 60) {
      stream << elapsedTime << 's';
    } else if (elapsedTime < 60*60) {
      stream << elapsedTime/60 << 'm' << ':' << elapsedTime%60 << 's';
    } else {
      stream << elapsedTime/(60*60) << 'h' << ':' << elapsedTime/60%60 << 'm';
    }
    
    stream << FLUSH;
    return stream.getString();
  }

  void compress(const String& source, const String& destination) throw() {
    Allocator<uint8> inputBuffer(BUFFER_SIZE);
    Allocator<uint8> outputBuffer(BUFFER_SIZE);

    if (!force && FileSystem::fileExists(destination)) {
      error("Destination already exists");
      return;
    }
    
    File sourceFile(source, File::READ, 0);
    FileOutputStream fos(
      destination,
      (force ? File::TRUNCATE : File::EXCLUSIVE) | File::CREATE
    );
    
    long long size = sourceFile.getSize();
    long long compressedSize = 0;
    long long position = 0;
    
    OwnershipPointer<Deflater> deflater;
    switch (method) {
    case METHOD_BZIP2:
      deflater = new BZip2Deflater(level);
      break;
    case METHOD_ZLIB:
      deflater = new ZLibDeflater(level);
      break;
    }
    
    unsigned int previousLength = 0; // length of previous progress line
    Timer timer;
    Timer updateTimer;
    
    if (progress) {
      String temp = getProgress(
        position,
        size,
        compressedSize,
        timer.getLiveMicroseconds()
      );
      fout << setWidth(previousLength) << temp << '\r' << (debug ? ENDL : FLUSH);
      previousLength = temp.getLength();
    }
    
    while ((position < size) && !isTerminated()) {
      unsigned int bytesToCompress =
        minimum<long long>(size - position, inputBuffer.getSize());
      unsigned int bytesRead = sourceFile.read(
        inputBuffer.getElements(),
        bytesToCompress
      );
      
      if (debug) {
        fout << "DEBUG: read"
             << " position:" << position
             << " size:" << bytesRead
             << ENDL;
      }
      
      position += bytesRead;

      unsigned int offset = 0;
      while (offset < bytesToCompress) {
        unsigned int pushed =
          deflater->push(
            inputBuffer.getElements() + offset,
            bytesToCompress - offset
          );
        if (debug) {
          fout << "DEBUG: push"
               << " offset:" << offset
               << " size:" << bytesToCompress - offset
               << " pushed:" << pushed
               << ENDL;
        }
        offset += pushed;
        while (true) { // end of stream not possible before pushEnd()
          unsigned int compressedBytes = deflater->pull(
            outputBuffer.getElements(),
            outputBuffer.getSize()
          );
          if (debug) {
            fout << "DEBUG: pull"
                 << " size:" << outputBuffer.getSize()
                 << " pulled:" << compressedBytes
                 << ENDL;
          }
          if (compressedBytes == 0) {
            break;
          }
          fos.write(outputBuffer.getElements(), compressedBytes);
          if (debug) {
            fout << "DEBUG: write"
                 << " position:" << compressedSize
                 << " size:" << compressedBytes
                 << ENDL;
          }
          compressedSize += compressedBytes;
          if (compressedBytes != outputBuffer.getSize()) {
            break;
          }
        }
        
        if (isTerminated()) {
          return;
        }
        
        if (progress && (updateTimer.getLiveMicroseconds() >= UPDATE_TIME)) {
          updateTimer.start();
          String temp = getProgress(position, size, compressedSize, timer.getLiveMicroseconds());
          fout << setWidth(previousLength) << temp << '\r' << (debug ? ENDL : FLUSH);
          previousLength = temp.getLength();
        }
      }
    }
    
    deflater->pushEnd();
    
    while (!deflater->atEnd()) {
      unsigned int compressedBytes = deflater->pull(
        outputBuffer.getElements(),
        outputBuffer.getSize()
      );
      if (debug) {
        fout << "DEBUG: pull"
             << " size:" << outputBuffer.getSize()
             << " pulled:" << compressedBytes
             << ENDL;
      }
      fos.write(outputBuffer.getElements(), compressedBytes);
      if (debug) {
        fout << "DEBUG: write"
             << " position:" << compressedSize
             << " size:" << compressedBytes
             << ENDL;
      }
      compressedSize += compressedBytes;
      if (compressedBytes != outputBuffer.getSize()) {
        break;
      }
    }
    
    if (progress) {
      String temp = getProgress(
        size,
        size,
        compressedSize,
        timer.getLiveMicroseconds()
      );
      fout << setWidth(previousLength) << temp << '\r' << ENDL;
    }
  }
  
  void decompress(const String& source, const String& destination) throw() {
    Allocator<uint8> inputBuffer(BUFFER_SIZE);
    Allocator<uint8> outputBuffer(BUFFER_SIZE);

    if (!force && FileSystem::fileExists(destination)) {
      error("Destination already exists");
      return;
    }
    
    File sourceFile(source, File::READ, 0);
    FileOutputStream fos(
      destination,
      (force ? File::TRUNCATE : File::EXCLUSIVE) | File::CREATE
    );
    
    long long size = sourceFile.getSize();
    long long decompressedSize = 0;
    long long position = 0;
    
    OwnershipPointer<Inflater> inflater;
    switch (method) {
    case METHOD_BZIP2:
      inflater = new BZip2Inflater();
      break;
    case METHOD_ZLIB:
      inflater = new ZLibInflater();
      break;
    }
    
    unsigned int previousLength = 0; // length of previous progress line
    Timer timer;
    Timer updateTimer;
    
    if (progress) {
      String temp = getProgress(
        position,
        size,
        decompressedSize,
        timer.getLiveMicroseconds()
      );
      fout << setWidth(previousLength)
           << temp << '\r' << (debug ? ENDL : FLUSH);
      previousLength = temp.getLength();
    }
    
    while ((position < size) && !isTerminated()) {
      unsigned int bytesToDecompress =
        minimum<long long>(size - position, inputBuffer.getSize());
      unsigned int bytesRead = sourceFile.read(
        inputBuffer.getElements(),
        bytesToDecompress
      );
      
      if (debug) {
        fout << "DEBUG: read"
             << " position:" << position
             << " size:" << bytesRead
             << ENDL;
      }
      
      position += bytesRead;
      
      unsigned int offset = 0;
      while (offset < bytesToDecompress) {
        unsigned int pushed = inflater->push(
          inputBuffer.getElements() + offset,
          bytesToDecompress - offset
        );
        if (debug) {
          fout << "DEBUG: push"
               << " offset:" << offset
               << " size:" << bytesToDecompress - offset
               << " pushed:" << pushed
               << ENDL;
        }
        offset += pushed;
        while (!inflater->atEnd()) {
          unsigned int decompressedBytes = inflater->pull(
            outputBuffer.getElements(),
            outputBuffer.getSize()
          );
          
          if (debug) {
            fout << "DEBUG: pull"
                 << " size:" << outputBuffer.getSize()
                 << " pulled:" << decompressedBytes
                 << ENDL;
          }
          
          if (decompressedBytes == 0) {
            break;
          }
          fos.write(outputBuffer.getElements(), decompressedBytes);
          
          if (debug) {
            fout << "DEBUG: write"
                 << " position:" << decompressedSize
                 << " size:" << decompressedBytes
                 << ENDL;
          }
          
          decompressedSize += decompressedBytes;
          if (decompressedBytes != outputBuffer.getSize()) {
            break;
          }
        }
  
        if (isTerminated()) {
          return;
        }
        
        if (progress && (updateTimer.getLiveMicroseconds() >= UPDATE_TIME)) {
          updateTimer.start();
          String temp = getProgress(
            position,
            size,
            decompressedSize,
            timer.getLiveMicroseconds()
          );
          fout << setWidth(previousLength) << temp << '\r'
               << (debug ? ENDL : FLUSH);
          previousLength = temp.getLength();
        }
      }
    }

    inflater->pushEnd(); // ok if eof already has been reached
    
    while (!inflater->atEnd()) {
      unsigned int decompressedBytes = inflater->pull(
        outputBuffer.getElements(),
        outputBuffer.getSize()
      );
      
      if (debug) {
        fout << "DEBUG: pull"
             << " size:" << outputBuffer.getSize()
             << " pulled:" << decompressedBytes
             << ENDL;
      }
      
      fos.write(outputBuffer.getElements(), decompressedBytes);
      
      if (debug) {
        fout << "DEBUG: write"
             << " position:" << decompressedSize
             << " size:" << decompressedBytes
             << ENDL;
      }
      
      decompressedSize += decompressedBytes;
      if (decompressedBytes != outputBuffer.getSize()) {
        if (!inflater->atEnd()) {
          error("Invalid compressed data");
          return;
        }
        break;
      }
    }
    
    if (progress) {
      String temp = getProgress(
        size,
        size,
        decompressedSize,
        timer.getLiveMicroseconds()
      );
      fout << setWidth(previousLength) << temp << '\r' << ENDL;
    }
  }
  
  void main() throw() {
    parseArguments();
    
    switch (command) {
    case COMMAND_HELP:
      help();
      break;
    case COMMAND_VERSION:
      version();
      break;
    case COMMAND_COMPRESS:
      compress(sourceFile, destinationFile);
      break;
    case COMMAND_DECOMPRESS:
      decompress(sourceFile, destinationFile);
      break;
    default:
      break;
    }
  }

  ~CompressApplication() throw() {
  }
};

STUB(CompressApplication);
