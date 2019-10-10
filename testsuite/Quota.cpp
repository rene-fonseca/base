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
#include <base/concurrency/Process.h>
#include <base/filesystem/FileSystem.h>
#include <base/TypeInfo.h>
#include <base/security/User.h>

using namespace com::azure::dev::base;

class QuotaApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  QuotaApplication(
    int numberOfArguments, const char* arguments[], const char* environment[])
    : Application("Quota", numberOfArguments, arguments, environment) {
  }
  
  void main() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "Copyright (C) 2002-2019 by Rene Moeller Fonseca" << EOL
         << ENDL;
    
    User user = User::getCurrentUser();
    String home = user.getHomeFolder();
    
    fout << "User: " << user.getName() << ENDL;
    fout << "Home: " << home << ENDL;
    
    FileSystem::Quota quota;
    try {
      quota = FileSystem::getQuota(home, user);
    } catch (FileSystemException& e) {
      ferr << "Error: " << e << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }

    fout << "Hard limit: " << quota.hardLimit << EOL
         << "Soft limit: " << quota.softLimit << EOL
         << "Current usage: " << quota.currentUsage << ENDL;
  }
};

STUB(QuotaApplication);
