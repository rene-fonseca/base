/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "FileInputStream.h"

FileInputStream::FileInputStream(const string& path) {
  open(path);
}
