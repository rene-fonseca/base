/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/OutOfBounds.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

OutOfBounds::OutOfBounds() throw() {
}

OutOfBounds::OutOfBounds(const char* message) throw() : Exception(message) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
