/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Exception.h"

Exception::Exception() {
}

Exception::Exception(const char* m) : message(m) {
}

Exception::Exception(const Exception& copy) : message(copy.message) {
}

const char* Exception::getMessage() const {
  return message;
}

Exception::~Exception() {
}
