/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/collection/Set.h>

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
  Set<int>::ReadOnlyEnumeration enu(si);
  int sum = 0;
  while (enu.hasNext()) {
    sum += *enu.next()->getValue();
  }
  fout << "sum: " << sum << EOL;

  fout << "Checking whether 4 is in set: " << si.isKey(4) << ENDL;
  fout << "Checking whether 0 is in set: " << si.isKey(0) << ENDL;

  fout << "Size: " << si.getSize() << ENDL;
  fout << "Removeing all elements from the set" << ENDL;
  si.removeAll();
  fout << "Size: " << si.getSize() << ENDL;

  return 0;
}
