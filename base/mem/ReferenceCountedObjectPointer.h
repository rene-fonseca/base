/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTED_OBJECT_POINTER_H
#define _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTED_OBJECT_POINTER_H

#include <base/mem/ReferenceCountedObject.h>
#include <base/mem/NullPointer.h>
#include <base/Base.h>
#include <base/Functor.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class is only introduced to give the general
  ReferenceCountedObjectPointer template class access to the
  ReferenceCountedObject. You should not used this class directly.

  @see ReferenceCountedObjectPointer
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.1
*/

class ReferenceCountedObjectPointerImpl {
public:

  /**
    Returns the number of references for the specified reference counted object.

    @param ptr Pointer to the reference counted object.
  */
  inline unsigned long& getReferenceCounter(const ReferenceCountedObject* ptr) const throw() {
    return ptr->references;
  }
};



/**
  Automation pointer for reference counted objects. This class is responsible
  for counting the total number of references to an object. The pointer
  automatically deletes a reference counted object when the number of
  references reaches zero. This pointer can only be used to reference count
  objects of type ReferenceCountedObject but is faster than ReferenceCounter.

  @short Automation pointer that counts the number of references to an object.
  @see ReferenceCountedObject ReferenceCounter
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.2
*/

template<class TYPE>
class ReferenceCountedObjectPointer : private ReferenceCountedObjectPointerImpl {
public:

  /** The type of the reference counted object. */
  typedef TYPE Value;
  /** The type of the reference to the reference counted object. */
  typedef TYPE& Reference;
  /** The type of the pointer to the reference counted object. */
  typedef TYPE* Pointer;
private:

  /** Pointer to shared reference counted object. */
  Pointer ptr; // protect pointer value from the evil programmers
public:

  /**
    Initializes an automation pointer as invalid (i.e. null).
  */
  inline ReferenceCountedObjectPointer() throw() : ptr(0) {}

  /**
    Initializes an automation pointer with the specified pointer value. The
    automation pointer may be implicitly initialized.

    <pre>
    ReferenceCountedObjectPointer<MyClass> rcop = new MyClass();
    </pre>

    @param value The desired pointer value.
  */
  inline ReferenceCountedObjectPointer(Pointer value) throw() : ptr(value) {
    if (ptr) {
      ++getReferenceCounter(ptr);
    }
  }

  /**
    Initialization of automation pointer from other automation pointer.
  */
  inline ReferenceCountedObjectPointer(const ReferenceCountedObjectPointer& copy) : ptr(copy.ptr) {
    if (ptr) {
      ++getReferenceCounter(ptr);
    }
  }

  /**
    Initialization of automation pointer from other automation pointer using
    compile time polymorphism.
  */
  template<class POLY>
  inline ReferenceCountedObjectPointer(const ReferenceCountedObjectPointer<POLY>& copy) : ptr(copy.ptr) {
    if (ptr) {
      ++getReferenceCounter(ptr);
    }
  }

  /**
    Assignment of normal pointer to this automation pointer.
  */
  inline ReferenceCountedObjectPointer& operator=(Pointer eq) throw() {
    setValue(eq);
    return *this;
  }

  /**
    Assignment of automation pointer to this automation pointer.
  */
  inline ReferenceCountedObjectPointer& operator=(const ReferenceCountedObjectPointer& eq) {
    if (&eq != this) { // protect against self assignment
      setValue(eq.ptr);
    }
    return *this;
  }

  /**
    Assignment of automation pointer to this automation pointer using compile
    time polymorphism.
  */
  template<class POLY>
  inline ReferenceCountedObjectPointer& operator=(const ReferenceCountedObjectPointer<POLY>& eq) {
    ASSERT(&eq != this); // no need to protect against self assignment
    setValue(eq.ptr);
    return *this;
  }

  /**
    Returns the pointer value of this automation pointer. Be careful when
    utilizing this member function. Its completely up to you to ensure that
    the reference counting rules aren't violated.
  */
  inline Pointer getValue() const throw() {
    return ptr;
  }

  /**
    Sets the pointer value of this automation pointer.
  */
  inline void setValue(Pointer value) {
    if (ptr) { // skip if pointer is invalid
      if (!--getReferenceCounter(ptr)) { // remove reference and possible destroy object
        delete ptr; // could throw exception if RCO is destroyed unsuccessfully
      }
    }
    ptr = value;
    if (ptr) { // skip if pointer is invalid
      ++getReferenceCounter(ptr); // add reference
    }
  }

  /**
    Returns true if the reference counted object is referenced by more than
    one automation pointer.

    @return False if the pointer is invalid (i.e. not pointing to an object).
  */
  inline bool isMultiReferenced() const throw() {
    return (ptr) && (getReferenceCounter(ptr) > 1); // false if the pointer is invalid
  }

  /**
    Makes a new copy of the reference counted object if referenced by more than
    one automation pointer. This member function is invocated by some classes
    before a object is modified. The reference counted object must implement
    the default copy constructor for this to work.
  */
  inline void copyOnWrite() throw() {
    if (isMultiReferenced()) { // do we have the object for our self
      --getReferenceCounter(ptr); // remove one reference (no need to delete object since multi-referenced)
      ptr = new Value(*ptr);
      ++getReferenceCounter(ptr); // add reference
    }
  }

  /**
    Returns true if the automation pointer is valid (i.e. it is pointing to an
    object).
  */
  inline bool isValid() const throw() {
    return ptr != 0;
  }

  /**
    Returns the reference counted object.
  */
  inline Reference operator*() const throw(NullPointer) {
    if (!ptr) {
      throw NullPointer();
    }
    return *ptr;
  }

  /**
    Returns the reference counted object.
  */
  inline Pointer operator->() const throw() {
    return ptr;
  }

  /**
    Destroys the automation pointer.
  */
  inline ~ReferenceCountedObjectPointer() {
    if (ptr) { // skip if pointer is invalid
      if (!--getReferenceCounter(ptr)) { // remove reference
        delete ptr; // could throw exception if RCO is destroyed unsuccessfully
      }
    }
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
