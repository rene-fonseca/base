/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/StringOutputStream.h>
#include <base/string/String.h>
#include <base/collection/List.h>
#include <base/collection/Set.h>
#include <base/collection/Map.h>
#include <base/Date.h>
#include <base/net/Url.h>
#include <base/net/InetAddress.h>
#include <base/Functor.h>
#include <base/ByteOrder.h>
#include <math.h>

using namespace base;

void test() {
  fout << "Testing FormatOutputStream..." << ENDL;

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

  fout << "127.0.0.1 - " << InetAddress("127.0.0.1", InetAddress::IP_VERSION_4) << ENDL;
  fout << ":: - " << InetAddress("::") << ENDL;
  fout << "::1 - " << InetAddress("::1") << ENDL;
  fout << "1080::8:800:200c:417a - " << InetAddress("1080::8:800:200c:417a") << ENDL;
  fout << "ffff::127.0.0.1 - " << InetAddress("ffff::127.0.0.1", InetAddress::IP_VERSION_6) << ENDL;
  fout << "::ffff:127.0.0.1 - " << InetAddress("::ffff:127.0.0.1") << ENDL;

  fout << FIXED << setPrecision(24) << 0.30102999566398119521373889472449L << ' '
       << FIXED << 1.0 << ' '
       << FIXED << 10.5 << ' '
       << FIXED << 149.9 << ENDL;

  for (int i = -12; i < 22; ++i) {
    long double temp = 0.30102999566398119521373889472449L * i * i * i * exp(i * log(10.0L));
    fout << setWidth(30) << SCIENTIFIC << NECESSARY << temp << " "
         << setWidth(10) << SCIENTIFIC << setPrecision(0) << temp << " "
         << setWidth(20) << ENGINEERING << setRadixPosition(5) << setPrecision(6) << temp << " "
         << setWidth(30) << FIXED << GROUPING << setRadixPosition(20) << setPrecision(2) << temp << ENDL;
  }
}

int main(int argc, const char* argv[], const char* envp[]) {
  Application app("io", argc, argv, envp);
  try {
    test();
  } catch(Exception& e) {
    return Application::getApplication()->exceptionHandler(e);
  } catch(...) {
    return Application::getApplication()->exceptionHandler();
  }
  return Application::EXIT_CODE_NORMAL;
}
