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

  uint32 status = 0;
  String statusText;

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  HINTERNET hInternet = nullptr;
  HINTERNET hConnect = nullptr;
  HINTERNET hRequest = nullptr; // HttpOpenRequest
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  CFHTTPMessageRef cfHttpReq = nullptr;
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
  
    CFStringRef s;
  public:
    
    MACString(const String& _s) {
      s = CFStringCreateWithCString(kCFAllocatorDefault, _s.native(), kCFStringEncodingUTF8);
    }
    
    inline operator CFStringRef() {
      return s;
    }

    static String getString(CFStringRef _text)
    {
      String result;
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
    
    ~MACString() {
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

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  Url url(_url);
  const String scheme = url.getScheme();
  unsigned int port = 0;
  if (scheme == "https") {
    port = INTERNET_DEFAULT_HTTPS_PORT;
  } else if (scheme == "http") {
    port = INTERNET_DEFAULT_HTTP_PORT;
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

  HINTERNET hInternet = InternetOpenW(L"Mozilla/5.0" /*toWide(agent).c_str()*/, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
  if (!hInternet) {
    throw HTTPException("Failed to open HTTP request.");
  }

  HINTERNET hConnect = InternetConnectW(
    hInternet,
    toWide(url.getHost()).c_str(),
    port,
    _user ? toWide(_user).c_str() : NULL,
    _password ? toWide(_password).c_str() : NULL,
    INTERNET_SERVICE_HTTP,
    0,
    NULL
  );
  if (!hConnect) {
    InternetCloseHandle(hInternet);
    throw HTTPException("Failed to open HTTP request.");
  }

  HINTERNET hRequest = HttpOpenRequestW(
    hConnect,
    toWide(_method).c_str(),
    toWide(url.getPath()).c_str() /*object*/,
    L"HTTP/1.1",
    NULL /*referrer*/,
    NULL /*acceptTypes*/,
    INTERNET_FLAG_RELOAD /*flags*/, // TAG: we can allow cache as option
    NULL
  );
  if (!hRequest) {
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
    throw HTTPException("Failed to open HTTP request.");
  }
  
  Reference<HTTPRequestHandle> handle = new HTTPRequestHandle();
  handle->hInternet = hInternet;
  handle->hConnect = hConnect;
  handle->hRequest = hRequest;
  this->handle = handle;

#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  MACString url(_url.native());
  CFURLRef cfUrl = CFURLCreateWithString(kCFAllocatorDefault, url, NULL);
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
  
  if (_user || _password) {
    // TAG: impl
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
  MACString headerFieldName(name.native());
  MACString headerFieldValue(value.native());
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

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // TAG: what is the difference for variables and body
  BOOL requestSent = HttpSendRequestW(_handle->hRequest, NULL, 0, const_cast<char*>(_body.native()), _body.getLength());
  if (!requestSent) {
    throw HTTPException("Failed to send HTTP request.");
  }
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  MACString body(_body.native()); // TAG: need support for \0 in body
  CFDataRef bodyData = CFStringCreateExternalRepresentation(kCFAllocatorDefault, body, kCFStringEncodingUTF8, 0);
  CFHTTPMessageSetBody(_handle->cfHttpReq, bodyData);
  CFRelease(bodyData);
  bodyData = nullptr;

#if 0
  CFHTTPMessageSetHeaderFieldValue(request, CFSTR("HOST"), url);
  CFHTTPMessageSetHeaderFieldValue(request, CFSTR("Content-Length"), bytes as string);
  CFHTTPMessageSetHeaderFieldValue(request, CFSTR("Content-Type"), CFSTR("charset=utf-8"));
#endif
#endif
}

unsigned int HTTPSRequest::getStatus()
{
  Reference<HTTPRequestHandle> _handle = handle.cast<HTTPRequestHandle>();
  if (!_handle) {
    throw HTTPException("HTTP request is not open.");
  }

  return _handle->status;

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
  CFReadStreamRef stream = CFReadStreamCreateForHTTPRequest(kCFAllocatorDefault, _handle->cfHttpReq);
  if (CFReadStreamSetProperty(stream, kCFStreamPropertyHTTPShouldAutoredirect, kCFBooleanTrue) == false) {
    // now what
  }
  CFHTTPMessageRef response = (CFHTTPMessageRef)CFReadStreamCopyProperty(stream, kCFStreamPropertyHTTPResponseHeader);
  if (!response) {
    CFRelease(stream);
    stream = nullptr;
    throw HTTPException("Failed to get status.");
  }
  const UInt32 code = CFHTTPMessageGetResponseStatusCode(response);
#pragma clang diagnostic pop
  CFRelease(response);
  response = nullptr;
  CFRelease(stream);
  stream = nullptr;
  return code;
#endif
  
  return 0;
}

String HTTPSRequest::getStatusText()
{
  String result;

  Reference<HTTPRequestHandle> _handle = handle.cast<HTTPRequestHandle>();
  if (!_handle) {
    throw HTTPException("HTTP request is not open.");
  }

  return _handle->statusText;

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
  CFReadStreamRef stream = CFReadStreamCreateForHTTPRequest(kCFAllocatorDefault, _handle->cfHttpReq);
  if (CFReadStreamSetProperty(stream, kCFStreamPropertyHTTPShouldAutoredirect, kCFBooleanTrue) == false) {
    // now what
  }
  CFHTTPMessageRef response = (CFHTTPMessageRef)CFReadStreamCopyProperty(stream, kCFStreamPropertyHTTPResponseHeader);
  if (!response) {
    CFRelease(stream);
    stream = nullptr;
    throw HTTPException("Failed to get status text.");
  }
#pragma clang diagnostic pop
  CFStringRef statusLine = CFHTTPMessageCopyResponseStatusLine(response);
  result = MACString::getString(statusLine);
  CFRelease(statusLine);
  statusLine = nullptr;
  CFRelease(response);
  response = nullptr;
  CFRelease(stream);
  stream = nullptr;
#endif

  return result;
}

String HTTPSRequest::getResponse()
{
  String result;

  Reference<HTTPRequestHandle> _handle = handle.cast<HTTPRequestHandle>();
  if (!_handle) {
    throw HTTPException("HTTP request is not open.");
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)

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

  if (!HttpQueryInfo(_handle->hRequest,
    HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER,
    &word,
    &size,
    NULL)) {
    // not what
  }

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
    result.append(MemorySpan(buffer, bytesRead));
  }

  // DWORD word = 0;
  size = sizeof(word);
  if (!HttpQueryInfo(_handle->hRequest,
                     HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
                     &word,
                     &size,
                     NULL)) {
    // not what
  }
  _handle->status = word;

  if (!HttpQueryInfo(_handle->hRequest,
                     HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER,
                     &word,
                     &size,
                     NULL)) {
    // not what
  }

  size = buffer.size();
  while (true) {
    if (!HttpQueryInfoW(_handle->hRequest,
                       HTTP_QUERY_RAW_HEADERS_CRLF, // TAG: get all relevant data
                       &buffer,
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
    const std::wstring text = reinterpret_cast<const wchar*>(static_cast<const uint8*>(buffer));
    String _text = text;
    break;
  }

  // HTTP_QUERY_CONTENT_LENGTH, HTTP_QUERY_STATUS_TEXT, HTTP_QUERY_RAW_HEADERS, HTTP_QUERY_RAW_HEADERS_CRLF, HTTP_QUERY_VERSION
  size = buffer.size();
  String name("Content-Type");
  while (true) {
    strcpy((char*)static_cast<uint8*>(buffer), name.native());
    if (!HttpQueryInfoA(_handle->hRequest,
      HTTP_QUERY_CUSTOM,
      &buffer,
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
    String text(reinterpret_cast<const char*>(static_cast<const uint8*>(buffer)));
    break;
  }

#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
  CFReadStreamRef stream = CFReadStreamCreateForHTTPRequest(kCFAllocatorDefault, _handle->cfHttpReq);
  if (CFReadStreamSetProperty(stream, kCFStreamPropertyHTTPShouldAutoredirect, kCFBooleanTrue) == false) {
    // now what
  }

  // TAG: need to use output stream
  
  PrimitiveArray<uint8> buffer(16 * 1024); // TAG: add secure clear support
  CFReadStreamOpen(stream);
  while (true) {
    CFIndex bytesRead = CFReadStreamRead(stream, buffer, buffer.size());
    if (bytesRead < 0) {
      CFStreamStatus status = CFReadStreamGetStatus(stream);

      CFReadStreamClose(stream);
      CFRelease(stream);
      stream = nullptr;
      throw IOException("Failed to read response.");
    }
    if (bytesRead == 0) {
      break;
    }
    result.append(MemorySpan(buffer, bytesRead));
  }
  
#if 1
  CFHTTPMessageRef response = (CFHTTPMessageRef)CFReadStreamCopyProperty(stream, kCFStreamPropertyHTTPResponseHeader);
  if (!response) {
  }

#if 0 // TAG: in open use CFHTTPMessageAddAuthentication
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
  
  UInt32 statusCode = CFHTTPMessageGetResponseStatusCode(response);
  CFStringRef statusLine = CFHTTPMessageCopyResponseStatusLine(response);
  String text = MACString::getString(statusLine);
  CFRelease(statusLine);
  statusLine = nullptr;
  // CFHTTPMessageSetBody(response, (CFDataRef)responseBytes);

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
  #pragma clang diagnostic pop

  CFReadStreamClose(stream);
  CFRelease(stream);
  stream = nullptr;
#endif
  
  return result;
}

void HTTPSRequest::getResponse(OutputStream* os)
{
  // TAG: optimize
  if (os) {
    String response = getResponse();
    os->write(reinterpret_cast<const uint8*>(response.native()), response.getLength(), false);
    os->close();
  }
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

#if 0
  wchar responseText[4096];
  DWORD responseTextSize = 4096;
  // TAG: handle ERROR_INSUFFICIENT_BUFFER
  if (!HttpQueryInfo(_handle->hRequest,
                     HTTP_QUERY_STATUS_CODE,
                     &responseText,
                     &responseTextSize,
                     NULL)) {
  }
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
