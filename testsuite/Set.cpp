/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/Object.h>
#include <base/collection/Set.h>
#include <iostream>

using namespace std;

int main() {

  cout << "Initializing set\n";
  Set<int> si;

  cout << "Adding elements to set\n";
  si.add(4);
  si.add(7);
  si.add(2);
  si.add(3);
  si.add(8);
  si.add(6);
  si.add(1);
  si.add(5);

  cout << "Size of set: " << si.getSize() << "\n";

  cout << "Non-modifying enumerating elements of set\n";
  Set<int>::ReadOnlyEnumeration enu(si);
  unsigned int position = 0;
  while (enu.hasNext()) {
    cout << "position " << position++ << ": " << *enu.next()->getValue() << "\n";
  }

  cout << "Checking whether 4 is in set: " << si.isKey(4) << "\n";
  cout << "Checking whether 0 is in set: " << si.isKey(0) << "\n";

  return 0;
}
