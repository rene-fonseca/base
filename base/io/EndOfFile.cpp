/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "EndOfFile.h"

EndOfFile::EndOfFile() {
}

EndOfFile::EndOfFile(const char* str) : IOException(str) {
}
