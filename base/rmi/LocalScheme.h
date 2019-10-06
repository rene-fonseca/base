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

#include <base/rmi/OrbScheme.h>
#include <base/collection/HashTable.h>
#include <base/concurrency/Thread.h>
#include <base/concurrency/SpinLock.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class LocalConnection;

/**
  Process local object request broker scheme.
  
  @short Process local object request broker scheme.
  @ingroup rmi
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class LocalScheme : public OrbScheme {
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
  LocalScheme() throw();

  /**
    Returns the global identifier of the scheme (e.g.
    "urn:rmi:orb:scheme:tcpip").
  */
  String getUrn() const throw();
  
  /**
    Returns the scheme identifying the scheme within the application (e.g.
    "local").
  */
  String getScheme() const throw();
  
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
  void terminate() throw();
  
  /**
    Closes the specified connection.
  */
  void close(Reference<OrbConnection> connection) throw(OrbException);
  
  /**
    Destroys the scheme.
  */
  ~LocalScheme() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
