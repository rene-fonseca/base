/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/Connection.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

OrbChannel::OrbChannel(Reference<OrbEncoder> _encoder, Reference<OrbDecoder> _decoder) throw()
  : encoder(_encoder), decoder(_decoder) {
}

String OrbChannel::getOrbIdentifier(const OrbStub& object) const throw() {
  return ""; // TAG: fixme
}

String OrbChannel::getOrbInterfaceIdentifier(const OrbStub& object) const throw() {
  return ""; // TAG: fixme
}

OrbChannel::~OrbChannel() throw() {
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
