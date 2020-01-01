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

class _COM_AZURE_DEV__BASE__API WebSocketException : public NetworkException {
public:

  /**
    Initializes the exception object with no message.
  */
  inline WebSocketException() noexcept
  {
  }

  /**
    Initializes the exception object.

    @param message The message.
  */
  inline WebSocketException(const char* message) noexcept
    : NetworkException(message)
  {
  }

  /**
    Initializes the exception object without an associated message.
    
    @param type The identity of the type.
  */
  inline WebSocketException(const Type& type) noexcept
    : NetworkException(type)
  {
  }
  
  /**
    Initializes the exception object.
    
    @param message An NULL-terminated string (ASCII).
    @param type The identity of the type.
  */
  inline WebSocketException(const char* message, const Type& type) noexcept
    : NetworkException(message, type)
  {
  }
  
  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

/**
  WebSocket. See RFC6455.
*/
class _COM_AZURE_DEV__BASE__API WebSocket : public Object {
private:
  
  /** Request handle. */
  AnyReference handle;
  
  // TAG: Listener* listeners = nullptr;
  // String url;
  
  virtual void onOpen() {}
  
  virtual void onClose() {}
  
  virtual void onError() {}
  
  virtual void onMessage(const String& data)
  {
    BASSERT(!"HERE");
  }
public:
  
  /** Returns true if WebSocket is supported. */
  bool isSupported();

  enum ReadyState {
    CONNECTING = 0, // Socket has been created. The connection is not yet open.
    OPEN = 1, // The connection is open and ready to communicate.
    CLOSING = 2, // The connection is in the process of closing.
    CLOSED = 3 // The connection is closed or couldn't be opened.
  };
  
  class Event : public Object {
  public:
    
    String data;
  };
  
  // void addEventListener();
  
  /**
    Initializes the request.
  */
  WebSocket();

  ReadyState getReadyState();

  /**
    Opens socket.

    @param url The connection url.
  */
  bool open(const String& url, const String& protocols = String());

  /** Sends request. */
  void send(const String& body = String());
  
  // void send(const Reference<ObjectModel::Object>);

  void close();
  
  ~WebSocket();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
