/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_EXCEPTION_H
#define _BASE_EXCEPTION_H

#include "Object.h"
#include <string>

using std::string;

/**
  This class fundamental exception.

  @author René Møller Fonseca
  @version 1.0
*/

class Exception : public Object {
private:

  /**
    Message associated with the exception.
  */
  string* message;
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
    Initializes the exception object.

    @param message The message.
  */
  Exception(const string& message);

  /**
    Copy constructor.

    @param exception The exception object to be copied.
  */
  Exception(const Exception& exception);

  /**
    Returns the message associated with the exception.

    @return The message associated with the exception.
  */
  const string& getMessage() const;

  /**
    Writes a string representation of the object to a stream.
  */
  ostream& toString(ostream& stream) const;

  /**
    Free exception object.
  */
  virtual ~Exception();
};

#endif
