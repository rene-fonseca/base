/***************************************************************************
    begin       : Fri May 12 2000
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Exception.h"
#include <iostream>

using namespace ::std;

Exception::Exception() {
  message = new string();
}

Exception::Exception(const char* message) {
  this->message = new string(message);
}

Exception::Exception(const string& message) {
  this->message = new string(message);
}

Exception::Exception(const Exception& exception) {
  this->message = new string(exception.getMessage());
}

const string& Exception::getMessage() const {
  return *message;
}

void Exception::debug() const {
  cout << "CLASS/Exception\n"
       << "  message=" << message << "\n";
}

Exception::~Exception() {
  delete message;
}
