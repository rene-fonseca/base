/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/net/HTTPSRequest.h>
#include <base/net/Url.h>
#include <base/Primitives.h>
#include <base/UnsignedInteger.h>
#include <base/ResourceHandle.h>
#include <base/collection/ArrayMap.h>
#include <base/Profiler.h>
#include <base/UnitTest.h>
#include <base/Module.h>
#include <base/io/MemoryInputStream.h>
#include <base/io/MemoryOutputStream.h>
#include <base/io/EndOfFile.h>
#include <base/build.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <Windows.h>
#  include <WinInet.h>
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
#  include <CoreFoundation/CoreFoundation.h>
#  include <CFNetwork/CFNetwork.h>
#  include <CFNetwork/CFHTTPStream.h>
#else
#if defined(_COM_AZURE_DEV__BASE__USE_CURL)
#  include <curl/curl.h>
#endif
#endif

// see https://developer.apple.com/library/archive/documentation/Networking/Conceptual/CFNetwork/CFHTTPTasks/CFHTTPTasks.html#//apple_ref/doc/uid/TP30001132-CH5-SW2

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

// TAG: add ProfilerInputStream also to capture all info

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
namespace {

  template<typename TYPE>
  class CFHandle {
  private:

    TYPE handle = nullptr;
    
    CFHandle(const CFHandle&) = delete;
    CFHandle& operator=(const CFHandle&) = delete;
    
    inline void release() noexcept
    {
      if (handle) {
        CFRelease(handle);
        handle = nullptr;
      }
    }
  public:
  
    inline CFHandle() noexcept
    {
    }

    inline CFHandle(TYPE _handle) noexcept
      : handle(_handle)
    {
    }

    inline CFHandle(CFHandle&& move) noexcept
      : handle(moveObject(move.handle))
    {
      move.handle = nullptr;
    }

    inline CFHandle& operator=(CFHandle&& move) noexcept
    {
      if (&move != this) {
        release();
        handle = move.handle;
        move.handle = nullptr;
      }
      return *this;
    }

    inline operator TYPE() noexcept
    {
      return handle;
    }
  
    inline ~CFHandle() noexcept
    {
      release();
    }
  };
}
#elif defined(_COM_AZURE_DEV__BASE__USE_CURL)
namespace {

  class CurlInitialization {
  public:

    CurlInitialization()
    {
      curl_global_init(CURL_GLOBAL_DEFAULT);
    }
    
    ~CurlInitialization()
    {
      curl_global_cleanup();
    }
  };
}
#endif

const char* HTTPSRequest::METHOD_GET = "GET";
const char* HTTPSRequest::METHOD_POST = "POST";
const char* HTTPSRequest::METHOD_PUT = "PUT";
const char* HTTPSRequest::METHOD_DELETE = "DELETE";

class HTTPRequestHandle : public ResourceHandle {
public:

  bool sent = false;
  bool read = false;
  uint32 status = 0;
  String statusText;
  uint64 contentLength = 0;

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  HINTERNET hInternet = nullptr;
  HINTERNET hConnect = nullptr;
  HINTERNET hRequest = nullptr; // HttpOpenRequest
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  CFHandle<CFHTTPMessageRef> cfHttpReq;
  CFHandle<CFReadStreamRef> stream;
  CFHandle<CFHTTPMessageRef> response;
  uint8 pendingByte = 0;
#elif defined(_COM_AZURE_DEV__BASE__USE_CURL)
  CURL* curl = nullptr;
  struct curl_slist* requestHeaders = nullptr;
  ArrayMap<String, String> headers;
  Allocator<uint8> response; // TAG: not desired - we need to stream this instead
#endif

#if defined(_COM_AZURE_DEV__BASE__USE_CURL)
  void addHeader(const String& header)
  {
    auto index = header.indexOf(':');
    if (index >= 0) {
      String name = header.substring(0, index);
      ++index; // skip :
      if ((index < header.getLength()) && (header[index] == ' ')) {
        ++index; // skip first space
      }
      headers.add(name, header.substring(index));
    } else {
      headers.add(header, String());
    }
  }
#endif

  void close()
  {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    if (hRequest || hConnect || hInternet) {
      Profiler::HTTPSTask profile("HTTPSRequest::close()");

      if (hRequest) {
        INLINE_ASSERT(InternetCloseHandle(hRequest));
        hRequest = nullptr;
      }
      if (hConnect) {
        INLINE_ASSERT(InternetCloseHandle(hConnect));
        hConnect = nullptr;
      }
      if (hInternet) {
        INLINE_ASSERT(InternetCloseHandle(hInternet));
        hInternet = nullptr;
      }
    }
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
    if (response || stream || cfHttpReq) {
      Profiler::HTTPSTask profile("HTTPSRequest::close()");
      response = nullptr;
      stream = nullptr;
      cfHttpReq = nullptr;
    }
#elif defined(_COM_AZURE_DEV__BASE__USE_CURL)
    if (curl || requestHeaders) {
      Profiler::HTTPSTask profile("HTTPSRequest::close()");

      if (requestHeaders) {
        curl_slist_free_all(requestHeaders);
        requestHeaders = nullptr;
      }

      curl_easy_cleanup(curl);
      curl = nullptr;
    }
#else
    _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
  }

