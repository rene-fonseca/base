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
#include <base/Trace.h>
#include <base/Primitives.h>
#include <base/UnsignedInteger.h>
#include <base/concurrency/Thread.h>
#include <base/io/File.h>
#include <base/net/StreamSocket.h>
#include <base/net/InetEndPoint.h>
#include <base/net/InetInterface.h>
#include <base/net/InetService.h>
#include <base/net/ServerSocket.h>
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

  // Access control commands
  const Literal CMD_ACCOUNT = MESSAGE("ACCT"); // set account
  const Literal CMD_CDUP = MESSAGE("CDUP"); // change to parent directory
  const Literal CMD_CWD = MESSAGE("CWD"); // change working directory
  const Literal CMD_LOGOUT = MESSAGE("QUIT"); // logout
  const Literal CMD_PASSWORD = MESSAGE("PASS"); // set password
  const Literal CMD_REINITIALIZE = MESSAGE("REIN"); // reinitialize
  const Literal CMD_USER = MESSAGE("USER"); // set user

  // Transfer parameter commands
  const Literal CMD_DATA_PORT = MESSAGE("PORT"); // set host data connection port
  const Literal CMD_PASSIVE = MESSAGE("PASV"); // request passive mode
  const Literal CMD_REPRESENTATION = MESSAGE("TYPE"); // request data representation (AEIL)
  const Literal CMD_FILE_STRUCTURE = MESSAGE("STRU"); // request file structure (file/record/page)
  const Literal CMD_TRANSFER_MODE = MESSAGE("MODE"); // (stream/block/compressed)
}; // commands namespace

using namespace commands;

/**
  This exception is raised by the HTTP class.

  @short HTTP exception.
  @version 1.0
*/

class HTTPException : public IOException {
private:

  /** Specifies that the exception cannot be resolved. */
  bool permanent = true;
public:

  /**
    Initializes the exception object with no message.
  */
  inline HTTPException() {
  }
  
  /**
    Initializes the exception object.

    @param message The message.
  */
  inline HTTPException(const char* message) : IOException(message) {
  }
  
  inline bool isPermanent() {
    return permanent;
  }
};



/** HTTP formatting traits. */
class HTTPTraits {
public:

  static const char SP = ' ';
  
  static inline bool isLWS(char value) {
    return (value == ' ') || (value == '\t');
  }

  static inline bool isText(char value) {
    // TAG: RFC states that non-ASCII chars should be accepted! - I ignore this for now
    return ASCIITraits::isASCII(value) &&
      ((value == ' ') || (value == '\t') || !ASCIITraits::isControl(value));
  }

  static inline bool isSeparator(char value) {
    switch (value) {
    case '\t': case ' ':
    case '(': case ')': case '<': case '>': case '@': case ',': case ';': case ':':
    case '\\': case '"': case '/': case '[': case ']': case '?': case '=': case '{': case '}':
      return true;
    }
    return false;
  }

  static inline bool isToken(char value) {
    return ASCIITraits::isASCII(value) &&
      (!ASCIITraits::isControl(value) && !isSeparator(value));
  }

  static const char* SHORT_WEEKDAY[7];
  static const char* LONG_WEEKDAY[7];
  static const char* SHORT_MONTH[12];
};

const char* HTTPTraits::SHORT_WEEKDAY[7] = {
  "Mon",
  "Tue",
  "Wed",
  "Thu",
  "Fri",
  "Sat",
  "Sun"
};

const char* HTTPTraits::LONG_WEEKDAY[7] = {
  "Monday",
  "Tuesday",
  "Wednesday",
  "Thursday",
  "Friday",
  "Saturday",
  "Sunday"
};

const char* HTTPTraits::SHORT_MONTH[12] = {
  "Jan",
  "Feb",
  "Mar",
  "Apr",
  "May",
  "Jun",
  "Jul",
  "Aug",
  "Sep",
  "Oct",
  "Nov",
  "Dec"
};

