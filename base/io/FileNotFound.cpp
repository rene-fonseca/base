/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "FileNotFound.h"

FileNotFound::FileNotFound() {
}

FileNotFound::FileNotFound(const char* str) : IOException(str) {
}
