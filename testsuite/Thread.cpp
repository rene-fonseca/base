/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

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

using namespace com::azure::dev::base;

SpinLock lock;

class MyThread : public Runnable {
private:

  char value;
  unsigned int count;
public:

  MyThread(char _value, unsigned int _count) throw()
    : value(_value), count(_count) {
  }

  void run() throw() {
    {
      ExclusiveSynchronize<SpinLock> exclusiveSyncrhonize(lock);
      fout << "Written by MyThread object" << ENDL;
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
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  ThreadApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw()
    : Application(MESSAGE("Thread"), numberOfArguments, arguments, environment) {
  }

  void main() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "Copyright (C) 2001-2019 by Rene Moeller Fonseca" << EOL
         << ENDL;
    
    MyThread myThreadA('A', 4096);
    MyThread myThreadB('B', 4096);

    Thread myContextA(&myThreadA);
    Thread myContextB(&myThreadB);

    fout << "Starting threads..." << ENDL;
    myContextA.start();
    myContextB.start();

    fout << "Waiting for threads to complete..." << ENDL;
    myContextA.join();
    myContextB.join();
  }

};

STUB(ThreadApplication);
