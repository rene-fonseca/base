/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/mem/NullPointer.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

NullPointer::NullPointer() {
}

NullPointer::NullPointer(const char* message) : MemoryException(message) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
