/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/EOFException.h>

EOFException::EOFException() {
}

EOFException::EOFException(const char* message) : Exception(message) {
}
