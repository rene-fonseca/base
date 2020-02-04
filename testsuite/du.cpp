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
#include <base/string/StringOutputStream.h>

using namespace com::azure::dev::base;

class SpaceApplication : public Application {
private:
  
  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 1;

  enum Command {
    COMMAND_HELP,
    COMMAND_VERSION,
    COMMAND_SUM
  };
  
  Command command = COMMAND_SUM;
  bool showFiles = false;
  bool showFolders = false;
  bool summarize = false;
  uint64 blockSize = 1;
  Array<String> paths;
public:
  
  SpaceApplication()
    : Application("du")
  {
  }
  
  void parseArguments()
  {
    Array<String> arguments = getArguments();
    Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();
    while (enu.hasNext()) {
      String argument = enu.next();
      if (argument == "--help") {
        command = COMMAND_HELP;
        return;
      } else if (argument == "--summarize") {
        summarize = true;
      } else if (argument == "--bytes") {
        blockSize = 1;
      } else if (argument == "--kilobytes") {
        blockSize = 1024;
      } else if (argument == "--megabytes") {
        blockSize = 1024 * 1024;
      } else if (argument == "--files") {
        showFiles = true;
      } else if (argument == "--folders") {
        showFolders = true;
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
  
  void help() {
    version();
    fout << "Usage: " << getFormalName() << " [options] path(s)" << EOL
         << EOL
         << "--help      this message" << EOL
         << "--version   dump the version" << EOL
         << EOL
         << "--bytes     selects output in bytes" << EOL
         << "--kilobytes selects output in kilobytes (kb)" << EOL
         << "--megabytes selects output in megabytes (Mb)" << EOL
         << "--files     show total number of files" << EOL
         << "--folders   show total number of folders" << EOL
         << "--summarize only produce summary" << EOL
         << ENDL;
  }

  struct Sum {
    uint64 bytes;
    unsigned int numberOfFiles;
    unsigned int numberOfFolders;
    unsigned int totalNumberOfFiles;
    unsigned int totalNumberOfFolders;
  };

  void summary(Sum sum, const String& path) {
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
  
  Sum traverse(const String& folderPath) {
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
      const String entry = enu.next();
      if ((entry == ".") || (entry == "..")) {
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
      } catch (FileSystemException&) {
        // remember problem
      }
    }
    if (!summarize) {
      summary(result, folderPath);
    }
    return result;
  }
  
  void sum() {
    if (paths.getSize() == 0) {
      paths.append(".");
    }
    Array<String>::ReadEnumerator enu = paths.getReadEnumerator();
    
    while (enu.hasNext()) {
      String path = enu.next();
      
      if (!FileSystem::folderExists(path)) {
        ferr << "Error: " << "Not a folder: " << path << ENDL;
        setExitCode(EXIT_CODE_ERROR);
        return;
      }
      
      try {
        traverse(path);
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
    case COMMAND_SUM:
      sum();
      break;
    }
  }
  
  ~SpaceApplication() {
  }
};

APPLICATION_STUB(SpaceApplication);
