/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/string/FormatOutputStream.h>
#include <base/concurrency/Thread.h>

class MyThread : public Runnable {
public:

  MyThread() {}

  void run() {
    fout << "Written by MyThread" << EOL;
  }

};

void test() {
  fout << "Testing Thread...\n";

  MyThread myThread;
  Thread myContext(&myThread);
  myContext.start();
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
