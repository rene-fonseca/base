/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/collection/Map.h>

using namespace dk::sdu::mip::base;

class MapApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  MapApplication(int argc, const char* argv[], const char* env[]) throw()
    : Application(MESSAGE("Map"), argc, argv, env) {
  }
  
  void main() throw() {
    fout << getFormalName() << MESSAGE(" version ") << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;
    
    fout << MESSAGE("Initializing map") << ENDL;
    Map<int, int> mii;

    fout << MESSAGE("Adding associations to map ((2,2), (4,3), and (3,4))") << ENDL;
    mii.add(2, 2);
    mii.add(4, 3);
    mii.add(3, 4);
    fout << MESSAGE("mii: ") << mii << ENDL;

    fout << MESSAGE("size: ") << mii.getSize() << ENDL;

    {
      fout << MESSAGE("Modifying enumeration of values of map (multiply by 3)") << ENDL;
      Map<int, int>::ValueEnumerator enu = mii.getValueEnumerator();
      while (enu.hasNext()) {
        *enu.next() *= 3;
      }
      fout << MESSAGE("mii: ") << mii << ENDL;
    }

    {
      fout << MESSAGE("Non-modifying enumeration of map (calculate sum of values)") << ENDL;
      Map<int, int>::ReadEnumerator enu = mii.getReadEnumerator();
      int sum = 0;
      while (enu.hasNext()) {
        sum += *enu.next()->getValue();
      }
      fout << MESSAGE("sum: ") << sum << ENDL;
    }

    fout << MESSAGE("Adding associations to map ((1,6), (2,5), (4,2), and (5,1))") << ENDL;
    mii.add(1, 6);
    mii.add(2, 5);
    mii.add(4, 2);
    mii.add(5, 1);
    fout << MESSAGE("mii: ") << mii << ENDL;

    fout << MESSAGE("Removing associations from map (4 and 3)") << ENDL;
    mii.remove(4);
    mii.remove(3);
    fout << MESSAGE("mii: ") << mii << ENDL;

    fout << MESSAGE("Size: ") << mii.getSize() << ENDL;
    fout << MESSAGE("Removing all associations from the map") << ENDL;
    mii.removeAll();
    fout << MESSAGE("Size: ") << mii.getSize() << ENDL;
  }
};

STUB(MapApplication);
