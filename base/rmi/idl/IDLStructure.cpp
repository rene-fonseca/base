/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/idl/IDLStructure.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

IDLStructure::IDLStructure(const String& name) throw() : IDLNamedType(name) {
}

void IDLStructure::addField(const IDLField& field) throw() {
  fields.add(field);
}

const Array<IDLField>& IDLStructure::getFields() const throw() {
  return fields;
}

IDLStructure::~IDLStructure() throw() {
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
