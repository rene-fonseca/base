/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/string/FormatOutputStream.h>
#include <base/concurrency/Thread.h>

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
