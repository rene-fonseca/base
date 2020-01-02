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
#  include <websocket.h>
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
#  include <CoreFoundation/CoreFoundation.h>
#  include <CFNetwork/CFNetwork.h>
#  include <CFNetwork/CFHTTPStream.h>
#else
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if 0 && (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  pragma comment(lib, "websocket.lib") // TAG: use runtime linking instead
#endif

class WebSocketHandle : public ReferenceCountedObject {
public:

  bool sent = false;
  bool read = false;
  uint32 status = 0;
  String statusText;

#if 0 && (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  WEB_SOCKET_HANDLE handle;
  HINTERNET hInternet = nullptr;
  HINTERNET hConnect = nullptr;
  HINTERNET hRequest = nullptr; // HttpOpenRequest
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  CFHTTPMessageRef cfHttpReq = nullptr;
  CFReadStreamRef stream = nullptr;
  CFHTTPMessageRef response = nullptr;
  uint8 pendingByte = 0;
#endif

  WebSocketHandle()
  {
#if 0 && (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    clear(handle);
#endif
  }

  void close()
  {
#if 0 && (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    WebSocketDeleteHandle(handle);
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
    
    inline ~MACString()
    {
      CFRelease(s);
      s = nullptr;
    }
  };
#endif
}

bool WebSocket::isSupported()
{
#if 0 && (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return true;
#else
  return false;
#endif
}


WebSocket::WebSocket()
{
}

bool WebSocket::open(const String& _url, const String& _protocols)
{
  Reference<WebSocketHandle> _handle = handle.cast<WebSocketHandle>();
  if (_handle) {
    _throw WebSocketException("WebSocket is already open.");
  }

  Url url(_url);
  const String scheme = url.getScheme();
  unsigned int port = 0;
  if (scheme == "wss") {
  } else if (scheme == "ws") {
    } else {
    _throw WebSocketException("Failed to open WebSocket due to unsupported protocol.");
  }

  if (url.getPort()) {
    try {
      const unsigned int port = UnsignedInteger::parse(url.getPort(), UnsignedInteger::DEC);
    } catch (InvalidFormat&) {
      _throw WebSocketException("Failed to open WebSocket due to invalid port.");
    }
  }

  const String path = url.getPath() ? url.getPath() : "/";

#if 0 && (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)

  // readyState = CONNECTING;

  _handle = new WebSocketHandle();
  // WEB_SOCKET_PROPERTY properties[];
  HRESULT result = WebSocketCreateClientHandle(NULL /*&properties*/, 0, &(_handle->handle));
  if (!SUCCEEDED(result)) {
    return false;
    // throw WebSocketException("Failed to open WebSocket.");
  }

  // TAG: handshake

  this->handle = handle;

  // readyState = CONNECTED;

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
    _throw WebSocketException("WebSocket is not open.");
  }

  _handle->sent = true;

#if 0 && (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)

  // TAG: open PerformHandshake

  WEB_SOCKET_BUFFER buffer;
  buffer.Data.pbBuffer = reinterpret_cast<PBYTE>(const_cast<char*>(_body.native()));
  buffer.Data.ulBufferLength = _body.getLength();

  HRESULT result = WebSocketSend(_handle->handle, WEB_SOCKET_UTF8_MESSAGE_BUFFER_TYPE, &buffer, _handle.getValue());
  if (SUCCEEDED(result)) {
    DWORD error = GetLastError();
    _throw WebSocketException("Failed to send WebSocket.");
  }

  MemorySize bytesTransferred = 0;
  WEB_SOCKET_BUFFER buffers[2] = { 0 };
  ULONG bufferCount = getArraySize(buffers);
  WEB_SOCKET_ACTION action = WEB_SOCKET_NO_ACTION;
  WEB_SOCKET_BUFFER_TYPE bufferType = WEB_SOCKET_UTF8_MESSAGE_BUFFER_TYPE;
  void* applicationContext = nullptr;
  void* actionContext = nullptr;

  while (true) {
    HRESULT result = WebSocketGetAction(
      _handle->handle,
      WEB_SOCKET_ALL_ACTION_QUEUE,
      buffers,
      &bufferCount,
      &action,
      &bufferType,
      &applicationContext,
      &actionContext
    );
    if (!SUCCEEDED(result)) {
      WebSocketAbortHandle(_handle->handle);
    }

    bool failed = false;
    switch (action) {
    case WEB_SOCKET_NO_ACTION:
      break;
    case WEB_SOCKET_RECEIVE_FROM_NETWORK_ACTION:
      {
        BASSERT(bufferCount > 0);
        for (MemorySize i = 0; i < bufferCount; ++i) {
          // buffers[i].Data.pbBuffer, buffers[i].Data.ulBufferLength
          // TAG: read from InputStream*
          bytesTransferred += buffers[i].Data.ulBufferLength; // TAG: would be partial
        }
      }
      break;
    case WEB_SOCKET_INDICATE_RECEIVE_COMPLETE_ACTION:
      if (INLINE_ASSERT(bufferCount == 1)) {
      }
      break;
    case WEB_SOCKET_SEND_TO_NETWORK_ACTION:
      {
        BASSERT(bufferCount > 0);
        for (MemorySize i = 0; i < bufferCount; ++i) {
          // buffers[i].Data.pbBuffer, buffers[i].Data.ulBufferLength
          // TAG: write to OutputStream*
        }
      }
      break;
    case WEB_SOCKET_INDICATE_SEND_COMPLETE_ACTION:
      break;
    default:
      BASSERT(!"Unexpected action.");
      failed = true;
    }
    
    if (action == WEB_SOCKET_NO_ACTION) {
      break;
    }

    if (failed) {
      WebSocketAbortHandle(_handle->handle);
      break;
    }

    WebSocketCompleteAction(_handle->handle, actionContext, bytesTransferred);
  }

  // HRESULT result = WebSocketSend(_handle->handle, WEB_SOCKET_BINARY_MESSAGE_BUFFER_TYPE, WEB_SOCKET_BUFFER * pBuffer, _handle.getValue());
  // HRESULT result = WebSocketSend(_handle->handle, WEB_SOCKET_PING_PONG_BUFFER_TYPE, NULL, _handle.getValue());
  // HRESULT result = WebSocketReceive(_handle->handle, WEB_SOCKET_BUFFER* pBuffer, _handle.getValue());

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
    _throw WebSocketException("Failed to send WebSocket.");
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
    _throw WebSocketException("Failed to send WebSocket.");
  }
#pragma clang diagnostic pop

  const CFIndex code = CFHTTPMessageGetResponseStatusCode(response);
  _handle->status = static_cast<unsigned int>(code);
  
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
    _throw WebSocketException("WebSocket is not open.");
  }

  if (!_handle->sent) {
    _throw WebSocketException("WebSocket has not been sent.");
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
    _throw WebSocketException("WebSocket is not open.");
  }

  _handle->close();
  handle = nullptr;
}

WebSocket::~WebSocket()
{
  close();
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
