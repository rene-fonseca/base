/***************************************************************************
    The Base Framework (Test Suite)
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/string/FormatOutputStream.h>

using namespace com::azure::dev::base;

class AesApplication : public Application {
public:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
  
  AesApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[])
    : Application("aes", numberOfArguments, arguments, environment) {
  }

  void main() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "Copyright (C) 2003-2019 by Rene Moeller Fonseca" << EOL
         << ENDL;

    static uint8 DATA[5][256];
    
    for (unsigned int value = 0; value < 256; ++value) {
      DATA[0][value] = value;
    }

    for (unsigned int bit = 1; bit <= 3; ++bit) {
      for (unsigned int value = 0; value < 256; ++value) {
        uint8 intermediate = DATA[bit - 1][value];
        if (intermediate & 0x80) {
          DATA[bit][value] = (intermediate << 1) ^ 0x1b;
        } else {
          DATA[bit][value] = (intermediate << 1);
        }
      }
    }
    
    fout << EOL << EOL << ENDL;
    static const unsigned int FACTORS[] = {
      0x02, 0x03, 0x09, 0x0b, 0x0d, 0x0e
    };
    
    for (unsigned int i = 0; i < getArraySize(FACTORS); ++i) {
      const unsigned int factor = FACTORS[i];
      if (!(factor <= 0x0f)) {
        throw OutOfRange(this);
      }
      fout << "const uint8 Aes::DOT_"
           << ASCIITraits::valueToDigit(factor, true) << "[256] = {"
           << EOL;
      for (unsigned int value = 0; value < 256;) {
        uint8 result = 0;
        result ^= (factor & 1) ? DATA[0][value] : 0;
        result ^= (factor & 2) ? DATA[1][value] : 0;
        result ^= (factor & 4) ? DATA[2][value] : 0;
        result ^= (factor & 8) ? DATA[3][value] : 0;
        
        if (value % 8 == 0) {
          fout << SP << SP;
        }
        fout << HEX << ZEROPAD << PREFIX << setWidth(4) << (result & 0xff);
        
        ++value;
        if (value != 256) {
          fout << ',';
        }
        fout << SP;
        if (value % 8 == 0) {
          fout << "// " << HEX << PREFIX << ZEROPAD << setWidth(4)
               << (value - 8) << EOL;
        }
        
      }
      fout << '}' << ';' << EOL << ENDL;
    }
    fout << ENDL;
  }
};

APPLICATION_STUB(AesApplication);
