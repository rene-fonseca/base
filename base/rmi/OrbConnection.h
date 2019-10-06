/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Polymorphic.h>
#include <base/collection/SingleLinkedNode.h>
#include <base/concurrency/ExclusiveSynchronize.h>
#include <base/concurrency/MutualExclusion.h>
#include <base/io/PushInterface.h>
#include <base/io/PullInterface.h>
#include <base/mem/Allocator.h>
#include <base/mem/ReferenceCountedObject.h>
#include <base/rmi/OrbException.h>
#include <base/rmi/OrbBufferPool.h>
#include <base/rmi/OrbReference.h>
#include <base/string/String.h>
#include <base/rmi/OrbDecoder.h>
#include <base/rmi/OrbEncoder.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class Orb;

/**
  A bidirectional connection between a client and server.
  
  @short ORB connection.
  @ingroup rmi
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class OrbConnection : public Polymorphic,
                      public ReferenceCountedObject,
                      public PushInterface,
                      public PullInterface {
protected:

  ProtectedPointer<Orb> orb;
  /** Encoder. */
  // Reference<OrbEncoder> encoder;
  /** Decoder. */
  // Reference<OrbDecoder> decoder;
  /** The buffer pool. */
  Reference<OrbBufferPool> pool;
  /** Outgoing queue. */
  // OrbBufferQueue outgoing;
  /** Incoming queue. */
  // OrbBufferQueue incoming;
public:

  /**
    Initializes the connection.

    @param pool The buffer pool.
  */
  OrbConnection(
    ProtectedPointer<Orb> _orb,
    Reference<OrbBufferPool> _pool) throw()
    : orb(_orb), pool(_pool) /*, outgoing(_pool), incoming(_pool)*/ {
  }
  
  /**
    Returns the global identifier of the connection scheme (e.g.
    "urn:rmi:orb:scheme:tcpip").
  */
  virtual String getUrn() const throw() = 0;
  
  /**
    Pushes a buffer onto the outgoing queue of the connection.
    
    @param buffer The buffer.
  */
  inline void push(OrbBuffer* buffer) throw() {
    // outgoing.push(buffer);
  }
  
  /**
    Returns the identifier of the end point (e.g. "tcpip://host:1234").
  */
  virtual String getEndPoint() const throw() = 0;  

  /**
    Returns the reference for the specified connection local identifier.

    @param identifier Identifier (e.g. "/MyObject").
  */
  virtual OrbReference getReference(const String& identifier) throw() = 0;
  
  /**
    Destroys the connection.
  */
  virtual ~OrbConnection() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
