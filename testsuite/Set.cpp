/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/collection/Set.h>
#include <base/collection/Functor.h>
#include <base/Functor.h>

using namespace base;

int main() {
  fout << "Testing Set" << ENDL;

  fout << "Initializing set" << ENDL;
  Set<int> si;
  fout << "si: " << si << EOL;

  fout << "Adding elements to set (4, 7, 2, 3, 8, 6, 1, and 5)" << ENDL;
  si.add(4);
  si.add(7);
  si.add(2);
  si.add(3);
  si.add(8);
  si.add(6);
  si.add(1);
  si.add(5);
  fout << "si: " << si << EOL;

  fout << "Non-modifying enumerating elements of set (calculating sum)" << ENDL;
  Sum<int> sum;
  forEach(si, sum);
  fout << "sum: " << sum.getResult() << ENDL;

  fout << "Checking whether 4 is in set: " << si.isKey(4) << ENDL;
  fout << "Checking whether 0 is in set: " << si.isKey(0) << ENDL;

  fout << "Size: " << si.getSize() << ENDL;
  fout << "Removing all elements from the set" << ENDL;
  si.removeAll();
  fout << "Size: " << si.getSize() << ENDL;

  return 0;
}
