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
#include <base/collection/HashSet.h>
#include <base/collection/Functor.h>
#include <base/Functor.h>

using namespace com::azure::dev::base;

class HashSetApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  HashSetApplication()
    : Application("HashSet")
  {
  }
  
  void main()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << ENDL;
    
    fout << "Initializing set" << ENDL;
    HashSet<int> si;
    fout << "si: " << si << EOL;

    fout << "Adding elements to set (4, 7, 2, 3, 8, 6, 1, and 5)" << ENDL;
    si.add(4);
    
    fout << "si: " << si << EOL;
    
    si.add(7);
    si.add(2);
    si.add(3);
    si.add(8);
    si.add(6);
    si.add(1);
    si.add(5);
    fout << "si: " << si << EOL;

    HashSet<int>::Enumerator enu = si.getEnumerator();

    fout << "Non-modifying enumerating elements of set (calculating sum)" << ENDL;
    Sum<int> sum;
    forEach(si, sum);
    fout << "sum: " << sum.getResult() << ENDL;

    fout << "Checking whether 4 is in set: " << si.hasValue(4) << ENDL;
    fout << "Checking whether 0 is in set: " << si.hasValue(0) << ENDL;

    fout << "Size: " << si.getSize() << ENDL;
    fout << "Removing all elements from the set" << ENDL;
    si.removeAll();
    fout << "Size: " << si.getSize() << ENDL;

    static const Literal WORDS[] = {
      MESSAGE("hi"),
      MESSAGE("hello"),
      MESSAGE("world"),
      MESSAGE("the"),
      MESSAGE("base"),
      MESSAGE("framework"),
      MESSAGE("word"),
      MESSAGE("this"),
      MESSAGE("keyword"),
      MESSAGE("here"),
      MESSAGE("other"),
      MESSAGE("other2"),
      MESSAGE("other3"),
      MESSAGE("a"),
      MESSAGE("b"),
      MESSAGE("c"),
      MESSAGE("0")
    };
    
    HashSet<String> hashSet;
    for (unsigned int i = 0; i < getArraySize(WORDS); ++i) {
      hashSet.add(WORDS[i]);
    }
    fout << hashSet << ENDL;
    
    hashSet.remove(MESSAGE("other"));
    
    fout << "Removing non-existing value (exception InvalidNode expected)"
         << ENDL;
    try {
#if (!defined(_COM_AZURE_DEV__BASE__NO_EXCEPTIONS)) && \
    (_COM_AZURE_DEV__BASE__ARCH != _COM_AZURE_DEV__BASE__SPARC64) && \
    (_COM_AZURE_DEV__BASE__ARCH != _COM_AZURE_DEV__BASE__POWERPC64) && \
    (_COM_AZURE_DEV__BASE__ARCH != _COM_AZURE_DEV__BASE__S390X)
      hashSet.remove(MESSAGE("other"));
#endif
    } catch (InvalidNode& e) {
      fout << e << ENDL;
    }
  }
};

APPLICATION_STUB(HashSetApplication);
