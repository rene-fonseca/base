/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/OutOfDomain.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

OutOfDomain::OutOfDomain() throw() {
}

OutOfDomain::OutOfDomain(const char* message) throw() : Exception(message) {
}

OutOfDomain::OutOfDomain(const char* message, Type type) throw() : Exception(message, type) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
