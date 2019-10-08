/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/idl/IDLInterface.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

IDLInterface::IDLInterface(const String& name, unsigned int version) throw()
  : IDLNamedType(name) {
}

bool IDLInterface::isCompatible(const IDLInterface& interface) const throw() {
  Array<IDLMethod>::ReadEnumerator left = attributes.getReadEnumerator();
  Array<IDLMethod>::ReadEnumerator right = interface.attributes.getReadEnumerator();
  while (left.hasNext() && right.hasNext()) {
    left.next()->isCompatible(*right.next());
  }
  return !left.hasNext() && !right.hasNext();
}

IDLInterface::~IDLInterface() throw() {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
