/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/EndOfFile.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

EndOfFile::EndOfFile() {
}

EndOfFile::EndOfFile(const char* message) : IOException(message) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
