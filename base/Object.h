/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_OBJECT_H
#define _BASE_OBJECT_H

#include <string>
#include <iostream>

using std::string;
using std::ostream;

/**
  The base class of all non-abstract classes.

  @author René Møller Fonseca
  @version 1.0
*/

class Object {
private:

  /**
    Copy constructor. Prevent default copying of objects.
  */
  Object(const Object& object);

  /**
    Assignment operator. Prevent default assignment of objects.
  */
  Object* operator=(const Object& object);
public:

  /**
    Initializes object.
  */
  Object() {};

  /**
    Equality operator. Compares the addresses of the objects.
  */
  virtual bool operator==(const Object& object);

  /**
    Writes a string representation of the object to a stream.
  */
//  virtual ostream& toString(ostream& stream) const = 0;

  /**
    Destroys the object.
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
