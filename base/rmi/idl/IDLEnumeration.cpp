/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/idl/IDLEnumeration.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

IDLEnumeration::IDLEnumeration(const String& name) throw()
  : IDLNamedType(name) {
}

IDLEnumeration::~IDLEnumeration() throw() {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
