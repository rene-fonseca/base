/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__OBJECT_H
#define _DK_SDU_MIP__BASE__OBJECT_H

#include <config.h>

/**
  The base class of all non-abstract classes. Objects are not copyable as default.

  @author René Møller Fonseca
  @version 1.0
*/

class Object {
private:

  /* Disable the default copy constructor. */
  inline Object(const Object& copy) {}
  /* Disable the default assignment operator. */
  inline Object& operator=(const Object& eq) {return *this;}
public:

  /**
    Initializes object.
  */
  inline Object() throw() {}

  /**
    Destroys the object. Ensure envocation of correct destructor when using polymorphism.
  */
  virtual ~Object() throw();
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

#endif
