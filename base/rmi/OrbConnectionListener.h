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

#include <base/rmi/OrbException.h>
#include <base/mem/Reference.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class ConnectionFactory;
class Connection;

/**
  ORB connection listener.
  
  @short ORB connection listener.
  @ingroup rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API OrbConnectionListener {
public:

  /**
    Invoked on incoming connection.
    
    @param connectionFactory The connection factory.
    @param connection The ORB connection.
  */
  virtual void onIncomingConnection(
    Reference<ConnectionFactory> connectionFactory,
    Reference<Connection> connection) = 0;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
