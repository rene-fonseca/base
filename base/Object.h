/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_OBJECT_H
#define _BASE_OBJECT_H

#include "Copyable.h"
#include "Comparable.h"

#include <string>
#include <iostream>

using std::string;
using std::ostream;

/**
  The base class of all non-abstract classes. Objects are not copyable as default.

  @author René Møller Fonseca
  @version 1.0
*/

class Object {
private:

  /* Disable the default copy constructor. */
//  Object(const Object& object);
  /* Disable the default assignment operator. */
//  Object& operator=(const Object& object);
public:

  /**
    Initializes object.
  */
  Object() {};

  /**
    Destroys the object. Ensure envocation of correct destructor when using polymophism.
  */
  virtual ~Object() = 0;
};



/**
  This operator allocates memory for an object on the heap. Throws an exception if unable to allocate object.

  @param size The size of the object.
  @return The object.
*/
//void* operator new(size_t size) throw(MemoryException);

/**
  This operator releases memeory used by an object.

  @param p The object to be released.
*/
//void operator delete(void* p);



/*
  This routine writes a string representation of the object to a stream.

  @param stream The stream the object should be written to.
  @param object The object to be written to the stream.
  @return The specified stream.
*/
#define TOSTRING(TYPE) ostream& operator<<(ostream& stream, const TYPE& object) {return object.toString(stream);}

#endif
