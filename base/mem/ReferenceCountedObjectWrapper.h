/***************************************************************************
    copyright            : (C) 2000 by René Møller Fonseca
    email                : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTED_OBJECT_WRAPPER_H
#define _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTED_OBJECT_WRAPPER_H

#include "ReferenceCountedObject.h"
#include "NullPointer.h"

/**
  This is a wrapper for a Reference Counted Object. Use this class if you need to reference count an object that is not itself a Reference Counted Object. Allocate objects on the heap not the stack.

  @author René Møller Fonseca
  @version 1.0
*/

  template<class TYPE>
  class ReferenceCountedObjectWrapper : public ReferenceCountedObject {
  public:

    /** Type of object. */
    typedef TYPE Value;
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

    inline Pointer operator->() throw() {return obj;}

    inline Reference operator*() throw(NullPointer) {
      if (!obj) {
        throw NullPointer();
      }
      return *obj;
    }

    /** Type cast to Pointer. */
    inline operator Pointer() {
      return obj;
    }

    /** Type cast to const Pointer. */
    inline operator const Pointer() const {
      return obj;
    }

    /**
      Destroys the wrapper.
    */
    inline ~ReferenceCountedObjectWrapper() {delete obj;}
  };

#endif
