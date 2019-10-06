/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/LocalConnection.h>
#include <base/rmi/LocalScheme.h>
#include <base/concurrency/Thread.h>
#include <base/rmi/OrbRequest.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

LocalConnection::LocalConnection(
  ProtectedPointer<Orb> orb,
  const String& _identifier) throw(InvalidFormat, OrbException)
  : OrbConnection(orb, 0), identifier(_identifier) {
  bassert(!identifier.isProper(), InvalidFormat(this));
}

String LocalConnection::getUrn() const throw() {
  return Literal("urn:rmi:orb:scheme:local");
}

String LocalConnection::getEndPoint() const throw() {
  return identifier;
}

// TAG: need method to negotiation encodings on connection open

OrbReference LocalConnection::getReference(const String& identifier) throw() {
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
  } catch (OrbException& e) {
    encoder->release();
    throw;
  }
  
  OrbReference result(0);
  return result;
}

unsigned int LocalConnection::push(
  const uint8* buffer, unsigned int size) throw(IOException) {
  return 0;
}

void LocalConnection::pushEnd() throw(IOException) {
}

unsigned int LocalConnection::pull(
  uint8* buffer, unsigned int size) throw(IOException) {
  return 0;
}

void LocalConnection::reconnect() throw(OrbException) {
  // TAG: fixme - should this be replaced by other method in other class
}

LocalConnection::~LocalConnection() throw() {
  fout << "DEBUG: " << "LocalConnection destroyed" << ENDL;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
