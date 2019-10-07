/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/FileInputStream.h>
#include <base/io/FileOutputStream.h>
#include <base/filesystem/FileSystem.h>
#include <base/string/FormatOutputStream.h>
#include <base/Application.h>
#include <base/security/AdvancedEncryptionStandard.h>
#include <base/UnexpectedFailure.h>
#include <base/Timer.h>

using namespace dk::sdu::mip::base;

class CipherApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
  static const unsigned int BLOCK_SIZE = 4096 * 4;
  
  uint8 buffer[BLOCK_SIZE];
public:
  
  CipherApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application(
        "cipher",
        numberOfArguments,
        arguments,
        environment) {
  }

  void testAes128() throw() {
    static const uint8 KEY[4 * 4] = {
      0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
      0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
    };
    static const uint8 CLEAR_TEXT[4 * 4] = {
      0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d,
      0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34
    };
    static const uint8 CIPHER_TEXT[4 * 4] = {
      0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb,
      0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32
    };
    
    fout << "Aes-128:" << EOL;
    fout << indent(2) << " clear text: ";
    for (unsigned int i = 0; i < 16; i++) {
      fout << HEX << ZEROPAD << PREFIX << setWidth(4) << CLEAR_TEXT[i] << SP;
    }
    fout << ENDL;
    
    AdvancedEncryptionStandard forward(
      KEY,
      AdvancedEncryptionStandard::CIPHER_128
    );
    uint8 cipherText[16];
    unsigned int cipherBytes =
      forward.push(cipherText, CLEAR_TEXT, sizeof(CLEAR_TEXT));
    bassert(cipherBytes == sizeof(CLEAR_TEXT), UnexpectedFailure(this));
    
    fout << indent(2) << "cipher text: ";
    for (unsigned int i = 0; i < 16; i++) {
      fout << HEX << ZEROPAD << PREFIX << setWidth(4) << cipherText[i] << SP;
    }
    fout << ENDL;
    
    bassert(
      compare(cipherText, CIPHER_TEXT, sizeof(CIPHER_TEXT)) == 0,
      UnexpectedFailure(this)
    );
    
    AdvancedEncryptionStandard inverse(
      KEY,
      AdvancedEncryptionStandard::CIPHER_128,
      true
    );
    uint8 clearText[sizeof(CIPHER_TEXT)];
    inverse.push(clearText, cipherText, sizeof(cipherText));
    fout << indent(2) << " clear text: ";
    for (unsigned int i = 0; i < 16; i++) {
      fout << HEX << ZEROPAD << PREFIX << setWidth(4) << clearText[i] << SP;
    }
    fout << ENDL;
    
    bassert(
      compare(clearText, CLEAR_TEXT, sizeof(CLEAR_TEXT)) == 0,
      UnexpectedFailure(this)
    );
  }

  void testAes128Second() throw() {
    static const uint8 KEY[4 * 4] = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
      0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
    };
    static const uint8 CLEAR_TEXT[4 * 4] = {
      0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
      0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
    };
    static const uint8 CIPHER_TEXT[4 * 4] = {
      0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b, 0x04, 0x30,
      0xd8, 0xcd, 0xb7, 0x80, 0x70, 0xb4, 0xc5, 0x5a
    };
    
    fout << "Aes-128:" << EOL;
    fout << indent(2) << " clear text: ";
    for (unsigned int i = 0; i < 16; i++) {
      fout << HEX << ZEROPAD << PREFIX << setWidth(4) << CLEAR_TEXT[i] << SP;
    }
    fout << ENDL;
    
    AdvancedEncryptionStandard forward(
      KEY,
      AdvancedEncryptionStandard::CIPHER_128
    );
    uint8 cipherText[16];
    unsigned int cipherBytes =
      forward.push(cipherText, CLEAR_TEXT, sizeof(CLEAR_TEXT));
    bassert(cipherBytes == sizeof(CLEAR_TEXT), UnexpectedFailure(this));
    
    fout << indent(2) << "cipher text: ";
    for (unsigned int i = 0; i < 16; i++) {
      fout << HEX << ZEROPAD << PREFIX << setWidth(4) << cipherText[i] << SP;
    }
    fout << ENDL;
    
    bassert(
      compare(cipherText, CIPHER_TEXT, sizeof(CIPHER_TEXT)) == 0,
      UnexpectedFailure(this)
    );
    
    AdvancedEncryptionStandard inverse(
      KEY,
      AdvancedEncryptionStandard::CIPHER_128,
      true
    );
    uint8 clearText[sizeof(CIPHER_TEXT)];
    inverse.push(clearText, cipherText, sizeof(cipherText));
    fout << indent(2) << " clear text: ";
    for (unsigned int i = 0; i < 16; i++) {
      fout << HEX << ZEROPAD << PREFIX << setWidth(4) << clearText[i] << SP;
    }
    fout << ENDL;
    
    bassert(
      compare(clearText, CLEAR_TEXT, sizeof(CLEAR_TEXT)) == 0,
      UnexpectedFailure(this)
    );
  }

  void testAes192() throw() {
    static const uint8 KEY[4 * 6] = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
      0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
      0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17
    };
    static const uint8 CLEAR_TEXT[4 * 4] = {
      0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
      0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
    };
    static const uint8 CIPHER_TEXT[4 * 4] = {
      0xdd, 0xa9, 0x7c, 0xa4, 0x86, 0x4c, 0xdf, 0xe0,
      0x6e, 0xaf, 0x70, 0xa0, 0xec, 0x0d, 0x71, 0x91
    };
    
    fout << "Aes-192:" << EOL;
    fout << indent(2) << " clear text: ";
    for (unsigned int i = 0; i < 16; i++) {
      fout << HEX << ZEROPAD << PREFIX << setWidth(4) << CLEAR_TEXT[i] << SP;
    }
    fout << ENDL;
    
    AdvancedEncryptionStandard forward(
      KEY,
      AdvancedEncryptionStandard::CIPHER_192
    );
    uint8 cipherText[16];
    unsigned int cipherBytes =
      forward.push(cipherText, CLEAR_TEXT, sizeof(CLEAR_TEXT));
    fout << "cipherBytes: " << cipherBytes << ENDL;
    bassert(cipherBytes == sizeof(CLEAR_TEXT), UnexpectedFailure(this));
    
    fout << indent(2) << "cipher text: ";
    for (unsigned int i = 0; i < 16; i++) {
      fout << HEX << ZEROPAD << PREFIX << setWidth(4) << cipherText[i] << SP;
    }
    fout << ENDL;
    
    bassert(
      compare(cipherText, CIPHER_TEXT, sizeof(CIPHER_TEXT)) == 0,
      UnexpectedFailure(this)
    );
    
    AdvancedEncryptionStandard inverse(
      KEY,
      AdvancedEncryptionStandard::CIPHER_192,
      true
    );
    uint8 clearText[sizeof(CIPHER_TEXT)];
    inverse.push(clearText, cipherText, sizeof(cipherText));
    fout << indent(2) << " clear text: ";
    for (unsigned int i = 0; i < 16; i++) {
      fout << HEX << ZEROPAD << PREFIX << setWidth(4) << clearText[i] << SP;
    }
    fout << ENDL;
    
    bassert(
      compare(clearText, CLEAR_TEXT, sizeof(CLEAR_TEXT)) == 0,
      UnexpectedFailure(this)
    );
  }
  
  void testAes256() throw() {
    static const uint8 KEY[4 * 8] = {
//       0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
//       0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
//       0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
//       0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4
      
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
      0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
      0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
      0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
    };
    static const uint8 CLEAR_TEXT[4 * 4] = {
      0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
      0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
    };
    static const uint8 CIPHER_TEXT[4 * 4] = {
      0x8e, 0xa2, 0xb7, 0xca, 0x51, 0x67, 0x45, 0xbf,
      0xea, 0xfc, 0x49, 0x90, 0x4b, 0x49, 0x60, 0x89
    };
    
    fout << "Aes-256:" << EOL;
    fout << indent(2) << " clear text: ";
    for (unsigned int i = 0; i < 16; i++) {
      fout << HEX << ZEROPAD << PREFIX << setWidth(4) << CLEAR_TEXT[i] << SP;
    }
    fout << ENDL;
    
    AdvancedEncryptionStandard forward(
      KEY,
      AdvancedEncryptionStandard::CIPHER_256
    );
    uint8 cipherText[16];
    unsigned int cipherBytes =
      forward.push(cipherText, CLEAR_TEXT, sizeof(CLEAR_TEXT));
    fout << "cipherBytes: " << cipherBytes << ENDL;
    bassert(cipherBytes == sizeof(CLEAR_TEXT), UnexpectedFailure(this));
    
    fout << indent(2) << "cipher text: ";
    for (unsigned int i = 0; i < 16; i++) {
      fout << HEX << ZEROPAD << PREFIX << setWidth(4) << cipherText[i] << SP;
    }
    fout << ENDL;
    
    bassert(
      compare(cipherText, CIPHER_TEXT, sizeof(CIPHER_TEXT)) == 0,
      UnexpectedFailure(this)
    );
    
    AdvancedEncryptionStandard inverse(
      KEY,
      AdvancedEncryptionStandard::CIPHER_256,
      true
    );
    uint8 clearText[sizeof(CIPHER_TEXT)];
    inverse.push(clearText, cipherText, sizeof(cipherText));
    fout << indent(2) << " clear text: ";
    for (unsigned int i = 0; i < 16; i++) {
      fout << HEX << ZEROPAD << PREFIX << setWidth(4) << clearText[i] << SP;
    }
    fout << ENDL;
    
    bassert(
      compare(clearText, CLEAR_TEXT, sizeof(CLEAR_TEXT)) == 0,
      UnexpectedFailure(this)
    );
  }

  void rate() throw() {
    static const uint8 KEY[4 * 4] = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
      0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
    };
    
    Allocator<uint8> clearText(4096 * 256);
    Allocator<uint8> cipherText(4096 * 256);
    AdvancedEncryptionStandard forward(
      KEY,
      AdvancedEncryptionStandard::CIPHER_128
    );

    Timer timer;
    uint64 totalSize = 0;
    while (!isTerminated()) {
      unsigned int cipherBytes =
        forward.push(
          cipherText.getElements(),
          clearText.getElements(),
          clearText.getSize()
        );
      totalSize += cipherBytes;
      fout << setPrecision(2)
           << (totalSize*1000000.0)/timer.getLiveMicroseconds()
           << "\r"
           << FLUSH;
    }
    fout << ENDL;
  }

  void rateInverse128() throw() {
    static const uint8 KEY[4 * 6] = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
      0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
      0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17
    };
    
    Allocator<uint8> clearText(4096 * 256);
    Allocator<uint8> cipherText(4096 * 256);
    AdvancedEncryptionStandard inverse(
      KEY,
      AdvancedEncryptionStandard::CIPHER_128,
      true
    );
    
    Timer timer;
    uint64 totalSize = 0;
    while (!isTerminated()) {
      unsigned int cipherBytes =
        inverse.push(
          cipherText.getElements(),
          clearText.getElements(),
          clearText.getSize()
        );
      totalSize += cipherBytes;
      fout << setPrecision(2)
           << (totalSize*1000000.0)/timer.getLiveMicroseconds()
           << "\r"
           << FLUSH;
    }
    fout << ENDL;
  }

  void rate192() throw() {
    static const uint8 KEY[4 * 6] = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
      0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
      0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17
    };
    
    uint8 clearText[4096 * 256];
    uint8 cipherText[4096 * 256];
    AdvancedEncryptionStandard forward(
      KEY,
      AdvancedEncryptionStandard::CIPHER_192
    );
    
    Timer timer;
    uint64 totalSize = 0;
    while (!isTerminated()) {
      unsigned int cipherBytes =
        forward.push(cipherText, clearText, sizeof(clearText));
      totalSize += cipherBytes;
      fout << setPrecision(2)
           << (totalSize*1000000.0)/timer.getLiveMicroseconds()
           << "\r"
           << FLUSH;
    }
    fout << ENDL;
  }
  
  // TAG: need UnitTestException?
  
  void cipher(const String& srcPath, const String& destPath) {
    testAes128();
    testAes128Second();
    testAes192();
    testAes256();
    // rateInverse128();
    return;
    
    static const uint8 KEY[4 * 4] = {
      0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
      0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
    };
    
    unsigned int type;
    try {
      type = FileSystem::getType(srcPath);
    } catch (FileSystemException& e) {
      ferr << "Error: " << "File does not exist" << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }
    
    if (type & FileSystem::REGULAR) {
      FileInputStream input(srcPath);
      FileOutputStream output(
        destPath,
        FileOutputStream::CREATE | FileOutputStream::EXCLUSIVE
      );
      uint8 clearText[BLOCK_SIZE];
      uint8 cipherText[BLOCK_SIZE];
      AdvancedEncryptionStandard cipher(
        KEY,
        AdvancedEncryptionStandard::CIPHER_128
      );
      unsigned int count = 1;
      unsigned int cipherBytes = 0;
      while (count > 0) {
        count = input.read(clearText, sizeof(clearText), true);
        cipherBytes = cipher.push(cipherText, clearText, count);
        output.write(cipherText, cipherBytes);
      }
      cipherBytes = cipher.pushEnd(cipherText, sizeof(cipherText));
      output.write(cipherText, cipherBytes);
    } else {
      ferr << "Error: " << "Invalid filesystem entry" << ENDL;
      setExitCode(EXIT_CODE_ERROR);
    }
  }

  void usage() throw() {
    fout << "Usage: " << getFormalName() << " input output" << ENDL;
  }
  
  void main() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "http://www.mip.sdu.dk/~fonseca/base" << EOL
         << "Copyright (C) 2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>" << EOL
         << ENDL;
    
    Array<String> arguments = getArguments();
    String srcPath;
    String destPath;
    
    switch (arguments.getSize()) {
    case 2:
      srcPath = arguments[0]; // file path
      destPath = arguments[1]; // file path
      break;
    default:
      usage();
      return;
    }
    
    try {
      cipher(srcPath, destPath);
    } catch (Exception& e) {
      setExitCode(Application::EXIT_CODE_ERROR);
    }
  }
};

STUB(CipherApplication);
