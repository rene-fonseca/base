/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Ren� M�ller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
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
  Map<int, int>::ReadEnumerator enu = mii.getReadEnumerator();
  int sum = 0;
  while (enu.hasNext()) {
    sum += *enu.next()->getValue();
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
