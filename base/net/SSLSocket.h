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

  // void connnect(StreamSocket socket);
  
  ~SSLSocket();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
