/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Exception::Exception() throw() : message(0) {
}

Exception::Exception(const char* _message) throw() : message(_message) {
}

Exception::Exception(Type _type) throw() : message(0), type(_type) {
}

Exception::Exception(const char* _message, Type _type) throw() : message(_message), type(_type) {
}

Exception::Exception(const Exception& copy) throw() : message(copy.message), type(copy.type) {
}

const char* Exception::getMessage() const throw() {
  return message;
}

Type Exception::getType() const throw() {
  return type;
}

Exception::~Exception() throw() {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
