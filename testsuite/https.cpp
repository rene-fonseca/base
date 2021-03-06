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
#include <base/string/Format.h>
#include <base/string/ANSIEscapeSequence.h>
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
  virtual MemorySize pull(uint8* buffer, MemorySize size) = 0;
};

class PushToNothing {
public:

  bool pushBegin(uint64 totalSize)
  {
    return true;
  }

  MemorySize push(const uint8* buffer, MemorySize size)
  {
    return size;
  }

  void pushEnd()
  {
  }
};

class PushToStandardOutput : public virtual Object, public PushInterface {
public:

  PushToStandardOutput()
  {
  }

  bool pushBegin(uint64 totalSize)
  {
    return true;
  }

  MemorySize push(const uint8* buffer, MemorySize size)
  {
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

  static String formatBytes(uint64 bytes)
  {
    StringOutputStream sos;;
    if (bytes > 5ULL * 1024 * 1024 * 1024) {
      return sos << setPrecision(1) << bytes*10/1024/1024/10.0 << " Gb";
    } else if (bytes > 5 * 1024 * 1024) {
      return sos << setPrecision(1) << bytes * 10 / (1024 * 1024) / 10.0 << " Mb";
    } else if (bytes > 5 * 1024) {
      return sos << setPrecision(1) << bytes * 10 / 1024 / 10.0 << " Kb";
    }
    return sos << bytes;
  }
  
  String formatProgress(uint64 bytes, uint64 total)
  {
    return format() << base::FIXED << setPrecision(1) << (bytes * 1000 / total) / 10.0 << '%';
  }

  String formatTime(uint64 microseconds, bool showMS = false)
  {
    // HH:MM / HH:MM:SS / HH:MM:SS.mmm / HH:MM:SS.mmmmmm
    Timer::ElapsedTime time(microseconds * 1000);
    StringOutputStream sos;
    sos << ZEROPAD << time.getHours() << ':'
      << ZEROPAD << setWidth(2) << time.getNMinutes() << ':'
      << ZEROPAD << setWidth(2) << time.getNSeconds();
    if (showMS) {
      sos << '.' << setWidth(3) << ZEROPAD << time.getNMilliseconds();
    }
    return sos;
  }

  String formatRate(uint64 bytes, uint64 microseconds)
  {
    double rate = (1000000. * static_cast<double>(bytesWritten) / microseconds);
    StringOutputStream sos;
    if (rate > 1024 * 1024 * 1024) {
      return sos << base::FIXED << setPrecision(1) << rate/(1024*1024*1024) << " Gb/s";
    } else if (rate > 1024 * 1024) {
      return sos << base::FIXED << setPrecision(1) << rate/(1024*1024) << " Mb/s";
    } else if (rate > 1024) {
      return sos << base::FIXED << setPrecision(1) << rate/1024 << " kb/s";
    } else {
      return sos << base::FIXED << setPrecision(1) << rate << " b/s";
    }
  }

  // TAG: aborted by user needs to write on new line or clear current line

  MemorySize push(const uint8* buffer, MemorySize size)
  {
    unsigned int result = file.write(buffer, size);
    // TAG: run download in separate thread
    // TAG: show time since last byte read when running in separate thread
    // TAG: ansi color
    StringOutputStream sos;
    sos << base::FIXED << setPrecision(1);
    sos << 123.; // TAG: add test
    String temp = sos;

    // TAG: 1.0e2 %

    BASSERT(result == size);
    bytesWritten += size;
    const uint64 microseconds = timer.getLiveMicroseconds();
    if (totalSize > 0) {
      fout << "Read=" << setForeground(ANSIEscapeSequence::GREEN) << formatBytes(bytesWritten) << normal()
        << "  Progress=" << setForeground(ANSIEscapeSequence::GREEN) << formatProgress(bytesWritten, totalSize) << normal()
        << "  Time=" << setForeground(ANSIEscapeSequence::GREEN) << formatTime(microseconds) << normal()
        << "  ETA=" << setForeground(ANSIEscapeSequence::GREEN) << formatTime((bytesWritten > 0) ? (totalSize - bytesWritten)/bytesWritten*microseconds : 0) << normal()
        << "  Rate=" << setForeground(ANSIEscapeSequence::GREEN) << formatRate(bytesWritten, microseconds) << normal()
        << "           \r" << FLUSH;
    } else {
      fout << "Read=" << formatBytes(bytesWritten)
        << "  Time=" << formatTime(microseconds)
        << "  Rate=" << formatRate(bytesWritten, microseconds)
        << "           \r" << FLUSH;
    }
    // Thread::microsleep(125*1000);
    return size;
  }
  
  // TAG: show SHA256/...
  // TAG: allow continue from previous download
  // TAG: autorestart if connection is slow

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
  bool showJSON = false;
  String method = "GET";
  String body;
public:
  
  HTTPSApplication()
    : Application("https")
  {
  }

  void onTermination() noexcept override
  {
    fout << (String(" ") * 79U) << EOL;
    Application::onTermination();
  }

  bool parseArguments()
  {
    Array<String> arguments = getArguments();
    Array<String>::ReadEnumerator enu = arguments.getReadEnumerator();
    while (enu.hasNext()) {
      const String argument = enu.next();
      if (argument == "--post") {
        method = "POST";
      } else if (argument == "--method") {
        if (!enu.hasNext()) {
          ferr << "Expected method." << ENDL;
          return false;
        }
        method = enu.next();
        // TAG: change to name and value in 1 argument
      } else if (argument == "--field") {
        if (!enu.hasNext()) {
          ferr << "Expected header field name." << ENDL;
          return false;
        }
        auto name = enu.next();
        if (!enu.hasNext()) {
          ferr << "Expected header field value." << ENDL;
          return false;
        }
        auto value = enu.next();
        fields.add(name, value);
      } else if (argument == "--body") {
        if (!enu.hasNext()) {
          ferr << "Expected body." << ENDL;
          return false;
        }
        body = enu.next();
      } else if (argument == "--header") {
        showHeader = true;
      } else if (argument == "--json") {
        showJSON = true;
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

  void main() override
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
         << ENDL;

    if (!parseArguments()) {
      setExitCode(Application::EXIT_CODE_ERROR);
      return;
    }

    // fout << "Usage: " << getFormalName() << " [url] [output]" << ENDL;

    try {
      HTTPSRequest request;
      if (!request.open(method, url)) {
        setExitCode(Application::EXIT_CODE_ERROR);
        return;
      }

      auto enu = fields.getReadEnumerator();
      while (enu.hasNext()) {
        auto nv = enu.next();
        request.setRequestHeader(nv.getKey(), nv.getValue());
      }
      
      // request.setRequestHeader("Keep-Alive", "30");
      // TAG: allow form data - get from options

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
        if (showJSON) {
          String response = request.getResponse();
          auto r = JSON().parse(response).cast<ObjectModel::Object>(); // TAG: do this via Value class
          if (!r) {
            error("Response is not valid JSON.", Application::EXIT_CODE_ERROR);
            return;
          }
          auto data = JSON::getJSON(r, ObjectModel::DEFAULT_FORMATTING |
            (fout.isANSITerminal() ? ObjectModel::FLAG_COLOR : 0));
          fout << data << ENDL;
        } else {
          PushToStandardOutput push;
          request.getResponse(&push);
        }
      }

      request.close();
    } catch (...) {
      ferr << "Error: " << "Failed to read response." << ENDL;
      setExitCode(Application::EXIT_CODE_ERROR);
      return;
    }
  }
};

APPLICATION_STUB(HTTPSApplication);
