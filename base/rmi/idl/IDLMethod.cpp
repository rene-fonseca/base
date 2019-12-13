/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/idl/IDLMethod.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

IDLMethod::IDLMethod(const String& name) noexcept {
}

void addArgument(const IDLType& type) {
  // arguments.add(type); IDLPrimitive, IDLEnumeration, IDLStructure, IDLInterface (in or out)
}

void addException(const IDLException& exception) {
  exceptions.add(exception);
}

bool IDLMethod::hasMethod(const String& name) noexcept {
  return methods.isKey(name);
}

bool IDLMethod::raises(const IDLException& exception) const {
  return exceptions.isKey(exception.getName());
}

bool IDLMethod::isCompatible(const IDLMethod& method) const noexcept {
//   Array<IDL>::ReadEnumerator left = attributes.getReadEnumerator();
//   Array<IDLMethod>::ReadEnumerator right = interface.attributes.getReadEnumerator();
//   while (left.hasNext() && right.hasNext()) {
//     left.next()->isCompatible(*right.next());
//   }
//   return !left.hasNext() && !right.hasNext();
  // check result type
  // resultType ~ method.getResultType
  
  // check arguments
  
  // check raised exceptions
  Array<IDLException>::ReadEnumerator enu = exception.getReadEnumerator();
  while (enu.hasNext()) {
    if (!method.exceptions.isKey(*enu.next())) {
      return false;
    }
  }
  
  return true;
}

IDLMethod::~IDLMethod() noexcept {
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
