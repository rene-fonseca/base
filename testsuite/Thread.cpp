/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/FormatOutputStream.h>
#include <base/concurrency/Thread.h>

using namespace base;

class MyThread : public Runnable {
private:

  char value;
  unsigned int count;
public:

  MyThread(char v, unsigned int c) throw() : value(v), count(c) {}

  void run() {
    fout << "Written by MyThread object" << ENDL;

    while (count--) {
      fout << value << FLUSH;
    }
  }

};

void test() {
  fout << "Testing Thread..." << ENDL;

  MyThread myThreadA('A', 4096);
  MyThread myThreadB('B', 4096);

  Thread myContextA(&myThreadA);
  Thread myContextB(&myThreadB);

  fout << "Starting threads" << ENDL;
  myContextA.start();
  myContextB.start();

  fout << "Waiting for threads to complete" << ENDL;
  myContextA.join();
  myContextB.join();
}

int main() {
  try {
    test();
  } catch(Exception& e) {
    ferr << "Exception: " << e.getMessage() << ENDL;
  } catch(...) {
    ferr << "Unknown exception" << ENDL;
  }

  fout << "Completed" << ENDL;
  return 0;
}
