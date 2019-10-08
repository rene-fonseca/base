/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/mem/NullPointer.h>
#include <base/Base.h>
#include <atomic>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Automation pointer for reference counted objects. This class is responsible
  for counting the total number of references to an object. The pointer
  automatically deletes a reference counted object when the number of
  references reaches zero. If possible, you should use the
  CombinedReferenceCounter in preference to this class. ReferenceCounter is a
  bit slower than CombinedReferenceCounter but works for any class being
  reference counted.

  @code
  ReferenceCounter<MyClass> myObject = new MyClass();
  ReferenceCounter<MyClass> myOtherObject = myObject;
  @endcode

  @short Automation pointer that counts the number of references to an object.
  @ingroup memory
  @see CombinedReferenceCounter
  @version 1.0
*/

template<class TYPE>
class _DK_SDU_MIP__BASE__API ReferenceCounter {
public:

  /** The type of the reference counter. */
  typedef std::atomic<MemorySize> Counter; // out of memory before counter overflow
private:

  /** Pointer to shared reference counted object. */
  TYPE* value = nullptr; // protect pointer value from the evil programmers
  /** Holds the total number of references. */
  Counter* references = nullptr;
public:

  /**
    Initializes automation pointer as invalid (i.e. null).
  */
  inline ReferenceCounter() throw() : references(new Counter(1)) {
  }

  /**
    Initializes automation pointer with the specified pointer value. The
    automation pointer may be implicitly initialized.

    @code
    ReferenceCounter<MyClass> rcop = new MyClass();
    @endcode

    @param value The desired pointer value.
  */
  inline ReferenceCounter(TYPE* _value) throw()
    : value(_value), references(new Counter(1)) {
  }

  /**
    Initialization of automation pointer from other automation pointer.
  */
  inline ReferenceCounter(const ReferenceCounter& copy) throw()
    : value(copy.value), references(copy.references) {
    ++*references;
  }

  /**
    Initialization of automation pointer from other automation pointer using
    compile time polymorphism.
  */
  template<class POLY>
  inline ReferenceCounter(const ReferenceCounter<POLY>& copy) throw()
    : value(copy.value), references(copy.references) {
    ++*references;
  }

  /**
    Assignment of normal pointer to this automation pointer.
  */
  inline ReferenceCounter& operator=(TYPE* eq) throw() {
    if (!--*references) { // remove reference and possible destroy object
      if (value) { // skip if pointer is invalid
        delete value; // could throw exception if RCO is destroyed unsuccessfully
      }
      ++*references; // one reference
    } else {
      references = new Counter(1); // one reference
    }
    value = eq;
    return *this;
  }

  /**
    Assignment of automation pointer to this automation pointer.
  */
  inline ReferenceCounter& operator=(const ReferenceCounter& eq) /*throw(...)*/ {
    if (&eq != this) { // protect against self assignment
      if (!--*references) { // remove reference and possible destroy object
        if (value) { // skip if pointer is invalid
          delete value; // could throw exception if RCO is destroyed unsuccessfully
        }
        delete references;
      }
      value = eq.value;
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
  inline ReferenceCounter& operator=(const ReferenceCounter<POLY>& eq) /*throw(...)*/ {
    ASSERT(&eq != this); // no need to protect against self assignment
    if (!--*references) { // remove reference and possible destroy object
      if (value) { // skip if pointer is invalid
        delete value; // could throw exception if RCO is destroyed unsuccessfully
      }
      delete references;
    }
    value = eq.value;
    references = eq.references;
    ++*references; // add reference
    return *this;
  }

  /**
    Returns the pointer value of this automation pointer. Be careful when
    utilizing this member function. Its completely up to you to ensure that
    the reference counting rules are not violated.
  */
  inline TYPE* getValue() const throw() {
    return value;
  }

  /**
    Sets the pointer value of this automation pointer.
  */
  inline void setValue(TYPE* _value) /*throw(...)*/ {
    if (!--*references) { // remove reference and possible destroy object
      if (value) { // skip if pointer is invalid
        delete value; // could throw exception if RCO is destroyed unsuccessfully
      }
      ++*references; // one reference
    } else {
      references = new Counter(1); // one reference
    }
    value = _value;
    return *this;
  }

  /**
    Returns true if the reference counted object is referenced by more than
    one automation pointer.

    @return False if the pointer is invalid (i.e. not pointing to an object).
  */
  inline bool isMultiReferenced() const throw() {
    return value && (*references > 1); // false if the pointer is invalid
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
      value = new TYPE(*value);
      references = new Counter(1); // one reference
    }
  }

  /**
    Invalidates the reference.
  */
  inline void invalidate() /*throw(...)*/ {
    if (value) { // skip if pointer is invalid
      if (--*references) {
        delete value;
      }
    }
    value = nullptr;
  }
  
  /**
    Returns true if the automation pointer is valid (i.e. it is pointing to an
    object).
  */
  inline bool isValid() const throw() {
    return value != nullptr;
  }

  /**
    Returns the reference counted object.
    
    @deprecated
  */
  inline TYPE& operator*() const throw(NullPointer) {
    bassert(value, NullPointer(this));
    return *value;
  }

  /**
    Returns the reference counted object.
  */
  inline TYPE* operator->() const throw() {
    return value;
  }

  /**
    Destroys the automation pointer.
  */
  inline ~ReferenceCounter() {
    if (!--*references) { // remove reference
      if (value) { // skip if pointer is invalid
        delete value; // could throw exception if RCO is destroyed unsuccessfully
      }
      delete references;
    }
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
