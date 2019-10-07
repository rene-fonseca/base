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
#include <base/string/FormatOutputStream.h>
#include <base/filesystem/FolderInfo.h>
#include <base/filesystem/FileInfo.h>
#include <base/filesystem/FileSystem.h>
#include <base/string/StringOutputStream.h>
#include <base/io/FileReader.h>

using namespace dk::sdu::mip::base;

class ElfInfoApplication : public Application {
private:
  
  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
  
  enum Command {
    COMMAND_ERROR,
    COMMAND_HELP,
    COMMAND_VERSION,
    COMMAND_DUMP
  };
  
  Command command;
  String path;

  // byte order?
  struct ElfHeader {
    uint8 identifier[16];
    uint16 type;
    uint16 machine;
    uint32 version;
    uint32 entry;
    uint32 phoff;
    uint32 shoff;
    uint32 flags;
    uint16 ehsize;
    uint16 phentsize;
    uint16 phnum;
    uint16 shentsize;
    uint16 shnum;
    uint16 shstrndx;
  } _DK_SDU_MIP__BASE__PACKED;

  // type
  enum {
    ET_NONE = 0, // no file type
    ET_REL = 1, // relocateable file
    ET_EXEC = 2, // executable file
    ET_DYN = 3, // shared object
    ET_CORE = 4, // core file
    ET_LOPROC = 0xff00, // processor specific
    ET_HIPROC = 0xffff // processor specific
  };

  // machine
  enum {
    EM_NONE = 0, // no machine
    EM_M32 = 1, // AT&T WE 32100
    EM_SPARC = 2, // SPARC
    EM_386 = 3, // Intel 80386
    EM_68K = 4, // Motorola 68000
    EM_88K = 5, // Motorola 88000
    EM_860 = 7, // Intel 80860
    EM_MIPS = 8 // MIPS RS3000
  };

  // version
  enum {
    EV_NONE = 0, // invalid version
    EV_CURRENT = 1 // current version
  };
  
public:
  
  ElfInfoApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application("elfinfo", numberOfArguments, arguments, environment) {
    command = COMMAND_ERROR;
  }
  
  void parseArguments() throw() {
    bool pathSpecified = false;
    
    Array<String> arguments = getArguments();
    Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();
    while (enu.hasNext()) {
      String argument = *enu.next();
      if (argument == "--help") {
        command = COMMAND_HELP;
        return;
      } else if (argument == "--version") {
        command = COMMAND_VERSION;
        return;
      } else {
        if (!pathSpecified) {
          path = argument;
          pathSpecified = true;
        } else {
          ferr << "Error: " << "Invalid argument" << ENDL;
          setExitCode(EXIT_CODE_ERROR);
          return;
        }
      }
    }
    
    if (!pathSpecified) {
      ferr << "Error: " << "Path must be specified" << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }

    if (command == COMMAND_ERROR) {
      command = COMMAND_DUMP;
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
    fout << "Usage: " << getFormalName() << " [options] path" << EOL
         << EOL
         << "Options:" << EOL
         << indent(2) << "--help      this message" << EOL
         << indent(2) << "--version   dump the version" << EOL
         << EOL
         << ENDL;
  }
  
  void dump() throw() {
    if (!FileSystem::fileExists(source)) {
      ferr << "Error: " << "Source does not exist" << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }
    
    try {
      unsigned int type = FileSystem::getType(source);
      unsigned int previousLength = 0;
      
      if (type & FileSystem::REGULAR) {
        Timer timer;
        Timer updateTimer;
        try {
          File sourceFile(source, File::READ, 0);
          FileReader reader(sourceFile, 0);
          long long position = 0;
          const long long size = sourceFile.getSize();
          
          if (progress) {
            String temp = getProgress(position, size, timer.getLiveMicroseconds());
            fout << setWidth(previousLength) << temp << '\r' << FLUSH;
            previousLength = temp.getLength();
          }

          ElfHeader header;
          if (file.getSize() < sizeof(header)) {
            ferr << "not elf object" << ENDL;
            return; // TAG: fixme
          }

          // check if valid ELF header
          header.identification[0] == 0x7f;
          header.identification[1] == 'E';
          header.identification[2] == 'L';
          header.identification[3] == 'F';
          
          while (position < size) {
            reader.seek(position);
            position += reader.getSize();
          }
          
          if (progress) {
            String temp = getProgress(size, size, timer.getLiveMicroseconds());
            fout << setWidth(previousLength) << temp << '\r' << ENDL;
          }
        } catch (FileSystemException& e) {
          ferr << source << ": " << e << ENDL; // ignore exception
          setExitCode(EXIT_CODE_ERROR);
          return;
        }
      } else if (type & FileSystem::FOLDER) {
        ferr << "Error: " << "Folder not supported" << ENDL;
        setExitCode(EXIT_CODE_ERROR);
        return;
      } else { // unknown entry type
        ferr << "Error: " << "Unknown entry" << ENDL;
        setExitCode(EXIT_CODE_ERROR);
        return;
      }
    } catch (IOException& e) {
      ferr << source << ": " << e << ENDL; // ignore exception
      setExitCode(EXIT_CODE_ERROR);
      return;
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
    case COMMAND_DUMP:
      copy();
      break;
    default:
      break;
    }
  }
  
  ~ElfInfoApplication() throw() {
  }
};

STUB(ElfInfoApplication);