class MessageHeader : public Object {
private:

  String name;
  String value;
public:

  MessageHeader(const String& line) {
    String::ReadIterator begin = line.getBeginReadIterator();
    const String::ReadIterator end = line.getEndReadIterator();
    String::ReadIterator i = begin;
    String::ReadIterator name = i;
    for (; (i < end) && HTTPTraits::isToken(*i); ++i) { // skip name
    }
    bassert(name < i, HTTPException("Invalid message header."));
    this->name = line.substring(name - begin, i - begin);
    bassert(*i++ == ':', HTTPException("Invalid message header."));
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
      bassert(
        (i == --endValue) && (*i++ == '"'),
        HTTPException("Invalid message header.")
      );
    }
    this->value = line.substring(value - begin, endValue - begin);
  }

  String getName() const {
    return name;
  }

  String getValue() const {
    return value;
  }
};

class PushInterface {
public:

  virtual bool pushBegin(long long totalSize) = 0;
  virtual MemorySize push(const uint8* buffer, MemorySize size) = 0;
  virtual void pushEnd() = 0;
};

class PullInterface {
public:

  virtual long long pullBegin() const = 0;
  virtual MemorySize pull(uint8* buffer, MemorySize size) = 0;
};

class PushToNothing {
public:

  bool pushBegin(long long totalSize)
  {
    return true;
  }

  MemorySize push(const uint8* buffer, MemorySize size)
  {
    return size;
  }

  void pushEnd() {
  }
};

class PushToStandardOutput : public virtual Object, public PushInterface {
public:

  PushToStandardOutput() {
  }

  bool pushBegin(long long totalSize)
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
  long long bytesWritten = 0;
  long long totalSize = 0;
public:

  PushToFile(File _file) : file(_file) {
  }

  bool pushBegin(long long totalSize) {
    this->totalSize = totalSize;
    timer.start();
    return true;
  }
  
  MemorySize push(const uint8* buffer, MemorySize size)
  {
    unsigned int result = file.write(buffer, size);
    BASSERT(result == size);
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

/**
  Hypertext Transfer Protocol (HTTP/1.1) client (uses a subset of RFC 2616).

  @short HTTP client.
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
    inline InvalidResponse() : HTTPException() {
    }
    
    /**
      Initializes the exception object.

      @param message The message.
    */
    inline InvalidResponse(const char* message) : HTTPException(message) {
    }
  };

  typedef HTTPTraits Traits;

  /** Verbosity levels. */
  enum Verbosity {SILENT, SHORT, ALL, DEBUG_NORMAL, DEBUG_EXTENDED};
  /** Status code classes. */
  enum StatusClass {INFORMATION, SUCCESS, REDIRECTION, CLIENT_ERROR, SERVER_ERROR};
  /** Methods. */
  enum Method {OPTIONS, GET, HEAD, POST, PUT, DELETE, TRACE, CONNECT};
  /** Content types. */
  enum ContentType {TEXT, IMAGE, UNSPECIFIED}; // FIXME: need mime-type support
  
  struct Status {
    StatusClass statusClass = CLIENT_ERROR;
    int code = 0;
  };
private:

  const String host;
  InetEndPoint endPoint;
  /** The control connection. */
  StreamSocket controlConnection;

  /** Specifies that a new response is pending. */
  bool responsePending = false;
  /** The last line of the last response. */
  String response;
  /** The last status code. */
  Status status;
  /** Reason phrase of last reply. */
  String reasonPhrase;
  /** Specifies the verbosity. */
  Verbosity verbosity = ALL;
  
  /** The retry delay in seconds. */
  unsigned int retryDelay = 0;
  /** The number of retry attempts. */
  unsigned int retryAttempts = 0;
  /** Read buffer. */
  Allocator<uint8> buffer;
protected:

