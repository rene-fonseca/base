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

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <Windows.h>
#  include <WinInet.h>
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
#  include <CoreFoundation/CoreFoundation.h>
#  include <CFNetwork/CFNetwork.h>
#  include <CFNetwork/CFHTTPStream.h>
#else
#endif

// see https://developer.apple.com/library/archive/documentation/Networking/Conceptual/CFNetwork/CFHTTPTasks/CFHTTPTasks.html#//apple_ref/doc/uid/TP30001132-CH5-SW2

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

const char* HTTPSRequest::METHOD_GET = "GET";
const char* HTTPSRequest::METHOD_POST = "POST";
const char* HTTPSRequest::METHOD_PUT = "PUT";
const char* HTTPSRequest::METHOD_DELETE = "DELETE";

class HTTPRequestHandle : public ReferenceCountedObject {
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
  CFHTTPMessageRef cfHttpReq = nullptr;
  CFReadStreamRef stream = nullptr;
  CFHTTPMessageRef response = nullptr;
#endif

  void close()
  {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
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
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
    if (response) {
      CFRelease(response);
      response = nullptr;
    }
    if (stream) {
      CFRelease(stream);
      stream = nullptr;
    }
    if (cfHttpReq) {
      CFRelease(cfHttpReq);
      cfHttpReq = nullptr;
    }
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
  
    CFStringRef s = nullptr;
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
      PrimitiveArray<char> buffer(4096);
      while (buffer.size() < (1024 * 1024)) {
        if (CFStringGetCString(_text, buffer, buffer.size(), kCFStringEncodingUTF8)) {
          result = buffer;
          return result;
        }
        buffer.resize(buffer.size() * 2);
      }
      return result;
    }
    
    inline ~MACString()
    {
      CFRelease(s);
      s = nullptr;
    }
  };
#endif
}

HTTPSRequest::HTTPSRequest()
{
}

