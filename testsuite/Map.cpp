/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/collection/Map.h>

int main() {
  fout << "Testing Map implementation\n";

  fout << "Initializing map\n";
  Map<int, int> mii;

  fout << "Adding associations to map ((2,2), (4,3), and (3,4))\n";
  mii.add(2, 2);
  mii.add(4, 3);
  mii.add(3, 4);
  fout << "mii: " << mii << EOL;

  fout << "size: " << mii.getSize() << EOL;

  fout << "Non-modifying enumeration of map (calculate sum of values)\n";
  Map<int, int>::ReadOnlyEnumeration enu(mii);
  int sum = 0;
  while (enu.hasNext()) {
    const Map<int, int>::Node* node = enu.next()->getValue();
    sum += node->getValue();
  }
  fout << "sum: " << sum << EOL;

  fout << "Adding associations to map ((1,6), (2,5), (4,2), and (5,1))\n";
  mii.add(1, 6);
  mii.add(2, 5);
  mii.add(4, 2);
  mii.add(5, 1);
  fout << "mii: " << mii << EOL;

  fout << "Removing associations from map (4 and 3)\n";
  mii.remove(4);
  mii.remove(3);
  fout << "mii: " << mii << EOL;

  fout << "Size: " << mii.getSize() << "\n";
  fout << "Removing all associations from the map\n";
  mii.removeAll();
  fout << "Size: " << mii.getSize() << "\n";

  return 0;
}
