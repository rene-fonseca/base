/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__EXCEPTION_H
#define _DK_SDU_MIP__BASE__EXCEPTION_H

#include "Object.h"

/**
  This class fundamental exception.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

class Exception : public Object {
private:

  /**
    Message associated with the exception.
  */
  const char* message;
public:

  /**
    Initializes the exception object with no message.
  */
  Exception();

  /**
    Initializes the exception object.

    @param message The message.
  */
  Exception(const char* message);

  /**
    Copy constructor.

    @param exception The exception object to be copied.
  */
  Exception(const Exception& exception);

  /**
    Returns the message associated with the exception.

    @return The message associated with the exception.
  */
  const char* getMessage() const;

  /**
    Destroys exception object.
  */
  ~Exception();
};

#endif
