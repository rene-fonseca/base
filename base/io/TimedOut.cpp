/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "TimedOut.h"

TimedOut::TimedOut() throw() : IOException() {
}

TimedOut::TimedOut(const char* message) throw() : IOException(message) {
}
