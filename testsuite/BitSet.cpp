/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/collection/BitSet.h>
#include <base/collection/Functor.h>
#include <base/Functor.h>
#include <base/TypeInfo.h>

using namespace dk::sdu::mip::base;

class BitSetApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:
  
  BitSetApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application("BitSet", numberOfArguments, arguments, environment) {
  }
  
  void main() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "http://www.mip.sdu.dk/~fonseca/base" << EOL
         << "Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>" << EOL
         << ENDL;
    
    BitSet bitSet(1024, false);
    fout << "Size: " << bitSet.getSize() << ENDL;
    
    bitSet[256] = !bitSet[256];
    bitSet[512] = true;
    
    {
      BitSet::ReadEnumerator enu = bitSet.getReadEnumerator();
      fout << "Bits: ";
      while (enu.hasNext()) {
        fout << (*enu.next() ? '1' : '0');
      }
      fout << ENDL;
    }
    
    bitSet.flip();
    bitSet >>= 128 - 1;
    bitSet <<= 128 - 2;
    fout << "Size: " << bitSet.getSize() << ENDL;
    
    {
      BitSet::Enumerator enu = bitSet.getEnumerator();
      bool value = false;
      while (enu.hasNext()) {
        *enu.next() ^= value;
        value = !value;
      }
    }
    
    {
      BitSet::ReadEnumerator enu = bitSet.getReadEnumerator();
      fout << "Bits: ";
      while (enu.hasNext()) {
        fout << (*enu.next() ? '1' : '0');
      }
      fout << ENDL;
    }
  }
};

STUB(BitSetApplication);
