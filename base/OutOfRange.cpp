/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/OutOfRange.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

OutOfRange::OutOfRange() throw() {
}

OutOfRange::OutOfRange(const char* message) throw() : Exception(message) {
}

OutOfRange::OutOfRange(const char* message, Type type) throw() : Exception(message, type) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
