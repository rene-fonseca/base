/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/collection/Queue.h>
#include <base/string/FormatOutputStream.h>
#include <typeinfo>

int main() {
  fout << "Testing implementation of the Queue class\n";

  fout << "Initializing queue\n";

  Queue<int> qi;

  fout << "Pushing values (1, 2, 3, 4, and 5)\n";
  qi.push(1);
  qi.push(2);
  qi.push(3);
  qi.push(4);
  qi.push(5);

  fout << "getSize() (5): " << qi.getSize() << EOL;
  fout << "isEmpty() (false): " << qi.isEmpty() << EOL;

  try {
    fout << "pop(): " << qi.pop() << EOL;
    fout << "pop(): " << qi.pop() << EOL;
    fout << "pop(): " << qi.pop() << EOL;
    fout << "pop(): " << qi.pop() << EOL;
    fout << "pop(): " << qi.pop() << EOL;
    fout << "pop(): " << qi.pop() << EOL;
  } catch(Exception& e) {
    ferr << typeid(e).name() << ": " << e.getMessage() << EOL;
  }
  fout << "InvalidNode exception is expected\n";

  fout << "getSize() (0): " << qi.getSize() << EOL;
  fout << "isEmpty() (true): " << qi.isEmpty() << EOL;

  return 0;
}
