/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "StringOutputStream.h"
/*
StringOutputStream::StringOutputStream(unsigned int increment) throw(BindException) : FormatOutputStream(this), str(1) {
}

void StringOutputStream::write(const char* value) throw(IOException) {
}
*/
const String<>& StringOutputStream::getString() const throw() {
  return str;
}
