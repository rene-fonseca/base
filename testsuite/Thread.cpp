/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/string/FormatOutputStream.h>
#include <base/concurrency/Thread.h>
#include <unistd.h>

extern int THREADSTATE;

class MyThread : public Runnable {
private:

  int state;
public:

  MyThread() throw() : state(0) {}

  int getState() const throw() {return state;}

  void run() {
    fout << "Written by MyThread object\n";
  }

};

void test() {
  fout << "Testing Thread...\n";

  MyThread myThread;
  Thread myContext(&myThread);
  fout << "Starting myThread\n";
  myContext.start();
  fout << "Waiting for myThread to complete\n";
  myContext.join();
}

int main() {
  try {
    test();
  } catch(Exception& e) {
    ferr << "Exception: " << e.getMessage() << "\n";
  } catch(...) {
    ferr << "Unknown exception\n";
  }

  return 0;
}
