/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/idl/IDLStructure.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

IDLStructure::IDLStructure(const String& name) throw() : IDLNamedType(name) {
}

void IDLStructure::addField(const IDLField& field) throw() {
  fields.add(field);
}

Array<IDLField> IDLStructure::getFields() const throw() {
  return fields;
}

IDLStructure::~IDLStructure() throw() {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
