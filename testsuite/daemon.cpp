/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/Daemon.h>
#include <base/Primitives.h>
#include <base/SystemLogger.h>
#include <base/concurrency/Thread.h>
#include <base/io/FileInputStream.h>
#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/StringOutputStream.h>

using namespace base;

class MyDaemon : public Runnable {
public:

  void run() {    
    SystemLogger::write(SystemLogger::INFORMATION, "Daemon is running...");
    StringOutputStream stream;
    stream << Thread::getThread() << FLUSH;
    SystemLogger::write(SystemLogger::INFORMATION, stream.getString());

    while (!Thread::getThread()->isTerminated()) {
      if (Application::getApplication() && Application::getApplication()->isTerminated()) {
        Thread::getThread()->terminate();
      }
      Thread::sleep(0);
    }
    SystemLogger::write(SystemLogger::INFORMATION, "Daemon has completed");
  }
};

class DaemonApplication : public Application {
public:

  DaemonApplication(int argc, const char* argv[], const char* env[])
    : Application(MESSAGE("daemon"), argc, argv, env) {
  }
  
  void main() throw() {
    if (getArguments().getSize() > 0) {
    } else {
      SystemLogger::write(SystemLogger::INFORMATION, "Attempting to start daemon...");
      MyDaemon myDaemon;
      Daemon daemon(&myDaemon);
    }
  }
};

int main(int argc, const char* argv[], const char* env[]) {
  DaemonApplication application(argc, argv, env);
  try {
    application.main();
  } catch(Exception& e) {
    return Application::getApplication()->exceptionHandler(e);
  } catch(...) {
    return Application::getApplication()->exceptionHandler();
  }
  return Application::getApplication()->getExitCode();
}
