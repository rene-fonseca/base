/***************************************************************************
    begin       : Fri May 12 2000
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _OBJECT_H
#define _OBJECT_H

#include <string>
#include <ostream>

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
    Writes the object to standard out. This method is intended to be used for debugging purposes.
  */
  virtual void debug() const;

  /**
    Destroys the object.
  */
  virtual ~Object() = 0;
};



/**
  This operator allocates memeory for an object on the heap.

  @param size The size of the object.
  @return The object.
*/
void* operator new(size_t size);

/**
  This operator releases memeory used by an object.

  @param p The object to be released.
*/
void operator delete(void* p);



/**
  This routine writes an object to a stream.

  @param stream The stream the object should be written to.
  @param object The object to be written to the stream.
  @return The specified stream.
*/
ostream& operator<<(ostream& stream, Object& object);

#endif
