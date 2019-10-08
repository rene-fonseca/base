/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/FileInputStream.h>
#include <base/io/FileReader.h>
#include <base/filesystem/FileSystem.h>
#include <base/filesystem/FolderInfo.h>
#include <base/string/FormatOutputStream.h>
#include <base/Application.h>
#include <base/security/Adler32.h>
#include <base/security/CRC32.h>
#include <base/security/MD5Sum.h>
#include <base/security/SHA1.h>
#include <base/security/SHA256.h>
#include <base/security/SHA384.h>
#include <base/security/SHA512.h>

using namespace dk::sdu::mip::base;

class DigestApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
  static const unsigned int BLOCK_SIZE = 4096 * 4;
  
  uint8 buffer[BLOCK_SIZE];
  unsigned int job;
public:
  
  class Job {
  public:
    
    enum {
      ADLER32,
      CRC32,
      MD5SUM,
      SHA1,
      SHA256,
      SHA384,
      SHA512
    };
  };
  
  DigestApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) throw()
    : Application("digest", numberOfArguments, arguments, environment) {
    job = Job::SHA1;
  }  
  
  void jobAdler32(const String& path) {
    FileInputStream file(path, 0);
    
    Adler32 sum;
    unsigned int count;
    while ((count = file.read(buffer, sizeof(buffer), true)) > 0) {
      sum.push(buffer, count);
    }
    sum.pushEnd();
    
    fout << HEX << setWidth(8) << NOPREFIX << ZEROPAD << sum.getValue()
         << indent(2) << path << ENDL;
  }
  
  void jobCRC32(const String& path) {
    FileInputStream file(path, 0);
    
    CRC32 sum;
    unsigned int count;
    while ((count = file.read(buffer, sizeof(buffer), true)) > 0) {
      sum.push(buffer, count);
    }
    sum.pushEnd();
    
    fout << HEX << setWidth(8) << NOPREFIX << ZEROPAD << sum.getValue()
         << indent(2) << path << ENDL;
  }
  
  void jobMD5Sum(const String& path) {
    FileInputStream file(path, 0);
    
    MD5Sum sum;
    unsigned int count;
    while ((count = file.read(buffer, sizeof(buffer), true)) > 0) {
      sum.push(buffer, count);
    }
    sum.pushEnd();
    
    fout << sum.getValue() << indent(2) << path << ENDL;
  }
  
  void jobSHA1(const String& path) {
    FileInputStream file(path, 0);

    SHA1 sum;
    unsigned int count;
    while ((count = file.read(buffer, sizeof(buffer), true)) > 0) {
      sum.push(buffer, count);
    }
    sum.pushEnd();

    fout << sum.getValue() << indent(2) << path << ENDL;
  }
  
  void jobSHA256(const String& path) {
    FileInputStream file(path, 0);

    SHA256 sum;
    unsigned int count;
    while ((count = file.read(buffer, sizeof(buffer), true)) > 0) {
      sum.push(buffer, count);
    }
    sum.pushEnd();
    
    fout << sum.getValue() << indent(2) << path << ENDL;
  }
  
  void jobSHA384(const String& path) {
    FileInputStream file(path, 0);

    SHA384 sum;
    unsigned int count;
    while ((count = file.read(buffer, sizeof(buffer), true)) > 0) {
      sum.push(buffer, count);
    }
    sum.pushEnd();
    
    fout << sum.getValue() << indent(2) << path << ENDL;
  }

  void jobSHA512(const String& path) {
    FileInputStream file(path, 0);

    SHA512 sum;
    unsigned int count;
    while ((count = file.read(buffer, sizeof(buffer), true)) > 0) {
      sum.push(buffer, count);
    }
    sum.pushEnd();
    
    fout << sum.getValue() << indent(2) << path << ENDL;
  }

  void fileDigest(const String& path) {
    switch (job) {
    case Job::ADLER32:
      jobAdler32(path);
      break;
    case Job::CRC32:
      jobCRC32(path);
      break;
    case Job::MD5SUM:
      jobMD5Sum(path);
      break;
    case Job::SHA1:
      jobSHA1(path);
      break;
    case Job::SHA256:
      jobSHA256(path);
      break;
    case Job::SHA384:
      jobSHA384(path);
      break;
    case Job::SHA512:
      jobSHA512(path);
      break;
    }
  }
  
  void digest(const String& path) {
    unsigned int type;
    try {
      type = FileSystem::getType(path);
    } catch (FileSystemException&) {
      ferr << "Error: " << "File or folder does not exist" << ENDL;
      setExitCode(EXIT_CODE_ERROR);
      return;
    }
    
    if (type & FileSystem::REGULAR) {
      fileDigest(path);
    } else if (type & FileSystem::FOLDER) {
      FolderInfo folder(path);
      Array<String> entries = folder.getEntries();
      FileSystem::setCurrentFolder(path);
      
      Array<String>::ReadEnumerator enu = entries.getReadEnumerator();
      while (enu.hasNext()) {
        const String entry = *enu.next();
        try {
          unsigned int type = FileSystem::getType(entry);
          if (type & FileSystem::REGULAR) {
            fileDigest(entry);
          } else if (type & FileSystem::FOLDER) {
            // ignore or recursive
          }
        } catch (Exception&) {
        }
      }
    } else {
      ferr << "Error: " << "Invalid filesystem entry" << ENDL;
      setExitCode(EXIT_CODE_ERROR);
    }
  }

  void usage() throw() {
    fout << "Usage: " << getFormalName()
         << " [ADLER32|CRC32|MD5SUM|SHA1|SHA256|SHA384|SHA512] file" << ENDL;
  }
  
  void main() throw() {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "http://www.mip.sdu.dk/~fonseca/base" << EOL
         << "Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>" << EOL
         << ENDL;
    
    Array<String> arguments = getArguments();
    String jobString;
    String path;
    
    switch (arguments.getSize()) {
    case 2:
      jobString = arguments[0]; // job
      if (jobString == "ADLER32") {
        job = Job::ADLER32;
      } else if (jobString == "CRC32") {
        job = Job::CRC32;
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
      } else {
        usage();
        setExitCode(EXIT_CODE_ERROR);
        return;
      }
      path = arguments[1]; // file path
      break;
    default:
      usage();
    }
    
    try {
      digest(path);
    } catch (Exception&) {
      setExitCode(Application::EXIT_CODE_ERROR);
    }
  }
};

STUB(DigestApplication);
