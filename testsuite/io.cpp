/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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
#include <base/collection/Pair.h>
#include <base/Date.h>
#include <base/net/Url.h>
#include <base/net/InetAddress.h>
#include <base/Functor.h>
#include <base/ByteOrder.h>
#include <base/mathematics/Constants.h>
#include <base/mathematics/Math.h>

using namespace dk::sdu::mip::base;

FormatOutputStream& operator<<(FormatOutputStream& stream, const Pair<float, float>& value) throw(IOException) {
  FormatOutputStream::PushContext push(stream);
  return stream << '{' << value.getFirst() << ',' << value.getSecond() << '}';
}

FormatOutputStream& operator<<(FormatOutputStream& stream, const Pair<double, double>& value) throw(IOException) {
  return stream << '{' << value.getFirst() << ',' << value.getSecond() << '}';
}

class IOApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:
  
  IOApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw()
    : Application(MESSAGE("io"), numberOfArguments, arguments, environment) {
  }
  
  void main() throw() {
    fout << getFormalName() << MESSAGE(" version ") << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;
    
    fout << 0.000009 << ENDL;
    fout << 0.00001 << ENDL;
    fout << 0.0002 << ENDL;
    fout << 0.003 << ENDL;
    fout << 0.04 << ENDL;
    fout << 0.5 << ENDL;
    fout << 10.5 << ENDL;
    fout << 10. << ENDL;
    fout << 100. << ENDL;
    fout << 132. << ENDL;
    fout << 1123. << ENDL;
    fout << 112300000. << ENDL;
    fout << 1123600000. << ENDL;
    fout << 11236700000. << ENDL;
    fout << 1123000000000000000000000000000000. << ENDL;

    fout << MESSAGE("Testing FormatOutputStream...") << ENDL;
  
    fout << MESSAGE("Writing built-in types to fout") << ENDL;
    fout << MESSAGE("short (-1234): ") << short(-1234) << ENDL;
    fout << MESSAGE("int (-123456): ") << int(-123456) << ENDL;
    fout << MESSAGE("long (-123456): ") << -123456L << ENDL;
    fout << MESSAGE("long long (-1234567890): ") << -1234567890LL << ENDL;
    fout << MESSAGE("char (A): ") << 'A' << ENDL;
    fout << MESSAGE("char* (Hello, World!): ") << "Hello, World!" << ENDL;
    fout << MESSAGE("void* (0): ") << static_cast<void*>(0) << ENDL;
    fout << MESSAGE("void* (Application::getApplication): ") << Application::getApplication() << ENDL;  
  
    ferr << MESSAGE("Writing to ferr: ") << MESSAGE("This is written to standard error") << ENDL;
  
    fout << MESSAGE("String<char>: ") << String("This is a String object") << ENDL;
    fout << MESSAGE("Date (now): ") << Date::getNow() << ENDL;
  
    Pair<float, float> myFloatPair(1.23456789, 1.23456789);
    fout << MESSAGE("Pair<float, float>: ") << setPrecision(3) << myFloatPair << ENDL;
    Pair<double, double> myDoublePair(1.23456789, 1.23456789);
    fout << MESSAGE("Pair<double, double>: ") << setPrecision(3) << myDoublePair << ENDL;
  
    StringOutputStream stream;
    stream << MESSAGE("This ") << MESSAGE("is ") << MESSAGE("written ") << MESSAGE("to ") << MESSAGE("a ") << MESSAGE("string ") << MESSAGE("stream") << FLUSH;
    fout << MESSAGE("String: ") << stream.getString() << ENDL;
  
    List<int> li;
    for (unsigned int i = 0; i < 10; i++) {
      li.add(i);
    }
    fout << MESSAGE("List<int>: ") << li << ENDL;

    Set<int > si;
    for (unsigned int i = 0; i < 10; i++) {
      si.add(i);
    }
    fout << MESSAGE("Set<int>: ") << si << ENDL;
  
    Map<int, int > mss;
    for (unsigned int i = 0; i < 10; i++) {
      mss.add(i, i);
    }
    fout << MESSAGE("Map<int, int>: ") << mss << ENDL;

    fout << MESSAGE("Url: ") << Url("http://fonseca:password@www.mip.sdu.dk:80/~fonseca/base/", false) << ENDL;

    fout << MESSAGE("127.0.0.1 - ") << InetAddress("127.0.0.1", InetAddress::IP_VERSION_4) << ENDL;
    fout << MESSAGE(":: - ") << InetAddress("::") << ENDL;
    fout << MESSAGE("::1 - ") << InetAddress("::1") << ENDL;
    fout << MESSAGE("1080::8:800:200c:417a - ") << InetAddress("1080::8:800:200c:417a") << ENDL;
    fout << MESSAGE("ffff::127.0.0.1 - ") << InetAddress("ffff::127.0.0.1", InetAddress::IP_VERSION_6) << ENDL;
    fout << MESSAGE("::ffff:127.0.0.1 - ") << InetAddress("::ffff:127.0.0.1") << ENDL;

    fout << MESSAGE("float [0, -0, 1, -1]: ") << static_cast<float>(0) << ' ' << static_cast<float>(-1e-5000) << ' ' << static_cast<float>(1) << ' ' << static_cast<float>(-1) << ENDL;
    fout << MESSAGE("double [0, -0, 1, -1]: ") << static_cast<double>(0) << ' ' << static_cast<double>(-1e-5000) << ' ' << static_cast<double>(1) << ' ' << static_cast<double>(-1) << ENDL;
    fout << MESSAGE("long double [0, -0, 1, -1]: ") << 0l << ' ' << -1e-5000 << ' ' << 1l << ' ' << -1l << ENDL;

    fout << MESSAGE("log10(2): ") << setPrecision(36) << constant::LOG102 << EOL
         << MESSAGE("10.5: ") << 10.5 << EOL
         << MESSAGE("149.9: ") << 149.9 << ENDL;

    for (int i = -12; i < 22; ++i) {
      long double temp = constant::LOG102 * i * i * i * Math::exp(i * constant::LN10);
      fout << setWidth(30) << SCIENTIFIC << NECESSARY << temp << ' '
           << setWidth(10) << SCIENTIFIC << setPrecision(0) << temp << ' '
           << setWidth(20) << ENGINEERING << setRadixPosition(5) << setPrecision(6) << temp << ' '
           << setWidth(30) << base::FIXED << GROUPING << setRadixPosition(20) << setPrecision(2) << temp << ENDL;
    }
  }
};

STUB(IOApplication);
