/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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

using namespace base;

class FileTypeApplication : public Application {
private:
  
  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 1;
  
  enum Command {
    COMMAND_ERROR,
    COMMAND_HELP,
    COMMAND_VERSION,
    COMMAND_FILE_TYPE
  };
  
  Command command;
  bool followLink;
  bool force;
  bool recursive;
  Array<String> paths;
public:
  
  FileTypeApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw()
    : Application(MESSAGE("filetype"), numberOfArguments, arguments, environment) {
    command = COMMAND_ERROR;
    force = false;
    followLink = false;
    recursive = false;
  }
  
  void parseArguments() throw() {
    Array<String> arguments = getArguments();
    Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();
    while (enu.hasNext()) {
      String argument = *enu.next();
      if (argument == MESSAGE("--help")) {
        command = COMMAND_HELP;
        return;
      } else if (argument == MESSAGE("--follow")) {
        followLink = true;
      } else if (argument == MESSAGE("--force")) {
        force = true;
      } else if (argument == MESSAGE("--recursive")) {
        recursive = true;
      } else if (argument == MESSAGE("--version")) {
        command = COMMAND_VERSION;
        return;
      } else {
        paths.append(argument);
      }
    }
    
    if (paths.getSize() == 0) {
      ferr << MESSAGE("Error: ") << MESSAGE("Path(s) must be specified") << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }   
    
    if (command == COMMAND_ERROR) {
      command = COMMAND_FILE_TYPE;
    }
  }
  
  void version() throw() {
    fout << getFormalName() << MESSAGE(" version ") << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;
  }
  
  void help() throw() {
    version();
    fout << MESSAGE("Usage: ") << getFormalName() << MESSAGE(" [options] paths...") << EOL
         << EOL
         << MESSAGE("--help      this message") << EOL
         << MESSAGE("--version   dump the version") << EOL
         << EOL
         << MESSAGE("--follow    follow links") << EOL
         << MESSAGE("--force     continue on errors") << EOL
         << MESSAGE("--recursive traverse folders recursively") << EOL
         << ENDL;
  }

  void onEntry(const String& path) throw(FileSystemException) {
    unsigned int type = FileSystem::getType(path);
    
    if ((type & FileSystem::LINK) && followLink) {
      type = FileSystem::getType(FileSystem::getLink(path));
    }
    
    fout << path << ':' << ' ';
    if (type & FileSystem::BLOCK) {
      fout << MESSAGE("BLOCK");
    }
    if (type & FileSystem::CHARACTER) {
      fout << MESSAGE("CHARACTER");
    }
    if (type & FileSystem::DEVICE) {
      fout << MESSAGE("DEVICE");
    }
    if (type & FileSystem::FIFO) {
      fout << MESSAGE("FIFO");
    }
    if (type & FileSystem::FOLDER) {
      fout << MESSAGE("FOLDER");
    }
    if (type & FileSystem::LINK) {
      fout << MESSAGE("LINK");
    }
    if (type & FileSystem::MESSAGE_QUEUE) {
      fout << MESSAGE("MESSAGE QUEUE");
    }
    if (type & FileSystem::REGULAR) {
      fout << MESSAGE("REGULAR");
    }
    if (type & FileSystem::SOCKET) {
      fout << MESSAGE("SOCKET");
    }
    if (type & FileSystem::SEMPAHORE) {
      fout << MESSAGE("SEMAPHORE");
    }
    if (type & FileSystem::SHARED_MEMORY) {
      fout << MESSAGE("SHARED MEMORY");
    }
    fout << ENDL;
  }
  
  void onFolder(const String& folderPath) throw(FileSystemException) {
    FolderInfo folder(folderPath);
    Array<String> entries = folder.getEntries();
    Array<String>::ReadEnumerator enu = entries.getReadEnumerator();
    while (enu.hasNext()) {
      const String entry = *enu.next();
      if ((entry == MESSAGE(".")) || (entry == MESSAGE(".."))) {
        continue;
      }
      
      String fullPath = FileSystem::getPath(folderPath, entry);
      try {
        unsigned int type = FileSystem::getType(fullPath);
        onEntry(fullPath);
        
        if (type & FileSystem::FOLDER) {
          onFolder(fullPath);
        }
      } catch (FileSystemException& e) {
        if (force) {
          fout << fullPath << ':' << ' ' << MESSAGE("unable to get examine") << ENDL;
          continue;
        }
        
        ferr << entry << MESSAGE(": ") << e << ENDL;
        setExitCode(EXIT_CODE_ERROR);
        throw;
      }
    }
  }
  
  void type() throw() {
    Array<String>::ReadEnumerator enu = paths.getReadEnumerator();
    
    while (enu.hasNext()) {
      String path = *enu.next();
      
      if (!FileSystem::entryExists(path)) {
        if (force) {
          fout << path << ':' << ' ' << MESSAGE("does not exist") << ENDL;
          continue;
        }
        ferr << MESSAGE("Error: ") << MESSAGE("Entry does not exist: ") << path << ENDL;
        setExitCode(EXIT_CODE_ERROR);
        return;
      }
      
      try {
        unsigned int type = FileSystem::getType(path);
        onEntry(path);
        if (recursive && (type & FileSystem::FOLDER)) {
          onFolder(path);
        }
      } catch (FileSystemException& e) {
        if (force) {
          fout << path << ':' << ' ' << MESSAGE("unable to examine") << ENDL;
          continue;
        }
        ferr << path << MESSAGE(": ") << e << ENDL; // ignore exception
        setExitCode(EXIT_CODE_ERROR);
        return;
      }
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
    case COMMAND_FILE_TYPE:
      type();
      break;
    default:
      break;
    }
  }
  
  ~FileTypeApplication() throw() {
  }
};

STUB(FileTypeApplication);
