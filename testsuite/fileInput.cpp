/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/FileInputStream.h>
#include <base/io/FileOutputStream.h> // temp
#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <base/Application.h>
#include <base/Primitives.h>
#include <base/security/MD5Sum.h>
#include <base/security/SHA1.h>

using namespace base;

enum Job {TEST, MD5SUM, SHA, DUMP};

void calculateMD5Checksum(const String& str) {
  MD5Sum checksum;
  checksum.push(pointer_cast<const byte*>(str.getElements()), str.getLength());
  checksum.pushEnd();

  fout << "Total number of bytes: " << checksum.getTotalSize() << EOL
       << "Original message: " << str << EOL
       << "MD5 Message digest (hex): " << checksum.getValue() << EOL
       << ENDL;
}

void calculateSHA1Checksum(const String& str) {
  SHA1 checksum;
  checksum.push(pointer_cast<const byte*>(str.getElements()), str.getLength());
  checksum.pushEnd();

  fout << "Total number of bytes: " << checksum.getTotalSize() << EOL
       << "Original message: " << str << EOL
       << "SHA-1 Message digest (hex): " << checksum.getValue() << EOL
       << ENDL;
}

void calculateChecksums() {
  calculateMD5Checksum("");
  calculateMD5Checksum("a");
  calculateMD5Checksum("abc");
  calculateMD5Checksum("message digest");
  calculateMD5Checksum("abcdefghijklmnopqrstuvwxyz");
  calculateMD5Checksum("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

  calculateSHA1Checksum("");
  calculateSHA1Checksum("a");
  calculateSHA1Checksum("abc"); // A9993E36 4706816A BA3E2571 7850C26C 9CD0D89D
  calculateSHA1Checksum("message digest");
  calculateSHA1Checksum("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"); // 84983E44 1C3BD26E BAAE4AA1 F95129E5 E54670F1
  calculateSHA1Checksum("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
}

void jobMD5Sum(const String& filePath) {
  fout << "Calculating MD5 checksum";
  FileInputStream file(filePath, 0);

  byte buffer[4096];
  MD5Sum sum;
  unsigned int count;
  while ((count = file.read(getCharAddress(buffer[0]), 4096, true)) > 0) {
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
  fout << "Calculating SHA1 checksum";
  FileInputStream file(filePath, 0);

  byte buffer[4096];
  SHA1 sum;
  unsigned int count;
  while ((count = file.read(getCharAddress(buffer[0]), 4096, true)) > 0) {
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

void entry(Job job, const String& filePath) {
  switch (job) {
  case TEST:
    calculateChecksums();
    break;
  case MD5SUM:
    jobMD5Sum(filePath);
    break;
  case SHA:
    jobSHA1(filePath);
    break;
  case DUMP:
    jobDump(filePath);
    break;
  }
}

int main(int argc, const char* argv[], const char *env[]) {
  fout << MESSAGE("Testing implementation of the FileInputStream") << ENDL;
  Application app("fileInput", argc, argv, env);

  Array<String> arguments = Application::getApplication()->getArguments();
  Job job = TEST;
  String jobString;
  String filePath = "/etc/services";

  switch (arguments.getSize()) {
  case 0: // use default
    break;
  case 2:
    jobString = arguments[0]; // job
    if (jobString == "TEST") {
      job = TEST;
    } else if (jobString == "MD5SUM") {
      job = MD5SUM;
    } else if (jobString == "SHA1") {
      job = SHA;
    } else if (jobString == "DUMP") {
      job = DUMP;
    } else {
      fout << "usage: " << Application::getApplication()->getName() << " MD5SUM|DUMP file" << ENDL;
      return Application::EXIT_CODE_NORMAL; // stop
    }
    filePath = arguments[1]; // file path
    break;
  default:
    fout << "usage: " << Application::getApplication()->getName() << " MD5SUM|DUMP file" << ENDL;
    return Application::EXIT_CODE_NORMAL; // stop
  }

  try {
    entry(job, filePath);
  } catch(Exception& e) {
    return Application::getApplication()->exceptionHandler(e);
  } catch(...) {
    return Application::getApplication()->exceptionHandler();
  }
  return Application::getApplication()->getExitCode();
}
