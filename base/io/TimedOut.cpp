/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/TimedOut.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

TimedOut::TimedOut() throw() : IOException() {
}

TimedOut::TimedOut(const char* message) throw() : IOException(message) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
