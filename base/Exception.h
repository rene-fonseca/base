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

/** Thrown on value greater than maximum valid value. */
class Overflow : public Exception {};
/** Thrown on value less than minimum valid value. */
class Underflow : public Exception {};

/* Exception used to specify that an index is out of range. */
class OutOfRange : public Exception {};
/* Out of bounds exception. */
class OutOfBounds : public Exception {};
/** Exception used to specify that a variable has been assigned a value not in the variables domain. */
class OutOfDomain : public Exception {};

/** Thrown on IO error. */
class IOException : public Exception {};

/** Thrown if required resource could not be found. */
class NotFoundException : public Exception {};

/** Thrown on object construction error. */
class Construct : public Exception {};
/** Thrown on object deconstruction error. Used to detect resource leaks. */
class Destruct : public Exception {};

/** Thrown on resource allocation/deallocation error. */
class ResourceException : public Exception {};
/** Thrown on memory allocation/deallocation error. */
class MemoryException : public ResourceException {};
/** Thrown on device access error. */
class DeviceException : public ResourceException {};

#endif
