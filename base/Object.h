/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__OBJECT_H
#define _DK_SDU_MIP__BASE__OBJECT_H

#include <base/features.h>
#include <base/Functor.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  The base class of all non-abstract classes. Objects are not copyable by default.

  @author René Møller Fonseca
  @version 1.0
*/

class Object {
private:

  /* Disable the default copy constructor. */
  Object(const Object& copy);
  /* Disable the default assignment operator. */
  Object& operator=(const Object& eq);
public:

  /**
    Initializes object.
  */
  inline Object() throw() {}
};

template<>
inline bool isRelocateable<Object>() throw() {return true;}



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

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
