/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/collection/Map.h>

using namespace base;

int main() {
  fout << "Testing Map implementation" << ENDL;

  fout << "Initializing map" << ENDL;
  Map<int, int> mii;

  fout << "Adding associations to map ((2,2), (4,3), and (3,4))" << ENDL;
  mii.add(2, 2);
  mii.add(4, 3);
  mii.add(3, 4);
  fout << "mii: " << mii << ENDL;

  fout << "size: " << mii.getSize() << ENDL;

  fout << "Non-modifying enumeration of map (calculate sum of values)" << ENDL;
  Map<int, int>::ReadOnlyEnumeration enu(mii);
  int sum = 0;
  while (enu.hasNext()) {
    const Map<int, int>::Node* node = enu.next()->getValue();
    sum += node->getValue();
  }
  fout << "sum: " << sum << ENDL;

  fout << "Adding associations to map ((1,6), (2,5), (4,2), and (5,1))" << ENDL;
  mii.add(1, 6);
  mii.add(2, 5);
  mii.add(4, 2);
  mii.add(5, 1);
  fout << "mii: " << mii << ENDL;

  fout << "Removing associations from map (4 and 3)" << ENDL;
  mii.remove(4);
  mii.remove(3);
  fout << "mii: " << mii << ENDL;

  fout << "Size: " << mii.getSize() << ENDL;
  fout << "Removing all associations from the map" << ENDL;
  mii.removeAll();
  fout << "Size: " << mii.getSize() << ENDL;

  return 0;
}
