/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MATHEMATICS__EXPRESSION_EXCEPTION_H
#define _DK_SDU_MIP__BASE_MATHEMATICS__EXPRESSION_EXCEPTION_H

#include <base/Exception.h>

/**
  This exception is thrown by ExpressionParser on errors in an expression
  representation.

  @see ExpressionParser
  @author René Møller Fonseca
  @version 1.0
*/

class ExpressionException : public Exception {
protected:

  /** Specifies the index the error in the expression representation. */
  unsigned int index;
public:

  /**
    Initializes the exception with no message.
  */
  ExpressionException() throw();

  /**
    Initializes the exception with the specified message.

    @param message The message.
  */
  ExpressionException(const char* message) throw();

  /**
    Initializes the exception with the specified index and message.

    @param index The index of the error.
    @param message The message.
  */
  ExpressionException(unsigned int index, const char* message) throw();

  /**
    Returns the index of the error in the expression representation.
  */
  unsigned int getIndex() const throw();
};

#endif