  void translateStatus(const String& value) {
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

    bassert(valid && validVersion && validCode && validPhrase, HTTPException("Invalid response."));
  }

  /* See chapter 5 of RFC */
  String makeRequest(Method method, const String& host, const String& resourceUri) {
    static const Literal AGENT = MESSAGE("http/0.1 (Base Framework)");
    static const Literal methods[] = {
      METHOD_OPTIONS,
      METHOD_GET,
      METHOD_HEAD,
      METHOD_POST,
      METHOD_PUT,
      METHOD_DELETE,
      METHOD_TRACE,
      METHOD_CONNECT
    };

    bassert(resourceUri != "", HTTPException("Empty resourceUri."));
    
    StringOutputStream stream;
    stream << methods[method] << Traits::SP << resourceUri << Traits::SP
           << "HTTP/1.1" << CRLF // Request-Line
           << "Host: " << host << CRLF // Section 14.23 (required)
           << "User-Agent: " << AGENT << CRLF // Section 14.43
           << CRLF << FLUSH;
    if (verbosity >= DEBUG_NORMAL) {
      fout << "Request: " << stream.getString() << ENDL;
    }
    return stream.getString();
  }

  // See chapter 6 in RFC
  void getResponse(PushInterface* push) {
    controlConnection.wait();
    
    FormatInputStream instream(controlConnection);

    if (verbosity >= DEBUG_NORMAL) {
      fout << "DEBUG: bytes available: " << instream.available() << ENDL;
    }
    BASSERT(instream.available() == controlConnection.available());
    
    // int terminationCode = -1; // invalidate
    
    // Status-Line - HTTP-Version SP Status-Code SP Reason-Phrase CRLF - See section 6.1 in RFC
    String statusLine;
    instream >> statusLine;
    
    if (verbosity >= DEBUG_NORMAL) {
      fout << "Status-Line: " << statusLine << ENDL;
    }
    translateStatus(statusLine);
    
    bool chunkedTransferEncoding = false;
    bool hasContentLength = false;
    unsigned int contentLength = 0;
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
      fout << "name=" << header.getName() << Traits::SP
           << "value=" << header.getValue() << ENDL;
      
      if (header.getName() == "Transfer-Encoding") {
        if (header.getValue().toLowerCase() == "chunked") {
          chunkedTransferEncoding = true;
        }
      } else if (header.getName() == "Content-Length") {
        try {
          contentLength = UnsignedInteger(header.getValue());
          hasContentLength = true;
        } catch (InvalidFormat&) {
          throw HTTPException("Invalid value for Content-Length field.");
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
        bassert(
          (i < end) && ASCIITraits::isHexDigit(*i),
          InvalidResponse("Chunk size invalid")
        );
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
        BASSERT(push);
        if (push->pushBegin(0)) { // total size is unknown
          long long bytesRead = 0;
          while (bytesRead < chunkSize) {
            unsigned int bytesToRead = minimum<long long>(buffer.getSize(), chunkSize - bytesRead);
            unsigned int result = instream.read(buffer.getElements(), bytesToRead);
            bytesRead += result;
            push->push(Cast::pointer<const uint8*>(buffer.getElements()), result);
          }
        }

        instream >> line;
        BASSERT(line.isEmpty());
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
      if (verbosity >=  DEBUG_NORMAL) {
        fout << "Reading content: " << contentLength << " byte(s)" << ENDL;
      }

      if (push) {
        if (push->pushBegin(contentLength)) {
          long long bytesRead = 0;
          while (bytesRead < contentLength) {
            unsigned int bytesToRead = minimum<long long>(buffer.getSize(), contentLength - bytesRead);
            unsigned int result = instream.read(buffer.getElements(), bytesToRead);
            bytesRead += result;
            push->push(Cast::pointer<const uint8*>(buffer.getElements()), result);
          }
          push->pushEnd();
        }
      } else {
        if (verbosity >= DEBUG_NORMAL) {
          fout << "DEBUG: skipping " << contentLength << " byte(s)" << ENDL;
        }
        instream.skip(contentLength);
      }
    }
  }
public:

  static bool isValidString(const String& str) {
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

  static bool isValidPrintableString(const String& str) {
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

  HypertextTransferProtocolClient(
    const String& _host,
    InetEndPoint _endPoint,
    Verbosity _verbosity = DEBUG_EXTENDED)
    : host(_host),
      endPoint(_endPoint),
      verbosity(_verbosity),
      retryDelay(DEFAULT_RETRY_DELAY),
      retryAttempts(DEFAULT_RETRY_ATTEMPTS),
      buffer(4096 * 64) {
  }

  unsigned int getRetryDelay() const {
    return retryDelay;
  }

  void setRetryDelay(unsigned int value) {
    retryDelay = value;
  }

  unsigned int getRetryAttempts() const {
    return retryAttempts;
  }

  void setRetryAttempts(unsigned int value) {
    retryAttempts = value;
  }

  void connect() {
    if (verbosity >= DEBUG_NORMAL) {
      fout << "DEBUG: Establishing control connection to: "
           << "address=" << endPoint.getAddress() << ' '
           << "port=" << endPoint.getPort() << ENDL;
    }
    controlConnection.connect(endPoint.getAddress(), endPoint.getPort());
    controlConnection.getName();
  }

  void getOptions() {
    String request = makeRequest(OPTIONS, host, "*");
    FormatOutputStream outstream(controlConnection);
    outstream << request << FLUSH;
    PushToStandardOutput push;
    getResponse(&push);
  }

  void getResource(const String& resource, PushInterface* push) {
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
    if (verbosity >= DEBUG_NORMAL) {
      fout << "DEBUG: Closing sockets..." << ENDL;
    }
    controlConnection.shutdownOutputStream();
    controlConnection.close();
  }
};

class HTTPClient : public Object {
public:

  HTTPClient(const String& resource, const String& filename) {
    Url url(resource, false);

    if (url.getScheme().isEmpty()) {
      url.setScheme("http");
    }

    fout << "Individual parts of the specified url:" << EOL
         << "  scheme: " << url.getScheme() << EOL
         << "  user: " << url.getUser() << EOL
         << "  password: " << url.getPassword() << EOL
         << "  host: " << url.getHost() << EOL
         << "  port: "
         << (url.getPort().isProper() ? url.getPort() : String("80")) << EOL
         << "  path: " << url.getPath() << ENDL;

    if (url.getScheme() != "http") {
      fout << "Invalid url" << ENDL;
      return;
    }

    InetAddress address; // the address of the remote host
    {
      fout << "Server addresses:" << ENDL;
      List<InetAddress> addresses =
        InetAddress::getAddressesByName(url.getHost());
      List<InetAddress>::ReadEnumerator enu = addresses.getReadEnumerator();
      unsigned int index = 0;
      while (enu.hasNext()) {
        const InetAddress* temp = enu.next();
        if (index == 0) { // use the first address
          address = *temp;
          fout << "  address " << index++ << ": "
               << *temp << " (USING THIS)" << ENDL;
        } else {
          fout << "  address " << index++ << ": " << *temp << ENDL;
        }
      }
    }

    InetEndPoint endPoint(
      address, (url.getPort().isProper() ? url.getPort() : String("80"))
    );

    String host;
    String port = url.getPort();
    if (port.isProper()) {
      host = url.getHost() + String(":") + port;  
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
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:
  
  HTTPApplication(
    int numberOfArguments,
    const char* arguments[],
    const char* environment[]) 
    : Application("http", numberOfArguments, arguments, environment)
  {
  }
  
  void main()
  {
    fout << getFormalName() << " version "
         << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << "The Base Framework (Test Suite)" << EOL
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
    HTTPClient client(url, file);
  }
};

APPLICATION_STUB(HTTPApplication);
