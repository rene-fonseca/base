/***************************************************************************
    copyright            : (C) 2000 by René Møller Fonseca
    email                : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTED_OBJECT_WRAPPER_H
#define _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTED_OBJECT_WRAPPER_H

#include <base/mem/ReferenceCountedObject.h>
#include <base/mem/NullPointer.h>

/**
  This is a wrapper for a Reference Counted Object. Use this class if you need
  to reference count an object of a class that is not a subclass of
  ReferenceCountedObject. Allocate objects on the heap not the stack.

  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE>
class ReferenceCountedObjectWrapper : public ReferenceCountedObject {
public:

  /** Pointer to objcet. */
  typedef TYPE* Pointer;
  /** Reference to object. */
  typedef TYPE& Reference;
private:

  /** Ordinary object. */
  Pointer obj;
public:

  /**
    Initializes the wrapper.

    @param value The object to be reference counted.
  */
  inline ReferenceCountedObjectWrapper(Pointer value) : obj(value) {}

  /**
    Returns the object.
  */
  inline Pointer operator->() throw() {return obj;}

  /**
    Returns the object.
  */
  inline Reference operator*() throw(NullPointer) {
    if (!obj) {
      throw NullPointer();
    }
    return *obj;
  }

  /**
    Type cast to Pointer.
  */
  inline operator Pointer() const {
    return obj;
  }

  /**
    Destroys the wrapper.
  */
  inline ~ReferenceCountedObjectWrapper() {delete obj;}
};

#endif
