/***************************************************************************
    copyright            : (C) 2000 by Ren� M�ller Fonseca
    email                : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__OWNERSHIP_POINTER_H
#define _DK_SDU_MIP__BASE_MEM__OWNERSHIP_POINTER_H

#include <base/mem/NullPointer.h>

//BEGIN_OF_BASE_NAMESPACE

/**
  Automation pointer that allows transfer of ownership and automatic deletion of the object. Do not construct more than one automation pointer from the 'normal' pointer.

  @author Ren� M�ller Fonseca
  @version 1.0
*/

  template<class TYPE>
  class OwnershipPointer {
  public:

    /** Object type. */
    typedef TYPE Value;
    /** Pointer to objcet type. */
    typedef TYPE* Pointer;
    /** Reference to object type. */
    typedef TYPE& Reference;
  private:

    /** Pointer to object. */
    Pointer ptr;
  public:

    /**
      Initializes the ownership pointer.

      @param value The pointer to be automated.
    */
    explicit inline OwnershipPointer(Pointer value = NULL) throw() : ptr(value) {}

    /**
      Copy constructor. Transfers ownership from copy to this object (copy loses ownership).
    */
    inline OwnershipPointer(OwnershipPointer& copy) throw() : ptr(copy.relinquishOwnership()) {}

    /**
      Assignment operator.
    */
    inline OwnershipPointer& operator=(OwnershipPointer& obj) throw() {
      if (&obj != this) { // protect against self assignment
        delete ptr;
        ptr = obj.relinquishOwnership();
      }
      return *this;
    }

    /**
      Assignment operator.
    */
    template<class POLY>
    inline OwnershipPointer& operator=(OwnershipPointer<POLY>& obj) throw() {
      if (obj.getValue() != this->getValue()) { // protect against self assignment
        delete ptr;
        ptr = obj.relinquishOwnership();
      }
      return *this;
    }

    /**
      Assignment operator.
    */
    inline OwnershipPointer& operator=(Pointer value) {
      delete ptr;
      ptr = value;
      return *this;
    }

    /**
      Makes the automation pointer relinquish its ownership. Please note that the pointer may not have the ownership to begin with. Throws any exceptions that the destructor of the real object throws.

      @return Pointer to object.
    */
    inline Pointer relinquishOwnership() {
      Pointer temp = ptr;
      ptr = NULL;
      return temp;
    }

    /**
       Returns the pointer to mutable object.
    */
    inline Pointer getValue() throw() {
      return ptr;
    }

    /**
       Returns the pointer to constant object.
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
      Destroys the ownership pointer.
    */
    ~OwnershipPointer() throw() {
      delete ptr;
    }
  };

//END_OF_BASE_NAMESPACE

#endif
