/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "NullPointer.h"

NullPointer::NullPointer() {
}

NullPointer::NullPointer(const string& str) : MemoryException(str) {
}
