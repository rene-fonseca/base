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

bool HTTPSRequest::open(const String& _url)
{
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
  MACString requestMethod("GET");
  
  String _body = "BODY";
  MACString body(_body.native());
  CFDataRef bodyData = CFStringCreateExternalRepresentation(kCFAllocatorDefault, body, kCFStringEncodingUTF8, 0);
  
  // kCFHTTPVersion1_0
  // kCFHTTPVersion1_1
  // kCFHTTPVersion2_0
  CFHTTPMessageRef cfHttpReq = CFHTTPMessageCreateRequest(kCFAllocatorDefault, requestMethod, cfUrl, kCFHTTPVersion1_1);
  // CFHTTPMessageIsRequest
  MACString headerFieldName("X-My-Favorite-Field");
  MACString headerFieldValue("Dreams");
  CFHTTPMessageSetHeaderFieldValue(cfHttpReq, headerFieldName, headerFieldValue);

  CFHTTPMessageSetBody(cfHttpReq, bodyData);

  CFReadStreamRef stream = nullptr;
#if 0
  CFReadStreamRef stream = CFReadStreamCreateForHTTPRequest(kCFAllocatorDefault, cfHttpReq);
  if (CFReadStreamSetProperty(stream, kCFStreamPropertyHTTPShouldAutoredirect, kCFBooleanTrue) == false) {
    // something went wrong, exit
  }
  
  CFHTTPMessageRef response = (CFHTTPMessageRef)CFReadStreamCopyProperty(stream, kCFStreamPropertyHTTPResponseHeader);
  // CFIndex CFHTTPMessageGetResponseStatusCode(CFHTTPMessageRef response);
#endif
  
  CFReadStreamOpen(stream);
  uint8 buffer[1024];
  while (true) {
    CFIndex bytesRead = CFReadStreamRead(stream, buffer, getArraySize(buffer));
    CFStreamStatus status = CFReadStreamGetStatus(stream);
    if (bytesRead < 0) {
      // TAG: release
      throw IOException("Failed to read response.");
    }
    if (bytesRead == 0) {
      break;
    }
  }
  CFReadStreamClose(stream);
  
  CFRelease(cfHttpReq);
  cfHttpReq = nullptr;
  CFRelease(cfUrl);
  cfUrl = nullptr;
#endif

  return false;
}

void HTTPSRequest::send(const String& body)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
#endif
}

void HTTPSRequest::close()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
#endif
}

HTTPSRequest::~HTTPSRequest()
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
#endif
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
