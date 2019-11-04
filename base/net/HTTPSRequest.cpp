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
#include <base/Primitives.h>

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

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  CFHTTPMessageRef cfHttpReq = nullptr;
#endif
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

#if 0 && (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  String agent;
  HINTERNET hInternet = InternetOpenW(toOS(agent), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
  HINTERNET hConnect = InternetConnectW(hInternet, url, 80, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
  HINTERNET hRequest = HttpOpenRequestW(hConnect, L"GET", L"", NULL, NULL, acceptTypes, 0, 0);
  BOOL bRequestSent = HttpSendRequestW(hRequest, NULL, 0, NULL, 0);
  bKeepReading = InternetReadFile(hRequest, buff, nBuffSize, &dwBytesRead);
#endif
  
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
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

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  // CFHTTPMessageIsRequest
  MACString headerFieldName(name.native());
  MACString headerFieldValue(value.native());
  CFHTTPMessageSetHeaderFieldValue(_handle->cfHttpReq, headerFieldName, headerFieldValue);
#endif
}

void HTTPSRequest::send(const String& _body)
{
  Reference<HTTPRequestHandle> _handle = handle.cast<HTTPRequestHandle>();
  if (!_handle) {
    throw HTTPException("HTTP request is not open.");
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
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

// TAG: set agent

unsigned int HTTPSRequest::getStatus()
{
  Reference<HTTPRequestHandle> _handle = handle.cast<HTTPRequestHandle>();
  if (!_handle) {
    throw HTTPException("HTTP request is not open.");
  }


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
#endif
  
  return code;
}

String HTTPSRequest::getStatusText()
{
  String result;

  Reference<HTTPRequestHandle> _handle = handle.cast<HTTPRequestHandle>();
  if (!_handle) {
    throw HTTPException("HTTP request is not open.");
  }

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

void HTTPSRequest::close()
{
  if (!handle) {
    return; // ignore
  }
  
  Reference<HTTPRequestHandle> _handle = handle.cast<HTTPRequestHandle>();
  if (!_handle) {
    throw HTTPException("HTTP request is not open.");
  }

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  CFRelease(_handle->cfHttpReq);
  _handle->cfHttpReq = nullptr;
  handle = nullptr;
#endif
}

HTTPSRequest::~HTTPSRequest()
{
  close();
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
