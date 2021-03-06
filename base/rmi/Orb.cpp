/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/Orb.h>
#include <base/concurrency/Thread.h>
#include <base/net/Urn.h>
#include <base/collection/Array.h>
#include <base/Literal.h>

#include <base/rmi/LisaEncoding.h> // TAG: remove when done

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class AmbiguousRegistration : public Exception {
public:
};

Reference<OrbEncoding> Orb::getDefaultEncoding() noexcept {
  return new LisaEncoding();
}

// TAG: the Orb should be a skeleton: with identify orb method - fixed urn
// getVersion
// getVendor

Orb::Orb() {
}

unsigned int Orb::getNumberOfStubs() const noexcept {
  return 0; // TAG: fixme - stubs.getSize();
}

unsigned int Orb::getNumberOfSkeletons() const noexcept
{
  return skeletons.getNumberOfSkeletons();
}

unsigned int Orb::getNumberOfConnections() const noexcept
{
  return 0; // TAG: fixme
}
 
HashSet<String> Orb::getEncodings() const
{
  HashSet<String> result;
  HashTable<String, Reference<OrbEncoding> >::ReadEnumerator enu =
    encodings.getReadEnumerator();
  while (enu.hasNext()) {
    result.add(enu.next()->getKey());
  }
  return result;
}

HashSet<String> Orb::getSchemes() const
{
  HashSet<String> result;
  HashTable<String, Reference<OrbScheme> >::ReadEnumerator enu =
    schemes.getReadEnumerator();
  while (enu.hasNext()) {
    result.add(enu.next().getKey());
  }
  return result;
}

HashSet<String> Orb::getFactories() const
{
  HashSet<String> result;
  HashSet<Reference<OrbConnectionFactory> >::ReadEnumerator enu =
    factories.getReadEnumerator();
  while (enu.hasNext()) {
    Reference<OrbConnectionFactory> factory = enu.next();
    result.add(factory->getEndPoint());
  }
  return result;
}

void Orb::registerEncoding(
  Reference<OrbEncoding> encoding,
  unsigned int flags)
  {
  
  String urn = encoding->getUrn();
  bassert(Urn::isUrn(urn), InvalidFormat(this));
//   bassert(
//     urn.startsWith(OrbEncoding::getUrnNamespace()),
//     OrbException(this).setCause(OrbException::INVALID_ENCODING_NAMESPACE)
//   );
  bassert(!encodings.isKey(urn), AmbiguousRegistration(this));
  encodings.add(urn, encoding);
}

void Orb::registerScheme(
  Reference<OrbScheme> scheme)
  {
  
  String urn = scheme->getUrn();
  bassert(Urn::isUrn(urn), InvalidFormat(this));
  bassert(!schemes.isKey(urn), AmbiguousRegistration(this));
  schemeIds.add(scheme->getScheme(), scheme);
  schemes.add(urn, scheme);
}

void Orb::registerFactory(Reference<OrbConnectionFactory> factory)
 
{
  String urn = factory->getUrn();
  bassert(
    schemes.isKey(urn),
    OrbException(this).setCause(OrbException::SCHEME_NOT_REGISTERED)
  );
  factories.add(factory);
}

void Orb::openFactory(const String& identifier)
 
{
  const int colon = identifier.indexOf("://");
  bassert(
    colon > 0,
    OrbException(this).setCause(OrbException::SCHEME_MISSING)
  );
  const String schemeId = identifier.substring(0, colon);
  bassert(
    schemeIds.isKey(schemeId),
    OrbException(this).setCause(OrbException::SCHEME_NOT_REGISTERED)
  );
  Reference<OrbScheme> scheme = schemes[schemeId];
  registerFactory(scheme->openFactory(identifier));
}

// class OrbConnectionReference {
// private:

//   OrbReference reference;
// public:

//   OrbConnectionReference
  
//   Reference<OrbConnection> getConnection() const noexcept {
//     return connection;
//   }
  
//   inline OrbReference getReference() const noexcept {
//     return reference;
//   }
// };

Orb::OrbConnectionReference Orb::getConnectionReference(const String& identifier)
{
  fout << "DEBUG: " << "Orb::getConnectionReference(\""
       << identifier << "\")" << ENDL;

  // TAG: enable later
  // if (stubs.isKey(identifier)) {
  //   return stubs[identifier];
  // }
  
  const int colon = identifier.indexOf("://");
  bassert(
    colon >= 0,
    OrbException(this).setCause(OrbException::SCHEME_MISSING)
  );
  
  const String schemeId = identifier.substring(0, colon);
  bassert(
    schemeIds.isKey(schemeId),
    OrbException(this).setCause(OrbException::SCHEME_NOT_REGISTERED)
  );
  Reference<OrbScheme> scheme = schemeIds[schemeId];
  
  int slash = identifier.indexOf('/', colon + Literal("://").getLength());
  bassert(slash >= 0, InvalidFormat("Missing.", this));
  String endPoint = identifier.substring(
    colon + Literal("://").getLength(),
    slash
  );
  
  Reference<OrbConnection> connection = scheme->open(this, endPoint);
  fout << "DEBUG: " << "connection " << connection->getUrn()
       << ' ' << identifier.substring(slash + 1) << ENDL;

  OrbReference reference = connection->getReference(
    identifier.substring(slash + 1)
  );
  return OrbConnectionReference(connection, reference);
}

