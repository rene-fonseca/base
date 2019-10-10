/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/communication/IEEE1394Exception.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

IEEE1394Exception::IEEE1394Exception() throw() : CommunicationsException() {
}

IEEE1394Exception::IEEE1394Exception(const char* message) throw()
  : CommunicationsException(message) {
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
