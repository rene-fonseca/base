/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "MemoryException.h"

MemoryException::MemoryException() throw() {
}

MemoryException::MemoryException(const char* message) throw() : ResourceException(message) {
}
