/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/IOException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

IOException::IOException() throw() : Exception() {
}

IOException::IOException(const char* message) throw() : Exception(message) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
