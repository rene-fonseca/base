/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Ren� M�ller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/string/FormatOutputStream.h>
#include <base/string/StringOutputStream.h>
#include <base/string/String.h>
#include <base/collection/List.h>
#include <base/collection/Set.h>
#include <base/collection/Map.h>
#include <base/Date.h>
#include <base/net/Url.h>

using namespace base;

void test() {
  fout << "Testing Streaming..." << ENDL;

  fout << "Writing built-in types to fout" << ENDL;
  fout << "short (-1234): " << short(-1234) << ENDL;
  fout << "int (-123456): " << int(-123456) << ENDL;
  fout << "long (-123456): " << -123456L << ENDL;
  fout << "long long (-1234567890): " << -1234567890LL << ENDL;
  fout << "char (A): " << 'A' << ENDL;
  fout << "char* (Hello, World!): " << "Hello, World!" << ENDL;

  ferr << "Writing to ferr: " << "This is written to standard error" << ENDL;

  fout << "String<char>: " << String("This is a String object") << ENDL;
  fout << "Date (now): " << Date::getNow() << ENDL;

  StringOutputStream stream;
  stream << "This " << "is " << "written " << "to " << "a " << "string " << "stream" << FLUSH;
  fout << "String: " << stream.getString() << ENDL;

  List<int> li;
  for (unsigned int i = 0; i < 10; i++) {
    li.add(i);
  }
  fout << "List<int>: " << li << ENDL;

  Set<int > si;
  for (unsigned int i = 0; i < 10; i++) {
    si.add(i);
  }
  fout << "Set<int>: "<< si << ENDL;

  Map<int, int > mss;
  for (unsigned int i = 0; i < 10; i++) {
    mss.add(i, i);
  }
  fout << "Map<int, int>: "<< mss << ENDL;

  fout << "Url: " << Url("http://fonseca:password@www.mip.sdu.dk:80/~fonseca/base/", false) << ENDL;
}

int main() {
  try {
    test();
  } catch(Exception& e) {
    ferr << "Exception: " << e.getMessage() << ENDL;
    return 1;
  } catch(...) {
    ferr << "Unknown exception" << ENDL;
    return 1;
  }
  return 0;
}
