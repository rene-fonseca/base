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

#include <base/net/StreamSocket.h>
#include <base/io/InputStream.h>
#include <base/io/OutputStream.h>
#include <base/Resource.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  SSL Socket.
*/

class SSLSocket : public Resource {
private:

  StreamSocket socket;
public:

  SSLSocket();

  /**
    Creates secure socket.
  */
  void createSecure(StreamSocket socket, const String& pem);

  void connect(const InetAddress& address, unsigned short port);

  /**
    Fills the buffer with bytes from the socket input stream. Blocks if asked
    to read more bytes than available. Raises EndOfFile if end of stream has
    been reached.
    
    @param buffer The buffer.
    @param size The size of the buffer.
    @param nonblocking Select nonblocking mode.
    @return The actual number of bytes read.
  */
  unsigned int read(
    uint8* buffer,
    unsigned int size,
    bool nonblocking = false);

  /**
    Writes bytes in buffer to stream.

    @param buffer The buffer containing the bytes to be written.
    @param size The number of bytes to be written.
    @param nonblocking Select nonblocking mode.
    @return The actual number of bytes written.
  */
  unsigned int write(
    const uint8* buffer,
    unsigned int size,
    bool nonblocking = false);

  ~SSLSocket();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
