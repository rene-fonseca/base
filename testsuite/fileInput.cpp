/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

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
#include <base/Type.h>
#include <base/security/MD5Sum.h>

using namespace base;

enum Job {MD5SUM, DUMP};

void testChecksum(const String& str) {
  MD5Sum checksum;
  checksum.push((const byte*)str.getElements(), str.getLength());
  checksum.pushEnd();

  fout << "Total number of bytes: " << checksum.getTotalSize() << EOL
       << "Original message: " << str << EOL
       << "Checksum (hex): " << checksum.getValue() << EOL
       << ENDL;
}

void testChecksums() {
  testChecksum("");
  testChecksum("a");
  testChecksum("abc");
  testChecksum("message digest");
  testChecksum("abcdefghijklmnopqrstuvwxyz");
  testChecksum("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
}

void jobMD5Sum(const String& filePath) {
  fout << "Calculating MD5 checksum";
  FileInputStream file(filePath, 0);

  byte buffer[4096];
  MD5Sum sum;
  unsigned int count;
  while ((count = file.read((char*)&buffer[0], 4096, true)) > 0) {
    fout << '.' << FLUSH;
    sum.push(buffer, count);
  }
  sum.pushEnd();
  fout << ENDL;

  fout << "Total number of bytes: " << sum.getTotalSize() << EOL
       << "Checksum (hex encoding): " << sum.getValue() << EOL
       << "Checksum (Base64 encoding): " << sum.getBase64() << ENDL;
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
  case MD5SUM:
    jobMD5Sum(filePath);
    break;
  case DUMP:
    jobDump(filePath);
    break;
  }
}

int main(int argc, const char* argv[], const char *envp[]) {
  fout << "Testing implementation of the FileInputStream" << ENDL;
  Application app("fileInput", argc, argv, envp);

  Array<String> arguments = Application::getApplication()->getArguments();
  Job job = MD5SUM;
  String jobString;
  String filePath = "/etc/services";

  switch (arguments.getSize()) {
  case 0: // use default
    break;
  case 2:
    jobString = arguments[0]; // job
    if (jobString == "MD5SUM") {
      job = MD5SUM;
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
  return Application::EXIT_CODE_NORMAL;
}
