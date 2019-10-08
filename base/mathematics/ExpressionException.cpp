/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/mathematics/ExpressionException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

ExpressionException::ExpressionException() throw() : index(0) {
}

ExpressionException::ExpressionException(const char* message) throw()
  : Exception(message), index(0) {
}

ExpressionException::ExpressionException(
  unsigned int _index,
  const char* message) throw()
  : Exception(message), index(_index) {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
