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
#include <base/Daemon.h>
#include <base/Primitives.h>
#include <base/SystemLogger.h>
#include <base/concurrency/Thread.h>
#include <base/io/FileInputStream.h>
#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/StringOutputStream.h>

using namespace com::azure::dev::base;

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
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  DaemonApplication(int argc, const char* argv[], const char* env[])
    : Application(MESSAGE("daemon"), argc, argv, env) {
  }
  
  void main() {
    if (getArguments().getSize() > 0) {
    } else {
      SystemLogger::write(SystemLogger::INFORMATION, "Attempting to start daemon...");
      MyDaemon myDaemon;
      Daemon daemon(&myDaemon);
    }
  }
};

STUB(DaemonApplication);
