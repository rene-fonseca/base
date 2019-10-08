/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/OrbObject.h>
#include <base/Literal.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

OrbObject::OrbObject() throw(OrbException) {
}

String OrbObject::getOrbIdentifier() const throw(OrbException) {
  return String("1"); // TAG: fixme
}

OrbObject::~OrbObject() throw() {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
