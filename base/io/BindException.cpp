/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/BindException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

BindException::BindException() {
}

BindException::BindException(const char* message) : IOException(message) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
