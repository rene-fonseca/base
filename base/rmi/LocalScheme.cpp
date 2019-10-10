/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/LocalScheme.h>
#include <base/rmi/LocalConnection.h>
#include <base/rmi/LocalConnectionFactory.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

LocalScheme::LocalScheme() throw() {
}

String LocalScheme::getUrn() const throw() {
  return Literal("urn:rmi:orb:scheme:local");
}

String LocalScheme::getScheme() const throw() {
  return Literal("local");
}

Reference<OrbConnectionFactory> LocalScheme::openFactory(
  const String& identifier) throw(InvalidFormat, OrbException) {
  Reference<LocalConnectionFactory> factory =
    new LocalConnectionFactory(identifier);
  return new LocalConnectionFactory(identifier);
}

Reference<OrbConnection> LocalScheme::open(
  ProtectedPointer<Orb> orb,
  const String& endPoint) throw(InvalidFormat, OrbException) {
  fout << "DEBUG: "
       << "LocalScheme::open(" << '"' << endPoint << '"' << ')' << ENDL;
  
  bassert(
    !endPoint.isProper(),
    InvalidFormat("Invalid local end-point", this)
  );
  // TAG: always share a local connection (only one connection required)
  if (connections.isKey(endPoint)) {
    return static_cast<Reference<LocalConnection> >(connections[endPoint]);
  } else {
    Reference<LocalConnection> connection = new LocalConnection(orb, endPoint);
    ExclusiveSynchronize<Guard> _guard(guard);
    connections.add(endPoint, connection);
    return connection;
  }
}

void LocalScheme::add(
  Reference<OrbConnection> connection) throw(CastException) {
  Reference<LocalConnection> temp = connection.cast<LocalConnection>();
  ExclusiveSynchronize<Guard> _guard(guard);
  connections.add(temp->getEndPoint(), temp);
}

void LocalScheme::remove(
  Reference<OrbConnection> connection) throw(CastException, InvalidKey) {
  Reference<LocalConnection> temp = connection.cast<LocalConnection>();
  ExclusiveSynchronize<Guard> _guard(guard);
  connections.remove(temp->getEndPoint());
}

void LocalScheme::close(
  Reference<OrbConnection> connection) throw(OrbException) {
  // TAG: should not be required if reference counting is used appropriately
}

Reference<OrbConnection> LocalScheme::getConnection(
  const String& endPoint) throw(InvalidKey) {
  return static_cast<Reference<LocalConnection> >(connections[endPoint]);
}

void LocalScheme::run(ProtectedPointer<Orb> orb) throw(OrbException) {
  Thread* thread = Thread::getThread();
  while (!thread->isTerminated()) {
    HashTable<String, Reference<LocalConnection> >::ValueEnumerator enu =
      connections.getValueEnumerator();
    
    // if (numberOfSelected > 1) {
      // orb->scheduleScheme(this);
    // }
  }
}

void LocalScheme::terminate() throw() {
  fout << "DEBUG: " << "LocalScheme terminating" << ENDL;
}

LocalScheme::~LocalScheme() throw() {
  fout << "DEBUG: " << "LocalScheme destroyed" << ENDL;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
