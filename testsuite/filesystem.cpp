/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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

using namespace base;

class FileSystemApplication : public Application {
private:
  
  unsigned int currentYear;
  String thisYearFormat;
  String otherYearFormat;
public:

  FileSystemApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw() :
    Application(MESSAGE("filesystem"), numberOfArguments, arguments, environment) {
    currentYear = Date::getNow().getYear();
    thisYearFormat = MESSAGE("%b %#d %H:%M");
    otherYearFormat = MESSAGE("%b %#d  %Y");
  }

  String getTime(const Date& date) const throw() {
    return date.format((date.getYear() == currentYear) ? thisYearFormat : otherYearFormat, false);
  }
  
  void main() throw() {
    fout << Application::getFormalName() << MESSAGE(" version 1.1") << EOL
         << MESSAGE("Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << MESSAGE("Testing file system classes")
         << EOL
         << ENDL;

    try {
//       FileSystem::makeLink(MESSAGE("D:\\WWWpublic"), MESSAGE("mysymlinkdir"));
//       FileSystem::makeLink(MESSAGE("D:\\WWWpublic\\gip-framework.chm"), MESSAGE("mysymlink"));
//      FileSystem::makeHardLink(MESSAGE("D:\\WWWpublic\\base-framework.chm"), MESSAGE("myhardlink"));
    } catch (Exception& e) {
      ferr << e << ENDL;
    }
    
    String path = MESSAGE("."); // use current folder by default
    
    Array<String> arguments = getArguments();
    switch (arguments.getSize()) {
    case 0:
      // use defaults
      break;
    case 1:
      path = arguments[0]; // get the path
      break;
    default:
      fout << MESSAGE("filesystem [path]") << ENDL;
      setExitCode(Application::EXIT_CODE_ERROR);
      return;
    }
    
    FolderInfo folder(path);
    Array<String> entries = folder.getEntries();
    
    fout << MESSAGE("Folder: ") << folder.getPath() << EOL
         << MESSAGE("Number of entries: ") << entries.getSize() << EOL
         << MESSAGE("Entries: ") << entries << EOL
         << EOL;
    
    fout << MESSAGE("Support for symbolic links: ") << FileSystem::supportsLinks() << EOL
         << EOL;
    
    FileSystem::setCurrentFolder(path);
    
    User cachedOwner; // we only cache the last owner
    Group cachedGroup; // we only cache the last group
    String ownerName;
    String groupName;
    
    Array<String>::ReadEnumerator enu = entries.getReadEnumerator();
    while (enu.hasNext()) {
      const String entry = *enu.next();
      unsigned int type = FileSystem::getType(entry);
      try {
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
          ferr << entry << MESSAGE(": ") << e << ENDL; // ignore exception
        }
        
        {
	  if (type & FileSystem::REGULAR) {
            FileInfo info(entry);
            unsigned int mode = info.getMode();
            char flags[10];
            flags[0] = (link) ? 'l' : '-';
            flags[1] = (mode & FileInfo::RUSR) ? 'r' : '-';
            flags[2] = (mode & FileInfo::WUSR) ? 'w' : '-';
            flags[3] = (mode & FileInfo::XUSR) ? 'x' : '-';
            flags[4] = (mode & FileInfo::RGRP) ? 'r' : '-';
            flags[5] = (mode & FileInfo::WGRP) ? 'w' : '-';
            flags[6] = (mode & FileInfo::XGRP) ? 'x' : '-';
            flags[7] = (mode & FileInfo::ROTH) ? 'r' : '-';
            flags[8] = (mode & FileInfo::WOTH) ? 'w' : '-';
            flags[9] = (mode & FileInfo::XOTH) ? 'x' : '-';

            if (cachedOwner != info.getOwner()) {
              cachedOwner = info.getOwner();
              try {
                ownerName = cachedOwner.getName();
              } catch (User::UserException&) {
                StringOutputStream stream;
                stream << cachedOwner << FLUSH;
                ownerName = stream.getString();
              }
            }
            if (cachedGroup != info.getGroup()) {
              cachedGroup = info.getGroup();
              try {
                groupName = cachedGroup.getName();
              } catch (Group::GroupException&) {
                StringOutputStream stream;
                stream << cachedGroup << FLUSH;
                groupName = stream.getString();
              }
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
              fout << MESSAGE(" -> ") << target;
            }
            fout << EOL;    
          } else if (type & FileSystem::FOLDER) {
            FolderInfo info(entry);
            
            unsigned int mode = info.getMode();
            char flags[10];
            flags[0] = (link) ? 'l' : 'd';
            flags[1] = (mode & FolderInfo::RUSR) ? 'r' : '-';
            flags[2] = (mode & FolderInfo::WUSR) ? 'w' : '-';
            flags[3] = (mode & FolderInfo::XUSR) ? 'x' : '-';
            flags[4] = (mode & FolderInfo::RGRP) ? 'r' : '-';
            flags[5] = (mode & FolderInfo::WGRP) ? 'w' : '-';
            flags[6] = (mode & FolderInfo::XGRP) ? 'x' : '-';
            flags[7] = (mode & FolderInfo::ROTH) ? 'r' : '-';
            flags[8] = (mode & FolderInfo::WOTH) ? 'w' : '-';
            flags[9] = (mode & FolderInfo::XOTH) ? 'x' : '-';
            
            if (cachedOwner != info.getOwner()) {
              cachedOwner = info.getOwner();
              try {
                ownerName = cachedOwner.getName();
              } catch (User::UserException&) {
                StringOutputStream stream;
                stream << cachedOwner << FLUSH;
                ownerName = stream.getString();
              }
            }
            if (cachedGroup != info.getGroup()) {
              cachedGroup = info.getGroup();
              try {
                groupName = cachedGroup.getName();
              } catch (Group::GroupException&) {
                StringOutputStream stream;
                stream << cachedGroup << FLUSH;
                groupName = stream.getString();
              }
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
              fout << MESSAGE(" -> ") << target;
            }
            fout << EOL;
          } else {            
            fout << MESSAGE("----------") << ' '
                 << setWidth(4) << ' ' << ' '
                 << setWidth(16) << ' ' << ' '
                 << setWidth(16) << ' ' << ' '
                 << setWidth(8) << ' ' << ' '
                 << setWidth(12) << ' ' << ' '
                 << entry
		 << EOL;
          }
        }
      } catch (Exception& e) {
        ferr << entry << MESSAGE(": ") << e << ENDL; // ignore exception - unless in debug mode?
      }
    }
  }
  
};

int main(int argc, const char* argv[], const char* env[]) {
  FileSystemApplication application(argc, argv, env);
  try {
    application.main();
  } catch(Exception& e) {
    return Application::getApplication()->exceptionHandler(e);
  } catch(...) {
    return Application::getApplication()->exceptionHandler();
  }
  return Application::getApplication()->getExitCode();
}
