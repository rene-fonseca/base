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

#include <base/Polymorphic.h>
#include <base/collection/HashTable.h>
#include <base/mem/Reference.h>
#include <base/rmi/OrbConnection.h>
#include <base/rmi/OrbConnectionFactory.h>
#include <base/rmi/OrbException.h>
#include <base/string/InvalidFormat.h>
#include <base/CastException.h>
#include <base/mem/ProtectedPointer.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class Orb;
class OrbConnectionFactory;

/**
  ORB communication scheme. This class is responsible for managing all
  connections for this particular scheme.
  
  @short ORB communication scheme.
  @ingroup rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API OrbScheme : public Polymorphic, public ReferenceCountedObject {
public:
  
  /**
    Returns the global identifier of the scheme (e.g.
    "urn:rmi:orb:scheme:tcpip").
  */
  virtual String getUrn() const throw() = 0;
  
  /**
    Returns the scheme identifying the scheme within the application (e.g.
    "tcpip").
  */
  virtual String getScheme() const throw() = 0;
  
  /**
    Returns the current number of connections.
  */
  virtual unsigned int getNumberOfConnections() const throw() = 0;

  /**
    Returns a connection factory for the specified end point. This may not be
    supported by the scheme.
  */
  virtual Reference<OrbConnectionFactory> openFactory(
    const String& identifier) throw(InvalidFormat, OrbException);
  
  /**
    Returns a connection for the specified identifier. New connections are
    established on demand.
  */
  virtual Reference<OrbConnection> open(
    ProtectedPointer<Orb> orb,
    const String& identifier) throw(InvalidFormat, OrbException) = 0;
  
  /**
    Adds a connection.
  */
  virtual void add(
    Reference<OrbConnection> connection) throw(CastException) = 0;
  
  /**
    Removes a connection.
  */
  virtual void remove(
    Reference<OrbConnection> connection) throw(CastException, InvalidKey) = 0;
  
  /**
    Signals the scheme to be terminated.
  */
  virtual void terminate() noexcept = 0;
  
  /**
    Invoked by ORB.
  */
  virtual void run(ProtectedPointer<Orb> orb) throw(OrbException) = 0;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
