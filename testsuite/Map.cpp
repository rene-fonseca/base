/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/Object.h>
#include <base/collection/Map.h>
#include <iostream>

using namespace std;

int main() {

  cout << "Initializing map\n";
  Map<int, int> mii;

  cout << "Adding elements to map\n";
  mii.add(1, 2);
  mii.add(2, 3);
  mii.add(3, 4);

  cout << "Size of map: " << mii.getSize() << "\n";

//  cout << "Modifying enumerating elements of list\n";
//  List<int>::Enumeration enu(li);
//  unsigned int position = 0;
//  while (enu.hasNext()) {
//    int* value = enu.next();
//    cout << "position " << position++ << ": " << *value << "\n";
//    *value *= 2;
//  }

  cout << "Non-modifying enumerating elements of map\n";
  Map<int, int>::ReadOnlyEnumeration enu1(mii);
  while (enu1.hasNext()) {
    const Map<int, int>::Node* node = enu1.next()->getValue();
    cout << "[" << node->getKey() << "]=" << node->getValue() << "\n";
  }

  cout << "Adding elements to map\n";
  mii.add(2, 5);
  mii.add(4, 2);
  mii.add(5, 1);

  cout << "Non-modifying enumerating elements of map\n";
  Map<int, int>::ReadOnlyEnumeration enu2(mii);
  while (enu2.hasNext()) {
    const Map<int, int>::Node* node = enu2.next()->getValue();
    cout << "[" << node->getKey() << "]=" << node->getValue() << "\n";
  }

  return 0;
}
