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

#include <base/rmi/OrbConnection.h>
#include <base/rmi/Orb.h>
#include <base/mem/ProtectedPointer.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class LocalScheme;

/**
  Local connection.
  
  @short Local connection.
  @see LocalConnectionFactory
  @ingroup rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API LocalConnection : public OrbConnection {
  friend class LocalScheme;
private:
  
  /** The identifier of the end point. */
  String identifier;
public:
  
  /**
    Initializes the conenction.
    
    @param identifier The end point identifier.
  */
  LocalConnection(
    ProtectedPointer<Orb> orb,
    const String& identifier);
  
  /**
    Returns the global identifier of the scheme (e.g.
    "urn:rmi:orb:scheme:local").
  */
  String getUrn() const noexcept;
  
  /**
    Returns the identifier of the end point (e.g. "local:///").
  */
  String getEndPoint() const noexcept;

  /**
    Returns the reference for the specified connection local identifier.

    @param identifier Identifier (e.g. "/MyObject").
  */
  OrbReference getReference(const String& identifier) noexcept;
  
  /**
    Pushes data onto the stream. This method may return 0 if the internal
    buffer is full. EndOfFile is raised if the end has been reached.
    
    @param buffer The data to be pushed onto the stream.
    @param size The size of the buffer.
    
    @return The number of bytes pushed.
  */
  MemorySize push(const uint8* buffer, MemorySize size);
  
  /**
    This method should be invoked when all data has been pushed onto the
    stream. Do not invoke push() or pushEnd() hereafter.
  */
  void pushEnd();
  
  /**
    Pulls data from the stream. EndOfFile is raised if atEnd() would return
    true.
    
    @param buffer The buffer to receive the data.
    @param size The size of the buffer.
    
    @return The number of bytes pulled.
  */
  MemorySize pull(uint8* buffer, MemorySize size);
  
  /**
    Reestablishes the connection.
  */
  void reconnect();
  
  /**
    Destroys the local connection.
  */
  ~LocalConnection() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
