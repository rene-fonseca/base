/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/mem/Allocator.h>
#include <base/mem/ReferenceCountedAllocator.h>
#include <base/mem/CapacityAllocator.h>
#include <base/mem/ReferenceCountedCapacityAllocator.h>
#include <base/string/String.h>

int main() {
  fout << "Testing String implementation..." << EOL << EOL;

  Allocator<char> a(1234);
  fout << "Size of Allocator<char> (a): " << a.getSize() << EOL;
  Allocator<char>::Enumeration enua(a);
  unsigned int counta = 0;
  while (enua.hasNext()) {
    enua.next();
    ++counta;
  }
  fout << "Counted number of elements in (a): " << counta << EOL;
  fout << EOL;

  ReferenceCountedAllocator<char> ra(4321);
  fout << "Size of ReferenceCountedAllocator<char> (ra): " << ra.getSize() << EOL;
  ReferenceCountedAllocator<char>::Enumeration enura(ra);
  unsigned int countra = 0;
  while (enura.hasNext()) {
    enura.next();
    ++countra;
  }
  fout << "Counted number of elements in (ra): " << countra << EOL;
  fout << EOL;

  CapacityAllocator<char> b(1234, 256);
  fout << "CapacityAllocator<char>::getSize(): " << b.getSize() << EOL;
  fout << "CapacityAllocator<char>::getGranularity(): " << b.getGranularity() << EOL;
  fout << "CapacityAllocator<char>::getCapacity(): " << b.getCapacity() << EOL;
  CapacityAllocator<char>::Enumeration enub(b);
  unsigned int countb = 0;
  while (enub.hasNext()) {
    enub.next();
    ++countb;
  }
  fout << "Counted number of elements in (b): " << countb << EOL;
  b.optimizeCapacity();
  fout << "CapacityAllocator<char>::getCapacity(): " << b.getCapacity() << EOL;
  fout << EOL;

  ReferenceCountedCapacityAllocator<char> rb(1234, 256);
  fout << "ReferenceCountedCapacityAllocator<char>::getSize(): " << rb.getSize() << EOL;
  fout << "ReferenceCountedCapacityAllocator<char>::getGranularity(): " << rb.getGranularity() << EOL;
  fout << "ReferenceCountedCapacityAllocator<char>::getCapacity(): " << rb.getCapacity() << EOL;
  ReferenceCountedCapacityAllocator<char>::Enumeration enurb(rb);
  unsigned int countrb = 0;
  while (enurb.hasNext()) {
    enurb.next();
    ++countrb;
  }
  fout << "Counted number of elements in (rb): " << countrb << EOL;
  rb.optimizeCapacity();
  fout << "ReferenceCountedCapacityAllocator<char>::getCapacity(): " << rb.getCapacity() << EOL;
  fout << EOL;

  fout << "Initializing empty string str1\n";
  String<> str1;
  fout << "Length of str1: " << str1.length() << EOL;
  fout << "Value of str1: " << str1 << EOL;

  fout << "Explicit initialization of string str2\n";
  String<> str2 = "Hello, World!";
  fout << "Length of str2: " << str2.length() << EOL;
  fout << "Value of str2: " << str2 << EOL;

  fout << "Concatenation: " << String<>("first") + String<>("SECOND") << EOL;

  return 0;
}
