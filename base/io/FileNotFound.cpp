/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/io/FileNotFound.h>

FileNotFound::FileNotFound() {
}

FileNotFound::FileNotFound(const char* message) : IOException(message) {
}
