/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/FileInputStream.h>
#include <base/io/FileOutputStream.h> // temp
#include <base/io/FileReader.h>
#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <base/Application.h>
#include <base/Primitives.h>
#include <base/security/MD5Sum.h>
#include <base/security/SHA1.h>
#include <base/security/SHA256.h>
#include <base/security/SHA384.h>
#include <base/security/SHA512.h>

using namespace dk::sdu::mip::base;

class FileInputApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 2;
  static const unsigned int BLOCK_SIZE = 4096 * 4;
public:
  
  FileInputApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application("fileInput", numberOfArguments, arguments, environment) {
  }

  class Job {
  public:

    enum {
      TEST,
      MD5SUM,
      SHA1,
      SHA256,
      SHA384,
      SHA512,
      DUMP
    };
  };
  
  void calculateMD5Sum(const String& string) {
    MD5Sum checksum;
    checksum.push(Cast::pointer<const uint8*>(string.getElements()), string.getLength());
    checksum.pushEnd();

    fout << "Total number of bytes: " << checksum.getTotalSize() << EOL
         << "Original message: " << string << EOL
         << "MD5 Message digest (hex): " << checksum.getValue() << EOL
         << ENDL;
  }

  void calculateSHA1(const String& string) {
    SHA1 checksum;
    checksum.push(Cast::pointer<const uint8*>(string.getElements()), string.getLength());
    checksum.pushEnd();

    fout << "Total number of bytes: " << checksum.getTotalSize() << EOL
         << "Original message: " << string << EOL
         << "SHA-1 Message digest (hex): " << checksum.getValue() << EOL
         << ENDL;
  }
  
  void calculateSHA256(const String& string) {
    SHA256 checksum;
    checksum.push(Cast::pointer<const uint8*>(string.getElements()), string.getLength());
    checksum.pushEnd();

    fout << "Total number of bytes: " << checksum.getTotalSize() << EOL
         << "Original message: " << string << EOL
         << "SHA-256 Message digest (hex): " << checksum.getValue() << EOL
         << ENDL;
  }

  void calculateSHA384(const String& string) {
    SHA384 checksum;
    checksum.push(Cast::pointer<const uint8*>(string.getElements()), string.getLength());
    checksum.pushEnd();

    fout << "Total number of bytes: " << checksum.getTotalSize() << EOL
         << "Original message: " << string << EOL
         << "SHA-384 Message digest (hex): " << checksum.getValue() << EOL
         << ENDL;
  }
  
  void calculateSHA512(const String& string) {
    SHA512 checksum;
    checksum.push(Cast::pointer<const uint8*>(string.getElements()), string.getLength());
    checksum.pushEnd();

    fout << "Total number of bytes: " << checksum.getTotalSize() << EOL
         << "Original message: " << string << EOL
         << "SHA-512 Message digest (hex): " << checksum.getValue() << EOL
         << ENDL;
  }

  void calculateChecksums() {
    calculateMD5Sum("");
    calculateMD5Sum("a");
    calculateMD5Sum("abc");
    calculateMD5Sum("message digest");
    calculateMD5Sum("abcdefghijklmnopqrstuvwxyz");
    calculateMD5Sum("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    
    calculateSHA1("");
    calculateSHA1("a");
    calculateSHA1("abc"); // a9993e36 4706816a ba3e2571 7850c26c 9cd0d89d
    calculateSHA1("message digest");
    calculateSHA1("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"); // 84983e44 1c3bd26e baae4aa1 f95129e5 e54670f1
    calculateSHA1("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    
    calculateSHA256("abc"); // ba7816bf 8f01cfea 414140de 5dae2223 b00361a3 96177a9c b410ff61 f20015ad
    calculateSHA256("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"); // 248d6a61 d20638b8 e5c02693 0c3e6039 a33ce459 64ff2167 f6ecedd4 19db06c1

    calculateSHA384("abc"); // cb00753f45a35e8b b5a03d699ac65007 272c32ab0eded163 1a8b605a43ff5bed 8086072ba1e7cc23 58baeca134c825a7
    calculateSHA384("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu"); // 09330c33f71147e8 3d192fc782cd1b47 53111b173b3b05d2 2fa08086e3b0f712 fcc7c71a557e2db9 66c3e9fa91746039
    
    calculateSHA512("abc"); // ddaf35a193617aba cc417349ae204131 12e6fa4e89a97ea2 0a9eeee64b55d39a 2192992a274fc1a8 36ba3c23a3feebbd 454d4423643ce80e 2a9ac94fa54ca49f
    calculateSHA512("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu"); // 8e959b75dae313da 8cf4f72814fc143f 8f7779c6eb9f7fa1 7299aeadb6889018 501d289e4900f7e4 331b99dec4b5433a c7d329eeb6dd2654 5e96e55b874be909
  }

  void jobMD5Sum(const String& filePath) {
    fout << "Calculating MD5 checksum";
    FileInputStream file(filePath, 0);

    uint8 buffer[BLOCK_SIZE];
    MD5Sum sum;
    unsigned int count;
    while ((count = file.read(buffer, sizeof(buffer), true)) > 0) {
      fout << '.' << FLUSH;
      sum.push(buffer, count);
    }
    sum.pushEnd();
    fout << ENDL;

    fout << "Total number of bytes: " << sum.getTotalSize() << EOL
         << "Message digest (hex): " << sum.getValue() << EOL
         << "Message digest (Base64): " << sum.getBase64() << ENDL;
  }

  void jobSHA1(const String& filePath) {
    fout << "Calculating SHA-1 message digest...";
    FileInputStream file(filePath, 0);

    uint8 buffer[BLOCK_SIZE];
    SHA1 sum;
    unsigned int count;
    while ((count = file.read(buffer, sizeof(buffer), true)) > 0) {
      fout << '.' << FLUSH;
      sum.push(buffer, count);
    }
    sum.pushEnd();
    fout << ENDL;

    fout << "Total number of bytes: " << sum.getTotalSize() << EOL
         << "Message digest (hex): " << sum.getValue() << EOL
         << "Message digest (Base64): " << sum.getBase64() << ENDL;
  }
  
  void jobSHA256(const String& filePath) {
    fout << "Calculating SHA-256 message digest...";
    FileInputStream file(filePath, 0);

    uint8 buffer[BLOCK_SIZE];
    SHA256 sum;
    unsigned int count;
    while ((count = file.read(buffer, sizeof(buffer), true)) > 0) {
      fout << '.' << FLUSH;
      sum.push(buffer, count);
    }
    sum.pushEnd();
    fout << ENDL;
    
    fout << "Total number of bytes: " << sum.getTotalSize() << EOL
         << "Message digest (hex): " << sum.getValue() << EOL
         << "Message digest (Base64): " << sum.getBase64() << ENDL;
  }
  
  void jobSHA384(const String& filePath) {
    fout << "Calculating SHA-384 message digest...";
    FileInputStream file(filePath, 0);

    uint8 buffer[BLOCK_SIZE];
    SHA384 sum;
    unsigned int count;
    while ((count = file.read(buffer, sizeof(buffer), true)) > 0) {
      fout << '.' << FLUSH;
      sum.push(buffer, count);
    }
    sum.pushEnd();
    fout << ENDL;
    
    fout << "Total number of bytes: " << sum.getTotalSize() << EOL
         << "Message digest (hex): " << sum.getValue() << EOL
         << "Message digest (Base64): " << sum.getBase64() << ENDL;
  }

  void jobSHA512(const String& filePath) {
    fout << "Calculating SHA-512 message digest...";
    FileInputStream file(filePath, 0);

    uint8 buffer[BLOCK_SIZE];
    SHA512 sum;
    unsigned int count;
    while ((count = file.read(buffer, sizeof(buffer), true)) > 0) {
      fout << '.' << FLUSH;
      sum.push(buffer, count);
    }
    sum.pushEnd();
    fout << ENDL;
    
    fout << "Total number of bytes: " << sum.getTotalSize() << EOL
         << "Message digest (hex): " << sum.getValue() << EOL
         << "Message digest (Base64): " << sum.getBase64() << ENDL;
  }
  
  void jobDump(const String& filePath) {
    fout << "Dumping contents of file" << ENDL;
    FileInputStream file(filePath, 0);
    FormatInputStream stream(file);

    String line;
    while (stream.available()) {
      stream >> line;
      fout << line << EOL;
    }
    fout << FLUSH;
    // TAG: does not work!
  }

  void entry(unsigned int job, const String& filePath) {
    switch (job) {
    case Job::TEST:
      calculateChecksums();
      break;
    case Job::MD5SUM:
      jobMD5Sum(filePath);
      break;
    case Job::SHA1:
      jobSHA1(filePath);
      break;
    case Job::SHA256:
      jobSHA256(filePath);
      break;
    case Job::SHA384:
      jobSHA384(filePath);
      break;
    case Job::SHA512:
      jobSHA512(filePath);
      break;
    case Job::DUMP:
      jobDump(filePath);
      break;
    }
  }

  void usage() throw() {
    fout << "Usage: " << getFormalName()
         << " MD5SUM|SHA1|SHA256|SHA384|SHA512|DUMP file" << ENDL;
  }
  
  void main() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "http://www.mip.sdu.dk/~fonseca/base" << EOL
         << "Copyright (C) 2001-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>" << EOL
         << ENDL;
    
    Array<String> arguments = getArguments();
    unsigned int job = Job::TEST;
    String jobString;
    String filePath = "/etc/services";

    switch (arguments.getSize()) {
    case 0: // use default
      break;
    case 2:
      jobString = arguments[0]; // job
      if (jobString == "TEST") {
        job = Job::TEST;
      } else if (jobString == "MD5SUM") {
        job = Job::MD5SUM;
      } else if (jobString == "SHA1") {
        job = Job::SHA1;
      } else if (jobString == "SHA256") {
        job = Job::SHA256;
      } else if (jobString == "SHA384") {
        job = Job::SHA384;
      } else if (jobString == "SHA512") {
        job = Job::SHA512;
      } else if (jobString == "DUMP") {
        job = Job::DUMP;
      } else {
        usage();
      }
      filePath = arguments[1]; // file path
      break;
    default:
      usage();
    }

    try {
      entry(job, filePath);
    } catch (Exception& e) {
      setExitCode(Application::EXIT_CODE_ERROR);
    }
  }
};

STUB(FileInputApplication);
