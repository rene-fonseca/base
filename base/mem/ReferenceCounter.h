/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTER_H
#define _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTER_H

#include <base/mem/NullPointer.h>
#include <base/Base.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Automation pointer for reference counted objects. This class is responsible
  for counting the total number of references to an object. The pointer
  automatically deletes a reference counted object when the number of
  references reaches zero. If possible, you should use the
  CombinedReferenceCounter in preference to this class. ReferenceCounter is a
  bit slower than CombinedReferenceCounter but works for any class being
  reference counted.

  <pre>
  ReferenceCounter<MyClass> myObject = new MyClass();
  ReferenceCounter<MyClass> myOtherObject = myObject;
  </pre>

  @short Automation pointer that counts the number of references to an object.
  @see CombinedReferenceCounter
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TYPE>
class ReferenceCounter {
public:

  /** The type of the reference counted object. */
  typedef TYPE Value;
  /** The type of the reference to the reference counted object. */
  typedef TYPE& Reference;
  /** The type of the pointer to the reference counted object. */
  typedef TYPE* Pointer;
  /** The type of the reference counter. */
  typedef long Counter; // out of memory before counter overflow
private:

  /** Pointer to shared reference counted object. */
  Pointer ptr; // protect pointer value from the evil programmers
  /** Holds the total number of references. */
  Counter* references;
public:

  /**
    Initializes automation pointer as invalid (i.e. null).
  */
  inline ReferenceCounter() throw() : ptr(0), references(new Counter(1)) {}

  /**
    Initializes automation pointer with the specified pointer value. The
    automation pointer may be implicitly initialized.

    <pre>
    ReferenceCounter<MyClass> rcop = new MyClass();
    </pre>

    @param value The desired pointer value.
  */
  inline ReferenceCounter(Pointer value) throw() : ptr(value), references(new Counter(1)) {}

  /**
    Initialization of automation pointer from other automation pointer.
  */
  inline ReferenceCounter(const ReferenceCounter& copy) : ptr(copy.ptr), references(copy.references) {
    ++*references;
  }

  /**
    Initialization of automation pointer from other automation pointer using
    compile time polymorphism.
  */
  template<class POLY>
  inline ReferenceCounter(const ReferenceCounter<POLY>& copy) : ptr(copy.ptr), references(copy.references) {
    ++*references;
  }

  /**
    Assignment of normal pointer to this automation pointer.
  */
  inline ReferenceCounter& operator=(Pointer eq) throw() {
    if (!--*references) { // remove reference and possible destroy object
      if (ptr) { // skip if pointer is invalid
        delete ptr; // could throw exception if RCO is destroyed unsuccessfully
      }
      ++*references; // one reference
    } else {
      references = new Counter(1); // one reference
    }
    ptr = eq;
    return *this;
  }

  /**
    Assignment of automation pointer to this automation pointer.
  */
  inline ReferenceCounter& operator=(const ReferenceCounter& eq) {
    if (&eq != this) { // protect against self assignment
      if (!--*references) { // remove reference and possible destroy object
        if (ptr) { // skip if pointer is invalid
          delete ptr; // could throw exception if RCO is destroyed unsuccessfully
        }
        delete references;
      }
      ptr = eq.ptr;
      references = eq.references;
      ++*references; // add reference
    }
    return *this;
  }

  /**
    Assignment of automation pointer to this automation pointer using compile
    time polymorphism.
  */
  template<class POLY>
  inline ReferenceCounter& operator=(const ReferenceCounter<POLY>& eq) {
    ASSERT(&eq != this); // no need to protect against self assignment
    if (!--*references) { // remove reference and possible destroy object
      if (ptr) { // skip if pointer is invalid
        delete ptr; // could throw exception if RCO is destroyed unsuccessfully
      }
      delete references;
    }
    ptr = eq.ptr;
    references = eq.references;
    ++*references; // add reference
    return *this;
  }

  /**
    Returns the pointer value of this automation pointer. Be careful when
    utilizing this member function. Its completely up to you to ensure that
    the reference counting rules are not violated.
  */
  inline Pointer getValue() const throw() {
    return ptr;
  }

  /**
    Sets the pointer value of this automation pointer.
  */
  inline void setValue(Pointer value) {
    if (!--*references) { // remove reference and possible destroy object
      if (ptr) { // skip if pointer is invalid
        delete ptr; // could throw exception if RCO is destroyed unsuccessfully
      }
      ++*references; // one reference
    } else {
      references = new Counter(1); // one reference
    }
    ptr = value;
    return *this;
  }

  /**
    Returns true if the reference counted object is referenced by more than
    one automation pointer.

    @return False if the pointer is invalid (i.e. not pointing to an object).
  */
  inline bool isMultiReferenced() const throw() {
    return (ptr) && (*references > 1); // false if the pointer is invalid
  }

  /**
    Makes a new copy of the reference counted object if referenced by more than
    one automation pointer. This member function is invocated by some classes
    before a object is modified. The reference counted object must implement
    the default copy constructor for this to work.
  */
  inline void copyOnWrite() throw() {
    if (isMultiReferenced()) { // do we have the object for our self
      --*references; // remove one reference (no need to delete object since multi-referenced)
      ptr = new Value(*ptr);
      references = new Counter(1); // one reference
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
    assert(ptr, NullPointer(this));
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
  inline ~ReferenceCounter() {
    if (!--*references) { // remove reference
      if (ptr) { // skip if pointer is invalid
        delete ptr; // could throw exception if RCO is destroyed unsuccessfully
      }
      delete references;
    }
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
