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

#include <base/collection/HashSet.h>
#include <base/collection/HashTable.h>
#include <base/collection/Queue.h>
#include <base/concurrency/Semaphore.h>
#include <base/mem/Reference.h>
#include <base/rmi/OrbBufferPool.h>
#include <base/rmi/OrbException.h>
#include <base/rmi/OrbSkeleton.h>
#include <base/rmi/OrbStub.h>
#include <base/rmi/OrbSkeletonManager.h>
#include <base/rmi/OrbEncoding.h>
#include <base/rmi/OrbConnectionFactory.h>
#include <base/rmi/OrbScheme.h>
#include <base/string/String.h>
#include <base/string/InvalidFormat.h>
#include <base/AutomationObject.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class OrbScheme;
class OrbStub;
class OrbEncoding;

/**
  @defgroup rmi Remote Method Invocation (RMI)
*/

/**
  Object Request Broker (ORB).
  
  @short Object Request Broker (ORB).
  @ingroup rmi
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Orb : public Object {
  friend class TCPIPConnection; // TAG: remove
  friend class OrbDecoder;
  friend class OrbEncoder;
private:

  /** Buffer pool. */
  OrbBufferPool pool;
  /** The registered encodings. */
  HashTable<String, Reference<OrbEncoding> > encodings;
  /** The connection factories. */
  HashSet<Reference<OrbConnectionFactory> > factories;
  /** The registered schemes. */
  HashTable<String, Reference<OrbScheme> > schemes;
  /** The registered scheme identifiers. */
  HashTable<String, Reference<OrbScheme> > schemeIds;
  /** Stub registry. */
  HashTable<String, Reference<OrbStub> > stubs;
  /** Skeleton registry. */
  OrbSkeletonManager skeletons;
  
  /** Incoming connections. */
  Queue<Reference<OrbConnection> > incomingConnections;
  /** Incoming factories. */
  Queue<Reference<OrbConnectionFactory> > incomingFactories;
  /** Attention. */
  Semaphore attention;
  
  /** Schema jobs. */
  Queue<Reference<OrbScheme> > schemaJobs;
  /** Job queue semaphore. */
  Semaphore schemaJob;
  
  /**
    Acquires a buffer from the pool.
  */
  inline OrbBuffer* acquireBuffer(unsigned int holding) throw(OrbException) {
    return pool.acquire(holding);
  }
  
  /**
    Invoked by connection scheme when processing is required.
  */
//   inline void onJob(Reference<OrbScheme> scheme) throw() {
//     {
//       ExclusiveSynchronize<Guard> _guard(guard);
//       // make schema as job available - schema.getJobId();
//     }
//     schemas.post();
//   }
  
  /**
    Puts the buffer back into the pool.
  */
  inline void releaseBuffers(OrbBuffer* buffers) throw() {
    // pool.release(buffers);
  }
  
  inline void pushRequest(
    const uint8* buffer, unsigned int size) throw(OrbException) {
    WRITE_SOURCE_LOCATION();
  }
  
  /**
    Writes a request to the specified connection.
  */
  inline void pushRequest(
    Reference<OrbConnection>& connection,
    const SingleLinkedNode<Allocator<uint8>*>* buffers,
    unsigned int size) throw(OrbException) {
//     try {
//       connection->push(buffers, size);
//     } catch (IOException&) {
//       fout << "DEBUG: " << "Write to connection failed "
//            << connection->getEndPoint() << ENDL;
//       // release connection and open new if client connection
//       // scheme.open(connection->getEndPoint());
//       // negotiate
//       // push
//       throw; // TAG: temp fix
//     }
  }

//   inline bool getObjectReference(Reference<OrbConnection>& connection, const String& name) const throw() {
//     // make response
//     skeletons.hasSkeleton(name);
//     // write(reference);
//   }

  class OrbConnectionReference : public AutomationObject {
  private:

    Reference<OrbConnection> connection;
    OrbReference reference;
  public:
    
    inline OrbConnectionReference(
      const Reference<OrbConnection>& _connection, OrbReference& _reference)
      : connection(_connection), reference(_reference) {
    }

    inline OrbConnectionReference(const OrbConnectionReference& copy) throw()
      : connection(copy.connection), reference(copy.reference) {
    }
    
    inline Reference<OrbConnection> getConnection() const throw() {
      return connection;
    }
    
    inline OrbReference getReference() const throw() {
      return reference;
    }
  };
  
  /**
    Returns a stub to the specified object.
  */
  OrbConnectionReference getConnectionReference(const String& identifier)
    throw(InvalidFormat, OrbException);
