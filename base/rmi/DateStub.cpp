/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/DateStub.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if 0
OrbDateStub::OrbDateStub() {
}

long long OrbDateStub::getDate() const {
  OrbRequest getDateRequest(skeleton, MESSAGE("getDate"));
  channel->writeRequest(request);
  OrbResponse response = channel->readResponse();
  bassert(response.isLongLongType(), OrbException(this));
  return response.getLongLongValue();
}

OrbDateStub::~OrbDateStub() noexcept {
}
#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