  ~HTTPRequestHandle()
  {
    close();
  }
};

namespace {

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  class MACString {
  private:
  
    CFHandle<CFStringRef> s;
  public:
    
    inline MACString(const String& _s)
    {
      s = CFStringCreateWithBytes(kCFAllocatorDefault, reinterpret_cast<const uint8*>(_s.native()), _s.getLength(), kCFStringEncodingUTF8, false);
      // s = CFStringCreateWithCString(kCFAllocatorDefault, _s.native(), kCFStringEncodingUTF8);
    }
    
    inline MACString(const MemorySpan& _s)
    {
      s = CFStringCreateWithBytes(kCFAllocatorDefault, _s.begin, _s.getSize(), kCFStringEncodingUTF8, false);
    }
    
    inline operator CFStringRef()
    {
      return s;
    }

    static String getString(CFStringRef _text)
    {
      String result;
      if (!_text) {
        return result;
      }
      if (const char* native = CFStringGetCStringPtr(_text, kCFStringEncodingUTF8)) {
        result = native;
        return result;
      }
      PrimitiveStackArray<char> buffer(4096);
      while (buffer.size() < (1024 * 1024)) {
        if (CFStringGetCString(_text, buffer, buffer.size(), kCFStringEncodingUTF8)) {
          result = buffer;
          return result;
        }
        buffer.resize(buffer.size() * 2);
      }
      return result;
    }
  };
#endif
}

HTTPSRequest::HTTPSRequest()
{
}

bool HTTPSRequest::isSupported() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS) || \
    defined(_COM_AZURE_DEV__BASE__USE_CURL)
  return true;
#else
  return false;
#endif
}

// TAG: add support for setting certificate for server

#if defined(_COM_AZURE_DEV__BASE__USE_CURL)
namespace {

  size_t onResponseHeader(char* buffer, size_t size, size_t nitems, void* userdata)
  {
    // fdebug << "onResponseHeader(): buffer=" << buffer << " size=" << size << " items=" << nitems << " context="  << userdata << ENDL;
    if (!INLINE_ASSERT(buffer && (size == 1) && userdata)) {
      return nitems * size;
    }
    HTTPRequestHandle& handle = *reinterpret_cast<HTTPRequestHandle*>(userdata);
    String header;
    if (nitems >= 2) {
      if ((buffer[nitems - 2] == '\r') && (buffer[nitems - 1] == '\n')) {
        header = String(buffer, nitems - 2);
      } else {
        header = String(buffer, nitems);
      }
    } else {
      header = String(buffer, nitems);
    }
    if (header) {
      handle.addHeader(header);
    }
    return nitems * size;
  }

  size_t onReadData(char* buffer, size_t size, size_t nitems, void* userdata)
  {
    // fout << "onReadData(): buffer=" << buffer << " size=" << size << " items=" << nitems << " context="  << userdata << ENDL;
    if (!INLINE_ASSERT(buffer && userdata)) {
      return 0;
    }
    InputStream* is = reinterpret_cast<InputStream*>(userdata);
    if (!is) {
      return 0;
    }
    try {
      size_t bytesRead = is->read(reinterpret_cast<uint8*>(buffer), size * nitems, false);
      return bytesRead;
    } catch (EndOfFile&) {
      return 0;
    }
  }

  size_t onWriteData(char* buffer, size_t size, size_t nitems, void* userdata)
  {
    // fout << "onWriteData(): buffer=" << buffer << " size=" << size << " items=" << nitems << " context="  << userdata << ENDL;
    if (!INLINE_ASSERT(buffer && userdata)) {
      return 0;
    }
    OutputStream* os = reinterpret_cast<OutputStream*>(userdata);
    if (!os) {
      return 0;
    }
    try {
      size_t bytesWritten = os->write(reinterpret_cast<const uint8*>(buffer), size * nitems, false);
      return bytesWritten;
    } catch (EndOfFile&) {
      return 0;
    }
  }
}
#endif