bool HTTPSRequest::open(const String& _method, const String& _url, const String& _user, const String& _password)
{
  Reference<HTTPRequestHandle> _handle = handle.cast<HTTPRequestHandle>();
  if (_handle) {
    throw HTTPException("HTTP request is already open.");
  }

  Url url(_url);
  const String scheme = url.getScheme();
  unsigned int port = 0;
  if (scheme == "https") {
    port = 443;
  } else if (scheme == "http") {
    port = 80;
  } else {
    throw HTTPException("Failed to open HTTP request due to unsupported protocol.");
  }

  if (url.getPort()) {
    try {
      const unsigned int port = UnsignedInteger::parse(url.getPort(), UnsignedInteger::DEC);
    } catch (InvalidFormat&) {
      throw HTTPException("Failed to open HTTP request due to invalid port.");
    }
  }

  const String user = url.getUser() ? url.getUser() : _user;
  const String password = url.getPassword() ? url.getPassword() : _password;
  const String path = url.getPath() ? url.getPath() : "/";

  // TAG: build url for macOS

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  HINTERNET hInternet = InternetOpenW(L"Mozilla/5.0" /*toWide(agent).c_str()*/, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
  if (!hInternet) {
    return false;
    // throw HTTPException("Failed to open HTTP request.");
  }

  HINTERNET hConnect = InternetConnectW(
    hInternet,
    toWide(url.getHost()).c_str(),
    443, // port,
    user ? toWide(user).c_str() : NULL,
    password ? toWide(password).c_str() : NULL,
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
    toWide(_method).c_str(),
    toWide(path).c_str() /*object*/,
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

#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  // we set auth explicitly
  url.setUser(String());
  url.setPassword(String());
  MACString url2(url.getUrl());
  CFURLRef cfUrl = CFURLCreateWithString(kCFAllocatorDefault, url2, NULL);
  MACString requestMethod(_method);
  
  // TAG: need option for version of HTTP
  // kCFHTTPVersion1_0
  // kCFHTTPVersion1_1
  // kCFHTTPVersion2_0
  CFHTTPMessageRef cfHttpReq = CFHTTPMessageCreateRequest(kCFAllocatorDefault, requestMethod, cfUrl, kCFHTTPVersion1_1);
  CFRelease(cfUrl);
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
#endif

  return true;
}

void HTTPSRequest::setRequestHeader(const String& name, const String& value)
{
  Reference<HTTPRequestHandle> _handle = handle.cast<HTTPRequestHandle>();
  if (!_handle) {
    throw HTTPException("HTTP request is not open.");
  }

  if (!name.isASCII() || !value.isASCII()) {
    throw HTTPException("Header must use ASCII only.");
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // ISO-8859 - 1 characters
  // WideCharToMultiByte() codepage 28591
  // TAG: HTTP_ADDREQ_FLAG_REPLACE
  INLINE_ASSERT(HttpAddRequestHeadersW(_handle->hRequest, toWide(name + ": " + value + "\r\n").c_str(), (DWORD)-1, HTTP_ADDREQ_FLAG_ADD));
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  // CFHTTPMessageIsRequest
  MACString headerFieldName(name);
  MACString headerFieldValue(value);
  CFHTTPMessageSetHeaderFieldValue(_handle->cfHttpReq, headerFieldName, headerFieldValue);
#else
  BASSERT(!"Not implemented");
#endif
}

void HTTPSRequest::send(const String& _body)
{
  Reference<HTTPRequestHandle> _handle = handle.cast<HTTPRequestHandle>();
  if (!_handle) {
    throw HTTPException("HTTP request is not open.");
  }

  _handle->sent = true;

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // TAG: what is the difference for variables and body
  BOOL requestSent = HttpSendRequestW(_handle->hRequest, NULL, 0, const_cast<char*>(_body.native()), _body.getLength());
  if (!requestSent) {
    DWORD error = GetLastError();
    throw HTTPException("Failed to send HTTP request.");
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

  PrimitiveArray<char> buffer(1024);
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
    _handle->statusText = static_cast<const char*>(buffer);
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
  CFDataRef bodyData = CFStringCreateExternalRepresentation(kCFAllocatorDefault, body, kCFStringEncodingUTF8, 0);
  CFHTTPMessageSetBody(_handle->cfHttpReq, bodyData);
  CFRelease(bodyData);
  bodyData = nullptr;

#if 0
  CFHTTPMessageSetHeaderFieldValue(request, CFSTR("HOST"), url);
  CFHTTPMessageSetHeaderFieldValue(request, CFSTR("Content-Length"), bytes as string);
  CFHTTPMessageSetHeaderFieldValue(request, CFSTR("Content-Type"), CFSTR("charset=utf-8"));
#endif
  
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
  CFReadStreamRef stream = CFReadStreamCreateForHTTPRequest(kCFAllocatorDefault, _handle->cfHttpReq);
  if (CFReadStreamSetProperty(stream, kCFStreamPropertyHTTPShouldAutoredirect, kCFBooleanTrue) == false) {
    // now what
  }
  if (!stream) {
    throw HTTPException("Failed to send HTTP request.");
  }
  _handle->stream = stream;
  String temp = getResponse(); // TAG: cannot get header until after read? how do we get content length
  CFHTTPMessageRef response = (CFHTTPMessageRef)CFReadStreamCopyProperty(stream, kCFStreamPropertyHTTPResponseHeader);
  if (!response) {
    CFRelease(stream);
    stream = nullptr;
    throw HTTPException("Failed to send HTTP request.");
  }
#pragma clang diagnostic pop

  const UInt32 code = CFHTTPMessageGetResponseStatusCode(response);
  _handle->status = code;
  
  CFStringRef statusLine = CFHTTPMessageCopyResponseStatusLine(response);
  _handle->statusText = MACString::getString(statusLine);
  if (statusLine) {
    CFRelease(statusLine);
    statusLine = nullptr;
  }

  _handle->response = response;

  try {
    unsigned int contentLength = UnsignedInteger::parse(getResponseHeader("Content-Length"), UnsignedInteger::DEC);
    _handle->contentLength = contentLength;
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

#endif
}

unsigned int HTTPSRequest::getStatus()
{
  Reference<HTTPRequestHandle> _handle = handle.cast<HTTPRequestHandle>();
  if (!_handle) {
    throw HTTPException("HTTP request is not open.");
  }

  if (!_handle->sent) {
    throw HTTPException("HTTP request has not been sent.");
  }

  return _handle->status;
}

String HTTPSRequest::getStatusText()
{
  Reference<HTTPRequestHandle> _handle = handle.cast<HTTPRequestHandle>();
  if (!_handle) {
    throw HTTPException("HTTP request is not open.");
  }

  if (!_handle->sent) {
    throw HTTPException("HTTP request has not been sent.");
  }

  return _handle->statusText;
}

class PushToString : public PushInterface {
public:
  
  String text;
  
  bool pushBegin(uint64 totalSize)
  {
    text.ensureCapacity(minimum<MemorySize>(totalSize, 4 * 1024 * 1024));
    return true;
  }
  
  unsigned int push(const uint8* buffer, unsigned int size) {
    text.append(MemorySpan(buffer, size));
    return size;
  }
  
  void pushEnd() {
  }
};

class PushToOutputStream : public PushInterface {
public:
  
  OutputStream* os = nullptr;
  
  bool pushBegin(uint64 totalSize)
  {
    return true;
  }
  
  unsigned int push(const uint8* buffer, unsigned int size)
  {
    return os->write(buffer, size, false);
  }
  
  void pushEnd()
  {
    os->close();
  }
};

String HTTPSRequest::getResponse()
{
  PushToString push;
  getResponse(&push);
  return push.text;
}

void HTTPSRequest::getResponse(OutputStream* os)
{
  if (!os) {
    throw HTTPException("Output stream not set.");
  }

  PushToOutputStream push;
  push.os = os;
  getResponse(&push);
}

void HTTPSRequest::getResponse(PushInterface* pi)
{
  if (!pi) {
    throw HTTPException("Output stream not set.");
  }

  Reference<HTTPRequestHandle> _handle = handle.cast<HTTPRequestHandle>();
  if (!_handle) {
    throw HTTPException("HTTP request is not open.");
  }

  if (!_handle->sent) {
    throw HTTPException("HTTP request has not been sent.");
  }

  if (_handle->read) {
    throw HTTPException("HTTP response has already been read.");
  }
  _handle->read = true;
  
  pi->pushBegin(_handle->contentLength);

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)

  PrimitiveArray<uint8> buffer(16 * 1024); // TAG: add secure clear support
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
      throw IOException("Failed to read response.");
    }
    pi->push(static_cast<const uint8*>(buffer), bytesRead);
  }

#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)

  BASSERT(_handle->stream);
  
  PrimitiveArray<uint8> buffer(16 * 1024);
  CFReadStreamOpen(_handle->stream);
  while (true) {
    CFIndex bytesRead = CFReadStreamRead(_handle->stream, buffer, buffer.size());
    if (bytesRead < 0) {
      CFStreamStatus status = CFReadStreamGetStatus(_handle->stream);
      CFReadStreamClose(_handle->stream);
      throw IOException("Failed to read response.");
    }
    if (bytesRead == 0) {
      break;
    }
    pi->push(static_cast<const uint8*>(buffer), bytesRead);
  }
  CFReadStreamClose(_handle->stream);

#else
  BASSERT(!"Not implemented");
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
    throw HTTPException("HTTP request is not open.");
  }

  if (!_handle->sent) {
    throw HTTPException("HTTP request has not been sent.");
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
    throw HTTPException("HTTP request is not open.");
  }

  if (!_handle->sent) {
    throw HTTPException("HTTP request has not been sent.");
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  PrimitiveArray<char> buffer(1024);
  DWORD size = buffer.size();
  while (size < (1024 * 1024)) {
    strcpy(static_cast<char*>(buffer), name.native());
    if (!HttpQueryInfoA(_handle->hRequest,
      HTTP_QUERY_CUSTOM,
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
    result = static_cast<const char*>(buffer);
    break;
  }

#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  BASSERT(_handle->response);
  MACString _name(name);
  CFStringRef _text = CFHTTPMessageCopyHeaderFieldValue(_handle->response, _name);
  result = MACString::getString(_text);
  if (_text) {
    CFRelease(_text);
    _text = nullptr;
  }
  
#else
  BASSERT(!"Not implemented");
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
    throw HTTPException("HTTP request is not open.");
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  PrimitiveArray<uint8> buffer(4 * 4096);
  DWORD size = buffer.size();
  while (size < (1024 * 1024)) {
    if (!HttpQueryInfoW(_handle->hRequest,
                        HTTP_QUERY_RAW_HEADERS_CRLF,
                        buffer,
                        &size,
                        NULL)) {
      DWORD error = GetLastError();
      if (error == ERROR_INSUFFICIENT_BUFFER) {
        buffer.resize(buffer.size() * 2);
        size = buffer.size();
        continue;
      }
      throw HTTPException("Failed to read HTTP response header.");
    }
    const std::wstring text = reinterpret_cast<const wchar*>(static_cast<const uint8*>(buffer));
    result = text; // no need to go through wstring here
    break;
  }
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)

  CFDataRef data = CFHTTPMessageCopySerializedMessage(_handle->response);
  CFDictionaryRef _dict = CFHTTPMessageCopyAllHeaderFields(_handle->response);
  auto count = CFDictionaryGetCount(_dict);
  PrimitiveArray<CFTypeRef> keys(count);
  PrimitiveArray<CFTypeRef> values(count);
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
  if (_dict) {
    CFRelease(_dict);
    _dict = nullptr;
  }
  
#else
  BASSERT(!"Not implemented");
#endif

  return result;
}

void HTTPSRequest::close()
{
  if (!handle) {
    return; // ignore
  }
  
  Reference<HTTPRequestHandle> _handle = handle.cast<HTTPRequestHandle>();
  if (!_handle) {
    throw HTTPException("HTTP request is not open.");
  }

  _handle->close();
  handle = nullptr;
}

HTTPSRequest::~HTTPSRequest()
{
  close();
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
