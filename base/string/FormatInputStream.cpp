/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "FormatInputStream.h"

FormatInputStream::FormatInputStream(InputStream* in) throw(BindException) :
  BufferedInputStream(in) {
  throw BindException();
}
