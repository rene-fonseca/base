/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/mem/Allocator.h>
#include <base/mem/ReferenceCountedAllocator.h>
#include <base/string/String.h>

int main() {
  fout << "Testing String implementation...\n";

  Allocator<char> a(1234);
  fout << "Size of Allocator<char> (a): " << a.getSize() << "\n";

  ReferenceCountedAllocator<char> ra(4321);
  fout << "Size of ReferenceCountedAllocator<char> (ra): " << ra.getSize() << "\n";

  fout << "Initializing empty string str1\n";
  String<> str1;
  fout << "Length of str1: " << str1.length() << "\n";
  fout << "Value of str1: " << str1 << "\n";

  fout << "Explicit initialization of string str2\n";
  String<> str2 = "Hello, World!";
  fout << "Length of str2: " << str2.length() << "\n";
  fout << "Value of str2: " << str2 << "\n";

  return 0;
}
