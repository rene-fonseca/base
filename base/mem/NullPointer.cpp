/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/mem/NullPointer.h>

NullPointer::NullPointer() {
}

NullPointer::NullPointer(const char* message) : MemoryException(message) {
}
