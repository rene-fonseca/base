/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "FileInputStream.h"

FileInputStream::FileInputStream(const string& path) {
  open(path);
}
