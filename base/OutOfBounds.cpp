/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/OutOfBounds.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

OutOfBounds::OutOfBounds() throw() {
}

OutOfBounds::OutOfBounds(const char* message) throw() : Exception(message) {
}

OutOfBounds::OutOfBounds(const char* message, Type type) throw() : Exception(message, type) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
