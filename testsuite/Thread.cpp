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
#include <base/concurrency/Thread.h>
#include <base/concurrency/ExclusiveSynchronize.h>
#include <base/string/String.h>

using namespace base;

SpinLock lock;

class MyThread : public Runnable {
private:

  char value;
  unsigned int count;
public:

  MyThread(char _value, unsigned int _count) throw() : value(_value), count(_count) {}

  void run() throw() {
    {
      ExclusiveSynchronize<SpinLock> exclusiveSyncrhonize(lock);
      fout << MESSAGE("Written by MyThread object") << ENDL;
    }

    while (count--) {
      {
        ExclusiveSynchronize<SpinLock> exclusiveSyncrhonize(lock);
        fout << value;
      }
      Thread::yield();
    }
  }

};

class ThreadApplication : public Application {
public:

  ThreadApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw() :
    Application(MESSAGE("Thread"), numberOfArguments, arguments, environment) {
  }

  void main() throw() {
    fout << Application::getFormalName() << MESSAGE(" version 1.0") << EOL
         << MESSAGE("Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;

    MyThread myThreadA('A', 4096);
    MyThread myThreadB('B', 4096);

    Thread myContextA(&myThreadA);
    Thread myContextB(&myThreadB);

    fout << MESSAGE("Starting threads...") << ENDL;
    myContextA.start();
    myContextB.start();

    fout << MESSAGE("Waiting for threads to complete...") << ENDL;
    myContextA.join();
    myContextB.join();
  }

};

int main(int argc, const char* argv[], const char* env[]) {
  ThreadApplication application(argc, argv, env);
  try {
    application.main();
  } catch(Exception& e) {
    return Application::getApplication()->exceptionHandler(e);
  } catch(...) {
    return Application::getApplication()->exceptionHandler();
  }
  return Application::getApplication()->getExitCode();
}
