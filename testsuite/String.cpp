/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/mem/Allocator.h>
#include <base/mem/ReferenceCountedAllocator.h>
#include <base/mem/CapacityAllocator.h>
#include <base/mem/ReferenceCountedCapacityAllocator.h>
#include <base/string/String.h>
#include <base/collection/Functor.h>

using namespace base;

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

int main() {
  fout << "Testing String implementation..." << EOL << ENDL;

  Allocator<char> a(1234);
  fout << "Size of Allocator<char> (a): " << a.getSize() << ENDL;
  Allocator<char>::Enumeration enua(a);
  unsigned int counta = 0;
  while (enua.hasNext()) {
    enua.next();
    ++counta;
  }
  fout << "Counted number of elements in (a): " << counta << ENDL;
  fout << ENDL;

  ReferenceCountedAllocator<char> ra(4321);
  fout << "Size of ReferenceCountedAllocator<char> (ra): " << ra.getSize() << ENDL;
  ReferenceCountedAllocator<char>::Enumeration enura(ra);
  unsigned int countra = 0;
  while (enura.hasNext()) {
    enura.next();
    ++countra;
  }
  fout << "Counted number of elements in (ra): " << countra << ENDL;
  fout << ENDL;

  CapacityAllocator<char> b(1234, 256);
  fout << "CapacityAllocator<char>::getSize(): " << b.getSize() << ENDL;
  fout << "CapacityAllocator<char>::getGranularity(): " << b.getGranularity() << ENDL;
  fout << "CapacityAllocator<char>::getCapacity(): " << b.getCapacity() << ENDL;
  CapacityAllocator<char>::Enumeration enub(b);
  unsigned int countb = 0;
  while (enub.hasNext()) {
    enub.next();
    ++countb;
  }
  fout << "Counted number of elements in (b): " << countb << ENDL;
  b.optimizeCapacity();
  fout << "CapacityAllocator<char>::getCapacity(): " << b.getCapacity() << ENDL;
  fout << ENDL;

  ReferenceCountedCapacityAllocator<char> rb(1234, 256);
  fout << "ReferenceCountedCapacityAllocator<char>::getSize(): " << rb.getSize() << ENDL;
  fout << "ReferenceCountedCapacityAllocator<char>::getGranularity(): " << rb.getGranularity() << ENDL;
  fout << "ReferenceCountedCapacityAllocator<char>::getCapacity(): " << rb.getCapacity() << ENDL;
  ReferenceCountedCapacityAllocator<char>::Enumeration enurb(rb);
  unsigned int countrb = 0;
  while (enurb.hasNext()) {
    enurb.next();
    ++countrb;
  }
  fout << "Counted number of elements in (rb): " << countrb << ENDL;
  rb.optimizeCapacity();
  fout << "ReferenceCountedCapacityAllocator<char>::getCapacity(): " << rb.getCapacity() << ENDL;
  fout << ENDL;

  fout << "Initializing empty string str1" << ENDL;
  String<> str1;
  fout << "Length of str1: " << str1.getLength() << ENDL;
  fout << "Value of str1: " << str1 << ENDL;

  fout << "Explicit initialization of string str2" << ENDL;
  String<> str2 = "Hello, World!";
  fout << "Length of str2: " << str2.getLength() << ENDL;
  fout << "Value of str2: " << str2 << ENDL;

  fout << "Inverting case of characters" << ENDL;
  InvertCase ii;
  transform(str2, ii);
  fout << "str2: " << str2 << ENDL;

  fout << "Concatenation: " << String<>("first") + String<>("SECOND") << ENDL;

  return 0;
}
