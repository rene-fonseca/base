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
#include <base/string/StringOutputStream.h>

using namespace base;

class SpaceApplication : public Application {
private:
  
  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 1;

  enum Command {
    COMMAND_HELP,
    COMMAND_VERSION,
    COMMAND_SUM
  };
  
  Command command;
  bool showFiles;
  bool showFolders;
  bool summarize;
  uint64 blockSize;
  Array<String> paths;
public:
  
  SpaceApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw()
    : Application(MESSAGE("du"), numberOfArguments, arguments, environment) {
    showFiles = false;
    showFolders = false;
    summarize = false;
    blockSize = 1;
    command = COMMAND_SUM;
  }
  
  void parseArguments() throw() {
    Array<String> arguments = getArguments();
    Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();
    while (enu.hasNext()) {
      String argument = *enu.next();
      if (argument == MESSAGE("--help")) {
        command = COMMAND_HELP;
        return;
      } else if (argument == MESSAGE("--summarize")) {
        summarize = true;
      } else if (argument == MESSAGE("--bytes")) {
        blockSize = 1;
      } else if (argument == MESSAGE("--kilobytes")) {
        blockSize = 1024;
      } else if (argument == MESSAGE("--megabytes")) {
        blockSize = 1024 * 1024;
      } else if (argument == MESSAGE("--files")) {
        showFiles = true;
      } else if (argument == MESSAGE("--folders")) {
        showFolders = true;
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
         << MESSAGE("--bytes     selects output in bytes") << EOL
         << MESSAGE("--kilobytes selects output in kilobytes (kb)") << EOL
         << MESSAGE("--megabytes selects output in megabytes (Mb)") << EOL
         << MESSAGE("--files     show total number of files") << EOL
         << MESSAGE("--folders   show total number of folders") << EOL
         << MESSAGE("--summarize only produce summary") << EOL
         << ENDL;
  }

  struct Sum {
    uint64 bytes;
    unsigned int numberOfFiles;
    unsigned int numberOfFolders;
    unsigned int totalNumberOfFiles;
    unsigned int totalNumberOfFolders;
  };

  void summary(Sum sum, const String& path) throw() {
    StringOutputStream streamFiles;
    streamFiles << sum.numberOfFiles << '/' << sum.totalNumberOfFiles << FLUSH;
    StringOutputStream streamFolders;
    streamFolders << sum.numberOfFolders << '/' << sum.totalNumberOfFolders << FLUSH;
    
    uint64 blockSum = sum.bytes/blockSize;
    fout << setWidth(10) << blockSum << ' ';
    if (showFiles) {
      fout << setWidth(8) << RIGHT << streamFiles.getString() << ' ';
    }
    if (showFolders) {
      fout << setWidth(8) << RIGHT << streamFolders.getString() << ' ';
    }
    fout << path << ENDL;
  }
  
  Sum traverse(const String& folderPath) throw(FileSystemException) {
    FolderInfo folder(folderPath);
    Array<String> entries = folder.getEntries();
    Array<String>::ReadEnumerator enu = entries.getReadEnumerator();
    
    Sum result;
    result.bytes = 0;
    result.numberOfFiles = 0;
    result.numberOfFolders = 0;
    result.totalNumberOfFiles = 0;
    result.totalNumberOfFolders = 0;
    
    while (enu.hasNext()) {
      const String entry = *enu.next();
      if ((entry == MESSAGE(".")) || (entry == MESSAGE(".."))) {
        continue;
      }
      String path = FileSystem::getPath(folderPath, entry);
      try {
        unsigned int type = FileSystem::getType(path);
        if (type & FileSystem::FOLDER) {
          Sum temp = traverse(path);
          result.bytes += temp.bytes;
          result.totalNumberOfFiles += temp.totalNumberOfFiles;
          result.totalNumberOfFolders += temp.totalNumberOfFolders;
          result.numberOfFolders++;
          result.totalNumberOfFolders++;
        } else if (type & FileSystem::REGULAR) {
          result.bytes += FileSystem::getSize(path);
          result.numberOfFiles++;
          result.totalNumberOfFiles++;
        } else {
          // ignore
        }
      } catch (FileSystemException& e) {
        // remember problem
      }
    }
    if (!summarize) {
      summary(result, folderPath);
    }
    return result;
  }
  
  void sum() throw() {
    if (paths.getSize() == 0) {
      paths.append(MESSAGE("."));
    }
    Array<String>::ReadEnumerator enu = paths.getReadEnumerator();
    
    while (enu.hasNext()) {
      String path = *enu.next();
      
      if (!FileSystem::folderExists(path)) {
        ferr << MESSAGE("Error: ") << MESSAGE("Not a folder: ") << path << ENDL;
        setExitCode(EXIT_CODE_ERROR);
        return;
      }
      
      try {
        traverse(path);
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
    case COMMAND_SUM:
      sum();
      break;
    }
  }
  
  ~SpaceApplication() throw() {
  }
};

STUB(SpaceApplication);
