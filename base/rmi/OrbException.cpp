/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/rmi/OrbException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

OrbException::OrbException() throw() : Exception() {
}

OrbException::OrbException(const char* message) throw() : Exception(message) {
}

OrbException::OrbException(Type type) throw() : Exception(type) {
}

OrbException::OrbException(const char* message, Type type) throw()
  : Exception(message, type) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
