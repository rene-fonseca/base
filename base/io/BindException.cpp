/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/BindException.h>

BindException::BindException() {
}

BindException::BindException(const char* message) : IOException(message) {
}
