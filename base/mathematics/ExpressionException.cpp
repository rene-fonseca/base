/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/mathematics/ExpressionException.h>

ExpressionException::ExpressionException() throw() : index(0) {
}

ExpressionException::ExpressionException(const char* message) throw() : Exception(message), index(0) {
}

ExpressionException::ExpressionException(unsigned int i, const char* message) throw() : Exception(message), index(i) {
}

unsigned int ExpressionException::getIndex() const throw() {
  return index;
}
