/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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
#include <base/mathematics/Constants.h>
#include <math.h> // TAG: alien header file

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
  fout << "void* (0): " << static_cast<void*>(0) << ENDL;
  fout << "void* (Application::getApplication): " << Application::getApplication() << ENDL;  

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

  fout << "float [0, -0, 1, -1]: " << static_cast<float>(0) << " " << static_cast<float>(-1e-5000) << " " << static_cast<float>(1) << " " << static_cast<float>(-1) << ENDL;
  fout << "double [0, -0, 1, -1]: " << static_cast<double>(0) << " " << static_cast<double>(-1e-5000) << " " << static_cast<double>(1) << " " << static_cast<double>(-1) << ENDL;
  fout << "long double [0, -0, 1, -1]: " << 0l << " " << -1e-5000 << " " << 1l << " " << -1l << ENDL;

  fout << "log10(2): " << setPrecision(36) << constant::LOG102 << EOL
       << "10.5: " << 10.5 << EOL
       << "149.9: " << 149.9 << ENDL;

  for (int i = -12; i < 22; ++i) {
    long double temp = constant::LOG102 * i * i * i * exp(i * constant::LN10);
    fout << setWidth(30) << SCIENTIFIC << NECESSARY << temp << " "
         << setWidth(10) << SCIENTIFIC << setPrecision(0) << temp << " "
         << setWidth(20) << ENGINEERING << setRadixPosition(5) << setPrecision(6) << temp << " "
         << setWidth(30) << base::FIXED << GROUPING << setRadixPosition(20) << setPrecision(2) << temp << ENDL;
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
