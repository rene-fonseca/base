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

#include <base/rmi/OrbScheme.h>
#include <base/collection/HashTable.h>
#include <base/concurrency/Thread.h>
#include <base/concurrency/SpinLock.h>
#include <base/mem/ProtectedPointer.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class LocalConnection;

/**
  Process local object request broker scheme.
  
  @short Process local object request broker scheme.
  @ingroup rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API LocalScheme : public OrbScheme {
private:

  typedef SpinLock Guard;
  /** Guard. */
  Guard guard;
  /** The current connections. */
  HashTable<String, Reference<LocalConnection> > connections;
public:
  
  /**
    Initializes the scheme.
  */
  LocalScheme() noexcept;

  /**
    Returns the global identifier of the scheme (e.g.
    "urn:rmi:orb:scheme:tcpip").
  */
  String getUrn() const noexcept;
  
  /**
    Returns the scheme identifying the scheme within the application (e.g.
    "local").
  */
  String getScheme() const noexcept;
  
  /**
    Returns the current number of connections.
  */
  inline unsigned int getNumberOfConnections() const throw() {
    return connections.getSize();
  }
  
  /**
    Creates a new factory for the specified local end point.
    
    @param identifier The identifier of the factory end point.
  */
  Reference<OrbConnectionFactory> openFactory(
    const String& identifier) throw(InvalidFormat, OrbException);
  
  /**
    Returns a connection for the specified identifier. New connections
    are established on demand. LocalConnection is assumed if the scheme is
    missing.
  */
  Reference<OrbConnection> open(
    ProtectedPointer<Orb> orb,
    const String& identifier) throw(InvalidFormat, OrbException);
  
  /**
    Adds a connection.
  */
  void add(Reference<OrbConnection> connection) throw(CastException);
  
  /**
    Removes a connection.
  */
  void remove(
    Reference<OrbConnection> connection) throw(CastException, InvalidKey);
  
  /**
    Returns the connection for the specified end point.
  */
  Reference<OrbConnection> getConnection(
    const String& endPoint) throw(InvalidKey);
  
  /**
    Invoked by ORB.
  */
  void run(ProtectedPointer<Orb> orb) throw(OrbException);
  
  /**
    Signals the protocol to be terminated.
  */
  void terminate() noexcept;
  
  /**
    Closes the specified connection.
  */
  void close(Reference<OrbConnection> connection) throw(OrbException);
  
  /**
    Destroys the scheme.
  */
  ~LocalScheme() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
