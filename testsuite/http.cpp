/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/Timer.h>
#include <base/Trace.h>
#include <base/Primitives.h>
#include <base/UnsignedInteger.h>
#include <base/concurrency/Thread.h>
#include <base/io/File.h>
#include <base/net/ClientSocket.h>
#include <base/net/InetEndPoint.h>
#include <base/net/InetInterface.h>
#include <base/net/InetService.h>
#include <base/net/ServerSocket.h>
#include <base/net/Url.h>
#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/StringOutputStream.h>

#undef OPTIONS
#undef GET
#undef HEAD
#undef POST
#undef PUT
#undef DELETE
#undef TRACE
#undef CONNECT

using namespace base;

namespace httpCommands {
  
  // Methods
  const StringLiteral METHOD_OPTIONS = MESSAGE("OPTIONS");
  const StringLiteral METHOD_GET = MESSAGE("GET");
  const StringLiteral METHOD_HEAD = MESSAGE("HEAD");
  const StringLiteral METHOD_POST = MESSAGE("POST");
  const StringLiteral METHOD_PUT = MESSAGE("PUT");
  const StringLiteral METHOD_DELETE = MESSAGE("DELETE");
  const StringLiteral METHOD_TRACE = MESSAGE("TRACE");
  const StringLiteral METHOD_CONNECT = MESSAGE("CONNECT");

  // Access control commands
  const StringLiteral CMD_ACCOUNT = MESSAGE("ACCT"); // set account
  const StringLiteral CMD_CDUP = MESSAGE("CDUP"); // change to parent directory
  const StringLiteral CMD_CWD = MESSAGE("CWD"); // change working directory
  const StringLiteral CMD_LOGOUT = MESSAGE("QUIT"); // logout
  const StringLiteral CMD_PASSWORD = MESSAGE("PASS"); // set password
  const StringLiteral CMD_REINITIALIZE = MESSAGE("REIN"); // reinitialize
  const StringLiteral CMD_USER = MESSAGE("USER"); // set user

  // Transfer parameter commands
  const StringLiteral CMD_DATA_PORT = MESSAGE("PORT"); // set host data connection port
  const StringLiteral CMD_PASSIVE = MESSAGE("PASV"); // request passive mode
  const StringLiteral CMD_REPRESENTATION = MESSAGE("TYPE"); // request data representation (AEIL)
  const StringLiteral CMD_FILE_STRUCTURE = MESSAGE("STRU"); // request file structure (file/record/page)
  const StringLiteral CMD_TRANSFER_MODE = MESSAGE("MODE"); // (stream/block/compressed)

  // FTP service commands
  const StringLiteral CMD_RETRIEVE = MESSAGE("RETR"); // retrieve file from server
  const StringLiteral CMD_STORE = MESSAGE("STOR"); // store file on server
  const StringLiteral CMD_STORE_UNIQUE = MESSAGE("STOU"); // store unique file on server
  const StringLiteral CMD_APPEND = MESSAGE("APPE"); // append data to file on server
  const StringLiteral CMD_ALLOCATE = MESSAGE("ALLO"); // allocate storage for specified number of bytes
  const StringLiteral CMD_RESTART = MESSAGE("REST"); // restart transfer at marker
  const StringLiteral CMD_RENAME_FROM = MESSAGE("RNFR"); // rename from
  const StringLiteral CMD_RENAME_TO = MESSAGE("RNTO"); // rename to
  const StringLiteral CMD_ABORT = MESSAGE("ABOR"); // abort transfers
  const StringLiteral CMD_DELETE = MESSAGE("DELE"); // delete file
  const StringLiteral CMD_REMOVE_DIRECTORY = MESSAGE("RMD"); // remove directory
  const StringLiteral CMD_MAKE_DIRECTORY = MESSAGE("MKD"); // make directory
  const StringLiteral CMD_PWD = MESSAGE("PWD"); // print working directory
  const StringLiteral CMD_LIST = MESSAGE("LIST"); // list directory or file content
  const StringLiteral CMD_NAME_LIST = MESSAGE("NLST"); // request name list
  const StringLiteral CMD_SITE = MESSAGE("SITE"); // site specific
  const StringLiteral CMD_SYSTEM = MESSAGE("SYST"); // system type
  const StringLiteral CMD_STATUS = MESSAGE("STAT"); // status
  const StringLiteral CMD_HELP = MESSAGE("HELP"); // request help
  const StringLiteral CMD_NOOP = MESSAGE("NOOP"); // no operation
  
}; // httpCommands namespace

