/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "EndOfFile.h"

EndOfFile::EndOfFile() {
}

EndOfFile::EndOfFile(const char* str) : IOException(str) {
}
