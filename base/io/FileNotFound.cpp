/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/FileNotFound.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

FileNotFound::FileNotFound() {
}

FileNotFound::FileNotFound(const char* message) : IOException(message) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
