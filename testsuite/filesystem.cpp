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

using namespace com::azure::dev::base;

class FileSystemApplication : public Application {
private:
  
  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 2;
  
  unsigned int currentYear;
  String thisYearFormat;
  String otherYearFormat;
public:

  FileSystemApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[])
    : Application("filesystem", numberOfArguments, arguments, environment) {
    currentYear = Date::getNow().getYear();
    thisYearFormat = "%b %#d %H:%M";
    otherYearFormat = "%b %#d  %Y";
  }

  inline String getTime(const Date& date) const {
    return date.format((date.getYear() == currentYear) ? thisYearFormat : otherYearFormat, false);
  }
  
  void main() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "Copyright (C) 2001-2019 by Rene Moeller Fonseca" << EOL
         << ENDL;

    String path = "."; // use current folder by default
    
    Array<String> arguments = getArguments();
    switch (arguments.getSize()) {
    case 0:
      // use defaults
      break;
    case 1:
      path = arguments[0]; // get the path
      break;
    default:
      fout << "filesystem [path]" << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }
    
    FolderInfo folder(path);
    Array<String> entries = folder.getEntries();
    
    fout << "Folder: " << folder.getPath() << EOL
         << "Number of entries: " << entries.getSize() << EOL
         << "Entries: " << entries << EOL
         << EOL;
    
    fout << "Support for symbolic links: " << FileSystem::supportsLinks() << EOL
         << EOL;
    
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
            target = "<unknown>";
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
            
            fout << ' '
                 << setWidth(16) << ownerName << ' '
                 << setWidth(16) << groupName << ' '
                 << setWidth(8) << info.getSize() << ' '
                 << getTime(info.getLastModification()) << ' '
                 << entry;
            if (link) {
              fout << " -> " << target;
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
            
            fout << ' '
                 << setWidth(16) << ownerName << ' '
                 << setWidth(16) << groupName << ' '
                 << setWidth(8) << ' ' << ' '
                 << getTime(info.getLastModification()) << ' '
                 << entry;
            if (link) {
              fout << " -> " << target;
            }
            fout << EOL;
          } else { // unknown entry type
            fout << "----------" << ' '
                 << setWidth(4) << ' ' << ' '
                 << setWidth(16) << ' ' << ' '
                 << setWidth(16) << ' ' << ' '
                 << setWidth(8) << ' ' << ' '
                 << setWidth(12) << ' ' << ' '
                 << entry;
            if (link) {
              fout << " -> " << target;
            }
            fout << EOL;
          }
        }
      } catch (Exception& e) {
        ferr << entry << ": " << e << ENDL; // ignore exception - unless in debug mode?
      }
    }
  }

  ~FileSystemApplication() {
  }
};

APPLICATION_STUB(FileSystemApplication);
