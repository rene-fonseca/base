/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/string/FormatOutputStream.h>
#include <base/concurrency/Thread.h>
#include <unistd.h>

class MyThread : public Runnable {
private:

  char value;
  unsigned int count;
public:

  MyThread(char v, unsigned int c) throw() : value(v), count(c) {}

  void run() {
    fout << "Written by MyThread object\n";

    while (count--) {
      fout << value;
    }
  }

};

void test() {
  fout << "Testing Thread...\n";

  MyThread myThreadA('A', 409600);
  MyThread myThreadB('B', 409600);

  Thread myContextA(&myThreadA);
  Thread myContextB(&myThreadB);

  fout << "Starting threads\n";
  myContextA.start();
  myContextB.start();

  fout << "Waiting for threads to complete\n";
  myContextA.join();
  myContextB.join();
}

int main() {
  try {
    test();
  } catch(Exception& e) {
    ferr << "Exception: " << e.getMessage() << "\n";
  } catch(...) {
    ferr << "Unknown exception\n";
  }

  fout << "Completed\n";
  return 0;
}
