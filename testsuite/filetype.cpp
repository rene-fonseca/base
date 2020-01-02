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
#include <base/Timer.h>
#include <base/string/FormatOutputStream.h>
#include <base/filesystem/FolderInfo.h>
#include <base/filesystem/FileInfo.h>
#include <base/filesystem/FileSystem.h>
#include <base/string/StringOutputStream.h>
#include <base/io/FileReader.h>

using namespace com::azure::dev::base;

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
  
  Command command = COMMAND_ERROR;
  bool followLink = false;
  bool force = false;
  bool recursive = false;
  Array<String> paths;
public:
  
  FileTypeApplication()
    : Application("filetype")
  {
  }
  
  void parseArguments()
  {
    Array<String> arguments = getArguments();
    Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();
    while (enu.hasNext()) {
      String argument = *enu.next();
      if (argument == "--help") {
        command = COMMAND_HELP;
        return;
      } else if (argument == "--follow") {
        followLink = true;
      } else if (argument == "--force") {
        force = true;
      } else if (argument == "--recursive") {
        recursive = true;
      } else if (argument == "--version") {
        command = COMMAND_VERSION;
        return;
      } else {
        paths.append(argument);
      }
    }
    
    if (paths.getSize() == 0) {
      ferr << "Error: " << "Path(s) must be specified" << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }   
    
    if (command == COMMAND_ERROR) {
      command = COMMAND_FILE_TYPE;
    }
  }
  
  void version()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << ENDL;
  }
  
  void help() {
    version();
    fout << "Usage: " << getFormalName() << " [options] paths..." << EOL
         << EOL
         << "--help      this message" << EOL
         << "--version   dump the version" << EOL
         << EOL
         << "--follow    follow links" << EOL
         << "--force     continue on errors" << EOL
         << "--recursive traverse folders recursively" << EOL
         << ENDL;
  }

  void onEntry(const String& path) {
    unsigned int type = FileSystem::getType(path);
    
    if ((type & FileSystem::LINK) && followLink) {
      type = FileSystem::getType(FileSystem::getLink(path));
    }
    
    fout << path << ':' << ' ';
    if (type & FileSystem::BLOCK) {
      fout << "BLOCK";
    }
    if (type & FileSystem::CHARACTER) {
      fout << "CHARACTER";
    }
    if (type & FileSystem::DEVICE) {
      fout << "DEVICE";
    }
    if (type & FileSystem::FIFO) {
      fout << "FIFO";
    }
    if (type & FileSystem::FOLDER) {
      fout << "FOLDER";
    }
    if (type & FileSystem::LINK) {
      fout << "LINK";
    }
    if (type & FileSystem::MESSAGE_QUEUE) {
      fout << "MESSAGE QUEUE";
    }
    if (type & FileSystem::REGULAR) {
      fout << "REGULAR";
    }
    if (type & FileSystem::SOCKET) {
      fout << "SOCKET";
    }
    if (type & FileSystem::SEMPAHORE) {
      fout << "SEMAPHORE";
    }
    if (type & FileSystem::SHARED_MEMORY) {
      fout << "SHARED MEMORY";
    }
    fout << ENDL;
  }
  
  void onFolder(const String& folderPath) {
    FolderInfo folder(folderPath);
    Array<String> entries = folder.getEntries();
    Array<String>::ReadEnumerator enu = entries.getReadEnumerator();
    while (enu.hasNext()) {
      const String entry = *enu.next();
      if ((entry == ".") || (entry == "..")) {
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
          fout << fullPath << ':' << ' ' << "unable to get examine" << ENDL;
          continue;
        }
        
        ferr << entry << ": " << e << ENDL;
        setExitCode(EXIT_CODE_ERROR);
        _rethrow;
      }
    }
  }
  
  void type() {
    Array<String>::ReadEnumerator enu = paths.getReadEnumerator();
    
    while (enu.hasNext()) {
      String path = *enu.next();
      
      if (!FileSystem::entryExists(path)) {
        if (force) {
          fout << path << ':' << ' ' << "does not exist" << ENDL;
          continue;
        }
        ferr << "Error: " << "Entry does not exist: " << path << ENDL;
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
          fout << path << ':' << ' ' << "unable to examine" << ENDL;
          continue;
        }
        ferr << path << ": " << e << ENDL; // ignore exception
        setExitCode(EXIT_CODE_ERROR);
        return;
      }
    }
  }
  
  void main() {
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
  
  ~FileTypeApplication() {
  }
};

APPLICATION_STUB(FileTypeApplication);
