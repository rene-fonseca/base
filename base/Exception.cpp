/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/Exception.h>

Exception::Exception() throw() : message(0) {
}

Exception::Exception(const char* m) throw() : message(m) {
}

Exception::Exception(const Exception& copy) throw() : message(copy.message) {
}

const char* Exception::getMessage() const throw() {
  return message;
}

Exception::~Exception() throw() {
}
