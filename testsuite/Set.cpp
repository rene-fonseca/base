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
#include <base/collection/Set.h>
#include <base/collection/Functor.h>
#include <base/Functor.h>

using namespace com::azure::dev::base;

class SetApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  SetApplication(int argc, const char* argv[], const char* env[]) throw()
    : Application("Set", argc, argv, env) {
  }
  
  void main() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "https://www.fonseca.dk/base" << EOL
         << "Copyright (C) 2001-2019 by Rene Moeller Fonseca" << EOL
         << ENDL;
    
    fout << "Initializing set" << ENDL;
    Set<int> si;
    fout << "si: " << si << EOL;

    fout << "Adding elements to set (4, 7, 2, 3, 8, 6, 1, and 5)" << ENDL;
    si.add(4);
    si.add(7);
    si.add(2);
    si.add(3);
    si.add(8);
    si.add(6);
    si.add(1);
    si.add(5);
    fout << "si: " << si << EOL;

    Set<int>::Enumerator enu = si.getEnumerator();

    fout << "Non-modifying enumerating elements of set (calculating sum)" << ENDL;
    Sum<int> sum;
    forEach(si, sum);
    fout << "sum: " << sum.getResult() << ENDL;

    fout << "Checking whether 4 is in set: " << si.isKey(4) << ENDL;
    fout << "Checking whether 0 is in set: " << si.isKey(0) << ENDL;

    fout << "Size: " << si.getSize() << ENDL;
    fout << "Removing all elements from the set" << ENDL;
    si.removeAll();
    fout << "Size: " << si.getSize() << ENDL;
  }
};

STUB(SetApplication);
