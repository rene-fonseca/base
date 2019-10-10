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

class CopyApplication : public Application {
private:
  
  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 1;
  static const unsigned int UPDATE_TIME = 500000;
  
  enum Command {
    COMMAND_ERROR,
    COMMAND_HELP,
    COMMAND_VERSION,
    COMMAND_COPY
  };
  
  Command command;
  bool force;
  bool progress;
  bool silent;
  bool statistics;
  String source;
  String destination;
public:
  
  CopyApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application("cp", numberOfArguments, arguments, environment) {
    command = COMMAND_ERROR;
    force = false;
    progress = false;
    silent = false;
    statistics = false;
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
      } else if (argument == "--force") {
        force = true;
      } else if (argument == "--progress") {
        progress = true;
      } else if (argument == "--silent") {
        silent = true;
      } else if (argument == "--version") {
        command = COMMAND_VERSION;
        return;
      } else {
        if (!sourceSpecified) {
          source = argument;
          sourceSpecified = true;
        } else if (!destinationSpecified) {
          destination = argument;
          destinationSpecified = true;
        } else {
          ferr << "Error: " << "Invalid argument" << ENDL;
          setExitCode(EXIT_CODE_ERROR);
          return;
        }
      }
    }
    
    if ((!sourceSpecified) && (!destinationSpecified)) {
      ferr << "Error: " << "Source and destination must be specified" << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }
    
    if (!sourceSpecified) {
      ferr << "Error: " << "Source must be specified" << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }
    
    if (!destinationSpecified) {
      ferr << "Error: " << "Destination must be specified" << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }

    if (command == COMMAND_ERROR) {
      command = COMMAND_COPY;
    }
  }
  
  void version() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "Copyright (C) 2002-2019 by Rene Moeller Fonseca" << EOL
         << ENDL;
  }
  
  void help() throw() {
    version();
    fout << "Usage: " << getFormalName() << " [options] source destination" << EOL
         << EOL
         << "Options:" << EOL
         << indent(2) << "--help      this message" << EOL
         << indent(2) << "--version   dump the version" << EOL
         << EOL
         << indent(2) << "--force     force copying" << EOL
         << indent(2) << "--link      make link" << EOL
         << indent(2) << "--preserve  preserve attributes" << EOL
         << indent(2) << "--progress  show progress while copying" << EOL
         << indent(2) << "--recursive copy folders recursively" << EOL
         << indent(2) << "--silent    do not output" << EOL
         << ENDL;
  }
  
  void copyFolder(const String& source, const String& destination) throw() {
    // TAG: fixme
  }
  
  String getProgress(long long position, long long size, long long elapsedTime) throw() {
    unsigned int percent = position * 100/size;
    long long rate = (position * 1000000)/elapsedTime;
    long long remainingBytes = size - position;
    long long remainingTime = elapsedTime * remainingBytes/(position + 1)/1000000;
    
    StringOutputStream stream;
    stream << position << '/' << size << ' '
           << '(' << percent << '%' << ')' << ' '
           << "rate:" << rate/1024 << "kb/s" << ' ';
    
    stream << "remaining:";
    if (remainingTime < 60) {
      stream << remainingTime << 's';
    } else if (remainingTime < 60*60) {
      stream << remainingTime/60 << 'm' << ':' << remainingTime%60 << 's';
    } else {
      stream << remainingTime/(60*60) << 'h' << ':' << remainingTime/60%60 << 'm';
    }
    stream << ' ';
    
    stream << "elapsed:";
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
  
  void copy() throw() {
    if (!FileSystem::fileExists(source)) {
      ferr << "Error: " << "Source does not exist" << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }

    if (!force && FileSystem::fileExists(destination)) {
      ferr << "Error: " << "Destination already exists" << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }
    
    if (!silent) {
      fout << "copying: " << source << " -> " << destination << ENDL;
    }
    
    try {
      unsigned int type = FileSystem::getType(source);
      unsigned int previousLength = 0;
      
      if (type & FileSystem::REGULAR) {
        Timer timer;
        Timer updateTimer;
        try {
          File sourceFile(source, File::READ, 0);
          File destinationFile(
            destination,
            File::WRITE,
            (force ? File::TRUNCATE : File::EXCLUSIVE) | File::CREATE
          );
          FileReader reader(sourceFile, 0);
          long long position = 0;
          const long long size = sourceFile.getSize();
          
          if (progress) {
            String temp = getProgress(
              position,
              size,
              timer.getLiveMicroseconds()
            );
            fout << setWidth(previousLength) << temp << '\r' << FLUSH;
            previousLength = temp.getLength();
          }
          
          while (position < size) {
            if (progress) {
              if (updateTimer.getLiveMicroseconds() >= UPDATE_TIME) {
                updateTimer.start();
                String temp = getProgress(
                  position,
                  size,
                  timer.getLiveMicroseconds()
                );
                fout << setWidth(previousLength) << temp << '\r' << FLUSH;
                previousLength = temp.getLength();
              }
            }
            reader.seek(position);
            destinationFile.write(reader.getBytes(), reader.getSize());
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
    case COMMAND_COPY:
      copy();
      break;
    default:
      break;
    }
  }
  
  ~CopyApplication() throw() {
  }
};

STUB(CopyApplication);
