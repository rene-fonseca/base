/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/EOFException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

EOFException::EOFException() {
}

EOFException::EOFException(const char* message) : Exception(message) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
