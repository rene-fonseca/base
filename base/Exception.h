/***************************************************************************
    begin       : Fri May 12 2000
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _EXCEPTION_H
#define _EXCEPTION_H

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
    Writes a string representation of this object to a stream. This method is intended to be used for debugging purposes.
  */
  void debug() const;

  /**
    Free exception object.
  */
  virtual ~Exception();
};

/** Thrown on value greater than maximum valid value. */
class Overflow : public Exception {};
/** Thrown on value less than minimum valid value. */
class Underflow : public Exception {};

/** Thrown on IO error. */
class IOException : public Exception {};

/** Thrown if required resource could not be found. */
class NotFoundException : public Exception {};

/** Thrown on object construction error. */
class Construct : public Exception {};

/** Thrown on resource allocation/deallocation error. */
class ResourceException : public Exception {};
/** Thrown on memory allocation/deallocation error. */
class MemoryException : public ResourceException {};

#endif
