/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "FileNotFound.h"

FileNotFound::FileNotFound() {
}

FileNotFound::FileNotFound(const char* str) : IOException(str) {
}
