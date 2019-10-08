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
#include <base/filesystem/FolderInfo.h>
#include <base/filesystem/FileInfo.h>
#include <base/filesystem/FileSystem.h>
#include <base/TypeInfo.h>
#include <base/string/StringOutputStream.h>
#include <base/collection/HashTable.h>
#include <base/string/ANSIEscapeSequence.h>

using namespace dk::sdu::mip::base;

class ListApplication : public Application {
private:
  
  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 1;
  static const unsigned int MAXIMUM_TRUSTEE_LENGTH = 10;
  
  static const ANSIEscapeSequence::Color COLOR_FOLDER =
    ANSIEscapeSequence::BLUE;
  static const ANSIEscapeSequence::Color COLOR_EXECUTABLE =
    ANSIEscapeSequence::YELLOW;
  static const ANSIEscapeSequence::Color COLOR_LINK =
    ANSIEscapeSequence::WHITE;

  enum Command {
    COMMAND_HELP,
    COMMAND_VERSION,
    COMMAND_LIST
  };
  
  int currentYear;
  String thisYearFormat;
  String otherYearFormat;
  bool limitTrustees;
  bool colorize;
  Command command;
  String path;
public:
  
  ListApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application("ls", numberOfArguments, arguments, environment) {
    
    currentYear = Date::getNow().getYear();
    thisYearFormat = MESSAGE("%b %#d %H:%M");
    otherYearFormat = MESSAGE("%b %#d  %Y");
    limitTrustees = false;
    colorize = false;
    command = COMMAND_LIST;
    path = MESSAGE(".");
  }
  
