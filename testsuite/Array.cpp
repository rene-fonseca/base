/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/collection/Array.h>
#include <iostream>

using namespace std;

int main() {

  cout << "Initializing array\n";
  Array<int> ai;

  cout << "Appending values (1, 2, 3, 4, and 5)\n";
  ai.append(1);
  ai.append(2);
  ai.append(3);
  ai.append(4);
  ai.append(5);

  cout << "Prepending (10, and 11)\n";
  ai.prepend(10);
  ai.prepend(11);

  cout << "Inserting 21 at index 0\n";
  ai.insert(0, 21);
  cout << "Inserting 22 at index 4\n";
  ai.insert(4, 22);
  cout << "Inserting 23 at index 9\n";
  ai.insert(9, 23);

  cout << "Size: " << ai.getSize() << "\n";

  cout << "Modifying enumeration of elements (multiply values by 2)\n";
  Array<int>::Enumeration enu(ai);
  unsigned int position = 0;
  while (enu.hasNext()) {
    int* value = enu.next();
    cout << "position " << position++ << ": " << *value << "\n";
    *value *= 2;
  }

  cout << "Removing element at index 2\n";
  ai.remove(2);
  cout << "Size: " << ai.getSize() << "\n";

  cout << "Non-modifying enumerating elements of list\n";
  Array<int>::ReadOnlyEnumeration roenu(ai);
  unsigned int roposition = 0;
  while (roenu.hasNext()) {
    cout << "position " << roposition++ << ": " << *roenu.next() << "\n";
  }

  cout << "Removing all the elements\n";
  ai.removeAll();
  cout << "Size: " << ai.getSize() << "\n";

  return 0;
}
