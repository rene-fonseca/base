/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "MemoryException.h"

MemoryException::MemoryException() {
}

MemoryException::MemoryException(const char* message) : ResourceException(message) {
}
