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
#include <base/concurrency/Process.h>
#include <base/filesystem/FileSystem.h>
#include <base/TypeInfo.h>
#include <base/security/User.h>

using namespace base;

class QuotaApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  QuotaApplication(int numberOfArguments, const char* arguments[], const char* environment[])
    : Application(MESSAGE("Quota"), numberOfArguments, arguments, environment) {
  }
  
  void main() throw() {
    fout << getFormalName() << MESSAGE(" version ") << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;
    Trustee truestee(User::getCurrentUser());
    
    fout << MESSAGE("User: ") << User::getCurrentUser().getName() << ENDL;
    
    FileSystem::Quota quota;
    try {
      quota = FileSystem::getQuota(MESSAGE("/dev/hda1"), truestee /*User::getCurrentUser()*/);
    } catch (FileSystemException&) {
      setExitCode(EXIT_CODE_ERROR);
      return;
    }

    fout << MESSAGE("Hard limit: ") << quota.hardLimit << EOL
         << MESSAGE("Soft limit: ") << quota.softLimit << EOL
         << MESSAGE("Current usage: ") << quota.currentUsage << ENDL;
  }
};

STUB(QuotaApplication);
