/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/string/FormatOutputStream.h>
#include <base/FloatingPoint.h>

using namespace dk::sdu::mip::base;

class FloatingPointApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  FloatingPointApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw()
    : Application(MESSAGE("floatingPoint"), numberOfArguments, arguments, environment) {
  }

  enum FloatingPointRepresentation {
    IEEE_754_SINGLE_PRECISION,
    IEEE_754_DOUBLE_PRECISION,
    IEEE_EXTENDED_DOUBLE_PRECISION_96,
    IEEE_EXTENDED_DOUBLE_PRECISION_128,
    IEEE_QUADRUPLE_PRECISION,
    UNSPECIFIED
  };

  template<class TYPE>
  static FloatingPointRepresentation getRepresentation() throw() {
    if (typeid(TYPE) == typeid(FloatingPoint::IEEE754SinglePrecision)) {
      return IEEE_754_SINGLE_PRECISION;
    } else
    if (typeid(TYPE) == typeid(FloatingPoint::IEEE754DoublePrecision)) {
      return IEEE_754_DOUBLE_PRECISION;
    } else
    if (typeid(TYPE) == typeid(FloatingPoint::IEEEExtendedDoublePrecision96)) {
      return IEEE_EXTENDED_DOUBLE_PRECISION_96;
    } else
    if (typeid(TYPE) == typeid(FloatingPoint::IEEEExtendedDoublePrecision128)) {
      return IEEE_EXTENDED_DOUBLE_PRECISION_128;
    } else
    if (typeid(TYPE) == typeid(FloatingPoint::IEEEQuadruplePrecision)) {
      return IEEE_QUADRUPLE_PRECISION;
    } else {
      return UNSPECIFIED;
    }
  }

  template<class TYPE>
  static String getRepresentationAsString() throw() {
    static const StringLiteral REPRESENTATION[] = {
      MESSAGE("IEEE 754 single precision"),
      MESSAGE("IEEE 754 double precision"),
      MESSAGE("IEEE extended double precision 96 bit"),
      MESSAGE("IEEE extended double precision 128 bit"),
      MESSAGE("IEEE quadruple precision"),
      MESSAGE("Unspecified")
    };
    return REPRESENTATION[getRepresentation<TYPE>()];
  }
  
  void main() throw() {
    fout << getFormalName() << MESSAGE(" version ") << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;

    fout << MESSAGE("Representation of 'float': ")
         << getRepresentationAsString<FloatingPoint::FloatRepresentation>() << EOL
         << MESSAGE("Representation of 'double': ")
         << getRepresentationAsString<FloatingPoint::DoubleRepresentation>() << EOL
         << MESSAGE("Representation of 'long double': ")
         << getRepresentationAsString<FloatingPoint::LongDoubleRepresentation>() << ENDL;
    
    fout << setWidth(22) << SCIENTIFIC << 1.0096670247046187e-1 << ' ' << FLUSH
         << setWidth(10) << setPrecision(3) << SCIENTIFIC << 1.0096670247046187e-1 << ENDL;
    fout << setWidth(22) << SCIENTIFIC << 0.0949123123123 << ' ' << FLUSH
         << setWidth(10) << setPrecision(3) << SCIENTIFIC << 0.0949123123123 << ENDL;
    fout << setWidth(22) << SCIENTIFIC << 0.0952523123123 << ' ' << FLUSH
         << setWidth(10) << setPrecision(3) << SCIENTIFIC << 0.0952523123123 << ENDL;
    fout << setWidth(22) << SCIENTIFIC << 0.08950523123123 << ' ' << FLUSH
         << setWidth(10) << setPrecision(3) << SCIENTIFIC << 0.08950523123123 << ENDL;
    fout << setWidth(22) << SCIENTIFIC << 0.09850623123123 << ' ' << FLUSH
         << setWidth(10) << setPrecision(3) << SCIENTIFIC << 0.09850923123123 << ENDL;
    fout << setWidth(22) << SCIENTIFIC << 0.09940623123123 << ' ' << FLUSH
         << setWidth(10) << setPrecision(3) << SCIENTIFIC << 0.09940923123123 << ENDL;
    fout << setWidth(22) << SCIENTIFIC << 0.09950623123123 << ' ' << FLUSH
         << setWidth(10) << setPrecision(3) << SCIENTIFIC << 0.09950923123123 << ENDL;
    
    for (unsigned int i = 0; i < 100; ++i) {
      fout << setWidth(2) << i
           << setWidth(25) << SCIENTIFIC << i/931.0 << ' ' << FLUSH
           << setWidth(10) << setPrecision(3) << SCIENTIFIC << i/931.0 << FLUSH
           << setWidth(12) << setPrecision(3) << ENGINEERING << i/931.0 << FLUSH
           << setWidth(8) << setPrecision(3) << FIXED << i/931.0 << FLUSH
           << setWidth(25) << FIXED << i/931.0 << ENDL;
    }
  }
};

STUB(FloatingPointApplication);
