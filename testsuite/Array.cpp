/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/collection/Array.h>

int main() {
  fout << "Testing implementation of the Array class\n";

  fout << "Initializing array\n";

  Array<int> ai;

  fout << "Appending values (1, 2, 3, 4, and 5)\n";
  ai.append(1);
  ai.append(2);
  ai.append(3);
  ai.append(4);
  ai.append(5);
  fout << "ai: " << ai << EOL;

  fout << "Prepending (10, and 11)\n";
  ai.prepend(10);
  ai.prepend(11);
  fout << "ai: " << ai << EOL;

  fout << "Inserting 21 at index 0\n";
  ai.insert(0, 21);
  fout << "Inserting 22 at index 4\n";
  ai.insert(4, 22);
  fout << "Inserting 23 at index 9\n";
  ai.insert(9, 23);
  fout << "ai: " << ai << EOL;

  fout << "Removing element at index 2\n";
  ai.remove(2);
  fout << "ai: " << ai << EOL;

  fout << "Modifying enumeration of elements (multiply values by 2)\n";
  Array<int>::Enumeration enu(ai);
  unsigned int position = 0;
  while (enu.hasNext()) {
    *enu.next() *= 2;
  }
  fout << "ai: " << ai << EOL;

  fout << "Non-modifying enumerating elements of list (calculating sum)\n";
  Array<int>::ReadOnlyEnumeration roenu(ai);
  int sum = 0;
  while (roenu.hasNext()) {
    sum += *roenu.next();
  }
  fout << "sum: " << sum << EOL;

  fout << "Size: " << ai.getSize() << "\n";
  fout << "Removing all the elements\n";
  ai.removeAll();
  fout << "Size: " << ai.getSize() << "\n";

  return 0;
}
