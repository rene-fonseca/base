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
#include <base/collection/Set.h>
#include <base/collection/Functor.h>
#include <base/Functor.h>

using namespace base;

class SetApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  SetApplication(int argc, const char* argv[], const char* env[]) throw()
    : Application(MESSAGE("Set"), argc, argv, env) {
  }
  
  void main() throw() {
    fout << getFormalName() << MESSAGE(" version ") << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;
    
    fout << MESSAGE("Initializing set") << ENDL;
    Set<int> si;
    fout << MESSAGE("si: ") << si << EOL;

    fout << MESSAGE("Adding elements to set (4, 7, 2, 3, 8, 6, 1, and 5)") << ENDL;
    si.add(4);
    si.add(7);
    si.add(2);
    si.add(3);
    si.add(8);
    si.add(6);
    si.add(1);
    si.add(5);
    fout << MESSAGE("si: ") << si << EOL;

    Set<int>::Enumerator enu = si.getEnumerator();

    fout << MESSAGE("Non-modifying enumerating elements of set (calculating sum)") << ENDL;
    Sum<int> sum;
    forEach(si, sum);
    fout << MESSAGE("sum: ") << sum.getResult() << ENDL;

    fout << MESSAGE("Checking whether 4 is in set: ") << si.isKey(4) << ENDL;
    fout << MESSAGE("Checking whether 0 is in set: ") << si.isKey(0) << ENDL;

    fout << MESSAGE("Size: ") << si.getSize() << ENDL;
    fout << MESSAGE("Removing all elements from the set") << ENDL;
    si.removeAll();
    fout << MESSAGE("Size: ") << si.getSize() << ENDL;
  }
};

STUB(SetApplication);
