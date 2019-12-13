/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/idl/IDLField.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

IDLField::IDLField(const IDLType& _type) noexcept : type(_type) {
}

IDLField::~IDLField() noexcept {
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
