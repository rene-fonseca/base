/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/mem/NullPointer.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

NullPointer::NullPointer() {
}

NullPointer::NullPointer(const char* message) : MemoryException(message) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
