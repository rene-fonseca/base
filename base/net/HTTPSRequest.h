/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Object.h>
#include <base/string/String.h>
#include <base/net/NetworkException.h>
#include <base/io/OutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

// TAG: move to proper place
class _COM_AZURE_DEV__BASE__API PushInterface {
public:

  virtual bool pushBegin(uint64 totalSize) = 0;
  virtual MemorySize push(const uint8* buffer, MemorySize size) = 0;
  virtual void pushEnd() = 0;
};

class _COM_AZURE_DEV__BASE__API HTTPException : public NetworkException {
public:

  /**
    Initializes the exception object with no message.
  */
  inline HTTPException() noexcept
  {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline HTTPException(const char* message) noexcept : NetworkException(message) {
  }

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline HTTPException(const Type& type) noexcept
    : NetworkException(type)
  {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline HTTPException(const char* message, const Type& type) noexcept
    : NetworkException(message, type)
  {
  }
  
  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

/**
  HTTPS request.
*/
class _COM_AZURE_DEV__BASE__API HTTPSRequest : public Object {
public:

  static const char* METHOD_GET;
  static const char* METHOD_POST;
  static const char* METHOD_PUT;
  static const char* METHOD_DELETE;
private:
  
  /** Request handle. */
  Reference<ReferenceCountedObject> handle;
public:
  
  /**
    Initializes the request.
  */
  HTTPSRequest();
  
  /**
    Opens request.

    @param method The connection method. CONNECT, DELETE, GET, HEAD, OPTIONS, PATCH, POST, PUT, TRACE.
    @param url The connection url.
  */
  bool open(const String& method, const String& url, const String& user = String(), const String& password = String());
 
  /** Sets the request header. Call after open() and before send(). */
  void setRequestHeader(const String& name, const String& value);

  /** Sets the user agent header. Call after open() and before send(). */
  inline void setAgent(const String& agent)
  {
    setRequestHeader("User-Agent", agent);
  }

  /** Sends request. */
  void send(const String& body = String());
  
  void close();
  
  /** Returns the status. */
  unsigned int getStatus();

  /** Returns the status text. */
  String getStatusText();

  /** Returns the content length for the response. */
  uint64 getContentLength();

  /** Returns the value of the given header. */
  String getResponseHeader(const String& name);

  /** Returns the entire response header. */
  String getResponseHeader();

  /** Returns the response. */
  String getResponse();

  /** Returns the response. */
  void getResponse(OutputStream* os);

  /** Returns the response. */
  void getResponse(PushInterface* pi);

  ~HTTPSRequest();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
