/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/MemoryException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

MemoryException::MemoryException() throw() {
}

MemoryException::MemoryException(const char* message) throw() : ResourceException(message) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
