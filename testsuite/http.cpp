/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/string/FormatInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/StringOutputStream.h>
#include <base/UnsignedInteger.h>
#include <base/net/ClientSocket.h>
#include <base/net/ServerSocket.h>
#include <base/net/InetInterface.h>
#include <base/net/InetService.h>
#include <base/net/InetEndPoint.h>
#include <base/concurrency/Thread.h>
#include <base/Type.h>
#include <base/Timer.h>
#include <base/io/File.h>
#include <base/net/Url.h>

using namespace base;

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



/**
  This exception is thrown by the HTTP class.

  @short HTTP exception.
  @author René Møller Fonseca
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



/**
  Hypertext Transfer Protocol (HTTP) client (uses a subset of RFC 2616).

  @short HTTP client.
  @author René Møller Fonseca
  @version 1.0
*/

class HypertextTransferProtocolClient : public Object {
public:

  /** The default retry delay in seconds. */
  static const unsigned int DEFAULT_RETRY_DELAY = 15;
  /** The default number of retry attempts before giving up. */
  static const unsigned int DEFAULT_RETRY_ATTEMPTS = 5;

  typedef HTTPTraits Traits;

  /** Verbosity levels. */
  typedef enum {SILENT, SHORT, ALL, DEBUG, DEBUG_EXTENDED} Verbosity;
  /** Status code classes. */
  typedef enum {INFORMATION, SUCCESS, REDIRECTION, CLIENT_ERROR, SERVER_ERROR} StatusClass;
  /** Methods. */
  typedef enum {OPTIONS, GET, HEAD, POST, PUT, DELETE, TRACE, CONNECT} Method;
  /** Reply something. */
  typedef enum {} XXX;

  typedef struct {
    StatusClass statusClass;
    int code;
  } Status;
private:

  InetEndPoint endPoint;
  /** The control connection. */
  StreamSocket controlConnection;
  /** The data connection. */
  StreamSocket dataConnection;

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

  String makeRequest(Method method, const String& resource) throw(IOException) {
    static const StringLiteral AGENT = MESSAGE("http/0.1 (http://www.mip.sdu.dk/~fonseca/base)");
    static const StringLiteral methods[] = {METHOD_OPTIONS, METHOD_GET, METHOD_HEAD, METHOD_POST, METHOD_PUT, METHOD_DELETE, METHOD_TRACE, METHOD_CONNECT};

    StringLiteral CRLF = MESSAGE("\r\n");
    StringOutputStream stream;
    stream << methods[method] << ' ' << resource << ' ' << MESSAGE("HTTP/1.1") << CRLF
           << MESSAGE("User-Agent: ") << AGENT << CRLF
           << MESSAGE("Host: ") << "" << "" << CRLF
           << MESSAGE("Accept: ") << "" << CRLF
           << "" << CRLF
           << FLUSH;
    if (verbosity >= DEBUG) {
      fout << "Request: " << stream.getString() << ENDL;
    }
    return stream.getString();
  }

  void getResponse() throw(HTTPException) {
    FormatInputStream instream(controlConnection);

    int terminationCode = -1; // invalidate

    // read status line - HTTP-Version SP Status-Code SP Reason-Phrase CRLF
    {
      String statusLine;
      instream >> statusLine;
      if (verbosity >= DEBUG) {
        fout << MESSAGE("Status-Line: ") << statusLine << ENDL;
      }
      translateStatus(statusLine);
    }

    bool hasContentLength = false;
    unsigned int contentLength;

    while (true) { // read response
      String line;
      instream >> line;

      if (verbosity >= ALL) {
        fout << ">> " << line << ENDL;
      }

      if (line.isEmpty()) {
        break;
      }

      MessageHeader header(line);
      fout << MESSAGE("name=") << header.getName() << ' ' << MESSAGE("value=") << header.getValue() << ENDL;

      if (header.getName() == "Content-Length") {
        try {
          contentLength = UnsignedInteger(header.getValue());
          hasContentLength = true;
        } catch(InvalidFormat& e) {
          throw HTTPException("Invalid response");
        }
      }
    }

    if (hasContentLength) {
      if (verbosity >= DEBUG) {
        fout << "Reading content: " << contentLength << " byte(s)" << ENDL;
      }
      while (contentLength--) {
        instream.getCharacter();
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

  HypertextTransferProtocolClient(InetEndPoint ep, Verbosity v = DEBUG_EXTENDED) throw() :
    endPoint(ep),
    verbosity(v),
    retryDelay(DEFAULT_RETRY_DELAY),
    retryAttempts(DEFAULT_RETRY_ATTEMPTS) {
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
    String request = makeRequest(OPTIONS, "*");
    FormatOutputStream outstream(controlConnection);
    outstream << request << FLUSH;
    getResponse();
  }

  void getResource(const String& resource) throw(HTTPException) {
    if (resource.isProper()) {
      String request = makeRequest(GET, resource);
      FormatOutputStream outstream(controlConnection);
      outstream << request << FLUSH;
    } else {
      String request = makeRequest(GET, "http://www.mip.sdu.dk/~fonseca/index.html");
      FormatOutputStream outstream(controlConnection);
      outstream << request << FLUSH;
    }
    getResponse();
  }

  ~HypertextTransferProtocolClient() {
    if (verbosity >= DEBUG) {
      fout << "DEBUG: Closing sockets..." << ENDL;
    }
    dataConnection.close();
    controlConnection.close();
  }
};



void httpclient(const String& resource, const String& file) {
  Url url(resource, false);

  if (url.getScheme().isEmpty()) {
    url.setScheme("http");
  }
  if (url.getPort().isEmpty()) {
    url.setPort("80");
  }

  fout << "Individual parts of the specified url:" << EOL
       << "  scheme: " << url.getScheme() << EOL
       << "  user: " << url.getUser() << EOL
       << "  password: " << url.getPassword() << EOL
       << "  host: " << url.getHost() << EOL
       << "  port: " << url.getPort() << EOL
       << "  path: " << url.getPath() << ENDL;

  if (url.getScheme() != "http") {
    fout << "Invalid url" << ENDL;
    return;
  }

  InetAddress address; // the address of the remote host
  {
    fout << "Server addresses:" << ENDL;
    List<InetAddress> addresses = InetAddress::getAddressesByName(url.getHost());
    List<InetAddress>::ReadEnumerator enu = addresses.getReadEnumerator();
    unsigned int index = 0;
    while (enu.hasNext()) {
      const InetAddress* temp = enu.next();
      if (index == 0) { // use the first address
        address = *temp;
        fout << "  address " << index++ << ": " << *temp << " (USING THIS)" << ENDL;
      } else {
        fout << "  address " << index++ << ": " << *temp << ENDL;
      }
    }
  }

  InetEndPoint endPoint(address, url.getPort());

  HypertextTransferProtocolClient client(endPoint);
  client.connect();
  client.getOptions();
  client.getResource(url.getUrl());
}

int main(int argc, const char* argv[], const char* envp[]) {
  Application app("http", argc, argv, envp);

  Array<String> arguments = Application::getApplication()->getArguments();

  String url = "www.mip.sdu.dk/~fonseca/index.html"; // default url
  String file = "httpoutput"; // default file

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
    return Application::EXIT_CODE_NORMAL; // stop
  }

  try {
    fout << "Testing Hypertext Transfer Protocol (HTTP) class..." << ENDL;
    httpclient(url, file);
  } catch(Exception& e) {
    return Application::getApplication()->exceptionHandler(e);
  } catch(...) {
    return Application::getApplication()->exceptionHandler();
  }
  return Application::EXIT_CODE_NORMAL;
}
