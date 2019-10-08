/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/idl/IDLException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

IDLException::IDLException(const String& name, unsigned int version) throw() {
}

void IDLException::addAttribute(const IDLAttribute& attribute) throw() {
  attributes.add(attribute);
}
  
Array<IDLAttribute> IDLException::getAttributes() const throw() {
  return attributes;
}

bool IDLException::isCompatible(const IDLException& exception) const throw() {
  Array<IDLAttribute>::ReadEnumerator left = attributes.getReadEnumerator();
  Array<IDLAttribute>::ReadEnumerator right = exception.attributes.getReadEnumerator();
  while (left.hasNext() && right.hasNext()) {
    left.next()->isCompatible(*right.next());
  }
  return !left.hasNext() && !right.hasNext();
}

IDLException::~IDLException() throw() {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
