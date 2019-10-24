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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

IDLException::IDLException(const String& name, unsigned int version) noexcept {
}

void IDLException::addAttribute(const IDLAttribute& attribute) noexcept {
  attributes.add(attribute);
}
  
Array<IDLAttribute> IDLException::getAttributes() const noexcept {
  return attributes;
}

bool IDLException::isCompatible(const IDLException& exception) const noexcept {
  Array<IDLAttribute>::ReadEnumerator left = attributes.getReadEnumerator();
  Array<IDLAttribute>::ReadEnumerator right = exception.attributes.getReadEnumerator();
  while (left.hasNext() && right.hasNext()) {
    left.next()->isCompatible(*right.next());
  }
  return !left.hasNext() && !right.hasNext();
}

IDLException::~IDLException() noexcept {
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
