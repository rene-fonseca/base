/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Exception.h"

Exception::Exception() {
}

Exception::Exception(const char* str) : message(str) {
}

Exception::Exception(const Exception& exception) {
  message = exception.getMessage();
}

const char* Exception::getMessage() const {
  return message;
}

Exception::~Exception() {
}