using namespace httpCommands;

/**
  This exception is thrown by the HTTP class.

  @short HTTP exception.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class HTTPException : public IOException {
private:

  /** Specifies that the exception cannot be resolved. */
  bool permanent;
public:

  /**
    Initializes the exception object with no message.
  */
  inline HTTPException() : permanent(true) {}

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline HTTPException(const char* message) : IOException(message) {}

  inline bool isPermanent() throw() {return permanent;}
};



/** HTTP formatting traits. */
class HTTPTraits {
public:

  static const char SP = ' ';
  
  static inline bool isLWS(char value) throw() {
    return (value == ' ') || (value == '\t');
  }

  static inline bool isText(char value) throw() {
    // TAG: RFC states that non-ASCII chars should be accepted! - I ignore this for now
    return ASCIITraits::isASCII(value) && ((value == ' ') || (value == '\t') || !ASCIITraits::isControl(value));
  }

  static inline bool isSeparator(char value) throw() {
    switch (value) {
    case '\t': case ' ':
    case '(': case ')': case '<': case '>': case '@': case ',': case ';': case ':':
    case '\\': case '"': case '/': case '[': case ']': case '?': case '=': case '{': case '}':
      return true;
    }
    return false;
  }

  static inline bool isToken(char value) throw() {
    return ASCIITraits::isASCII(value) && (!ASCIITraits::isControl(value) && !isSeparator(value));
  }

  static const char* SHORT_WEEKDAY[7];
  static const char* LONG_WEEKDAY[7];
  static const char* SHORT_MONTH[12];
};

