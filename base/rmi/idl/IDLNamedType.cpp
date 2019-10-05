/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/idl/IDLNamedType.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

IDLNamedType::IDLNamedType(const String& _name) throw() : name(_name) {
}

IDLNamedType::~IDLNamedType() throw() {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
