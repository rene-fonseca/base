/***************************************************************************
    copyright            : (C) 2000 by Ren� M�ller Fonseca
    email                : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTED_OBJECT_POINTER_H
#define _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTED_OBJECT_POINTER_H

#include "ReferenceCountedObject.h"
#include "NullPointer.h"
#include "base/Base.h"

/**
  This class is only introduced to give the general
  ReferenceCountedObjectPointer template class access to the
  ReferenceCountedObject. You should not used this class directly.

  @see ReferenceCountedObjectPointer
  @author Ren� M�ller Fonseca
  @version 1.02
*/

class ReferenceCountedObjectFriend {
private:

  /** Type of pointer to reference counted object. */
  typedef const ReferenceCountedObject* Pointer;
  /** Pointer to shared reference counted object. */
  Pointer ptr; // protect pointer value from the evil programmers
public:

  /**
    Initializes automation pointer with the specified pointer value.
    Implicit initialization is allowed.

    @param value The desired value. Default is NULL.
  */
  inline ReferenceCountedObjectFriend(Pointer value = NULL) : ptr(value) {
    if (ptr) {
      ++ptr->references; // add reference
    }
  }

  /**
    Initialization of automation pointer from other automation pointer.
  */
  inline ReferenceCountedObjectFriend(const ReferenceCountedObjectFriend& copy) : ptr(copy.ptr) {
    if (ptr) {
      ++ptr->references; // add reference
    }
  }

  /**
    Returns the pointer value of this automation pointer.
  */
  inline Pointer getValue() const throw() {return ptr;};

  /**
    Sets the pointer value of this automation pointer.
  */
  inline void setValue(Pointer value) {
    if (ptr) { // skip if NULL pointer
      if (--ptr->references) { // remove reference
        delete ptr; // could throw exception if RCO is destroyed unsuccessfully
      }
    }
    ptr = value;
    if (ptr) { // skip if NULL pointer
      ++ptr->references; // add reference
    }
  }

  /**
    Returns true if the reference counted object only has one reference.
  */
  inline bool singleReference() throw() {
    return (!ptr) || (ptr->references <= 1); // false if NULL pointer
  }

  /**
    Destroys this automation pointer.
  */
  inline ~ReferenceCountedObjectFriend() {
    if (ptr) { // skip if NULL pointer
      if (--ptr->references) { // remove reference
        delete ptr; // could throw exception if object is destroyed unsuccessfully
        ptr = NULL;
      }
    }
  }
};



/**
  Automation pointer for reference counted objects. This class is responsible
  for counting the total number of references to an object. The pointer
  automatically deletes a reference counted object when the number of
  references reaches zero.

  @short Automation pointer that counts the number of references to an object.
  @see ReferenceCountedObject
  @author Ren� M�ller Fonseca
  @version 1.02
*/

template<class TYPE>
class ReferenceCountedObjectPointer : private ReferenceCountedObjectFriend {
public:

  /** Type of pointer to reference counted objcet. */
  typedef TYPE* Pointer;
  /** Type of reference to reference counted object. */
  typedef TYPE& Reference;

  /**
    Initializes automation pointer with the specified pointer value. Object
    may be implicitly initialized.

    @param value The desired value. Default is NULL.
  */
  inline ReferenceCountedObjectPointer(Pointer value = NULL) : ReferenceCountedObjectFriend(value) {};

  /**
    Initialization of automation pointer from other automation pointer.
  */
  inline ReferenceCountedObjectPointer(const ReferenceCountedObjectPointer& copy) : ReferenceCountedObjectFriend(copy) {};

  /**
    Initialization of automation pointer from other automation pointer using compile time polymorphism.
  */
  template<class POLY>
  inline ReferenceCountedObjectPointer(const ReferenceCountedObjectPointer<POLY>& copy) : ReferenceCountedObjectFriend(down_cast<Pointer>(copy.getValue())) {};

  /**
    Assignment of automation pointer to this automation pointer using compile time polymorphism.
  */
  inline ReferenceCountedObjectPointer& operator=(const ReferenceCountedObjectPointer& eq) {
    if (&eq != this) { // protect against self assignment
      setValue(eq.getValue());
    }
    return *this;
  }

  /**
    Assignment of automation pointer to this automation pointer.
  */
  template<class POLY>
  inline ReferenceCountedObjectPointer& operator=(const ReferenceCountedObjectPointer<POLY>& eq) {
    Pointer p = down_cast<Pointer>(eq.getValue());
    if (p != getValue()) { // protect against self assignment
      setValue(p);
    }
    return *this;
  }

  /**
    Returns the pointer value of this automation pointer.
  */
  inline Pointer getValue() const throw() {return static_cast<Pointer>(const_cast<ReferenceCountedObject*>(ReferenceCountedObjectFriend::getValue()));};

  /**
    Returns the reference counted object.
  */
  inline Reference operator*() const throw(NullPointer) {
    if (!getValue()) {
      throw NullPointer();
    }
    return *getValue();
  }

  /**
    Returns the reference counted object.
  */
  inline Pointer operator->() const throw() {
    return getValue();
  }
};

#endif
