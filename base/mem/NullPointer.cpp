/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "NullPointer.h"

NullPointer::NullPointer() {
}

NullPointer::NullPointer(const string& str) : MemoryException(str) {
}
