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
#include <base/mem/DynamicMemory.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class is only introduced to give the general
  ReferenceCountedObjectPointer template class access to the
  ReferenceCountedObject. You must not use this class directly.

  @see ReferenceCountedObjectPointer
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.2
*/

class ReferenceCountedObjectPointerImpl {
private:

  /** The reference counted object. */
  const ReferenceCountedObject& object;
  ReferenceCountedObjectPointerImpl(const ReferenceCountedObjectPointerImpl&) throw();
  ReferenceCountedObjectPointerImpl& operator=(const ReferenceCountedObjectPointerImpl& eq) throw();
public:

  /**
    Initializes reference to object.
  */
  inline ReferenceCountedObjectPointerImpl(const ReferenceCountedObject& _object) throw() : object(_object) {
  }

  /**
    Adds one reference to the object.
  */
  inline void addReference() const throw() {
    ++object.references;
  }

  /**
    Removes one reference to the object. The object must have at least one reference.

    @return True if all references have been released.
  */
  inline bool removeReference() const throw() {
    // ASSERT(object.references > 0);
    return --object.references == 0;
  }

  /**
    Returns true if the object has multiple references.
  */
  inline bool isMultiReferenced() const throw() {
    return object.references > 1;
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
class ReferenceCountedObjectPointer {
public:

  /** The type of the reference counted object. */
  typedef TYPE Value;
  /** The type of the reference to the reference counted object. */
  typedef TYPE& Reference;
  /** The type of the pointer to the reference counted object. */
  typedef TYPE* Pointer;
private:

  /** Pointer to shared reference counted object. */
  Pointer pointer; // protect pointer value from the evil programmers
public:

  /**
    Initializes an automation pointer as invalid (i.e. null).
  */
  inline ReferenceCountedObjectPointer() throw() : pointer(0) {}

  /**
    Initializes an automation pointer with the specified pointer value. The
    automation pointer may be implicitly initialized.

    <pre>
    ReferenceCountedObjectPointer<MyClass> rcop = new MyClass();
    </pre>

    @param value The desired pointer value.
  */
  inline ReferenceCountedObjectPointer(Pointer value) throw() : pointer(value) {
    if (pointer) {
      ReferenceCountedObjectPointerImpl(*pointer).addReference();
    }
  }

  /**
    Initialization of automation pointer from other automation pointer.
  */
  inline ReferenceCountedObjectPointer(const ReferenceCountedObjectPointer& copy) : pointer(copy.pointer) {
    if (pointer) {
      ReferenceCountedObjectPointerImpl(*pointer).addReference();
    }
  }

  /**
    Initialization of automation pointer from other automation pointer using
    compile time polymorphism.
  */
  template<class POLY>
  inline ReferenceCountedObjectPointer(const ReferenceCountedObjectPointer<POLY>& copy) : pointer(copy.pointer) {
    if (pointer) {
      ReferenceCountedObjectPointerImpl(*pointer).addReference();
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
      setValue(eq.pointer);
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
    setValue(eq.pointer);
    return *this;
  }

  /**
    Returns the pointer value of this automation pointer. Be careful when
    utilizing this member function. Its completely up to you to ensure that
    the reference counting rules aren't violated.
  */
  inline Pointer getValue() const throw() {
    return pointer;
  }

  /**
    Sets the pointer value of this automation pointer.
  */
  inline void setValue(Pointer value) {
    if (pointer) { // skip if pointer is invalid
      if (ReferenceCountedObjectPointerImpl(*pointer).removeReference()) { // remove reference and possible destroy object
        delete pointer; // could throw exception if RCO is destroyed unsuccessfully
      }
    }
    pointer = value;
    if (pointer) { // skip if pointer is invalid
      ReferenceCountedObjectPointerImpl(*pointer).addReference();
    }
  }

  /**
    Returns true if the reference counted object is referenced by more than
    one automation pointer.

    @return False if the pointer is invalid (i.e. not pointing to an object).
  */
  inline bool isMultiReferenced() const throw() {
    return pointer && ReferenceCountedObjectPointerImpl(*pointer).isMultiReferenced(); // false if the pointer is invalid
  }

  /**
    Makes a new copy of the reference counted object if referenced by more than
    one automation pointer. This member function is invocated by some classes
    before a object is modified. The reference counted object must implement
    the default copy constructor for this to work.
  */
  inline void copyOnWrite() throw() {
    if (isMultiReferenced()) { // do we have the object for our self
      ReferenceCountedObjectPointerImpl(*pointer).removeReference(); // remove one reference (no need to delete object since multi-referenced)
      pointer = new Value(*pointer);
      ReferenceCountedObjectPointerImpl(*pointer).addReference();
    }
  }

  /**
    Returns true if the automation pointer is valid (i.e. it is pointing to an
    object).
  */
  inline bool isValid() const throw() {
    return pointer != 0;
  }

  /**
    Returns the reference counted object.
  */
  inline Reference operator*() const throw(NullPointer) {
    if (!pointer) {
      throw NullPointer(this);
    }
    return *pointer;
  }

  /**
    Returns the reference counted object.
  */
  inline Pointer operator->() const throw() {
    // ASSERT(pointer);
    return pointer;
  }

  /**
    Destroys the automation pointer.
  */
  inline ~ReferenceCountedObjectPointer() {
    if (pointer) { // skip if pointer is invalid
      if (ReferenceCountedObjectPointerImpl(*pointer).removeReference()) {
        delete pointer; // could throw exception if RCO is destroyed unsuccessfully
      }
    }
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
