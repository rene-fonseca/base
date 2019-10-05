/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_RMI__ORB_CONNECTION_LISTENER_H
#define _DK_SDU_MIP__BASE_RMI__ORB_CONNECTION_LISTENER_H

#include <base/rmi/OrbException.h>
#include <base/mem/Reference.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class ConnectionFactory;
class Connection;

/**
  ORB connection listener.
  
  @short ORB connection listener.
  @ingroup rmi
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class OrbConnectionListener {
public:

  /**
    Invoked on incoming connection.
    
    @param connectionFactory The connection factory.
    @param connection The ORB connection.
  */
  virtual void onIncomingConnection(
    Reference<ConnectionFactory> connectionFactory,
    Reference<Connection> connection) throw(OrbException) = 0;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
