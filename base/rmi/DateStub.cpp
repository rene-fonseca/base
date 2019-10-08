/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/DateStub.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

OrbDateStub::OrbDateStub() throw(OrbException) {
}

long long OrbDateStub::getDate() const throw(OrbException) {
  OrbRequest getDateRequest(skeleton, MESSAGE("getDate"));
  channel->writeRequest(request);
  OrbResponse response = channel->readResponse();
  bassert(response.isLongLongType(), OrbException(this));
  return response.getLongLongValue();
}

OrbDateStub::~OrbDateStub() throw() {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