const char* HTTPTraits::SHORT_WEEKDAY[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
const char* HTTPTraits::LONG_WEEKDAY[7] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
const char* HTTPTraits::SHORT_MONTH[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

class MessageHeader : public Object {
private:

  String name;
  String value;
public:

  MessageHeader(const String& line) throw(HTTPException) {
    String::ReadIterator begin = line.getBeginReadIterator();
    const String::ReadIterator end = line.getEndReadIterator();
    String::ReadIterator i = begin;
    String::ReadIterator name = i;
    for (; (i < end) && HTTPTraits::isToken(*i); ++i) { // skip name
    }
    assert(name < i, HTTPException("Invalid message header"));
    this->name = line.substring(name - begin, i - begin);
    assert(*i++ == ':', HTTPException("Invalid message header"));
    for (; (i < end) && HTTPTraits::isLWS(*i); ++i) { // skip LWS
    }
    String::ReadIterator value = i;
    String::ReadIterator endValue = end;
    while (value < endValue) { // skip trailing LWS
      --endValue;
      if (!HTTPTraits::isLWS(*endValue)) {
        ++endValue;
        break;
      }
    }
    if (*i == '"') { // is value quoted
      ++i; // skip '"'
      ++value;
      for (; (i < endValue) && (*i != '"') && HTTPTraits::isText(*i); ++i) { // skip quoted string
        // TAG: need support for '\\'
      }
      assert((i == --endValue) && (*i++ == '"'), HTTPException("Invalid message header"));
    }
    this->value = line.substring(value - begin, endValue - begin);
  }

  String getName() const throw() {return name;}

  String getValue() const throw() {return value;}
};


class PushInterface {
public:

  virtual bool pushBegin(long long totalSize) throw() = 0;
  virtual void push(const char* buffer, unsigned int size) throw() = 0;
  virtual void pushEnd() throw() = 0;
};

class PullInterface {
public:

  virtual long long pullBegin() const throw() = 0;
  virtual unsigned int pull(char* buffer, unsigned int size) throw() = 0;
};

class PushToNothing {
public:

  bool pushBegin(long long totalSize) throw() {
  }

  void push(const char* buffer, unsigned int size) throw() {
  }

  void pushEnd() throw() {
  }
};

class PushToStandardOutput : public virtual Object, public PushInterface {
public:

  PushToStandardOutput() throw() {
  }

  bool pushBegin(long long totalSize) throw() {
    return true;
  }

  void push(const char* buffer, unsigned int size) throw() {
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
  }

  void pushEnd() throw() {
    fout << ENDL;
  }
};


class PushToFile : public virtual Object, public PushInterface {
private:

  File file;
  Timer timer;
  long long bytesWritten;
  long long totalSize;
public:

  PushToFile(File f) throw() : file(f) {
    bytesWritten = 0;
  }

  bool pushBegin(long long totalSize) throw() {
    this->totalSize = totalSize;
    timer.start();
    return true;
  }

  void push(const char* buffer, unsigned int size) throw() {
    unsigned int result = file.write(buffer, size);
    ASSERT(result == size);
    bytesWritten += size;
    if (totalSize > 0) {
      fout << "  bytes written=" << bytesWritten
           << "  completed=" << base::FIXED << setWidth(7) << setPrecision(3) << static_cast<long double>(bytesWritten)/totalSize*100 << "%"
           << "  time=" << base::FIXED << setWidth(6) << timer.getLiveMicroseconds()/1000000.
           << "  rate=" << base::FIXED << setWidth(12) << setPrecision(3) << (1000000./1024 * static_cast<long double>(bytesWritten)/timer.getLiveMicroseconds()) << "kbs\r" << FLUSH;
    } else {
      fout << "  bytes written=" << bytesWritten
           << "  time=" << base::FIXED << setWidth(6) << timer.getLiveMicroseconds()/1000000.
           << "  rate=" << base::FIXED << setWidth(12) << setPrecision(3) << (1000000./1024 * static_cast<long double>(bytesWritten)/timer.getLiveMicroseconds()) << "kbs\r" << FLUSH;
    }
  }

  void pushEnd() throw() {
    fout << ENDL;
    file.close();
  }
};

/**
  Hypertext Transfer Protocol (HTTP/1.1) client (uses a subset of RFC 2616).

  @short HTTP client.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class HypertextTransferProtocolClient : public Object {
public:

  /** The default retry delay in seconds. */
  static const unsigned int DEFAULT_RETRY_DELAY = 15;
  /** The default number of retry attempts before giving up. */
  static const unsigned int DEFAULT_RETRY_ATTEMPTS = 5;

  class InvalidResponse : public HTTPException {
  public:

    /**
      Initializes the exception object with no message.
    */
    inline InvalidResponse() : HTTPException() {}

    /**
      Initializes the exception object.

      @param message The message.
    */
    inline InvalidResponse(const char* message) : HTTPException(message) {}
  };

  typedef HTTPTraits Traits;

  /** Verbosity levels. */
  enum Verbosity {SILENT, SHORT, ALL, DEBUG, DEBUG_EXTENDED};
  /** Status code classes. */
  enum StatusClass {INFORMATION, SUCCESS, REDIRECTION, CLIENT_ERROR, SERVER_ERROR};
  /** Methods. */
  enum Method {OPTIONS, GET, HEAD, POST, PUT, DELETE, TRACE, CONNECT};
  /** Content types. */
  enum ContentType {TEXT, IMAGE, UNSPECIFIED}; // FIXME: need mime-type support

  struct Status {
    StatusClass statusClass;
    int code;
  };
private:

  const String host;
  InetEndPoint endPoint;
  /** The control connection. */
  StreamSocket controlConnection;

  /** Specifies that a new response is pending. */
  bool responsePending;
  /** The last line of the last response. */
  String response;
  /** The last status code. */
  Status status;
  /** Reason phrase of last reply. */
  String reasonPhrase;
  /** Specifies the verbosity. */
  Verbosity verbosity;

  /** The retry delay in seconds. */
  unsigned int retryDelay;
  /** The number of retry attempts. */
  unsigned int retryAttempts;
  /** Read buffer. */
  Allocator<char> buffer;
protected:

  void translateStatus(const String& value) throw(HTTPException) {
    String::ReadIterator i = value.getBeginReadIterator();
    String::ReadIterator end = value.getEndReadIterator();

    bool valid = true;
    bool validVersion = false;
    bool validCode = false;
    bool validPhrase = false;

    if ((i < end) && (*i++ == 'H') &&
        (i < end) && (*i++ == 'T') &&
        (i < end) && (*i++ == 'T') &&
        (i < end) && (*i++ == 'P') &&
        (i < end) && (*i++ == '/') &&
        (i < end) && ASCIITraits::isDigit(*i)) {
      while ((i < end) && ASCIITraits::isDigit(*i)) { // get major version
        ++i;
      }
      if ((i < end) && (*i++ == '.') && (i < end) && ASCIITraits::isDigit(*i)) {
        while ((i < end) && ASCIITraits::isDigit(*i)) { // get minor version
          ++i;
        }
        validVersion = true;
      }
    }

    if (!((i < end) && (*i++ == ' '))) { // check for field separator
      valid = false;
    }

    if (valid && validVersion) {
      if (end - i >= 3) { // need 3 digits
        char a = *i++;
        char b = *i++;
        char c = *i++;
        if ((a >= '1') && (a <= '5') && ASCIITraits::isDigit(b) && ASCIITraits::isDigit(c)) {
          static const StatusClass classes[] = {INFORMATION, SUCCESS, REDIRECTION, CLIENT_ERROR, SERVER_ERROR};
          status.statusClass = classes[a - '1'];
          status.code = (static_cast<int>(a-'0') * 10 + static_cast<int>(b-'0')) * 10 + static_cast<int>(c-'0');
          validCode = true;
        }
      }
    }

    if (!((i < end) && (*i++ == ' '))) { // check for field separator
      valid = false;
    }

    if (valid && validCode) {
      while ((i < end) && (*i > 0x1f) && (*i < 0x7f)) {
        reasonPhrase += *i++;
      }
      if (i == end) {
        validPhrase = true;
      }
    }

    assert(valid && validVersion && validCode && validPhrase, HTTPException("Invalid response"));
  }

  /* See chapter 5 of RFC */
  String makeRequest(Method method, const String& host, const String& resourceUri) throw(IOException) {
    static const StringLiteral AGENT = MESSAGE("http/0.1 (http://www.mip.sdu.dk/~fonseca/base)");
    static const StringLiteral methods[] = {METHOD_OPTIONS, METHOD_GET, METHOD_HEAD, METHOD_POST, METHOD_PUT, METHOD_DELETE, METHOD_TRACE, METHOD_CONNECT};

    assert(resourceUri != "", HTTPException("Empty resourceUri"));
    
    StringOutputStream stream;
    stream << methods[method] << Traits::SP << resourceUri << Traits::SP << MESSAGE("HTTP/1.1") << CRLF // Request-Line
           << MESSAGE("Host: ") << host << CRLF // Section 14.23 (required)
           << MESSAGE("User-Agent: ") << AGENT << CRLF // Section 14.43
           << CRLF << FLUSH;
    if (verbosity >= DEBUG) {
      fout << "Request: " << stream.getString() << ENDL;
    }
    return stream.getString();
  }

  // See chapter 6 in RFC
  void getResponse(PushInterface* push) throw(HTTPException) {
    controlConnection.wait();
    
    FormatInputStream instream(controlConnection);

    if (verbosity >= DEBUG) {
      fout << MESSAGE("DEBUG: bytes available: ") << instream.available() << ENDL;
    }
    ASSERT(instream.available() == controlConnection.available());

    int terminationCode = -1; // invalidate

    // Status-Line - HTTP-Version SP Status-Code SP Reason-Phrase CRLF - See section 6.1 in RFC
    String statusLine;
    instream >> statusLine;

    if (verbosity >= DEBUG) {
      fout << MESSAGE("Status-Line: ") << statusLine << ENDL;
    }
    translateStatus(statusLine);

    bool chunkedTransferEncoding = false;
    bool hasContentLength = false;
    unsigned int contentLength;
    String contentType;

    // Section 4.5, 6.2, and 7.1 in RFC
    while (true) { // read response
      String line;
      instream >> line;

      if (verbosity >= ALL) {
        fout << ">> " << line << ENDL;
      }

      if (line.isEmpty()) {
        break; // end of headers
      }

      MessageHeader header(line);
      fout << MESSAGE("name=") << header.getName() << Traits::SP << MESSAGE("value=") << header.getValue() << ENDL;

      if (header.getName() == "Transfer-Encoding") {
        if (header.getValue().toLowerCase() == "chunked") {
          chunkedTransferEncoding = true;
        }
      } else if (header.getName() == "Content-Length") {
        try {
          contentLength = UnsignedInteger(header.getValue());
          hasContentLength = true;
        } catch(InvalidFormat& e) {
          throw HTTPException("Invalid value for Content-Length field");
        }
      } else if (header.getName() == "Content-Type") {
        contentType = header.getValue();
      }
    }

    if (chunkedTransferEncoding) {
      unsigned long long totalLength = 0;
      String line;

      while (true) { // read all chunks
        instream >> line;

        String::ReadIterator i = line.getBeginReadIterator();
        const String::ReadIterator end = line.getEndReadIterator();
        while ((i < end) && (*i == Traits::SP)) { // skip spaces
          ++i;
        }
        assert((i < end) && ASCIITraits::isHexDigit(*i), InvalidResponse("Chunk size invalid"));
        unsigned int chunkSize = 0;
        while ((i < end) && ASCIITraits::isHexDigit(*i)) { // read chunk size
          chunkSize = chunkSize * 16 + ASCIITraits::digitToValue(*i);
          ++i;
        }

        // TAG: need to chunk extension
        if (chunkSize == 0) { // stop if last chunk
          break;
        }

        totalLength += chunkSize;
        ASSERT(push);
        if (push->pushBegin(0)) { // total size is unknown
          long long bytesRead = 0;
          while (bytesRead < chunkSize) {
            unsigned int bytesToRead = minimum<long long>(buffer.getSize(), chunkSize - bytesRead);
            unsigned int result = instream.read(buffer.getElements(), bytesToRead);
            bytesRead += result;
            push->push(buffer.getElements(), result);
          }
        }

        instream >> line;
        ASSERT(line.isEmpty());
      }
      push->pushEnd();

      while (true) { // read trailer
        String line;
        instream >> line;

        if (verbosity >= ALL) {
          fout << ">> " << line << ENDL;
        }

        if (line.isEmpty()) {
          break; // end of trailer
        }
      }
    } else if (hasContentLength) { // message-body - See section 7.2 in RFC
      if (verbosity >= DEBUG) {
        fout << MESSAGE("Reading content: ") << contentLength << MESSAGE(" byte(s)") << ENDL;
      }

      if (push) {
        if (push->pushBegin(contentLength)) {
          long long bytesRead = 0;
          while (bytesRead < contentLength) {
            unsigned int bytesToRead = minimum<long long>(buffer.getSize(), contentLength - bytesRead);
            unsigned int result = instream.read(buffer.getElements(), bytesToRead);
            bytesRead += result;
            push->push(buffer.getElements(), result);
          }
          push->pushEnd();
        }
      } else {
        if (verbosity >= DEBUG) {
          fout << MESSAGE("DEBUG: skipping ") << contentLength << MESSAGE(" byte(s)") << ENDL;
        }
        instream.skip(contentLength);
      }
    }
  }
public:

  static bool isValidString(const String& str) throw() {
    if (str.isEmpty()) {
      return false;
    }
    const String::ReadIterator end = str.getEndReadIterator();
    for (String::ReadIterator i = str.getBeginReadIterator(); i < end; ++i) {
      if (!String::Traits::isASCII(*i) || (*i == '\n') || (*i == '\r')) {
        return false;
      }
    }
    return true;
  }

  static bool isValidPrintableString(const String& str) throw() {
    if (str.isEmpty()) {
      return false;
    }
    const String::ReadIterator end = str.getEndReadIterator();
    for (String::ReadIterator i = str.getBeginReadIterator(); i < end; ++i) {
      if ((*i < 33) || (*i > 126)) {
        return false;
      }
    }
    return true;
  }

  HypertextTransferProtocolClient(const String& _host, InetEndPoint _endPoint, Verbosity _verbosity = DEBUG_EXTENDED) throw()
    : host(_host),
      endPoint(_endPoint),
      verbosity(_verbosity),
      retryDelay(DEFAULT_RETRY_DELAY),
      retryAttempts(DEFAULT_RETRY_ATTEMPTS),
      buffer(4096 * 64) {
  }

  unsigned int getRetryDelay() const throw() {
    return retryDelay;
  }

  void setRetryDelay(unsigned int value) throw() {
    retryDelay = value;
  }

  unsigned int getRetryAttempts() const throw() {
    return retryAttempts;
  }

  void setRetryAttempts(unsigned int value) throw() {
    retryAttempts = value;
  }

  void connect() throw(HTTPException) {
    if (verbosity >= DEBUG) {
      fout << "DEBUG: Establishing control connection to: "
           << "address=" << endPoint.getAddress() << " port=" << endPoint.getPort() << ENDL;
    }
    controlConnection.connect(endPoint.getAddress(), endPoint.getPort());
    controlConnection.getName();
  }

  void getOptions() throw(HTTPException) {
    String request = makeRequest(OPTIONS, host, "*");
    FormatOutputStream outstream(controlConnection);
    outstream << request << FLUSH;
    PushToStandardOutput push;
    getResponse(&push);
  }

  void getResource(const String& resource, PushInterface* push) throw(HTTPException) {
    if (resource.isProper()) {
      String request = makeRequest(GET, host, resource);
      FormatOutputStream outstream(controlConnection);
      outstream << request << FLUSH;
    } else {
      String request = makeRequest(GET, host, "/");
      FormatOutputStream outstream(controlConnection);
      outstream << request << FLUSH;
    }
    getResponse(push);
  }

  ~HypertextTransferProtocolClient() {
    if (verbosity >= DEBUG) {
      fout << MESSAGE("DEBUG: Closing sockets...") << ENDL;
    }
    controlConnection.shutdownOutputStream();
    controlConnection.close();
  }
};

class HTTPClient : public Object {
public:

  HTTPClient(const String& resource, const String& filename) throw() {
    Url url(resource, false);

    if (url.getScheme().isEmpty()) {
      url.setScheme("http");
    }

    fout << MESSAGE("Individual parts of the specified url:") << EOL
         << MESSAGE("  scheme: ") << url.getScheme() << EOL
         << MESSAGE("  user: ") << url.getUser() << EOL
         << MESSAGE("  password: ") << url.getPassword() << EOL
         << MESSAGE("  host: ") << url.getHost() << EOL
         << MESSAGE("  port: ") << (url.getPort().isProper() ? url.getPort() : String(MESSAGE("80"))) << EOL
         << MESSAGE("  path: ") << url.getPath() << ENDL;

    if (url.getScheme() != "http") {
      fout << MESSAGE("Invalid url") << ENDL;
      return;
    }

    InetAddress address; // the address of the remote host
    {
      fout << MESSAGE("Server addresses:") << ENDL;
      List<InetAddress> addresses = InetAddress::getAddressesByName(url.getHost());
      List<InetAddress>::ReadEnumerator enu = addresses.getReadEnumerator();
      unsigned int index = 0;
      while (enu.hasNext()) {
        const InetAddress* temp = enu.next();
        if (index == 0) { // use the first address
          address = *temp;
          fout << MESSAGE("  address ") << index++ << MESSAGE(": ") << *temp << MESSAGE(" (USING THIS)") << ENDL;
        } else {
          fout << MESSAGE("  address ") << index++ << MESSAGE(": ") << *temp << ENDL;
        }
      }
    }

    InetEndPoint endPoint(address, (url.getPort().isProper() ? url.getPort() : String(MESSAGE("80"))));

    String host;
    String port = url.getPort();
    if (port.isProper()) {
      host = url.getHost() + String(MESSAGE(":")) + port;  
    } else {
      host = url.getHost();
    }
    
    HypertextTransferProtocolClient client(host, endPoint); // FIXME: include port number if present
    client.connect();
    client.getOptions();
    
    if (filename.isProper()) {
      PushToFile push(File(filename, File::WRITE, File::CREATE | File::TRUNCATE));
      client.getResource("/" + url.getPath(), &push);
    } else {
      PushToStandardOutput push;
      client.getResource("/" + url.getPath(), &push);
    }
  }
};

class HTTPApplication : public Application {
public:
  
  HTTPApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw() 
    : Application(MESSAGE("http"), numberOfArguments, arguments, environment) {
  }
  
  static void main() throw() {
    fout << MESSAGE("Testing Hypertext Transfer Protocol (HTTP) class...") << ENDL;
    Array<String> arguments = Application::getApplication()->getArguments();

    String url = MESSAGE("www.mip.sdu.dk/~fonseca/index.html"); // default url
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
      fout << "usage: " << Application::getApplication()->getName() << " [url] [output]" << ENDL;
      Application::getApplication()->setExitCode(Application::EXIT_CODE_ERROR);
      return;
    }
    HTTPClient client(url, file);
  }
};

int main(int argc, const char* argv[], const char *env[]) {
  HTTPApplication app(argc, argv, env);
  try {
    HTTPApplication::main();
  } catch(Exception& e) {
    return Application::getApplication()->exceptionHandler(e);
  } catch(...) {
    return Application::getApplication()->exceptionHandler();
  }
  return Application::getApplication()->getExitCode();
}