public:
  
  /** The default thread concurrency level. */
  static const unsigned int DEFAULT_CONCURRENCY = 16;
  
  enum NamedObjects {
    OBJECT_ORB,
    OBJECT_REGISTRY
  };

  /** Flags for use with encoding scheme registration. */
  enum EncodingSchemeFlags {
    /** Encoding scheme should be available for incoming trafic. */
    INCOMING = 1,
    /** Encoding scheme should be available for outgoing trafic. */
    OUTGOING = 2
  };
  
  /**
    Initializes the ORB.
  */
  Orb() throw(OrbException);

  /**
    Returns the current number of registered encoding schemes.
  */
  inline unsigned int getNumberOfEncodings() const throw() {
    return encodings.getSize();
  }
  
  /**
    Returns the current number of registered schems.
  */
  inline unsigned int getNumberOfSchemes() const throw() {
    return schemes.getSize();
  }
  
  /**
    Returns the current number of registered factories.
  */
  inline unsigned int getNumberOfFactories() const throw() {
    return factories.getSize();
  }
  
  /**
    Returns the current number of stubs.
  */
  unsigned int getNumberOfStubs() const throw();
  
  /**
    Returns the current number of skeletons.
  */
  unsigned int getNumberOfSkeletons() const throw();

  /**
     Returns the current number of connections.
  */
  unsigned int getNumberOfConnections() const throw();

  /**
    Returns the current registered encoding schemes.
  */
  HashSet<String> getEncodings() const throw(MemoryException);

  /**
    Returns the current registered schemes.
  */
  HashSet<String> getSchemes() const throw(MemoryException);

  /**
    Returns the identifiers of the registered factories.
  */
  HashSet<String> getFactories() const throw(MemoryException);
  
  /**
    Registers the specified ORB encoding. Flags may be use to register the
    encoding for assymetric trafic.
    
    @param encoding The encoding scheme.
    @param flags The flags. The default is INCOMING|0UTGOING.
  */
  void registerEncoding(
    Reference<OrbEncoding> encoding,
    unsigned int flags = INCOMING|OUTGOING)
    throw(InvalidFormat, OrbException, AmbiguousRegistration, MemoryException);
  
  /**
    Registers the specified scheme.
  */
  void registerScheme(
    Reference<OrbScheme> scheme)
    throw(InvalidFormat, AmbiguousRegistration, OrbException, MemoryException);

  /**
    Registers a connection factory.

    @param factory The connection factory.
  */
  void registerFactory(Reference<OrbConnectionFactory> factory)
    throw(OrbException, MemoryException);

  /**
    Opens a factory for the specified end point (e.g. tcpip://localhost:1234).

    @param identifier The identifier of the end point.
  */
  void openFactory(const String& identifier)
    throw(InvalidFormat, OrbException, MemoryException);

  // TAG: temporary
  Reference<OrbEncoding> getDefaultEncoding() throw();
  
  template<class POLY>
  Reference<POLY> getObject(const String& identifier)
    throw(InvalidFormat, OrbException) {
    // TAG: check type?
    OrbConnectionReference temp = getConnectionReference(identifier);
    // TAG: add stub to registry - stubs.add(identifier, stub);
    Reference<OrbEncoding> encoding = getDefaultEncoding(); // TAG: temporary
    return new POLY(
      this,
      temp.getConnection(),
      encoding->getDecoder(this),
      encoding->getEncoder(this),
      temp.getReference()
    );
  }
  
  /**
    Registers the specified skeleton.
  */
  inline void registerSkeleton(void* skeleton)
    throw(AmbiguousRegistration, MemoryException) {
    skeletons.add(skeleton);
  }

  inline void registerSkeleton(const String& name, void* skeleton)
    throw(AmbiguousRegistration, MemoryException) {
    skeletons.add(name, skeleton);
  }
  
  /**
    Unregisters the specified skeleton.
  */
  inline void unregisterSkeleton(void* skeleton)
    throw(InvalidKey) {
    skeletons.remove(skeleton);
  }
  
  /**
    Returns the identifier of the specified skeleton.
  */
  String getIdentifier(void* skeleton) throw(OrbException);

  /**
    Invoked on incoming connection (new context).
  */
  void onConnection(Reference<OrbConnection> connection) throw();
  
  /**
    Invoked on incoming connection.
    
    @param factory The connection factory.
    @param connection The ORB connection.
  */
  void onIncomingConnection(
    Reference<OrbConnectionFactory> factory,
    Reference<OrbConnection> connection) throw(OrbException);
  
  /**
    Invoked by connection on incoming data.
  */
  unsigned int push(
    Reference<OrbConnection> connection,
    const uint8* buffer,
    unsigned int size) throw();
  
  /**
    Enters the ORB message dispatch loop.
  */
  void run() throw(OrbException);

  /**
    Terminates the message dispatch loop.
  */
  void terminate() throw();
  
  /**
    Destroys the orb.
  */
  ~Orb() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
