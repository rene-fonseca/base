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
#include <base/Timer.h>
#include <base/Primitives.h>
#include <base/UnsignedInteger.h>
#include <base/io/FileOutputStream.h>
#include <base/net/HTTPSRequest.h>
#include <base/net/Url.h>
#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/StringOutputStream.h>

using namespace com::azure::dev::base;

namespace commands {
  
  // Methods
  const Literal METHOD_OPTIONS = MESSAGE("OPTIONS");
  const Literal METHOD_GET = MESSAGE("GET");
  const Literal METHOD_HEAD = MESSAGE("HEAD");
  const Literal METHOD_POST = MESSAGE("POST");
  const Literal METHOD_PUT = MESSAGE("PUT");
  const Literal METHOD_DELETE = MESSAGE("DELETE");
  const Literal METHOD_TRACE = MESSAGE("TRACE");
  const Literal METHOD_CONNECT = MESSAGE("CONNECT");
}; // commands namespace

using namespace commands;

class PushInterface {
public:

  virtual bool pushBegin(uint64 totalSize) = 0;
  virtual unsigned int push(const uint8* buffer, unsigned int size) = 0;
  virtual void pushEnd() = 0;
};

class PullInterface {
public:

  virtual uint64 pullBegin() const = 0;
  virtual unsigned int pull(uint8* buffer, unsigned int size) = 0;
};

class PushToNothing {
public:

  bool pushBegin(uint64 totalSize) {
    return true;
  }

  unsigned int push(const uint8* buffer, unsigned int size) {
    return size;
  }

  void pushEnd() {
  }
};

class PushToStandardOutput : public virtual Object, public PushInterface {
public:

  PushToStandardOutput() {
  }

  bool pushBegin(uint64 totalSize) {
    return true;
  }

  unsigned int push(const uint8* buffer, unsigned int size) {
    for (unsigned int i = 0; i < size;) {
      char ch = *buffer++;
      ++i;
      if (ch == '\n') {
        fout << EOL;
        if ((i < size) && (*buffer == '\r')) {
          ++buffer; // skip
          ++i;
        }
      } else if (ch == '\r') {
        fout << EOL;
        if ((i < size) && (*buffer == '\n')) {
          ++buffer; // skip
          ++i;
        }
      } else if (ASCIITraits::isGraph(ch)) {
        fout << ch;
      } else if (ch != ' ') {
        fout << '.';
      } else {
        fout << ' ';
      }
    }
    return size;
  }

  void pushEnd() {
    fout << ENDL;
  }

  virtual ~PushToStandardOutput() {
  }
};


class PushToFile : public virtual Object, public PushInterface {
private:

  File file;
  Timer timer;
  uint64 bytesWritten = 0;
  uint64 totalSize = 0;
public:

  PushToFile(File _file) : file(_file) {
  }

  bool pushBegin(uint64 totalSize)
  {
    this->totalSize = totalSize;
    timer.start();
    return true;
  }
  
  unsigned int push(const uint8* buffer, unsigned int size) {
    unsigned int result = file.write(buffer, size);
    ASSERT(result == size);
    bytesWritten += size;
    if (totalSize > 0) {
      fout << "  bytes written=" << bytesWritten
           << "  completed=" << base::FIXED << setWidth(7) << setPrecision(3)
           << static_cast<long double>(bytesWritten)/totalSize*100 << '%'
           << "  time=" << base::FIXED << setWidth(6) << timer.getLiveMicroseconds()/1000000.
           << "  rate=" << base::FIXED << setWidth(12) << setPrecision(3)
           << (1000000./1024 * static_cast<long double>(bytesWritten)/timer.getLiveMicroseconds())
           << "kb/s\r" << FLUSH;
    } else {
      fout << "  bytes written=" << bytesWritten
           << "  time=" << base::FIXED << setWidth(6) << timer.getLiveMicroseconds()/1000000.
           << "  rate=" << base::FIXED << setWidth(12) << setPrecision(3)
           << (1000000./1024 * static_cast<long double>(bytesWritten)/timer.getLiveMicroseconds())
           << "kb/s\r" << FLUSH;
    }
    return size;
  }
  
  void pushEnd() {
    fout << ENDL;
    file.close();
  }
  
  virtual ~PushToFile() {
  }
};

class HTTPSApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:
  
  HTTPSApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[])
    : Application("https", numberOfArguments, arguments, environment)
  {
  }
  
  void main()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "Copyright (C) 2001-2019 by Rene Moeller Fonseca" << EOL
         << ENDL;

    Array<String> arguments = getArguments();

    String url = MESSAGE("www.google.com/"); // default url
    String file; // default file

    switch (arguments.getSize()) {
    case 0:
      // use defaults
      break;
    case 1:
      url = arguments[0]; // the address
      break;
    case 2:
      url = arguments[0]; // the address
      file = arguments[1]; // the service
      break;
    default:
      fout << "Usage: " << getFormalName() << " [url] [output]" << ENDL;
      setExitCode(Application::EXIT_CODE_ERROR);
      return;
    }

    try {
      HTTPSRequest request;
      if (!request.open(HTTPSRequest::METHOD_GET, url)) {
        setExitCode(Application::EXIT_CODE_ERROR);
        return;
      }

      // TAG: get from options
      // request.setRequestHeader("Content-Length", "0");
      // TAG: allow form data - get from options

      String body;
      request.send(body);
      // TAG: read from stdin/file with progress

      if (file) {
        FileOutputStream fos(file);
        request.getResponse(&fos);
      } else {
        const String response = request.getResponse();
        fout << response << FLUSH;
      }
      // TAG: write to stdout/file - with progress

      String header = request.getResponseHeader();
      fout << header << ENDL;

      try {
        fout << "Status: " << request.getStatus() << ENDL;
      } catch (...) {
      }
      try {
        fout << "Status text: " << request.getStatusText() << ENDL;
      } catch (...) {
      }
      request.close();
    } catch (...) {
      setExitCode(Application::EXIT_CODE_ERROR);
      return;
    }
  }
};

APPLICATION_STUB(HTTPSApplication);
