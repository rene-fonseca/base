/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

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
    : Application("Map", argc, argv, env) {
  }
  
  void main() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "https://www.fonseca.dk/base" << EOL
         << "Copyright (C) 2001-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>" << EOL
         << ENDL;
    
    fout << "Initializing map" << ENDL;
    Map<int, int> mii;

    fout << "Adding associations to map ((2,2), (4,3), and (3,4))" << ENDL;
    mii.add(2, 2);
    mii.add(4, 3);
    mii.add(3, 4);
    fout << "mii: " << mii << ENDL;

    fout << "size: " << mii.getSize() << ENDL;

    {
      fout << "Modifying enumeration of values of map (multiply by 3)" << ENDL;
      Map<int, int>::ValueEnumerator enu = mii.getValueEnumerator();
      while (enu.hasNext()) {
        *enu.next() *= 3;
      }
      fout << "mii: " << mii << ENDL;
    }

    {
      fout << "Non-modifying enumeration of map (calculate sum of values)" << ENDL;
      Map<int, int>::ReadEnumerator enu = mii.getReadEnumerator();
      int sum = 0;
      while (enu.hasNext()) {
        sum += enu.next()->getValue();
      }
      fout << "sum: " << sum << ENDL;
    }

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
  }
};

STUB(MapApplication);
