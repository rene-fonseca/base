/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/string/FormatOutputStream.h>
#include <base/string/String.h>
#include <base/collection/List.h>
#include <base/collection/Set.h>
#include <base/collection/Map.h>

void test() {
  fout << "Testing Streaming...\n";

  fout << "Writing built-in types to fout\n";
  fout << "short (-1234): " << short(-1234) << EOL;
  fout << "int (-123456): " << int(-123456) << EOL;
//  fout << "long long (-1234567890): " << -1234567890L << EOL;
  fout << "char (A): " << 'A' << EOL;
  fout << "char* (Hello, World!): " << "Hello, World!" << EOL;

  ferr << "Writing to ferr: " << "This is written to standard error" << EOL;

  fout << "String<char>: " << String<>("This is a String object") << EOL;

  List<int> li;
  for (unsigned int i = 0; i < 10; i++) {
    li.add(i);
  }
  fout << "List<int>: " << li << EOL;

  Set<int > si;
  for (unsigned int i = 0; i < 10; i++) {
    si.add(i);
  }
  fout << "Set<int>: "<< si << EOL;

  Map<int, int > mss;
  for (unsigned int i = 0; i < 10; i++) {
    mss.add(i, i);
  }
  fout << "Map<int, int>: "<< mss << EOL;

  fout.flush();
}

int main() {
  try {
    test();
  } catch(Exception& e) {
    ferr << "Exception: " << e.getMessage() << "\n";
    return 1;
  } catch(...) {
    ferr << "Unknown exception\n";
    return 1;
  }
  return 0;
}
