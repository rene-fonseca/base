/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    Copyright (C) 2001-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/mem/Allocator.h>
#include <base/mem/ReferenceCountedAllocator.h>
#include <base/mem/CapacityAllocator.h>
#include <base/mem/ReferenceCountedCapacityAllocator.h>
#include <base/string/String.h>
#include <base/collection/Functor.h>

using namespace dk::sdu::mip::base;

template<class TYPE>
class Validate {
private:

  TYPE value;
public:
  
  inline Validate(const TYPE& _value, const TYPE& expected) throw()
    : value(_value) {
    if (_value != expected) {
      ferr << "Error: expected " << expected << " but got " << value << ENDL;
    }
  }

  inline operator TYPE() const throw() {
    return value;
  }
};

/** Invert case of character. */
class InvertCase : public UnaryOperation<char, char> {
public:
  inline char operator()(char value) const throw() {
    if ((value >= 'A') && (value <= 'Z')) {
      return value - 'A' + 'a';
    } else if ((value >= 'a') && (value <= 'z')) {
      return value - 'a' + 'A';
    } else {
      return value;
    }
  }
};

class StringApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  StringApplication(int argc, const char* argv[], const char* env[])
    : Application(MESSAGE("String"), argc, argv, env) {
  }
  
  void main() throw() {
    fout << getFormalName() << MESSAGE(" version ") << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2001-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;
    
    Allocator<char> a(1234);
    fout << MESSAGE("Size of Allocator<char> (a): ") << a.getSize() << ENDL;
    Allocator<char>::ReadEnumerator enua = a.getReadEnumerator();
    unsigned int counta = 0;
    while (enua.hasNext()) {
      enua.next();
      ++counta;
    }
    fout << MESSAGE("Counted number of elements in (a): ") << counta << ENDL;
    fout << ENDL;

    ReferenceCountedAllocator<char> ra(4321);
    fout << MESSAGE("Size of ReferenceCountedAllocator<char> (ra): ") << ra.getSize() << ENDL;
    ReferenceCountedAllocator<char>::ReadEnumerator enura = ra.getReadEnumerator();
    unsigned int countra = 0;
    while (enura.hasNext()) {
      enura.next();
      ++countra;
    }
    fout << MESSAGE("Counted number of elements in (ra): ") << countra << ENDL;
    fout << ENDL;

    CapacityAllocator<char> b(1234, 256);
    fout << MESSAGE("CapacityAllocator<char>::getSize(): ") << b.getSize() << ENDL;
    fout << MESSAGE("CapacityAllocator<char>::getGranularity(): ") << b.getGranularity() << ENDL;
    fout << MESSAGE("CapacityAllocator<char>::getCapacity(): ") << b.getCapacity() << ENDL;
    CapacityAllocator<char>::ReadEnumerator enub = b.getReadEnumerator();
    unsigned int countb = 0;
    while (enub.hasNext()) {
      enub.next();
      ++countb;
    }
    fout << MESSAGE("Counted number of elements in (b): ") << countb << ENDL;
    b.optimizeCapacity();
    fout << MESSAGE("CapacityAllocator<char>::getCapacity(): ") << b.getCapacity() << ENDL;
    fout << ENDL;

    ReferenceCountedCapacityAllocator<char> rb(1234, 256);
    fout << MESSAGE("ReferenceCountedCapacityAllocator<char>::getSize(): ") << rb.getSize() << ENDL;
    fout << MESSAGE("ReferenceCountedCapacityAllocator<char>::getGranularity(): ") << rb.getGranularity() << ENDL;
    fout << MESSAGE("ReferenceCountedCapacityAllocator<char>::getCapacity(): ") << rb.getCapacity() << ENDL;
    ReferenceCountedCapacityAllocator<char>::ReadEnumerator enurb = rb.getReadEnumerator();
    unsigned int countrb = 0;
    while (enurb.hasNext()) {
      enurb.next();
      ++countrb;
    }
    fout << MESSAGE("Counted number of elements in (rb): ") << countrb << ENDL;
    rb.optimizeCapacity();
    fout << MESSAGE("ReferenceCountedCapacityAllocator<char>::getCapacity(): ") << rb.getCapacity() << ENDL;
    fout << ENDL;

    fout << MESSAGE("Initializing empty string str1") << ENDL;
    String str1;
    fout << MESSAGE("Length of str1: ") << str1.getLength() << ENDL;
    fout << MESSAGE("Value of str1: ") << str1 << ENDL;

    fout << MESSAGE("Explicit initialization of string str2") << ENDL;
    String str2 = "Hello, World!";
    fout << MESSAGE("Length of str2: ") << str2.getLength() << ENDL;
    fout << MESSAGE("Value of str2: ") << str2 << ENDL;

    fout << MESSAGE("Inverting case of characters") << ENDL;
    InvertCase ii;
    transform(str2, ii);
    fout << MESSAGE("str2: ") << str2 << ENDL;

    fout << MESSAGE("Concatenation: ") << String("first") + String("SECOND") << ENDL;
    fout << ENDL;

    String string(MESSAGE("This is a string"));
    const char* temp1 = string.getElements();
    fout << MESSAGE("Testing direct read access (This is a string): ")
         << temp1 << ENDL;

    char* temp2 = string.getElements();
    fout << MESSAGE("Testing direct read and write access (This is a string): ") << temp2 << ENDL;

    fout << MESSAGE("Test: ") << getFormalName() << ENDL;
    
    fout << "String(\"prefix suffix\").startsWith(\"prefix\"): "
         << Validate<bool>(String("prefix suffix").startsWith("prefix"), true) << ENDL;
    fout << "String(\"prefix suffix\").startsWith(\"suffix\"): "
         << Validate<bool>(String("prefix suffix").startsWith("suffix"), false) << ENDL;
    fout << "String(\"prefix suffix\").endsWith(\"prefix\"): "
         << Validate<bool>(String("prefix suffix").endsWith("prefix"), false) << ENDL;
    fout << "String(\"prefix suffix\").endsWith(\"suffix\"): "
         << Validate<bool>(String("prefix suffix").endsWith("suffix"), true) << ENDL;
    
    fout << String("String literal") << ENDL;
    
    fout << "String literal" << ENDL;
  }
};

STUB(StringApplication);
