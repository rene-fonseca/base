/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "MemoryException.h"

MemoryException::MemoryException() throw() {
}

MemoryException::MemoryException(const char* message) throw() : ResourceException(message) {
}
