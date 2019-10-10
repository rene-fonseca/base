/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/OrbRequest.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

OrbRequest::OrbRequest(
  const Reference<OrbStub>& _stub, const String& method) throw(OrbException)
  : stub(_stub) {
  methodId = stub->getMethodId(method);
  // channel.writeRequest(LOOKUP_METHOD);
  // channel.writeReference(reference);
  // channel.write(method);
  // flush
  
  unsigned int methodId;
  // channel->read(methodId);
  // bassert(methodId != 0, OrbException("Invalid method", this));
  
  bool compatible = true;
  bassert(compatible, OrbException("Incompatible methods", this));
}

OrbRequest::~OrbRequest() throw() {
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