class OrbConnectionThread : public Thread {
private:
  
  ProtectedPointer<Orb> orb;
  Reference<OrbConnection> connection;
public:
  
  inline OrbConnectionThread(
    ProtectedPointer<Orb> _orb,
    Reference<OrbConnection> _connection) noexcept
    : orb(_orb), connection(_connection) {
  }
  
  void run() noexcept {
    WRITE_SOURCE_LOCATION();
    orb->onConnection(connection);
  }

  ~OrbConnectionThread() noexcept {
  }
};

class OrbFactoryDispatcher : public Runnable {
private:

  ProtectedPointer<Orb> orb;
  Reference<OrbConnectionFactory> factory;
public:
  
  inline OrbFactoryDispatcher(
    ProtectedPointer<Orb> _orb,
    Reference<OrbConnectionFactory> _factory) noexcept
    : orb(_orb), factory(_factory) {
  }
  
  void run() noexcept {
    WRITE_SOURCE_LOCATION();
    factory->run(orb);
  }
};

// a request
void Orb::onConnection(Reference<OrbConnection> connection) noexcept {
  WRITE_SOURCE_LOCATION();
  
  Allocator<char> incoming(4096); // scheme specific buffer size
  Allocator<char> outgoing(4096);
  
  ProtectedPointer<Thread> thread = Thread::getThread();
  // TAG: check if orb is terminated
  while (!thread->isTerminated()) {
    // pull and push
    Thread::sleep(1);
    fout << "DEBUG: " << connection->getEndPoint() << ENDL;
  }
}

// void Orb::onClosedConnection() {
//   // register thread for destruction
// }

void Orb::onIncomingConnection(
  Reference<OrbConnectionFactory> factory,
  Reference<OrbConnection> connection) {
  
  fout << "DEBUG: " << "Incoming connection "
       << connection->getEndPoint() << ENDL;
  String urn = connection->getUrn();
  bassert(
    schemes.isKey(urn),
    OrbException(this).setCause(OrbException::SCHEME_NOT_REGISTERED)
  );
  Reference<OrbScheme> scheme = schemes[urn];
  scheme->add(connection);
}

// object reference, method, arguments

#if 0
// OrbSink responsible for redirecting data from connection to other sink - has a buffer
class OrbCollector {
private:
  
  /** Buffer for incoming data. */
  Allocator<uint8> incoming;
  /** Available bytes in buffer. */
  unsigned int availableBytes;

  class Window {
  private:

    Allocator<uint8> incoming;
    unsigned int availableBytes;
  public:

    Window() {
    }
  };
  
public:
  
  static unsigned int INCOMING_BUFFER_SIZE = 16 * 1024;
  
  OrbSink()
    : incoming(INCOMING_BUFFER_SIZE),
      availableBytes(0) {
  }
  
  // window - up to n requests concurrently
  unsigned int push(const uint8* buffer, unsigned int size) noexcept {
    // current window
    unsigned int bytesToPush = size;
    while (bytesToPush) {
      Window* window;
      unsigned int bytesToPush = minimum(, bytesToPush);
      window->push(buffer, size);
      // bytes to go for current window
      // copy data to respective buffers
    }
    return size;
    
    // get type of request
    unsigned int requet;
    // read(request);
    switch (request) {
    case LOOKUP_METHOD:
      // object id
      // method id
    case INVOCATION:
    case 0:
    default:
      throw OrbException(this).setCause(OrbException::INVALID_REQUEST);
    }
  }
};
#endif // disabled

unsigned int Orb::push(
  Reference<OrbConnection> connection,
  const uint8* buffer,
  unsigned int size) noexcept {
  fout << "DEBUG: " << "Incoming data " << size << ENDL;
  // lookup decoder
  return size;
}

void Orb::run() {
  HashSet<Reference<OrbConnectionFactory> >::Enumerator enu =
    factories.getEnumerator();
  Array<Thread*> threads;
  {
    fout << "DEBUG: " << "Starting factory threads" << ENDL;
    while (enu.hasNext()) {
      Reference<OrbConnectionFactory> factory = enu.next();
      OrbFactoryDispatcher dispatcher(this, factory);
      Thread* thread = new Thread(&dispatcher);
      threads.append(thread);
      thread->start();
    }
  }

  // Thread::wait(); // blocking wait for termination
  while (!Thread::getThread()->isTerminated()) {
    Thread::sleep(1); // TAG: fixme
  }
  
  // signal factory threads to terminate
  {
    fout << "DEBUG: " << "Terminating factories" << ENDL;
    Array<Thread*>::Enumerator enu = threads.getEnumerator();
    while (enu.hasNext()) {
      enu.next()->terminate();
    }
  }
  
  // wait for factory threads to complete
  {
    fout << "DEBUG: " << "Waiting for factories to complete" << ENDL;
    Array<Thread*>::Enumerator enu = threads.getEnumerator();
    while (enu.hasNext()) {
      enu.next()->join();
    }
  }
}

void Orb::terminate() noexcept {
  // TAG: fixme
}

Orb::~Orb() noexcept {
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
