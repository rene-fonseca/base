/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/LocalConnection.h>
#include <base/rmi/LocalScheme.h>
#include <base/concurrency/Thread.h>
#include <base/rmi/OrbRequest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

LocalConnection::LocalConnection(
  ProtectedPointer<Orb> orb,
  const String& _identifier)
  : OrbConnection(orb, 0), identifier(_identifier) {
  bassert(!identifier.isProper(), InvalidFormat(this));
}

String LocalConnection::getUrn() const noexcept {
  return Literal("urn:rmi:orb:scheme:local");
}

String LocalConnection::getEndPoint() const noexcept {
  return identifier;
}

// TAG: need method to negotiation encodings on connection open

OrbReference LocalConnection::getReference(const String& identifier) noexcept {
  // TAG: request reference for identifier

  fout << "DEBUG: " << "LocalConnection::getReference(\"" << identifier
       << "\")" << ENDL;

  Reference<OrbEncoder> encoder = orb->getDefaultEncoding()->getEncoder(orb);
  
  try {
    unsigned short request = OrbRequest::LOOKUP_OBJECT;
    encoder->write(request);
    encoder->write(identifier);
    encoder->flush();
    encoder->release();
  } catch (OrbException&) {
    encoder->release();
    _rethrow;
  }
  
  OrbReference result(0);
  return result;
}

unsigned int LocalConnection::push(
  const uint8* buffer, unsigned int size) {
  return 0;
}

void LocalConnection::pushEnd() {
}

unsigned int LocalConnection::pull(
  uint8* buffer, unsigned int size) {
  return 0;
}

void LocalConnection::reconnect() {
  // TAG: fixme - should this be replaced by other method in other class
}

LocalConnection::~LocalConnection() noexcept {
  fout << "DEBUG: " << "LocalConnection destroyed" << ENDL;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
