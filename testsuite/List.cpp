/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/Object.h>
#include <base/collection/List.h>
#include <iostream>

using namespace std;

int main() {

  cout << "Initializing list\n";
  List<int> li;

  cout << "Appending elements to list\n";
  li.append(int(4));
  li.append(int(5));
  li.append(int(6));
  li.append(int(7));

  cout << "Prepending elements to list\n";
  li.prepend(int(3));
  li.prepend(int(2));
  li.prepend(int(1));
  li.prepend(int(0));

  cout << "Adding elements to list\n";
  li.add(int(8));
  li.add(int(9));
  li.add(int(10));
  li.add(int(11));

  cout << "Size of list: " << li.getSize() << "\n";

  cout << "Modifying enumerating elements of list\n";
  List<int>::Enumeration enu(li);
  unsigned int position = 0;
  while (enu.hasNext()) {
    int* value = enu.next();
    cout << "position " << position++ << ": " << *value << "\n";
    *value *= 2;
  }

  cout << "Non-modifying enumerating elements of list\n";
  List<int>::ReadOnlyEnumeration roenu(li);
  unsigned int roposition = 0;
  while (roenu.hasNext()) {
    cout << "position " << roposition++ << ": " << *roenu.next() << "\n";
  }

  return 0;
}
