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
#include <base/string/FormatOutputStream.h>
#include <base/filesystem/FileSystem.h>
#include <base/filesystem/FolderInfo.h>

using namespace dk::sdu::mip::base;

class RemoveApplication : public Application {
private:
  
  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 1;

  enum Command {
    COMMAND_HELP,
    COMMAND_VERSION,
    COMMAND_REMOVE
  };
  
  Command command;
  bool silent;
  bool recursive;
  bool force;
  Array<String> paths;
public:
  
  RemoveApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw()
    : Application(MESSAGE("rm"), numberOfArguments, arguments, environment) {
    silent = false;
    recursive = false;
    force = false;
    command = COMMAND_REMOVE;
  }
  
  void parseArguments() throw() {
    Array<String> arguments = getArguments();
    Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();
    while (enu.hasNext()) {
      String argument = *enu.next();
      if (argument == MESSAGE("--help")) {
        command = COMMAND_HELP;
        return;
      } else if (argument == MESSAGE("--recursive")) {
        recursive = true;
      } else if (argument == MESSAGE("--force")) {
        force = true;
      } else if (argument == MESSAGE("--silent")) {
        silent = true;
      } else if (argument == MESSAGE("--version")) {
        command = COMMAND_VERSION;
        return;
      } else {
        paths.append(argument);
      }
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
    fout << MESSAGE("Usage: ") << getFormalName() << MESSAGE(" [options] path(s)") << EOL
         << EOL
         << MESSAGE("--help      this message") << EOL
         << MESSAGE("--version   dump the version") << EOL
         << EOL
         << MESSAGE("--recursive enable recursive mode") << EOL
         << MESSAGE("--force     continue on error") << EOL
         << MESSAGE("--silent    disable output") << EOL
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
      if ((entry == MESSAGE(".")) || (entry == MESSAGE(".."))) {
        continue;
      }
      try {
        unsigned int type = FileSystem::getType(entry);
        
        if (type & FileSystem::REGULAR) {
          if (!silent) {
            fout << MESSAGE("removing: ") << entry << ENDL;
          }
          FileSystem::removeFile(entry);
        } else if (type & FileSystem::FOLDER) {
          recursiveRemove(entry);
        } else {
          if (!silent) {
            ferr << MESSAGE("unable to remove entry: ") << entry << ENDL;
          }
          if (!force) {
            throw FileSystemException("Unable to remove entry", this);
          }
        }
      } catch (FileSystemException& e) {
        if (!force) {
          ferr << entry << MESSAGE(": ") << e << ENDL;
          setExitCode(EXIT_CODE_ERROR);
          throw;
        }
      }
    }
    if (!silent) {
      fout << MESSAGE("removing: ") << folderPath << ENDL;
    }
    // FileSystem::setCurrentFolder(originalFolder);
    FileSystem::removeFolder(folderPath);
  }
  
  void remove() throw() {
    Array<String>::ReadEnumerator enu = paths.getReadEnumerator();
    
    while (enu.hasNext()) {
      String path = *enu.next();
      
      if (!FileSystem::entryExists(path)) {
        ferr << MESSAGE("Error: ") << MESSAGE("Entry does not exist: ") << path << ENDL;
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
            fout << MESSAGE("removing: ") << path << ENDL;
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
              fout << MESSAGE("removing: ") << path << ENDL;
            }
            FileSystem::removeFolder(path);
          }
        } else {
          ferr << MESSAGE("Error: ") << MESSAGE("Unable to remove entry: ") << path << ENDL;
          if (!force) {
            setExitCode(EXIT_CODE_ERROR);
            return;
          }
        }
      } catch (FileSystemException& e) {
        ferr << MESSAGE("Error: ") << e.getMessage() << ENDL;
        setExitCode(EXIT_CODE_ERROR);
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
    case COMMAND_REMOVE:
      remove();
      break;
    }
  }
  
  ~RemoveApplication() throw() {
  }
};

STUB(RemoveApplication);
