/***************************************************************************
    copyright            : (C) 2000 by René Møller Fonseca
    email                : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTED_OBJECT_POINTER_H
#define _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTED_OBJECT_POINTER_H

#include "NullPointer.h"
#include "ReferenceCountedObject.h"

/**
  Automation pointer to Reference Counted Object. This pointer automatically deletes a Reference Counted Object when the number of reference reaches zero. Do not construct more than one automation pointer from the 'normal' pointer.

  @author René Møller Fonseca
  @version 1.0
*/

  template<class TYPE>
  class ReferenceCountedObjectPointer {
  public:

    /** Type of object. */
    typedef TYPE Value;
    /** Pointer to objcet. */
    typedef TYPE* Pointer;
    /** Reference to object. */
    typedef TYPE& Reference;
  private:

    /** Pointer to shared Reference Counted Object. */
    Pointer ptr;
    /** Sets the pointer. */
    inline void setValue(Pointer value) {
      if (ptr) {
        if (ptr->removeReferenceToObject()) {
          delete ptr; // could throw exception if RCO is destroyed unsuccessfully
        }
      }
      ptr = value;
      if (ptr) {
        ptr->addReferenceToObject();
      }
    }
  public:

    /**
      Initializes the pointer. NULL-pointer is default.

      @param value The desired value.
    */
    explicit inline ReferenceCountedObjectPointer(Pointer value = NULL) : ptr(value) {
      if (ptr) {
        ptr->addReferenceToObject();
      }
    }

    /**
      Copy constructor.
    */
    inline ReferenceCountedObjectPointer(const ReferenceCountedObjectPointer& copy) : ptr(copy.ptr) {
      if (ptr) {
        ptr->addReferenceToObject();
      }
    }

    /**
      Assignment operator.
    */
    inline ReferenceCountedObjectPointer& operator=(const ReferenceCountedObjectPointer& obj) {
      if (&obj != this) { // protect against self assignment
        setValue(obj.getValue());
      }
      return *this;
    }

    /**
      Assignment operator.
    */
    template<class POLY>
    inline ReferenceCountedObjectPointer& operator=(const ReferenceCountedObjectPointer<POLY>& obj) {
      if (obj.getValue() != getValue()) { // protect against self assignment
        setValue(obj.getValue());
      }
      return *this;
    }

    /**
      Assignment operator.
    */
    inline ReferenceCountedObjectPointer& operator=(Pointer value) {
      if (value != getValue()) {
        setValue(value);
      }
      return *this;
    }

    /**
      Forces a copy to be made of the reference counted object if the object is referenced more than once.
    */
    inline void ensureSingleReference() {
      if (ptr) {
        if (ptr->getReferencesToObject() > 1) {
          setValue(new TYPE(*ptr));
        }
      }
    }

    /**
      Returns pointer to mutable object.
    */
    inline Pointer getValue() throw() {
      return ptr;
    }

    /**
      Returns pointer to constant object.
    */
    inline const Pointer getValue() const throw() {
      return ptr;
    }

    /**
      Returns mutable object.
    */
    inline Reference operator*() throw(NullPointer) {
      if (!ptr) {
        throw NullPointer();
      }
      return *ptr;
    }

    /**
      Returns constant object.
    */
    inline const Reference operator*() const throw(NullPointer) {
      if (!ptr) {
        throw NullPointer();
      }
      return *ptr;
    }

    /**
      Returns mutable object.
    */
    inline Pointer operator->() throw() {
      return ptr;
    }

    /**
      Returns constant object.
    */
    inline const Pointer operator->() const throw() {
      return ptr;
    }

    /**
      Type cast to Pointer.
    */
    inline operator Pointer() throw() {
      return ptr;
    }

    /**
      Type cast to const Pointer.
    */
    inline operator const Pointer() const throw() {
      return ptr;
    }

    /**
      Destroys the pointer.
    */
    inline ~ReferenceCountedObjectPointer() {
      if (ptr) { // skip NULL-pointer
        if (ptr->removeReferenceToObject()) { // should we destroy the object
          delete ptr; // could throw exception if object is destroyed unsuccessfully
        }
      }
    }
  };

#endif
