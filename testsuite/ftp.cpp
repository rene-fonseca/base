/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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
#include <base/net/StreamSocket.h>
#include <base/net/ServerSocket.h>
#include <base/net/InetInterface.h>
#include <base/net/InetService.h>
#include <base/net/InetEndPoint.h>
#include <base/concurrency/Thread.h>
#include <base/Primitives.h>
#include <base/Timer.h>
#include <base/io/File.h>
#include <base/net/Url.h>

using namespace dk::sdu::mip::base;

// prepend with FileTransferProtocolClient::

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
  This exception is raised by the FTP class.

  @short FTP exception.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class FTPException : public IOException {
public:

  /**
    Initializes the exception object with no message.
  */
  inline FTPException() {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline FTPException(const char* message) : IOException(message) {
  }
};



/**
  File Transfer Protocol (FTP) client (uses a subset of RFC 959).

  @short FTP client.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class FileTransferProtocolClient : public Object {
public:

  /** The default retry delay in seconds. */
  static const unsigned int DEFAULT_RETRY_DELAY = 15;
  /** The default number of retry attempts before giving up. */
  static const unsigned int DEFAULT_RETRY_ATTEMPTS = 5;

  /** Verbosity levels. */
  enum Verbosity {SILENT, SHORT, ALL, DEBUG, DEBUG_EXTENDED};

  enum Reply {POSITIVE_PRELIMINARY, POSITIVE, POSITIVE_INTERMEDIATE, NEGATIVE, PERMANENT_NEGATIVE};
  enum ReplyGroup {SYNTAX, INFORMATION, CONNECTIONS, AUTHENTICATION, UNSPECIFIED, FILESYSTEM};
  enum Representation {ASCII, EBCDIC, BINARY, LOCALSIZE};
  enum Mode {STREAM, BLOCK, COMPRESSED};
  enum Structure {FILE, RECORD, PAGE};

  typedef struct {
    Reply reply;
    ReplyGroup group;
    int minor;
    int code;
    bool valid;
  } ReplyCode;
private:

  InetEndPoint endPoint;
  /** The control connection. */
  StreamSocket controlConnection;
  /** The data connection. */
  StreamSocket dataConnection;
  /** The active data connection. */
  ServerSocket serverDataConnection;
  /** The active data connection. */
  StreamSocket activeDataConnection;

  /** Specifies that a new response is pending. */
  bool responsePending;
  /** The last line of the last response. */
  String response;
  /** The last control code. */
  ReplyCode replyCode;
  /** Specifies the verbosity. */
  Verbosity verbosity;

  /** The retry delay in seconds. */
  unsigned int retryDelay;
  /** The number of retry attempts. */
  unsigned int retryAttempts;
protected:

  static bool translateReplyCode(char a, char b, char c, ReplyCode& result) throw() {
    result.valid = false;

    switch (a) {
    case '1':
      result.reply = POSITIVE_PRELIMINARY;
      break;
    case '2':
      result.reply = POSITIVE;
      break;
    case '3':
      result.reply = POSITIVE_INTERMEDIATE;
      break;
    case '4':
      result.reply = NEGATIVE;
      break;
    case '5':
      result.reply = PERMANENT_NEGATIVE;
      break;
    default:
      return false; // invalid code
    }

    switch (b) {
    case '0':
      result.group = SYNTAX;
      break;
    case '1':
      result.group = INFORMATION;
      break;
    case '2':
      result.group = CONNECTIONS;
      break;
    case '3':
      result.group = AUTHENTICATION;
      break;
    case '4':
      result.group = UNSPECIFIED;
      break;
    case '5':
      result.group = FILESYSTEM;
      break;
    default:
      return false; // invalid code
    }

    if ((c < '0') || (c > '9')) {
      return false; // invalid code
    }
    result.minor = static_cast<int>('c'-'0');

    result.code = (static_cast<int>(a-'0') * 10 + static_cast<int>(b-'0')) * 10 + static_cast<int>(c-'0');
    result.valid = true;
    return true;
  }

  void getResponse() throw(FTPException) {
    FormatInputStream instream(controlConnection);

    replyCode.valid = false; // invalidate
    bool multipleLines = false;
    int terminationCode = -1; // invalidate

    while (!instream.wait(1000000)) { // wait for input
      // throw Timeout("FTP connection timed out");
    }

    while (true) { // read response
      String line(256);

      while (true) { // read line
        char ch;
        instream >> ch;
        assert(String::Traits::isASCII(ch), FTPException("Reply contains invalid character"));
        // read until new line "\n" and remove "\r" and other annoying chars
        if (ch == '\n') {
          break;
        } else if (ch == '\r') {
          // next must be '\n'?
        } else {
          line.append(ch);
        }
      }

      if (verbosity >= ALL) {
        fout << MESSAGE(">> ") << line << ENDL;
      }

      ReplyCode replyCode;
      if ((line.getLength() >= 4) && translateReplyCode(line[0], line[1], line[2], replyCode)) {
        if (line[3] == '-') { // first line
          if (!multipleLines) { // only accept the first code
            terminationCode = replyCode.code;
            multipleLines = true;
          }
        } else if (line[3] == ' ') { // last line
          if ((verbosity >= SHORT) && (verbosity < ALL)) {
            fout << MESSAGE(">> ") << line << ENDL;
          }
          if (!multipleLines || (multipleLines && (replyCode.code == terminationCode))) {
            response = line;
            this->replyCode = replyCode;
            break;
          }
        }
      } else if (!multipleLines) {
        throw FTPException("Invalid reply");
      }
    }
    responsePending = false;
    assert(replyCode.valid, FTPException("Failed to get reply"));
  }

  /** Sends password to server. */
  void requestPassword(const String& password) throw(IOException) {
    if (verbosity >= DEBUG_EXTENDED) {
      fout << MESSAGE("DEBUG: Sending command '") << CMD_PASSWORD
           << MESSAGE("' with value '") << password << '\'' << ENDL;
    }
    responsePending = true;
    FormatOutputStream outstream(controlConnection);
    outstream << CMD_PASSWORD << ' ' << password << MESSAGE("\r\n") << FLUSH;
  }

  /** Sends command to server without an argument. */
  void request(const String& command) throw(IOException) {
    if (verbosity >= DEBUG_EXTENDED) {
      fout << MESSAGE("DEBUG: Sending command '") << command << MESSAGE("' with no value") << ENDL;
    }
    responsePending = true;
    FormatOutputStream outstream(controlConnection);
    outstream << command << MESSAGE("\r\n") << FLUSH;
  }

  /** Sends command to server with an argument. */
  void request(const String& command, const String& value) throw(IOException) {
    if (verbosity >= DEBUG_EXTENDED) {
      fout << MESSAGE("DEBUG: Sending command '") << command << MESSAGE("' with value '") << value << '\'' << ENDL;
    }
    responsePending = true;
    FormatOutputStream outstream(controlConnection);
    outstream << command << ' ' << value << MESSAGE("\r\n") << FLUSH;
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

  FileTransferProtocolClient(InetEndPoint ep, Verbosity v = DEBUG_EXTENDED) throw()
    : endPoint(ep),
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

  void connect() throw(IOException) {
    if (verbosity >= DEBUG) {
      fout << MESSAGE("DEBUG: Establishing control connection to: ")
           << endPoint << ENDL;
    }
    controlConnection.connect(endPoint.getAddress(), endPoint.getPort());
    controlConnection.getName();
    getResponse();
  }

  int sendUser(const String& username) throw(InvalidFormat, FTPException) {
    assert(isValidString(username), InvalidFormat("Invalid FTP string"));
    request(CMD_USER, username);
    getResponse();
    return replyCode.reply;
  }

  int sendPassword(const String& password) throw(InvalidFormat, FTPException) {
    assert(isValidString(password), InvalidFormat("Invalid FTP string"));
    requestPassword(password);
    getResponse();
    return replyCode.reply;
  }

  int sendAccount(const String& account) throw(InvalidFormat, FTPException) {
    assert(isValidString(account), InvalidFormat("Invalid FTP string"));
    request(CMD_ACCOUNT, account);
    getResponse();
    return replyCode.reply;
  }

  void sendReinitialize() throw(FTPException) {
    request(CMD_REINITIALIZE);
    getResponse();
  }

  void sendHelp() throw(FTPException) {
    request(CMD_HELP);
    getResponse();
  }

  void sendNoOperation() throw(FTPException) {
    request(CMD_NOOP);
    getResponse();
  }

  void getSystemType() throw(FTPException) {
    if (verbosity >= DEBUG) {
      fout << MESSAGE("DEBUG: Getting system type...") << ENDL;
    }
    request(CMD_SYSTEM);
    getResponse();
  }

  void getStatus() throw(FTPException) {
    if (verbosity >= DEBUG) {
      fout << MESSAGE("DEBUG: Getting status...") << ENDL;
    }
    request(CMD_STATUS);
    getResponse();
  }

  void setType(Representation representation) throw(FTPException) {
    if (verbosity >= DEBUG) {
      fout << MESSAGE("DEBUG: Setting type...") << ENDL;
    }
    switch (representation) {
    case ASCII:
      request(CMD_REPRESENTATION, "A");
      break;
    case BINARY:
      request(CMD_REPRESENTATION, "I");
      break;
    }
    getResponse();
  }

  void setStructure(Structure structure) throw(FTPException) {
    if (verbosity >= DEBUG) {
      fout << MESSAGE("DEBUG: Setting file structure...") << ENDL;
    }
    switch (structure) {
    case FILE:
      request(CMD_FILE_STRUCTURE, "F");
      break;
    case RECORD:
      request(CMD_FILE_STRUCTURE, "R");
      break;
    case PAGE:
      request(CMD_FILE_STRUCTURE, "P");
      break;
    }
    getResponse();
  }

  void setMode(Mode mode) throw(FTPException) {
    if (verbosity >= DEBUG) {
      fout << MESSAGE("DEBUG: Setting mode...") << ENDL;
    }
    switch (mode) {
    case STREAM:
      request(CMD_TRANSFER_MODE, "S");
      break;
    case BLOCK:
      request(CMD_TRANSFER_MODE, "B");
      break;
    case COMPRESSED:
      request(CMD_TRANSFER_MODE, "C");
      break;
    }
    getResponse();
  }

  void getList() throw(FTPException) {
    if (verbosity >= DEBUG) {
      fout << MESSAGE("DEBUG: List...") << ENDL;
    }
    request(CMD_LIST);
    getResponse();
  }

  void readDirectoryList() throw(IOException) {
    Allocator<char> buffer(4096);

    while (true) { // read line until response
      unsigned int result = dataConnection.read(buffer.getElements(), buffer.getSize(), true);
      if (result == 0) {
        break;
      }

      const char* i = buffer.getElements();
      const char* end = i + result;
      while (i < end) { // read all
        const char* p = i;
        while ((i < end) && (*i != '\n') && (*i != '\r')) {
          ++i;
        }
        String line(p, i - p);

        // skip new line
        if (*i == '\r') {
          ++i;
          if (*i == '\n') {
            ++i;
          }
        } else if (*i == '\n') {
          ++i;
          if (*i == '\r') {
            ++i;
          }
        }

        if (verbosity >= DEBUG) {
          fout << MESSAGE("-> ") << line << ENDL;
        }
      }
    }
  }

  void readData(long long sizeOfFile) throw(IOException) {
    Allocator<char> buffer(4096 * 16);
    File file("ftpoutput", File::WRITE, File::CREATE | File::TRUNCATE);

    Timer timer;
    long long bytesRead = 0;
    while (bytesRead < sizeOfFile) {
      unsigned int bytesToRead = minimum<long long>(buffer.getSize(), sizeOfFile - bytesRead);
      unsigned int result = dataConnection.read(buffer.getElements(), bytesToRead);
      bytesRead += result;

      file.write(buffer.getElements(), result);

      if (verbosity >= SHORT) {
        fout << MESSAGE("bytes read=") << bytesRead << ' '
             << MESSAGE("completed=") << static_cast<float>(bytesRead)/sizeOfFile*100 << '%' << ' '
             << MESSAGE("time=") << timer.getLiveMicroseconds()/1000000. << ' '
             << MESSAGE("rate=") << (1000000./1024. * static_cast<float>(bytesRead)/timer.getLiveMicroseconds()) << MESSAGE("kbs\r") << FLUSH;
      }
    }
    fout << ENDL;
  }

  void restart(const String& marker) throw(InvalidFormat, FTPException) {
    if (verbosity >= DEBUG) {
      fout << MESSAGE("DEBUG: Restarting...") << ENDL;
    }
    assert(isValidPrintableString(marker), InvalidFormat("Invalid marker"));
    request(CMD_RESTART, marker);
    getResponse();
  }

  // File commands

  void renameFile(const String& from, const String& to) throw(InvalidFormat, FTPException) {
    if (verbosity >= DEBUG) {
      fout << MESSAGE("DEBUG: Renaming file system object...") << ENDL;
    }
    assert(isValidString(from) && isValidString(to), InvalidFormat("Invalid path"));
    request(CMD_RENAME_FROM, from);
    request(CMD_RENAME_TO, to);
    getResponse();
  }

  void deleteFile(const String& path) throw(InvalidFormat, FTPException) {
    if (verbosity >= DEBUG) {
      fout << MESSAGE("DEBUG: Deleting file system object...") << ENDL;
    }
    assert(isValidString(path), InvalidFormat("Invalid path"));
    request(CMD_DELETE, path);
    getResponse();
  }

  // Directory commands

  void getCurrentDirectory() throw(FTPException) {
    if (verbosity >= DEBUG) {
      fout << MESSAGE("DEBUG: Getting current directory...") << ENDL;
    }
    request(CMD_PWD);
    getResponse();
  }

  void changeDirectory(const String& path) throw(InvalidFormat, FTPException) {
    if (verbosity >= DEBUG) {
      fout << MESSAGE("DEBUG: Changing working directory...") << ENDL;
    }
    if (path == "..") {
      request(CMD_CDUP);
      getResponse();
    } else {
      assert(isValidString(path), InvalidFormat("Invalid path"));
      request(CMD_CWD, path);
      getResponse();
    }
  }

  void makeDirectory(const String& path) throw(InvalidFormat, FTPException) {
    if (verbosity >= DEBUG) {
      fout << MESSAGE("DEBUG: Creating new directory...") << ENDL;
    }
    assert(isValidString(path), InvalidFormat("Invalid path"));
    request(CMD_MAKE_DIRECTORY);
    getResponse();
  }

  void removeDirectory(const String& path) throw(InvalidFormat, FTPException) {
    if (verbosity >= DEBUG) {
      fout << MESSAGE("DEBUG: Removing directory...") << ENDL;
    }
    assert(isValidString(path), InvalidFormat("Invalid path"));
    request(CMD_REMOVE_DIRECTORY);
    getResponse();
  }
private:

  String clipPassiveResponse(const String& response, int& index) throw(FTPException) {
    String result(3);
    for (int count = 3; count && (index < response.getLength()) && (response[index] >= '0') && (response[index] <= '9'); --count) {
      result += response[index++];
    }
    assert(!result.isEmpty(), FTPException("Invalid reply"));
    return result;
  }
public:

  void login(const String& username, const String& password) throw(InvalidFormat, FTPException) {
    if (verbosity >= DEBUG) {
      fout << MESSAGE("DEBUG: Sending user and password...") << ENDL;
    }
    switch (sendUser(username)) {
    case POSITIVE: // not expected but ok
      break;
    case POSITIVE_INTERMEDIATE:
      break;
    default:
      throw FTPException("Unable to login");
    }
    switch (sendPassword(password)) {
    case POSITIVE:
      break;
    default:
      throw FTPException("Unable to login");
    }
  }

  void logout() throw(FTPException) {
    if (verbosity >= DEBUG) {
      fout << MESSAGE("DEBUG: Requesting logout...") << ENDL;
    }
    request(CMD_LOGOUT);
    getResponse();
  }

  void getDirectoryList() throw(FTPException) {
    if (verbosity >= DEBUG) {
      fout << MESSAGE("DEBUG: Retrieving directory list...") << ENDL;
    }
    request(CMD_LIST);
    getResponse(); // wait for completed
    readDirectoryList();
    getResponse(); // wait for completed
  }

  void retrieveFile(const String& filename) throw(FTPException) {
    if (verbosity >= DEBUG) {
      fout << MESSAGE("DEBUG: Retrieving file...") << ENDL;
    }
    request(CMD_RETRIEVE, filename);
    getResponse();

    int i = response.indexOf(MESSAGE(" byte"), 4);
    assert(i >= 0, FTPException("Invalid reply"));
    --i; // possible last digit
    assert((response[i] >= '0') && (response[i] <= '9'), FTPException("Invalid reply"));
    while ((response[i] >= '0') && (response[i] <= '9')) { // find first digit
      --i;
    }
    ++i; // back to first digit

    long long numberOfBytes = 0;
    while ((response[i] >= '0') && (response[i] <= '9')) {
      numberOfBytes = (response[i++] - '0') + 10 * numberOfBytes; // TAG: use LongInteger to handle overflow
    }

    request(CMD_LIST);
    readData(numberOfBytes);
    getResponse(); // wait for completed
  }

//const StringLiteral CMD_RETRIEVE = MESSAGE("RETR"); // retrieve file from server
//const StringLiteral CMD_STORE = MESSAGE("STOR"); // store file on server
//const StringLiteral CMD_STORE_UNIQUE = MESSAGE("STOU"); // store unique file on server
//const StringLiteral CMD_APPEND = MESSAGE("APPE"); // append data to file on server
//const StringLiteral CMD_ALLOCATE = MESSAGE("ALLO"); // allocate storage for specified number of bytes
//const StringLiteral CMD_NAME_LIST = MESSAGE("NLST"); // request name list
//const StringLiteral CMD_SITE = MESSAGE("SITE"); // site specific

  void abort() throw(FTPException) {
    if (verbosity >= DEBUG) {
      fout << MESSAGE("DEBUG: Aborting...") << ENDL;
    }
    request(CMD_ABORT);
    getResponse(); // wait for completed
  }

  void requestActiveTransfer() throw(FTPException) {
    if (verbosity >= DEBUG) {
      fout << MESSAGE("DEBUG: Requesting active transfer mode...") << ENDL;
    }
    serverDataConnection = ServerSocket(controlConnection.getLocalAddress(), 0, 1); // address must be IPv4
    serverDataConnection.getName();
    const unsigned char* binaryAddress = serverDataConnection.getLocalAddress().getAddress(); // network byte order!
    unsigned short port = serverDataConnection.getLocalPort();
    StringOutputStream endPoint;
    endPoint << static_cast<unsigned int>(binaryAddress[0]) << ','
             << static_cast<unsigned int>(binaryAddress[1]) << ','
             << static_cast<unsigned int>(binaryAddress[2]) << ','
             << static_cast<unsigned int>(binaryAddress[3]) << ','
             << static_cast<unsigned int>(port >> 8) << ',' << static_cast<unsigned int>(port & 0xff) << FLUSH;
    request(CMD_DATA_PORT, endPoint.getString());
    getResponse();

    request(CMD_LIST);
    dataConnection = serverDataConnection.accept();
    fout << MESSAGE("Incoming connection from: ") << dataConnection.getAddress() << ENDL;
    getResponse(); // wait for completed
  }

  void waitForConnection() throw(FTPException) {
    dataConnection = serverDataConnection.accept();
    fout << MESSAGE("Incoming connection from: ") << dataConnection.getAddress() << ENDL;
  }

  void requestPassiveTransfer() throw(FTPException) {
    if (verbosity >= DEBUG) {
      fout << MESSAGE("DEBUG: Requesting passive transfer mode...") << ENDL;
    }
    request(CMD_PASSIVE);
    getResponse();

    int i;
    for (i = 4; (i < response.getLength()) && ((response[i] < '0') || (response[i] > '9')); ++i); // find first digit of addr
    assert(i < response.getLength(), FTPException("Invalid reply"));

    String address(15);
    for (unsigned int number = 0; number < 4; ++number) { // read address
      String result = clipPassiveResponse(response, i);
      assert(response[i++] == ',', FTPException("Invalid reply"));  // skip ','
      address += result;
      if (number < 3) {
        address += '.';
      }
    }

    String portHigh = clipPassiveResponse(response, i);
    assert(response[i++] == ',', FTPException("Invalid reply"));  // skip ','
    String portLow = clipPassiveResponse(response, i);
    unsigned short port = UnsignedInteger(portHigh).getValue() * 256 + UnsignedInteger(portLow).getValue(); // TAG: make UnsignedInteger class and use this here

    if (verbosity >= DEBUG) {
      fout << MESSAGE("DEBUG: Establishing data connection to: ")
           << InetEndPoint(address, port) << ENDL;
    }

    InetEndPoint endPoint(address, port);
    dataConnection = StreamSocket();
    dataConnection.connect(endPoint.getAddress(), endPoint.getPort());
  }

  ~FileTransferProtocolClient() {
    if (verbosity >= DEBUG) {
      fout << MESSAGE("DEBUG: Closing sockets...") << ENDL;
    }
    dataConnection.close();
    controlConnection.close();
  }
};



class MyThread : public Runnable {
private:

  FileTransferProtocolClient& client;
public:

  MyThread(FileTransferProtocolClient& _client) : client(_client) {
  }

  void run() throw(FTPException) {
    while (true) {
      client.waitForConnection();
    }
  }
};



void ftpclient(const String& resource, const String& file) {
  Url url(resource);

  if (url.getScheme().isEmpty()) {
    url.setScheme("ftp");
  }
  if (url.getUser().isEmpty()) {
    url.setUser("anonymous");
  }
  if (url.getPassword().isEmpty()) {
    url.setPassword("just@testing.my.app");
  }
  if (url.getPort().isEmpty()) {
    url.setPort("21");
  }

  fout << MESSAGE("Individual parts of the specified url:") << EOL
       << MESSAGE("  scheme: ") << url.getScheme() << EOL
       << MESSAGE("  user: ") << url.getUser() << EOL
       << MESSAGE("  password: ") << url.getPassword() << EOL
       << MESSAGE("  host: ") << url.getHost() << EOL
       << MESSAGE("  port: ") << url.getPort() << EOL
       << MESSAGE("  path: ") << url.getPath() << ENDL;

  if (url.getScheme() != "ftp") {
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

  InetEndPoint endPoint(address, url.getPort());

  FileTransferProtocolClient client(endPoint);
  client.connect();
  client.login(url.getUser(), url.getPassword());
  if (url.getPath().endsWith("/")) {
    client.changeDirectory(url.getPath());
  }
  client.getSystemType();
  client.getCurrentDirectory();
  client.setType(FileTransferProtocolClient::BINARY);
  client.setMode(FileTransferProtocolClient::STREAM);
  client.requestPassiveTransfer();
  client.getDirectoryList();
  client.makeDirectory("fonseca");
//  client.requestActiveTransfer();
//  client.getDirectoryList();
//  client.abort();
  if (url.getPath().isProper()) {
    client.setMode(FileTransferProtocolClient::BLOCK);
    client.requestPassiveTransfer();
    client.retrieveFile(url.getPath());
  }
  client.logout();
}

class FTPApplication : public Application {
private:

  static const unsigned int MAJOR_VERSION = 1;
  static const unsigned int MINOR_VERSION = 0;
public:
  
  FTPApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw()
    : Application(MESSAGE("ftp"), numberOfArguments, arguments, environment) {
  }

  void main() throw() {
    fout << getFormalName() << MESSAGE(" version ") << MAJOR_VERSION << '.' << MINOR_VERSION << EOL
         << MESSAGE("The Base Framework (Test Suite)") << EOL
         << MESSAGE("http://www.mip.sdu.dk/~fonseca/base") << EOL
         << MESSAGE("Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>") << EOL
         << ENDL;
    
    Array<String> arguments = getArguments();
    
    String url = MESSAGE("ftp.sunsite.auc.dk"); // default url
    String file = MESSAGE("welcome.msg"); // default file
    
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
      fout << MESSAGE("Usage: ") << getFormalName() << MESSAGE(" [url] [output]") << ENDL;
      return;
    }
    
    try {
      fout << MESSAGE("Testing File Transfer Protocol (FTP) class...") << ENDL;
      ftpclient(url, file);
    } catch (Exception& e) {
      setExitCode(EXIT_CODE_ERROR);
    }
  }
};

STUB(FTPApplication);
