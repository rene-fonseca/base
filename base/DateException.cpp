/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/features.h>
#include <base/DateException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

DateException::DateException() throw() {
}

DateException::DateException(const char* message) throw() : Exception(message) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
