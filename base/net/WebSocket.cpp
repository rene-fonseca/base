/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/net/WebSocket.h>
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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class WebSocketHandle : public ReferenceCountedObject {
public:

  bool sent = false;
  bool read = false;
  uint32 status = 0;
  String statusText;

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  HINTERNET hInternet = nullptr;
  HINTERNET hConnect = nullptr;
  HINTERNET hRequest = nullptr; // HttpOpenRequest
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  CFHTTPMessageRef cfHttpReq = nullptr;
  CFReadStreamRef stream = nullptr;
  CFHTTPMessageRef response = nullptr;
  uint8 pendingByte = 0;
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

  ~WebSocketHandle()
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

WebSocket::WebSocket()
{
}

bool WebSocket::open(const String& _url, const String& _protocols)
{
  Reference<WebSocketHandle> _handle = handle.cast<WebSocketHandle>();
  if (_handle) {
    throw WebSocketException("WebSocket is already open.");
  }

  Url url(_url);
  const String scheme = url.getScheme();
  unsigned int port = 0;
  if (scheme == "https") {
    port = 443;
  } else if (scheme == "http") {
    port = 80;
  } else {
    throw WebSocketException("Failed to open WebSocket due to unsupported protocol.");
  }

  if (url.getPort()) {
    try {
      const unsigned int port = UnsignedInteger::parse(url.getPort(), UnsignedInteger::DEC);
    } catch (InvalidFormat&) {
      throw WebSocketException("Failed to open WebSocket due to invalid port.");
    }
  }

  const String path = url.getPath() ? url.getPath() : "/";

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  HINTERNET hInternet = InternetOpenW(L"Mozilla/5.0" /*toWide(agent).c_str()*/, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
  if (!hInternet) {
    return false;
    // throw WebSocketException("Failed to open WebSocket.");
  }

  HINTERNET hConnect = InternetConnectW(
    hInternet,
    toWide(url.getHost()).c_str(),
    443, // port,
    NULL,
    NULL,
    INTERNET_SERVICE_HTTP,
    0,
    NULL
  );
  if (!hConnect) {
    InternetCloseHandle(hInternet);
    return false;
    // throw WebSocketException("Failed to open WebSocket.");
  }

  HINTERNET hRequest = HttpOpenRequestW(
    hConnect,
    L"",
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
    // throw WebSocketException("Failed to open WebSocket.");
  }

  Reference<WebSocketHandle> handle = new WebSocketHandle();
  handle->hInternet = hInternet;
  handle->hConnect = hConnect;
  handle->hRequest = hRequest;
  this->handle = handle;

#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  // we set auth explicitly
  MACString url2(url.getUrl());
  CFURLRef cfUrl = CFURLCreateWithString(kCFAllocatorDefault, url2, NULL);
  
  // TAG: need option for version of HTTP
  // kCFHTTPVersion1_0
  // kCFHTTPVersion1_1
  // kCFHTTPVersion2_0
  CFHTTPMessageRef cfHttpReq = CFHTTPMessageCreateRequest(kCFAllocatorDefault, CFSTR(""), cfUrl, kCFHTTPVersion1_1);
  CFRelease(cfUrl);
  cfUrl = nullptr;

  if (!cfHttpReq) {
    return false;
  }
    
  Reference<WebSocketHandle> handle = new WebSocketHandle();
  handle->cfHttpReq = cfHttpReq;
  this->handle = handle;
#endif

  return true;
}

void WebSocket::send(const String& _body)
{
  Reference<WebSocketHandle> _handle = handle.cast<WebSocketHandle>();
  if (!_handle) {
    throw WebSocketException("WebSocket is not open.");
  }

  _handle->sent = true;

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  // TAG: what is the difference for variables and body
  BOOL requestSent = HttpSendRequestW(_handle->hRequest, NULL, 0, const_cast<char*>(_body.native()), _body.getLength());
  if (!requestSent) {
    DWORD error = GetLastError();
    throw WebSocketException("Failed to send WebSocket.");
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

#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  // TAG: for long request body CFReadStreamCreateForStreamedHTTPRequest - use PullInterface
  MACString body(_body);
  CFDataRef bodyData = CFStringCreateExternalRepresentation(kCFAllocatorDefault, body, kCFStringEncodingUTF8, 0);
  CFHTTPMessageSetBody(_handle->cfHttpReq, bodyData);
  CFRelease(bodyData);
  bodyData = nullptr;
  
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
  CFReadStreamRef stream = CFReadStreamCreateForHTTPRequest(kCFAllocatorDefault, _handle->cfHttpReq);
  if (CFReadStreamSetProperty(stream, kCFStreamPropertyHTTPShouldAutoredirect, kCFBooleanTrue) == false) {
    // now what
  }
  if (!stream) {
    throw WebSocketException("Failed to send WebSocket.");
  }
  _handle->stream = stream;

  // read 1 byte to force response to become available
  uint8 buffer = 0;
  CFReadStreamOpen(stream);
  CFIndex bytesRead = CFReadStreamRead(stream, &buffer, 1); // wait for data to become available
  BASSERT(bytesRead == 1);
  _handle->pendingByte = buffer;
  // CFReadStreamClose(stream); // this breaks getResponse()
  
  // kCFStreamPropertyHTTPResponseHeader is not available until all data has been read
  CFHTTPMessageRef response = (CFHTTPMessageRef)CFReadStreamCopyProperty(stream, kCFStreamPropertyHTTPResponseHeader);
  if (!response) {
    CFRelease(stream);
    stream = nullptr;
    throw WebSocketException("Failed to send WebSocket.");
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
#endif
}

WebSocket::ReadyState WebSocket::getReadyState()
{
  Reference<WebSocketHandle> _handle = handle.cast<WebSocketHandle>();
  if (!_handle) {
    throw WebSocketException("WebSocket is not open.");
  }

  if (!_handle->sent) {
    throw WebSocketException("WebSocket has not been sent.");
  }

  return CONNECTING;
}

void WebSocket::close()
{
  if (!handle) {
    return; // ignore
  }
  
  Reference<WebSocketHandle> _handle = handle.cast<WebSocketHandle>();
  if (!_handle) {
    throw WebSocketException("WebSocket is not open.");
  }

  _handle->close();
  handle = nullptr;
}

WebSocket::~WebSocket()
{
  close();
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
