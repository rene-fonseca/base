/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/collection/List.h>

int main() {
  fout << "Testing List implementation\n";

  fout << "Initializing list\n";
  List<int> li;

  fout << "Appending elements to list (4, 5, 6, and 7)\n";
  li.append(4);
  li.append(5);
  li.append(6);
  li.append(7);
  fout << "li: " << li << EOL;

  fout << "Prepending elements to list (3, 2, 1, and 0)\n";
  li.prepend(3);
  li.prepend(2);
  li.prepend(1);
  li.prepend(0);
  fout << "li: " << li << EOL;

  fout << "Adding elements to list (8, 9, 10, and 11)\n";
  li.add(8);
  li.add(9);
  li.add(10);
  li.add(11);
  fout << "li: " << li << EOL;

  fout << "Size of list: " << li.getSize() << "\n";

  fout << "Modifying enumerating elements of list (multiplying by 2)\n";
  List<int>::Enumeration enu(li);
  while (enu.hasNext()) {
    *enu.next() *= 2;
  }

  fout << "Non-modifying enumerating elements of list (calculating sum)\n";
  List<int>::ReadOnlyEnumeration roenu(li);
  int sum = 0;
  while (roenu.hasNext()) {
    sum += *roenu.next();
  }

  fout << "Size: " << li.getSize() << "\n";
  fout << "Removing all the elements\n";
  li.removeAll();
  fout << "Size: " << li.getSize() << "\n";

  return 0;
}
