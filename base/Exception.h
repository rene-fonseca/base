/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__EXCEPTION_H
#define _DK_SDU_MIP__BASE__EXCEPTION_H

/**
  The base class of all exceptions.

  @author René Møller Fonseca
  @version 1.01
*/

class Exception {
private:

  /**
    Message associated with the exception.
  */
  const char* message;
public:

  /**
    Initializes the exception object with no message.
  */
  Exception() throw();

  /**
    Initializes the exception object.

    @param message The message.
  */
  Exception(const char* message) throw();

  /**
    Copy constructor.

    @param exception The exception object to be copied.
  */
  Exception(const Exception& copy) throw();

  /**
    Returns the message associated with the exception.

    @return The message associated with the exception.
  */
  const char* getMessage() const throw();

  /**
    Destroys exception object.
  */
  virtual ~Exception() throw();
};

#endif
