/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/string/StringOutputStream.h>
/*
StringOutputStream::StringOutputStream(unsigned int increment) throw(BindException) : FormatOutputStream(this), str(1) {
}

void StringOutputStream::write(const char* value) throw(IOException) {
}
*/
const String<>& StringOutputStream::getString() const throw() {
  return str;
}