bool HTTPSRequest::open(const String& _method, const String& _url, const String& _user, const String& _password)
{
  Profiler::ResourceCreateTask profile("HTTPSRequest::open()");
  
  Reference<HTTPRequestHandle> _handle = handle.cast<HTTPRequestHandle>();
  if (_handle) {
    _throw HTTPException("HTTP request is already open.");
  }

  Url url(_url);
  const String scheme = url.getScheme();
  unsigned int port = 0;
  if (scheme == "https") {
    port = 443;
  } else if (scheme == "http") {
    port = 80;
  } else {
    _throw HTTPException("Failed to open HTTP request due to unsupported protocol.");
  }

  if (url.getPort()) {
    try {
      port = UnsignedInteger::parse(url.getPort(), UnsignedInteger::DEC);
    } catch (InvalidFormat&) {
      _throw HTTPException("Failed to open HTTP request due to invalid port.");
    }
  }

  const String user = url.getUser() ? url.getUser() : _user;
  const String password = url.getPassword() ? url.getPassword() : _password;
  const String path = url.getPath() ? url.getPath() : "/";

  (void)port; // dont care about unused

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  HINTERNET hInternet = InternetOpenW(L"Mozilla/5.0" /*ToWCharString(agent)*/, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
  if (!hInternet) {
    return false;
    // throw HTTPException("Failed to open HTTP request.");
  }

  HINTERNET hConnect = InternetConnectW(
    hInternet,
    ToWCharString(url.getHost()),
    443, // port,
    user ? ToWCharString(user) : ToWCharString(),
    password ? ToWCharString(password) : ToWCharString(),
    INTERNET_SERVICE_HTTP,
    0,
    NULL
  );
  if (!hConnect) {
    InternetCloseHandle(hInternet);
    return false;
    // throw HTTPException("Failed to open HTTP request.");
  }

  HINTERNET hRequest = HttpOpenRequestW(
    hConnect,
    ToWCharString(_method),
    ToWCharString(path) /*object*/,
    NULL, // L"HTTP/1.1",
    NULL /*referrer*/,
    NULL /*acceptTypes*/,
    ((scheme == "https") ? INTERNET_FLAG_SECURE : 0) | INTERNET_FLAG_RELOAD /*flags*/, // TAG: we can allow cache as option
    NULL
  );
  if (!hRequest) {
    DWORD error = GetLastError();
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
    return false;
    // throw HTTPException("Failed to open HTTP request.");
  }

  Reference<HTTPRequestHandle> handle = new HTTPRequestHandle();
  handle->hInternet = hInternet;
  handle->hConnect = hConnect;
  handle->hRequest = hRequest;
  this->handle = handle;

  String meta = StringOutputStream() << "{METHOD=" << _method << " URL=" << _url << "}" << FLUSH;
  profile.setHandle(*handle, meta);

#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  // we set auth explicitly
  url.setUser(String());
  url.setPassword(String());
  MACString url2(url.getUrl());
  CFHandle<CFURLRef> cfUrl = CFURLCreateWithString(kCFAllocatorDefault, url2, NULL);
  MACString requestMethod(_method);
  
  // TAG: need option for version of HTTP
  // kCFHTTPVersion1_0
  // kCFHTTPVersion1_1
  // kCFHTTPVersion2_0
  CFHTTPMessageRef cfHttpReq = CFHTTPMessageCreateRequest(kCFAllocatorDefault, requestMethod, cfUrl, kCFHTTPVersion1_1);
  cfUrl = nullptr;

  if (!cfHttpReq) {
    return false;
  }
  
  if (user || password) {
    // CFHTTPAuthenticationRef authentication = CFHTTPAuthenticationCreateFromResponse(NULL, response);
    CFStreamError error;
    MACString _user(user);
    MACString _password(password);
    CFHTTPMessageRef msg = NULL;
    Boolean success = CFHTTPMessageAddAuthentication(_handle->cfHttpReq, msg, _user, _password, NULL, FALSE);
    if (!success) {
      // TAG: throw
    }
    BASSERT(success);
  }
  
  Reference<HTTPRequestHandle> handle = new HTTPRequestHandle();
  handle->cfHttpReq = cfHttpReq;
  this->handle = handle;

  String meta = StringOutputStream() << "{METHOD=" << _method << " URL=" << _url << "}" << FLUSH;
  profile.setHandle(*handle, meta);
#elif defined(_COM_AZURE_DEV__BASE__USE_CURL)
  
  static CurlInitialization initialize;

  Reference<HTTPRequestHandle> handle = new HTTPRequestHandle();
  handle->curl = curl_easy_init();
  if (!handle->curl) {
    return false;
  }

  url.setUser(String());
  url.setPassword(String());

  if (curl_easy_setopt(handle->curl, CURLOPT_URL, url.getUrl().native()) != CURLE_OK) {
    return false;
  }

  // verbose mode
  // curl_easy_setopt(handle->curl, CURLOPT_VERBOSE, 1L);

  // CONNECT, DELETE, GET, HEAD, OPTIONS, PATCH, POST, PUT, TRACE
  if (_method == "GET") {
    curl_easy_setopt(handle->curl, CURLOPT_HTTPGET, 1L);
  } else if (_method == "POST") {
    curl_easy_setopt(handle->curl, CURLOPT_POST, 1L);
  } else if (_method == "PUT") {
    curl_easy_setopt(handle->curl, CURLOPT_PUT, 1L);
  } else {
    curl_easy_setopt(handle->curl, CURLOPT_CUSTOMREQUEST, _method.native());
    // TAG: add support for Runtime messages that can be listened too
  }

#if 0
  if (false) {
    if (curl_easy_setopt(handle->curl, CURLOPT_USERAGENT, agent.native()) != CURLE_OK) {
      return false;
    }
  }
#endif
  if (user) {
    if (curl_easy_setopt(handle->curl, CURLOPT_USERNAME, user.native()) != CURLE_OK) {
      return false;
    }
  }
  if (password) {
    if (curl_easy_setopt(handle->curl, CURLOPT_USERPWD, password.native()) != CURLE_OK) {
      return false;
    }
  }
#if 0
  if (curl_easy_setopt(handle->curl, CURLOPT_SSL_VERIFYHOST, 2L) != CURLE_OK) {
    return false;
  }
#endif

  // hook headers
  CURLcode status = curl_easy_setopt(handle->curl, CURLOPT_HEADERFUNCTION, onResponseHeader);
  if (status != CURLE_OK) {
    BASSERT(!"Failed to get response headers.");
  }
  status = curl_easy_setopt(handle->curl, CURLOPT_HEADERDATA, handle.getValue());
  if (status != CURLE_OK) {
    BASSERT(!"Failed to get response headers.");
  }

  this->handle = handle;

  String meta = StringOutputStream() << "{METHOD=" << _method << " URL=" << _url << "}" << FLUSH;
  profile.setHandle(*handle, meta);
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif

  return true;
}

void HTTPSRequest::setRequestHeader(const String& header)
{
  // fout << "HTTPSRequest::setRequestHeader(): " << escape(header) << ENDL;

  Reference<HTTPRequestHandle> _handle = handle.cast<HTTPRequestHandle>();
  if (!_handle) {
    _throw HTTPException("HTTP request is not open.");
  }
  
  if (!header) {
    return;
  }
  if (!header.isASCII()) {
    _throw HTTPException("Header must use ASCII only.");
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // ISO-8859 - 1 characters
  // WideCharToMultiByte() codepage 28591
  // TAG: HTTP_ADDREQ_FLAG_REPLACE
  INLINE_ASSERT(HttpAddRequestHeadersW(_handle->hRequest, ToWCharString(header + "\r\n"), (DWORD)-1, HTTP_ADDREQ_FLAG_ADD));
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  // CFHTTPMessageIsRequest
  MemoryDiff index = header.indexOf(':');
  String name;
  String value;
  if (index >= 0) {
    name = header.substring(0, index);
    value = header.substring(index);
    if (value.startsWith(" ")) {
      value.removeAt(0);
    }
  } else {
    name = header;
  }
  MACString headerFieldName(name);
  MACString headerFieldValue(value);
  CFHTTPMessageSetHeaderFieldValue(_handle->cfHttpReq, headerFieldName, headerFieldValue);
#elif defined(_COM_AZURE_DEV__BASE__USE_CURL)
  // TAG: are we changing all headers - that would not be desired - we just want to add/replace
  _handle->requestHeaders = curl_slist_append(_handle->requestHeaders, header.native());
  if (INLINE_ASSERT(_handle->requestHeaders)) {
    if (!INLINE_ASSERT(curl_easy_setopt(_handle->curl, CURLOPT_HTTPHEADER, _handle->requestHeaders) == CURLE_OK)) {
    }
  }
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

void HTTPSRequest::setRequestHeader(const String& name, const String& value)
{
  // fout << "HTTPSRequest::setRequestHeader(): " << escape(name) << " " << escape(value) << ENDL;

  Reference<HTTPRequestHandle> _handle = handle.cast<HTTPRequestHandle>();
  if (!_handle) {
    _throw HTTPException("HTTP request is not open.");
  }

  if (!name.isASCII() || !value.isASCII()) {
    _throw HTTPException("Header must use ASCII only.");
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // ISO-8859 - 1 characters
  // WideCharToMultiByte() codepage 28591
  // TAG: HTTP_ADDREQ_FLAG_REPLACE
  INLINE_ASSERT(HttpAddRequestHeadersW(_handle->hRequest, ToWCharString(name + ": " + value + "\r\n"), (DWORD)-1, HTTP_ADDREQ_FLAG_ADD));
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  // CFHTTPMessageIsRequest
  MACString headerFieldName(name);
  MACString headerFieldValue(value);
  CFHTTPMessageSetHeaderFieldValue(_handle->cfHttpReq, headerFieldName, headerFieldValue);
#elif defined(_COM_AZURE_DEV__BASE__USE_CURL)
  String entry = name + ": " + value; // headers included in the linked list must not be CRLF-terminated
  // TAG: are we changing all headers - that would not be desired - we just want to add/replace
  _handle->requestHeaders = curl_slist_append(_handle->requestHeaders, entry.native());
  if (INLINE_ASSERT(_handle->requestHeaders)) {
    if (!INLINE_ASSERT(curl_easy_setopt(_handle->curl, CURLOPT_HTTPHEADER, _handle->requestHeaders) == CURLE_OK)) {
    }
  }
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

void HTTPSRequest::send(const String& _body)
{
  Reference<HTTPRequestHandle> _handle = handle.cast<HTTPRequestHandle>();
  if (!_handle) {
    _throw HTTPException("HTTP request is not open.");
  }

  Profiler::IOWriteTask profile("HTTPSRequest::send", reinterpret_cast<const uint8*>(_body.native()));
  profile.onBytesWritten(_body.getLength());

  if (_handle->sent) {
    _throw HTTPException("HTTP request has already been sent.");
  }
  _handle->sent = true;

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // TAG: what is the difference for variables and body
  BOOL requestSent = HttpSendRequestW(_handle->hRequest, NULL, 0, const_cast<char*>(_body.native()), _body.getLength());
  if (!requestSent) {
    DWORD error = GetLastError();
    _throw HTTPException("Failed to send HTTP request.");
  }

  DWORD word = 0;
  DWORD size = sizeof(word);
  if (!HttpQueryInfo(_handle->hRequest,
    HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
    &word,
    &size,
    NULL)) {
    // not what
  }
  _handle->status = word;

  PrimitiveStackArray<char> buffer(4096);
  size = buffer.size();
  while (true) {
    if (!HttpQueryInfoA(_handle->hRequest,
                        HTTP_QUERY_STATUS_TEXT,
                        buffer,
                        &size,
                        NULL)) {
      DWORD error = GetLastError();
      if (error == ERROR_INSUFFICIENT_BUFFER) {
        buffer.resize(buffer.size() * 2);
        size = buffer.size();
        continue;
      }
      break; // not what
    }
    _handle->statusText = String(buffer, size/sizeof(char));
    break;
  }

  word = 0;
  size = sizeof(word);
  if (!HttpQueryInfo(_handle->hRequest,
    HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER,
    &word,
    &size,
    NULL)) {
    // now what
  }
  _handle->contentLength = word;

#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  // TAG: for long request body CFReadStreamCreateForStreamedHTTPRequest - use PullInterface
  MACString body(_body);
  CFHandle<CFDataRef> bodyData = CFStringCreateExternalRepresentation(kCFAllocatorDefault, body, kCFStringEncodingUTF8, 0);
  CFHTTPMessageSetBody(_handle->cfHttpReq, bodyData);
  bodyData = nullptr;

#if 0
  CFHTTPMessageSetHeaderFieldValue(request, CFSTR("HOST"), url);
  CFHTTPMessageSetHeaderFieldValue(request, CFSTR("Content-Length"), bytes as string);
  CFHTTPMessageSetHeaderFieldValue(request, CFSTR("Content-Type"), CFSTR("charset=utf-8"));
#endif
  
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
  _handle->stream = CFReadStreamCreateForHTTPRequest(kCFAllocatorDefault, _handle->cfHttpReq);
  if (CFReadStreamSetProperty(_handle->stream, kCFStreamPropertyHTTPShouldAutoredirect, kCFBooleanTrue) == false) {
    // now what
  }
  if (!_handle->stream) {
    _throw HTTPException("Failed to send HTTP request.");
  }

  // read 1 byte to force response to become available
  uint8 buffer = 0;
  CFReadStreamOpen(_handle->stream);
  CFIndex bytesRead = CFReadStreamRead(_handle->stream, &buffer, 1); // wait for data to become available
  BASSERT(bytesRead == 1);
  _handle->pendingByte = buffer;
  // CFReadStreamClose(stream); // this breaks getResponse()
  
  // kCFStreamPropertyHTTPResponseHeader is not available until all data has been read
  CFHTTPMessageRef response = (CFHTTPMessageRef)CFReadStreamCopyProperty(_handle->stream, kCFStreamPropertyHTTPResponseHeader);
  if (!response) {
    _throw HTTPException("Failed to send HTTP request.");
  }
#pragma clang diagnostic pop

  const CFIndex code = CFHTTPMessageGetResponseStatusCode(response);
  _handle->status = static_cast<unsigned int>(code);
  
  CFHandle<CFStringRef> statusLine = CFHTTPMessageCopyResponseStatusLine(response);
  _handle->statusText = MACString::getString(statusLine);
  statusLine = nullptr;

  _handle->response = response;
  // fout << "Header: " << getResponseHeader() << ENDL;
  try {
    if (String cl = getResponseHeader("Content-Length")) {
      unsigned int contentLength = UnsignedInteger::parse(cl, UnsignedInteger::DEC);
      _handle->contentLength = contentLength;
    }
  } catch (...) {
  }
  
  #if 0
    String username;
    String password;
    if (username || password) {
      CFHTTPAuthenticationRef authentication = CFHTTPAuthenticationCreateFromResponse(NULL, response);
      CFStreamError error;
      MACString _username(username);
      MACString _password(password);
      Boolean success = CFHTTPMessageApplyCredentials(_handle->cfHttpReq, authentication, _username, _password, &error);
      if (!success) {
        // TAG: throw
      }
      BASSERT(success);
    }
  #endif

  #if 0
    unsigned int retries = 0;
    while (((statusCode == 401) || (statusCode == 407)) && (retries < 3)) {
      // new request
      // add authentication
      // get response
      statusCode = CFHTTPMessageGetResponseStatusCode(response);
      ++retries;
    }
  #endif
#elif defined(_COM_AZURE_DEV__BASE__USE_CURL)

  setRequestHeader("Content-Length", format() << _body.getLength());

  MemoryInputStream mis(_body);
  if (!INLINE_ASSERT(curl_easy_setopt(_handle->curl, CURLOPT_READDATA, &mis) == CURLE_OK)) {
    _throw HTTPException("Failed to send HTTP request.");
  }
  if (!INLINE_ASSERT(curl_easy_setopt(_handle->curl, CURLOPT_READFUNCTION, onReadData) == CURLE_OK)) {
    _throw HTTPException("Failed to send HTTP request.");
  }

  MemoryOutputStream mos;
  if (!INLINE_ASSERT(curl_easy_setopt(_handle->curl, CURLOPT_WRITEDATA, &mos) == CURLE_OK)) {
    _throw HTTPException("Failed to send HTTP request.");
  }
  if (!INLINE_ASSERT(curl_easy_setopt(_handle->curl, CURLOPT_WRITEFUNCTION, onWriteData) == CURLE_OK)) {
    _throw HTTPException("Failed to send HTTP request.");
  }

  CURLcode status = curl_easy_perform(_handle->curl);
  if (status != CURLE_OK) {
    ferr << "CURL ERROR: " << curl_easy_strerror(status) << ENDL;
  }

  long responseCode = 0;
  curl_easy_getinfo(_handle->curl, CURLINFO_RESPONSE_CODE, &responseCode);
  _handle->status = responseCode;
  // _handle->statusText = StringOutputStream() << responseCode; // TAG: add support for status text
  curl_off_t contentLength = 0;
  curl_easy_getinfo(_handle->curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &contentLength);
  if (contentLength >= 0) { // Content-Length may not be available
    _handle->contentLength = contentLength;
  }
  mos.swap(_handle->response);
  _handle->contentLength = _handle->response.getSize();
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif
}

unsigned int HTTPSRequest::getStatus()
{
  Reference<HTTPRequestHandle> _handle = handle.cast<HTTPRequestHandle>();
  if (!_handle) {
    _throw HTTPException("HTTP request is not open.");
  }

  if (!_handle->sent) {
    _throw HTTPException("HTTP request has not been sent.");
  }

  return _handle->status;
}

String HTTPSRequest::getStatusText()
{
  Reference<HTTPRequestHandle> _handle = handle.cast<HTTPRequestHandle>();
  if (!_handle) {
    _throw HTTPException("HTTP request is not open.");
  }

  if (!_handle->sent) {
    _throw HTTPException("HTTP request has not been sent.");
  }

  return _handle->statusText;
}

class PushToString : public PushInterface {
public:
  
  String text;
  
  bool pushBegin(uint64 totalSize) override
  {
    text.ensureCapacity(minimum<MemorySize>(totalSize, 4 * 1024 * 1024));
    return true;
  }
  
  MemorySize push(const uint8* buffer, MemorySize size) override
  {
    text.append(ConstSpan<char>(reinterpret_cast<const char*>(buffer), size));
    return size;
  }
  
  void pushEnd() override
  {
  }

  /** Returns the string. */
  inline const String& getString() const noexcept
  {
    return text;
  }
};

class PushToOutputStream : public PushInterface {
public:
  
  OutputStream* os = nullptr;
  
  bool pushBegin(uint64 totalSize) override
  {
    return true;
  }
  
  MemorySize push(const uint8* buffer, MemorySize size) override
  {
    return os->write(buffer, static_cast<unsigned int>(size), false);
  }
  
  void pushEnd() override
  {
    os->close();
  }
};

String HTTPSRequest::getResponse()
{
  String result;
  Profiler::IOReadTask profile("HTTPSRequest::getResponse()");
  PushToString push;
  getResponse(&push);
  result = push.text;
  profile.setBuffer(result.getBytes());
  profile.onBytesRead(result.getLength());
  return result;
}

void HTTPSRequest::getResponse(OutputStream* os)
{
  if (!os) {
    _throw HTTPException("Output stream not set.");
  }

  Profiler::IOReadTask profile("HTTPSRequest::getResponse(OutputStream*)");
  PushToOutputStream push;
  push.os = os;
  getResponse(&push);
  // profile.setBuffer(result.getBytes()));
  // profile.onBytesRead(result.getLength());
}

void HTTPSRequest::getResponse(PushInterface* pi)
{
  Profiler::IOReadTask profile("HTTPSRequest::getResponse(PushInterface*)");

  if (!pi) {
    _throw HTTPException("Output stream not set.");
  }

  Reference<HTTPRequestHandle> _handle = handle.cast<HTTPRequestHandle>();
  if (!_handle) {
    _throw HTTPException("HTTP request is not open.");
  }

  if (!_handle->sent) {
    _throw HTTPException("HTTP request has not been sent.");
  }

  if (_handle->read) {
    _throw HTTPException("HTTP response has already been read.");
  }
  _handle->read = true;
  
  pi->pushBegin(_handle->contentLength);

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)

  PrimitiveArray<uint8> buffer(16 * 1024);
  while (true) {
    DWORD bytesRead = 0;
    BOOL status = InternetReadFile(
      _handle->hRequest,
      buffer,
      buffer.size(),
      &bytesRead
    );
    if (bytesRead == 0) {
      break;
    }
    if (!status) {
      _throw IOException("Failed to read response.");
    }
    // dont really want multiple calls for profiling
    profile.onBytesRead(bytesRead);
    pi->push(static_cast<const uint8*>(buffer), bytesRead);
  }

#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)

  BASSERT(_handle->stream);
  
  PrimitiveArray<uint8> buffer(16 * 1024);
  // CFReadStreamOpen(_handle->stream); // we read in send()
  unsigned int offset = 1;
  buffer[0] = _handle->pendingByte; // read in send()
  while (true) {
    CFIndex bytesRead = CFReadStreamRead(_handle->stream, buffer + offset, buffer.size() - offset);
    if (bytesRead < 0) {
      /*CFStreamStatus status =*/ CFReadStreamGetStatus(_handle->stream);
      CFReadStreamClose(_handle->stream);
      _throw IOException("Failed to read response.");
    }
    const bool done = bytesRead == 0;
    bytesRead += offset;
    offset = 0;
    if (bytesRead == 0) {
      break;
    }
    // dont really want multiple calls for profiling
    profile.onBytesRead(bytesRead);
    pi->push(static_cast<const uint8*>(buffer), bytesRead);
    if (done) {
      break;
    }
  }
  CFReadStreamClose(_handle->stream);
#elif defined(_COM_AZURE_DEV__BASE__USE_CURL)
  profile.onBytesRead(_handle->response.getSize());
  pi->push(_handle->response.getElements(), _handle->response.getSize());
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif

  pi->pushEnd();
}

uint64 HTTPSRequest::getContentLength()
{
  if (!handle) {
    return 0; // ignore
  }

  Reference<HTTPRequestHandle> _handle = handle.cast<HTTPRequestHandle>();
  if (!_handle) {
    _throw HTTPException("HTTP request is not open.");
  }

  if (!_handle->sent) {
    _throw HTTPException("HTTP request has not been sent.");
  }

  return _handle->contentLength;
}

String HTTPSRequest::getResponseHeader(const String& name)
{
  String result;

  if (!handle) {
    return result; // ignore
  }

  Reference<HTTPRequestHandle> _handle = handle.cast<HTTPRequestHandle>();
  if (!_handle) {
    _throw HTTPException("HTTP request is not open.");
  }

  if (!_handle->sent) {
    _throw HTTPException("HTTP request has not been sent.");
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  PrimitiveStackArray<wchar> buffer(maximum<MemorySize>(name.getLength() + 1, 4096/sizeof(wchar)));
  DWORD size = buffer.size() * sizeof(wchar);
  while (size < (1024 * 1024)) {
    Unicode::UTF8ToWChar(buffer, reinterpret_cast<const uint8*>(name.native()), name.getLength() + 1); // with null terminator
    if (!HttpQueryInfoW(_handle->hRequest,
      HTTP_QUERY_CUSTOM,
      buffer,
      &size, // bytes not chars
      NULL)) {
      DWORD error = GetLastError();
      if (error == ERROR_INSUFFICIENT_BUFFER) {
        buffer.resize(buffer.size() * 2);
        size = buffer.size() * sizeof(wchar);
        continue;
      }
      break; // not what
    }
    result = String(buffer, size/sizeof(wchar));
    break;
  }

#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  BASSERT(_handle->response);
  MACString _name(name);
  CFHandle<CFStringRef> _text = CFHTTPMessageCopyHeaderFieldValue(_handle->response, _name);
  result = MACString::getString(_text);
#elif defined(_COM_AZURE_DEV__BASE__USE_CURL)
  if (auto node = _handle->headers.find(name)) {
    return node->getValue();
  }
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif

  return result;
}

String HTTPSRequest::getResponseHeader()
{
  String result;

  if (!handle) {
    return result; // ignore
  }

  Reference<HTTPRequestHandle> _handle = handle.cast<HTTPRequestHandle>();
  if (!_handle) {
    _throw HTTPException("HTTP request is not open.");
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  PrimitiveStackArray<wchar> buffer(4096/sizeof(wchar));
  DWORD size = buffer.size() * sizeof(wchar);
  while (size < (1024 * 1024)) {
    if (!HttpQueryInfoW(_handle->hRequest,
                        HTTP_QUERY_RAW_HEADERS_CRLF,
                        buffer,
                        &size, // size in bytes not chars
                        NULL)) {
      DWORD error = GetLastError();
      if (error == ERROR_INSUFFICIENT_BUFFER) {
        buffer.resize(buffer.size() * 2);
        size = buffer.size() * sizeof(wchar);
        continue;
      }
      _throw HTTPException("Failed to read HTTP response header.");
    }

    result = String(buffer, size/sizeof(wchar));
    break;
  }
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  
  CFHandle<CFDataRef> data = CFHTTPMessageCopySerializedMessage(_handle->response);
  CFHandle<CFDictionaryRef> _dict = CFHTTPMessageCopyAllHeaderFields(_handle->response);
  auto count = CFDictionaryGetCount(_dict);
  PrimitiveStackArray<CFTypeRef> keys(count);
  PrimitiveStackArray<CFTypeRef> values(count);
  CFDictionaryGetKeysAndValues(_dict, (const void **)static_cast<CFTypeRef*>(keys), (const void **)static_cast<CFTypeRef*>(values));
  for (MemorySize i = 0; i < count; ++i) {
    // CFTypeRef k = keys[i];
    // CFTypeRef v = values[i];
    const char* key2 = CFStringGetCStringPtr((CFStringRef)keys[i], kCFStringEncodingUTF8); // TAG: need to do this properly
    const char* value2 = CFStringGetCStringPtr((CFStringRef)values[i], kCFStringEncodingUTF8); // TAG: not working
    String key(key2);
    String value(value2 ? value2 : "<BAD>");
    result += key + ": " + value + "\r\n";
  }
#elif defined(_COM_AZURE_DEV__BASE__USE_CURL)
  for (auto nv : _handle->headers) {
    result += nv.getKey();
    result += ": ";
    result += nv.getValue();
    result += "\r\n";
  }
#else
  _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED();
#endif

  return result;
}

void HTTPSRequest::close()
{
  if (!handle) {
    return; // ignore
  }

  Profiler::HTTPSTask profile("HTTPSRequest::close()");

  Reference<HTTPRequestHandle> _handle = handle.cast<HTTPRequestHandle>();
  if (!_handle) {
    _throw HTTPException("HTTP request is not open.");
  }

  _handle->close();
  handle = nullptr;
}

HTTPSRequest::~HTTPSRequest()
{
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(HTTPSRequest) : public UnitTest {
public:

  TEST_PRIORITY(500);
  TEST_PROJECT("base/net");
  TEST_IMPACT(NORMAL);
  TEST_EXTERNAL();

  void run() override
  {
    if (!HTTPSRequest::isSupported()) {
      return;
    }
    String url = "https://www.google.com";
    String response;
    HTTPSRequest request;
    try {
      if (request.open(HTTPSRequest::METHOD_GET, url)) {
        request.send();
        response = request.getResponse();
        request.close();
      }
    } catch (...) {
    }
    TEST_ASSERT(response.startsWith("<!doctype html>"));
  }
};

StringPair HTTPSRequest::load(const String& url, const String& method, const String& body)
{
  HTTPSRequest request;
  request.open(method, url);
  request.send(body);
  String response = request.getResponse();
  String mimetype = request.getResponseHeader("Content-Type");
  return StringPair(response, mimetype);
}

TEST_REGISTER(HTTPSRequest);

#endif

#if (_COM_AZURE_DEV__BASE__FLAVOR != _COM_AZURE_DEV__BASE__WIN32) && \
    (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__MACOS) && \
    defined(_COM_AZURE_DEV__BASE__USE_CURL)
// can we convert CURLVERSION_NOW to version literal
MODULE_REGISTER_EXPLICIT( \
  _COM_AZURE_DEV__BASE__THIS_MODULE, "se.haxx.curl", "libcurl", \
  "", \
  "https://curl.haxx.se/libcurl/c/" \
);
#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
