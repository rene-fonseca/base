/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/rmi/OrbConnectionFactory.h>
#include <base/rmi/LocalConnection.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Local connection factory which produces LocalConnection connections.
  
  @short Local connection factory.
  @see LocalConnection
  @ingroup rmi
  @version 1.0
*/

class LocalConnectionFactory : public OrbConnectionFactory {
private:

  /** The identifier of the end point. */
  String identifier;
public:
  
  /**
    Initializes the connection factory with the specified end point.
    
    @param endPoint The identifier of the end point (e.g. local://).
  */
  LocalConnectionFactory(
    const String& endPoint) throw(InvalidFormat, OrbException);
  
  /**
    Returns the global identifier of the connection scheme (e.g.
    "urn:rmi:orb:scheme:local").
  */
  String getUrn() const throw();

  /**
    Returns the identifier of the end point.
  */
  String getEndPoint() const throw();
  
  /**
    Returns a stub for the specified protocol independent identifier using this
    protocol.
  */
  Reference<OrbStub> getObject(
    const String& identifier) throw(InvalidFormat, OrbException);
  
  /**
    Invoked by the orb.
  */
  void run(ProtectedPointer<Orb> orb) throw(OrbException);
  
  /**
    Destroys the connection factory.
  */
  ~LocalConnectionFactory() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
