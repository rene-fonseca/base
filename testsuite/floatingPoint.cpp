/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/string/FormatOutputStream.h>
#include <base/FloatingPoint.h>

using namespace com::azure::dev::base;

class FloatingPointApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:

  FloatingPointApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[])
    : Application("floatingPoint", numberOfArguments, arguments, environment)
  {
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
  static FloatingPointRepresentation getRepresentation()
  {
    if (std::is_same<TYPE, FloatingPoint::Representation::IEEE754SinglePrecision>()) {
      return IEEE_754_SINGLE_PRECISION;
    } else if (std::is_same<TYPE, FloatingPoint::Representation::IEEE754DoublePrecision>()) {
      return IEEE_754_DOUBLE_PRECISION;
    } else if (std::is_same<TYPE, FloatingPoint::Representation::IEEEExtendedDoublePrecision96>()) {
      return IEEE_EXTENDED_DOUBLE_PRECISION_96;
    } else if (std::is_same<TYPE, FloatingPoint::Representation::IEEEExtendedDoublePrecision128>()) {
      return IEEE_EXTENDED_DOUBLE_PRECISION_128;
    } else if (std::is_same<TYPE, FloatingPoint::Representation::IEEEQuadruplePrecision>()) {
      return IEEE_QUADRUPLE_PRECISION;
    } else {
      return UNSPECIFIED;
    }
  }

  template<class TYPE>
  static String getRepresentationAsString()
  {
    static const Literal REPRESENTATION[] = {
      MESSAGE("IEEE 754 single precision"),
      MESSAGE("IEEE 754 double precision"),
      MESSAGE("IEEE extended double precision 96 bit"),
      MESSAGE("IEEE extended double precision 128 bit"),
      MESSAGE("IEEE quadruple precision"),
      MESSAGE("Unspecified")
    };
    return REPRESENTATION[getRepresentation<TYPE>()];
  }
  
  void main()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << ENDL;

    fout << "Representation of 'float': "
         << getRepresentationAsString<FloatingPoint::FloatRepresentation>() << EOL
         << "Representation of 'double': "
         << getRepresentationAsString<FloatingPoint::DoubleRepresentation>() << EOL
         << "Representation of 'long double': "
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

APPLICATION_STUB(FloatingPointApplication);
