/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/FileInputStream.h>
#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/StringOutputStream.h>
#include <base/Application.h>
#include <base/Type.h>
#include <base/concurrency/Thread.h>
#include <base/Daemon.h>
#include <base/concurrency/Thread.h>
#include <base/SystemLogger.h>

using namespace base;

class MyDaemon : public Runnable {
public:

  void run() {
    //fout << "Testing implementation of the Daemon..." << ENDL;
    SystemLogger::write(SystemLogger::INFORMATION, "Daemon is running...");
    StringOutputStream stream;
    stream << HEX << (unsigned int)Thread::getThread() << FLUSH;
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

int entry() {
  //fout << "Testing implementation of the Daemon..." << ENDL;
  if (Application::getApplication()->getArguments().getSize() > 0) {
  } else {
    SystemLogger::write(SystemLogger::INFORMATION, "Attempting to start daemon...");
    MyDaemon myDaemon;
    Daemon daemon(&myDaemon);
  }
  return 0;
}

int main(int argc, const char* argv[], const char* envp[]) {
  Application app("daemon", argc, argv, envp);
  try {
    return entry();
  } catch(Exception& e) {
    return Application::getApplication()->exceptionHandler(e);
  } catch(...) {
    return Application::getApplication()->exceptionHandler();
  }
}
