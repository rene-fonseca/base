/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/idl/IDLPrimitive.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

IDLPrimitive::IDLPrimitive(Type _type) throw() : type(_type) {
}

IDLPrimitive::~IDLPrimitive() throw() {
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
