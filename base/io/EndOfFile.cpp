/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/EndOfFile.h>

EndOfFile::EndOfFile() {
}

EndOfFile::EndOfFile(const char* message) : IOException(message) {
}
