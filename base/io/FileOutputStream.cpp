/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Exception.h"
#include <iostream>

using namespace ::std;

Exception::Exception() {
}

Exception::Exception(const string& str) : message(str) {
}

Exception::Exception(const Exception& exception) {
  message = exception.getMessage();
}

const string& Exception::getMessage() const {
  return message;
}

ostream& Exception::toString(ostream& stream) const {
  return stream << message;
}

Exception::~Exception() {
}
