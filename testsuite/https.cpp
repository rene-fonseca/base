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
#include <base/concurrency/Thread.h>
#include <base/net/HTTPSRequest.h>
#include <base/net/Url.h>
#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/StringOutputStream.h>
#include <base/objectmodel/JSON.h>

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

  PushToFile(File _file)
    : file(_file)
  {
  }

  bool pushBegin(uint64 totalSize)
  {
    this->totalSize = totalSize;
    timer.start();
    return true;
  }
  
  unsigned int push(const uint8* buffer, unsigned int size)
  {
    unsigned int result = file.write(buffer, size);
    BASSERT(result == size);
    bytesWritten += size;
    if (totalSize > 0) {
      fout << "Read=" << bytesWritten
           << "  Progress=" << base::FIXED << (bytesWritten*1000/totalSize)/10.0 << '%'
           << "  Time=" << base::FIXED << timer.getLiveMicroseconds()/100000/10.0 << " s"
           << "  Rate=" << base::FIXED << setPrecision(1) << (1000000./1024 * static_cast<double>(bytesWritten)/timer.getLiveMicroseconds())
           << " kb/s          \r" << FLUSH;
    } else {
      fout << "Read=" << bytesWritten // TAG: show in kb/Mb/Gb/...
           << "  Time=" << base::FIXED << timer.getLiveMicroseconds()/100000/10.0 << " s" // use HH:MM:SS format
           << "  Rate=" << base::FIXED << setPrecision(1) << (1000000./1024 * static_cast<double>(bytesWritten)/timer.getLiveMicroseconds())
           << " kb/s          \r" << FLUSH;
    }
    // Thread::microsleep(125*1000); // TAG: temp test
    return size;
  }
  
  void pushEnd()
  {
    fout << ENDL;
    file.close();
  }
  
  virtual ~PushToFile()
  {
  }
};

class HTTPSApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;

  Map<String, String> fields;
  String url;
  String filepath;
  bool showHeader = false;
public:
  
  HTTPSApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[])
    : Application("https", numberOfArguments, arguments, environment)
  {
  }
  
  bool parseArguments()
  {
    Array<String> arguments = getArguments();
    Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();
    while (enu.hasNext()) {
      const String argument = *enu.next();
      if (argument == "--field") {
        if (!enu.hasNext()) {
          ferr << "Expected header field name." << ENDL;
          return false;
        }
        auto name = *enu.next();
        if (!enu.hasNext()) {
          ferr << "Expected header field value." << ENDL;
          return false;
        }
        auto value = *enu.next();
        fields.add(name, value);
      } else if (argument == "--header") {
        showHeader = true;
      } else {
        if (!url) {
          url = argument;
        } else if (!filepath) {
          filepath = argument;
        } else {
          ferr << "Invalid argument." << ENDL;
          return false;
        }
      }
    }
    return true;
  }

  void main()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << "Copyright (C) 2001-2019 by Rene Moeller Fonseca" << EOL
         << ENDL;

    if (!parseArguments()) {
      setExitCode(Application::EXIT_CODE_ERROR);
      return;
    }

    // fout << "Usage: " << getFormalName() << " [url] [output]" << ENDL;

    try {
      HTTPSRequest request;
      if (!request.open(HTTPSRequest::METHOD_GET, url)) {
        setExitCode(Application::EXIT_CODE_ERROR);
        return;
      }

      auto enu = fields.getReadEnumerator();
      while (enu.hasNext()) {
        auto nv = enu.next();
        request.setRequestHeader(nv->getKey(), nv->getValue());
      }
      
      // request.setRequestHeader("Keep-Alive", "30");
      // TAG: allow form data - get from options

      String body;
      request.send(body);
      // TAG: read from stdin/file with progress

      fout << "Status: " << request.getStatus() << ENDL;
      fout << "Status text: " << request.getStatusText() << ENDL;
      fout << "Content length: " << request.getContentLength() << ENDL;

      // fout << "Content length: " << request.getResponseHeader("Content-Length") << ENDL;
      // fout << "Content type: " << request.getResponseHeader("Content-Type") << ENDL;
      
      if (showHeader) {
        const String header = request.getResponseHeader(); // TAG: subst CRLFs with EOL
        fout << header << ENDL;
      }
      
      if (filepath) {
        PushToFile push(File(filepath, File::WRITE, File::CREATE | File::TRUNCATE));
        request.getResponse(&push);
      } else {
        // TAG: if JSON to show nicely
        PushToStandardOutput push;
        request.getResponse(&push);
      }

      request.close();
    } catch (...) {
      ferr << "Failed to read response." << ENDL;
      setExitCode(Application::EXIT_CODE_ERROR);
      return;
    }
  }
};

APPLICATION_STUB(HTTPSApplication);
