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

using namespace base;

class ProcessApplication : public Application {
public:

  ProcessApplication(int numberOfArguments, const char* arguments[], const char* environment[])
    : Application(MESSAGE("Process"), numberOfArguments, arguments, environment) {
  }
  
  void main() throw() {
    fout << MESSAGE("Testing implementation of the Process class") << ENDL;
    
    fout << MESSAGE("Is administrator: ") << User::getCurrentUser().isAdmin() << ENDL;
    fout << MESSAGE("Current user: ") << User::getCurrentUser().getName() << ENDL;
    fout << MESSAGE("Current process: ") << Process::getProcess().getId() << ENDL;
    fout << MESSAGE("Parent process: ") << Process::getParentProcess().getId() << ENDL;

    fout << MESSAGE("Name of process: ");
    try {
      fout << Process::getProcess().getName();
    } catch(NotSupported&) {
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

    // wait
    // terminate
    // Process::execute(self img);
  }
};

int main(int argc, const char* argv[], const char* env[]) {
  ProcessApplication application(argc, argv, env);
  try {
    application.main();
  } catch(Exception& e) {
    return Application::getApplication()->exceptionHandler(e);
  } catch(...) {
    return Application::getApplication()->exceptionHandler();
  }
  return Application::getApplication()->getExitCode();
}
