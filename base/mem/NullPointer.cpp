/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/mem/NullPointer.h>

NullPointer::NullPointer() {
}

NullPointer::NullPointer(const char* message) : MemoryException(message) {
}
