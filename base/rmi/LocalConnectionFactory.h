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

#include <base/rmi/OrbConnectionFactory.h>
#include <base/rmi/LocalConnection.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Local connection factory which produces LocalConnection connections.
  
  @short Local connection factory.
  @see LocalConnection
  @ingroup rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API LocalConnectionFactory : public OrbConnectionFactory {
private:

  /** The identifier of the end point. */
  String identifier;
public:
  
  /**
    Initializes the connection factory with the specified end point.
    
    @param endPoint The identifier of the end point (e.g. local://).
  */
  LocalConnectionFactory(
    const String& endPoint);
  
  /**
    Returns the global identifier of the connection scheme (e.g.
    "urn:rmi:orb:scheme:local").
  */
  String getUrn() const noexcept;

  /**
    Returns the identifier of the end point.
  */
  String getEndPoint() const noexcept;
  
  /**
    Returns a stub for the specified protocol independent identifier using this
    protocol.
  */
  Reference<OrbStub> getObject(
    const String& identifier);
  
  /**
    Invoked by the orb.
  */
  void run(ProtectedPointer<Orb> orb);
  
  /**
    Destroys the connection factory.
  */
  ~LocalConnectionFactory() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
