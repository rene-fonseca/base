/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/Object.h>
#include <base/mem/Allocator.h>
#include <base/mem/ReferenceCountedAllocator.h>
#include <base/string/String.h>
#include <iostream>

using namespace std;

int main() {
  cout << "Testing String...\n";

  Allocator<char> a(1234);
  cout << "Size of Allocator<char> (a): " << a.getSize() << "\n";

  ReferenceCountedAllocator<char> ra(4321);
  cout << "Size of ReferenceCountedAllocator<char> (ra): " << ra.getSize() << "\n";

  cout << "Initializing empty string str1\n";
  String<> str1;
  cout << "Length of str1: " << str1.length() << "\n";
  cout << "Value of str1: " << str1 << "\n";

  cout << "Explicit initialization of string str2\n";
  String<> str2 = "Hello, World!";
  cout << "Length of str2: " << str2.length() << "\n";
  cout << "Value of str2: " << str2 << "\n";

  return 0;
}