  inline String getTime(const Date& date) const throw() {
    return date.format(
      (date.getYear() == currentYear) ? thisYearFormat : otherYearFormat,
      true
    );
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
      } else if (argument == "--color") {
        colorize = true;
      } else if (argument == "--nocolor") {
        colorize = false;
      } else if (argument == "--limit") {
        limitTrustees = true;
      } else if (argument == "--nolimit") {
        limitTrustees = false;
      } else if (argument == "--version") {
        command = COMMAND_VERSION;
        return;
      } else {
        if (pathSpecified) {
          ferr << "Error: " << "Invalid argument" << ENDL;
          setExitCode(EXIT_CODE_ERROR);
          return;
        }
        path = argument;
        pathSpecified = true;
      }
    }
  }

  void version() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "https://www.fonseca.dk/base" << EOL
         << "Copyright (C) 2002-2019 by Rene Moeller Fonseca" << EOL
         << ENDL;
  }
  
  void help() throw() {
    version();
    fout << "Usage: " << getFormalName() << " [options] [path]" << EOL
         << EOL
         << "--help      this message" << EOL
         << "--version   dump the version" << EOL
         << EOL
         << "--color     enable colorization of output" << EOL
         << "--nocolor   disable colorization of output" << EOL
         << "--limit     limit trustee fields to fixed size" << EOL
         << "--nolimit   disable fixed trustee fields" << EOL
         << ENDL;
  }

  void list() throw() {
    if (!FileSystem::folderExists(path)) {
      ferr << "Error: " << "Path does not exist" << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }
    
    FolderInfo folder(path);
    Array<String> entries = folder.getEntries();
    
    FileSystem::setCurrentFolder(path);
    
    HashTable<User, String> owners;
    HashTable<Group, String> groups;
    
    Array<String>::ReadEnumerator enu = entries.getReadEnumerator();
    while (enu.hasNext()) {
      const String entry = *enu.next();
      try {
        unsigned int type = FileSystem::getType(entry);
        
        bool link = false;
        bool linkTarget = false;
        String target;
        try {
          if (FileSystem::isLink(entry)) {
            link = true;
            target = FileSystem::getLink(entry);
            linkTarget = true;
          }
        } catch (Exception& e) {
          if (link && !linkTarget) {
            target = MESSAGE("<unknown>");
          }
          ferr << entry << ": " << e << ENDL; // ignore exception
        }
        
        {
          if (type & FileSystem::REGULAR) {
            FileInfo info(entry);
            unsigned int mode = info.getMode();
            char flags[10];
            flags[0] = (link) ? 'l' : '-';
            flags[1] = (mode & FileInfo::RUSR) ? 'r' : '-';
            flags[2] = (mode & FileInfo::WUSR) ? 'w' : '-';
            if (mode & FileInfo::SET_UID) {
              flags[3] = (mode & FileInfo::XUSR) ? 's' : 'S';
            } else {
              flags[3] = (mode & FileInfo::XUSR) ? 'x' : '-';
            }
            flags[4] = (mode & FileInfo::RGRP) ? 'r' : '-';
            flags[5] = (mode & FileInfo::WGRP) ? 'w' : '-';
            if (mode & FileInfo::SET_GID) {
              flags[6] = (mode & FileInfo::XGRP) ? 's' : 'S';
            } else {
              flags[6] = (mode & FileInfo::XGRP) ? 'x' : '-';
            }
            flags[7] = (mode & FileInfo::ROTH) ? 'r' : '-';
            flags[8] = (mode & FileInfo::WOTH) ? 'w' : '-';
            if (mode & FileInfo::RESTRICT) {
              flags[9] = (mode & FileInfo::XOTH) ? 't' : 'T';
            } else {
              flags[9] = (mode & FileInfo::XOTH) ? 'x' : '-';
            }

            // TAG: need upper limit on number of elements to cache
            User owner = info.getOwner();
            String ownerName;
            if (owners.isKey(owner)) {
              ownerName = owners[owner];
            } else {
              try {
                ownerName = owner.getName();
              } catch (User::UserException&) {
                StringOutputStream stream;
                stream << owner << FLUSH;
                ownerName = stream.getString();
              }
              owners.add(owner, ownerName);
            }
            
            Group group = info.getGroup();
            String groupName;
            if (groups.isKey(group)) {
              groupName = groups[group];
            } else {
              try {
                groupName = group.getName();
              } catch (Group::GroupException&) {
                StringOutputStream stream;
                stream << group << FLUSH;
                groupName = stream.getString();
              }
              groups.add(group, groupName);
            }
            
            fout << Sequence<char>(flags, sizeof(flags)) << ' '
                 << setWidth(4);
            if (info.getLinks() != 0) {
              fout << info.getLinks();
            } else {
              fout << ' ';
            }
            
            if (limitTrustees) {
              if (ownerName.getLength() > MAXIMUM_TRUSTEE_LENGTH) {
                ownerName = ownerName.substring(
                  ownerName.getLength() - MAXIMUM_TRUSTEE_LENGTH
                );
              }
              if (groupName.getLength() > MAXIMUM_TRUSTEE_LENGTH) {
                groupName = groupName.substring(
                  groupName.getLength() - MAXIMUM_TRUSTEE_LENGTH
                );
              }
            }
            
            fout << ' '
                 << setWidth(MAXIMUM_TRUSTEE_LENGTH) << ownerName << ' '
                 << setWidth(MAXIMUM_TRUSTEE_LENGTH) << groupName << ' '
                 << setWidth(8) << info.getSize() << ' '
                 << getTime(info.getLastModification()) << ' ';
            if (link) {
              if (colorize) {
                fout << setForeground(COLOR_LINK) << entry << normal();
              } else {
                fout << entry;
              }
              fout << " -> " << target;
            } else {
              if (colorize && (mode & FileInfo::XUSR)) {
                fout << setForeground(COLOR_EXECUTABLE) << entry << normal();
              } else {
                fout << entry;
              }
            }
            fout << EOL;    
          } else if (type & FileSystem::FOLDER) {
            FolderInfo info(entry);
            
            unsigned int mode = info.getMode();
            char flags[10];
            flags[0] = (link) ? 'l' : 'd';
            flags[1] = (mode & FolderInfo::RUSR) ? 'r' : '-';
            flags[2] = (mode & FolderInfo::WUSR) ? 'w' : '-';
            if (mode & FolderInfo::SET_UID) {
              flags[3] = (mode & FolderInfo::XUSR) ? 's' : 'S';
            } else {
              flags[3] = (mode & FolderInfo::XUSR) ? 'x' : '-';
            }
            flags[4] = (mode & FolderInfo::RGRP) ? 'r' : '-';
            flags[5] = (mode & FolderInfo::WGRP) ? 'w' : '-';
            if (mode & FolderInfo::SET_GID) {
              flags[6] = (mode & FolderInfo::XGRP) ? 's' : 'S';
            } else {
              flags[6] = (mode & FolderInfo::XGRP) ? 'x' : '-';
            }
            flags[7] = (mode & FolderInfo::ROTH) ? 'r' : '-';
            flags[8] = (mode & FolderInfo::WOTH) ? 'w' : '-';
            if (mode & FolderInfo::RESTRICT) {
              flags[9] = (mode & FolderInfo::XOTH) ? 't' : 'T';
            } else {
              flags[9] = (mode & FolderInfo::XOTH) ? 'x' : '-';
            }
            
            User owner = info.getOwner();
            String ownerName;
            if (owners.isKey(owner)) {
              ownerName = owners[owner];
            } else {
              try {
                ownerName = owner.getName();
              } catch (User::UserException&) {
                StringOutputStream stream;
                stream << owner << FLUSH;
                ownerName = stream.getString();
              }
              owners.add(owner, ownerName);
            }
            
            Group group = info.getGroup();
            String groupName;
            if (groups.isKey(group)) {
              groupName = groups[group];
            } else {
              try {
                groupName = group.getName();
              } catch (Group::GroupException&) {
                StringOutputStream stream;
                stream << group << FLUSH;
                groupName = stream.getString();
              }
              groups.add(group, groupName);
            }
            
            fout << Sequence<char>(flags, sizeof(flags)) << ' '
                 << setWidth(4);
            if (info.getLinks() != 0) {
              fout << info.getLinks();
            } else {
              fout << ' ';
            }

            if (limitTrustees) {
              if (ownerName.getLength() > MAXIMUM_TRUSTEE_LENGTH) {
                ownerName = ownerName.substring(
                  ownerName.getLength() - MAXIMUM_TRUSTEE_LENGTH
                );
              }
              if (groupName.getLength() > MAXIMUM_TRUSTEE_LENGTH) {
                groupName = groupName.substring(
                  groupName.getLength() - MAXIMUM_TRUSTEE_LENGTH
                );
              }
            }
            
            fout << ' '
                 << setWidth(MAXIMUM_TRUSTEE_LENGTH) << ownerName << ' '
                 << setWidth(MAXIMUM_TRUSTEE_LENGTH) << groupName << ' '
                 << setWidth(8) << ' ' << ' '
                 << getTime(info.getLastModification()) << ' ';
            if (link) {
              if (colorize) {
                fout << setForeground(COLOR_LINK) << entry << normal();
              } else {
                fout << entry;
              }
              fout << " -> " << target;
              // TAG: colorize target: folder, link, or other?
            } else {
              if (colorize) {
                fout << setForeground(COLOR_FOLDER) << entry << normal();
              } else {
                fout << entry;
              }
            }
            fout << EOL;
          } else { // unknown entry type
            fout << "----------" << ' '
                 << setWidth(4) << ' ' << ' '
                 << setWidth(MAXIMUM_TRUSTEE_LENGTH) << ' ' << ' '
                 << setWidth(MAXIMUM_TRUSTEE_LENGTH) << ' ' << ' '
                 << setWidth(8) << ' ' << ' '
                 << setWidth(12) << ' ' << ' ';
            if (link) {
              if (colorize) {
                fout << setForeground(COLOR_LINK) << entry << normal();
              } else {
                fout << entry;
              }
              fout << " -> " << target;
              // TAG: colorize target: folder, link, or other?
            } else {
              fout <<  entry;
            }
            fout << EOL;
          }
        }
        fout << FLUSH;
      } catch (FileSystemException& e) {
        // TAG: ignore exception - unless in debug mode?
        ferr << entry << ": " << e << ENDL;
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
    case COMMAND_LIST:
      list();
      break;
    }
  }
  
  ~ListApplication() throw() {
  }
};

STUB(ListApplication);
