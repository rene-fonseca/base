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
#include <base/string/FormatOutputStream.h>
#include <base/filesystem/FileSystem.h>
#include <base/filesystem/FolderInfo.h>

using namespace com::azure::dev::base;

class RemoveApplication : public Application {
private:
  
  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 1;

  enum Command {
    COMMAND_HELP,
    COMMAND_VERSION,
    COMMAND_REMOVE
  };
  
  Command command = COMMAND_REMOVE;
  bool silent = false;
  bool recursive = false;
  bool force = false;
  Array<String> paths;
public:
  
  RemoveApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[])
    : Application("rm", numberOfArguments, arguments, environment)
  {
  }
  
  void parseArguments() {
    Array<String> arguments = getArguments();
    Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();
    while (enu.hasNext()) {
      String argument = *enu.next();
      if (argument == "--help") {
        command = COMMAND_HELP;
        return;
      } else if (argument == "--recursive") {
        recursive = true;
      } else if (argument == "--force") {
        force = true;
      } else if (argument == "--silent") {
        silent = true;
      } else if (argument == "--version") {
        command = COMMAND_VERSION;
        return;
      } else {
        paths.append(argument);
      }
    }
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
    fout << "Usage: " << getFormalName() << " [options] path(s)" << EOL
         << EOL
         << "--help      this message" << EOL
         << "--version   dump the version" << EOL
         << EOL
         << "--recursive enable recursive mode" << EOL
         << "--force     continue on error" << EOL
         << "--silent    disable output" << EOL
         << ENDL;
  }

  void recursiveRemove(const String& folderPath) throw(FileSystemException) {
    // String originalFolder = FileSystem::getCurrentFolder();
    
    // FileSystem::setCurrentFolder(path);
    
    FolderInfo folder(folderPath);
    Array<String> entries = folder.getEntries();
    Array<String>::ReadEnumerator enu = entries.getReadEnumerator();
    while (enu.hasNext()) {
      const String entry = *enu.next();
      if ((entry == ".") || (entry == "..")) {
        continue;
      }
      try {
        unsigned int type = FileSystem::getType(entry);
        
        if (type & FileSystem::REGULAR) {
          if (!silent) {
            fout << "removing: " << entry << ENDL;
          }
          FileSystem::removeFile(entry);
        } else if (type & FileSystem::FOLDER) {
          recursiveRemove(entry);
        } else {
          if (!silent) {
            ferr << "unable to remove entry: " << entry << ENDL;
          }
          if (!force) {
            throw FileSystemException("Unable to remove entry", this);
          }
        }
      } catch (FileSystemException& e) {
        if (!force) {
          ferr << entry << ": " << e << ENDL;
          setExitCode(EXIT_CODE_ERROR);
          throw;
        }
      }
    }
    if (!silent) {
      fout << "removing: " << folderPath << ENDL;
    }
    // FileSystem::setCurrentFolder(originalFolder);
    FileSystem::removeFolder(folderPath);
  }
  
  void remove() {
    Array<String>::ReadEnumerator enu = paths.getReadEnumerator();
    
    while (enu.hasNext()) {
      String path = *enu.next();
      
      if (!FileSystem::entryExists(path)) {
        ferr << "Error: " << "Entry does not exist: " << path << ENDL;
        if (!force) {
          setExitCode(EXIT_CODE_ERROR);
          return;
        }
        continue;
      }
      
      try {
        unsigned int type = FileSystem::getType(path);
        
        if (type & FileSystem::REGULAR) {
          if (!silent) {
            fout << "removing: " << path << ENDL;
          }
          FileSystem::removeFile(path);
        } else if (type & FileSystem::FOLDER) {
          if (recursive) {
            if (type & FileSystem::LINK) {
              FileSystem::removeFolder(path);
            } else {
              recursiveRemove(path);
            }
          } else {
            if (!silent) {
              fout << "removing: " << path << ENDL;
            }
            FileSystem::removeFolder(path);
          }
        } else {
          ferr << "Error: " << "Unable to remove entry: " << path << ENDL;
          if (!force) {
            setExitCode(EXIT_CODE_ERROR);
            return;
          }
        }
      } catch (FileSystemException& e) {
        ferr << "Error: " << e.getMessage() << ENDL;
        setExitCode(EXIT_CODE_ERROR);
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
    case COMMAND_REMOVE:
      remove();
      break;
    }
  }
  
  ~RemoveApplication() {
  }
};

APPLICATION_STUB(RemoveApplication);
