/***************************************************************************
    copyright   : (C) 2001 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/collection/Queue.h>
#include <base/string/FormatOutputStream.h>
#include <base/Type.h>

using namespace base;

int main() {
  fout << "Testing implementation of the Queue class" << ENDL;

  fout << "Initializing queue" << ENDL;

  Queue<int> qi;

  fout << "Pushing values (1, 2, 3, 4, and 5)" << ENDL;
  qi.push(1);
  qi.push(2);
  qi.push(3);
  qi.push(4);
  qi.push(5);

  fout << "getSize() (5): " << qi.getSize() << ENDL;
  fout << "isEmpty() (false): " << qi.isEmpty() << ENDL;

  try {
    fout << "pop(): " << qi.pop() << ENDL;
    fout << "pop(): " << qi.pop() << ENDL;
    fout << "pop(): " << qi.pop() << ENDL;
    fout << "pop(): " << qi.pop() << ENDL;
    fout << "pop(): " << qi.pop() << ENDL;
    fout << "pop(): " << qi.pop() << ENDL;
  } catch(Exception& e) {
    ferr << getTypename(e) << ": " << e.getMessage() << ENDL;
  }
  fout << "InvalidNode exception is expected" << ENDL;

  fout << "getSize() (0): " << qi.getSize() << ENDL;
  fout << "isEmpty() (true): " << qi.isEmpty() << ENDL;

  return 0;
}
