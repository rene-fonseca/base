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
#include <base/TypeInfo.h>
#include <base/security/User.h>

using namespace dk::sdu::mip::base;

class ProcessApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  ProcessApplication(int numberOfArguments, const char* arguments[], const char* environment[])
    : Application(MESSAGE("Process"), numberOfArguments, arguments, environment) {
  }

  void dumpLimit(const StringLiteral& literal, OperatingSystem::Resource resource) throw() {
    int64 softLimit = OperatingSystem::getResourceLimit(resource, OperatingSystem::SOFT_LIMIT);
    int64 hardLimit = OperatingSystem::getResourceLimit(resource, OperatingSystem::HARD_LIMIT);
    
    fout << indent(2) << literal << MESSAGE(" (soft): ");
    if (softLimit == -1) {
      fout << MESSAGE("INFINITE");
    } else {
      fout << softLimit;
    }
    fout << ENDL;
    
    fout << indent(2) << literal << MESSAGE(" (hard): ");
    if (hardLimit == -1) {
      fout << MESSAGE("INFINITE");
    } else {
      fout << hardLimit;
    }
    fout << ENDL;
  }
  
  void main() throw() {
    fout << getFormalName() << MESSAGE(" version ") << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;
    
    fout << MESSAGE("Is administrator: ") << User::getCurrentUser().isAdmin() << ENDL;
    fout << MESSAGE("Current user: ") << User::getCurrentUser().getName() << ENDL;
    fout << MESSAGE("Current process: ") << Process::getProcess().getId() << ENDL;
    fout << MESSAGE("Parent process: ") << Process::getParentProcess().getId() << ENDL;

    fout << MESSAGE("Name of process: ");
    try {
      fout << Process::getProcess().getName();
    } catch (NotSupported&) {
      fout << MESSAGE("unknown");
    }
    fout << ENDL;

    fout << MESSAGE("Is parent process alive: "); 
    try {
      fout << Process::getParentProcess().isAlive();
    } catch (Process::ProcessException&) {
      fout << MESSAGE("unknown");
    }
    fout << ENDL;
    
    fout << MESSAGE("Priority of process: ") << Process::getProcess().getPriority() << ENDL;
    fout << MESSAGE("Priority of parent process: ") << Process::getParentProcess().getPriority() << ENDL;

    fout << MESSAGE("Number of configured processors: ") << Process::getNumberOfConfiguredProcessors() << ENDL;
    fout << MESSAGE("Number of online processors: ") << Process::getNumberOfOnlineProcessors() << ENDL;
    fout << ENDL;
    
    fout << MESSAGE("Resource limits: ") << ENDL;
    dumpLimit(MESSAGE("Core file size"), OperatingSystem::RESOURCE_CORE);
    dumpLimit(MESSAGE("CPU time"), OperatingSystem::RESOURCE_CPU);
    dumpLimit(MESSAGE("Data segment size"), OperatingSystem::RESOURCE_DATA);
    dumpLimit(MESSAGE("File size"), OperatingSystem::RESOURCE_FILE_SIZE);
    dumpLimit(MESSAGE("Number of open files"), OperatingSystem::RESOURCE_OPEN_FILES);
    dumpLimit(MESSAGE("Stack size"), OperatingSystem::RESOURCE_STACK);
    dumpLimit(MESSAGE("Address space"), OperatingSystem::RESOURCE_ADDRESS_SPACE);

    // wait
    // terminate
    // Process::execute(self img);
  }
};

STUB(ProcessApplication);
