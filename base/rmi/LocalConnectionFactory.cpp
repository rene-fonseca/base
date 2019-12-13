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
#include <base/rmi/LocalConnectionFactory.h>
#include <base/rmi/LocalScheme.h>
#include <base/concurrency/Thread.h>
#include <base/string/StringOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

LocalConnectionFactory::LocalConnectionFactory(
  const String& endPoint) {
  bassert(!endPoint.isProper(), InvalidFormat(this));
  
  this->identifier = endPoint;
  try {
  } catch (...) {
    throw OrbException(this).setCause(OrbException::UNABLE_TO_INITIALIZE_FACTORY);
  }
}

String LocalConnectionFactory::getEndPoint() const noexcept {
  return identifier;
}

void LocalConnectionFactory::run(
  ProtectedPointer<Orb> orb) {
  Thread* thread = Thread::getThread();
  while (!thread->isTerminated()) {
    WRITE_SOURCE_LOCATION();
  }
  WRITE_SOURCE_LOCATION();
}

String LocalConnectionFactory::getUrn() const noexcept {
  return Literal("urn:rmi:orb:scheme:local");
}

Reference<OrbStub> LocalConnectionFactory::getObject(
  const String& identifier) {
  // check if connection already exists
  return 0; // TAG: fixme
}

LocalConnectionFactory::~LocalConnectionFactory() noexcept {
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
