/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__REFERENCE_H
#define _DK_SDU_MIP__BASE_MEM__REFERENCE_H

#include <base/mem/ReferenceCountedObject.h>
#include <base/mem/NullPointer.h>
#include <base/Functor.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/*
  This class is only introduced to give the general Reference template class
  access to the ReferenceCountedObject. You must not use this class directly.
  
  @short Reference counting automation pointer implementation class.
  @see Reference
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class ReferenceImpl {
private:

  /** The reference counted object. */
  const ReferenceCountedObject& object;
  ReferenceImpl(const ReferenceImpl&) throw();
  ReferenceImpl& operator=(const ReferenceImpl& eq) throw();
public:

  /**
    Initializes reference to object.
  */
  inline ReferenceImpl(const ReferenceCountedObject& _object) throw()
    : object(_object) {
  }
  
  /**
    Adds one reference to the object.
  */
  inline void addReference() const throw() {
    ++object.references;
  }

  /**
    Removes one reference to the object. The object must have at least one
    reference.
    
    @return True if all references have been released.
  */
  inline bool removeReference() const throw() {
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
  Automation pointer for reference counting objects. This class is responsible
  for counting the total number of references to an object. The pointer
  automatically deletes a reference counted object when the number of
  references reaches zero. This pointer can only be used to reference count
  objects of type ReferenceCountedObject but is faster than ReferenceCounter.
  The Reference class is relocateable.
  
  @short Automation pointer that counts the number of references to an object.
  @see ReferenceCountedObject ReferenceCounter
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TYPE>
class Reference {
private:
  
  /** Pointer to shared reference counted object. */
  TYPE* pointer;
public:

  /**
    Initializes an automation pointer as invalid (i.e. null).
  */
  inline Reference() throw() : pointer(0) {
  }

  /**
    Initializes an automation pointer with the specified pointer value. The
    automation pointer may be implicitly initialized.
    
    <pre>
    Reference<MyClass> rcop = new MyClass();
    </pre>
    
    @param value The desired pointer value.
  */
  inline Reference(TYPE* value) throw() : pointer(value) {
    if (pointer) {
      ReferenceImpl(*pointer).addReference();
    }
  }
  
  /**
    Initialization of automation pointer from other automation pointer.
  */
  inline Reference(const Reference& copy) : pointer(copy.pointer) {
    if (pointer) {
      ReferenceImpl(*pointer).addReference();
    }
  }

  /**
    Initialization of automation pointer from other automation pointer using
    compile time polymorphism.
  */
  template<class POLY>
  inline Reference(const Reference<POLY>& copy) : pointer(copy.getValue()) {
    if (pointer) {
      ReferenceImpl(*pointer).addReference();
    }
  }

  /**
    Assignment of automation pointer to this automation pointer.
  */
  inline Reference& operator=(const Reference& eq) /*throw(...)*/ {
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
  inline Reference& operator=(const Reference<POLY>& eq) /*throw(...)*/ {
    setValue(eq.getValue());
    return *this;
  }

  /**
    Assignment of normal pointer to this automation pointer.
  */
  inline Reference& operator=(TYPE* eq) throw() {
    setValue(eq);
    return *this;
  }
  
  /**
    Returns the pointer value of this automation pointer. Be careful when
    utilizing this member function. Its completely up to you to ensure that
    the reference counting rules aren't violated. Invocation of this method should
    be avoided.
  */
  inline TYPE* getValue() const throw() {
    return pointer;
  }
  
  /**
    Sets the pointer value of this automation pointer.
  */
  inline void setValue(TYPE* value) /*throw(...)*/ {
    if (pointer) { // skip if pointer is invalid
      // remove reference and possible destroy object
      if (ReferenceImpl(*pointer).removeReference()) {
        delete pointer;
      }
    }
    pointer = value;
    if (pointer) { // skip if pointer is invalid
      ReferenceImpl(*pointer).addReference();
    }
  }

  /**
    Returns true if the reference counted object is referenced by more than
    one automation pointer.

    @return False if the pointer is invalid (i.e. not pointing to an object).
  */
  inline bool isMultiReferenced() const throw() {
    return pointer && ReferenceImpl(*pointer).isMultiReferenced();
  }

  /**
    Makes a new copy of the reference counted object if referenced by more than
    one automation pointer. This member function is invocated by some classes
    before a object is modified. The reference counted object must implement
    the default copy constructor for this to work.
  */
  inline void copyOnWrite() throw() {
    if (isMultiReferenced()) { // do we have the object for our self
      // remove one reference (no need to delete object since multi-referenced)
      TYPE* temp = new TYPE(*pointer);
      ReferenceImpl(*temp).addReference();
      ReferenceImpl(*pointer).removeReference();
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
  inline TYPE* operator->() const throw() {
    return pointer;
  }

  /**
    Destroys the automation pointer.
  */
  inline ~Reference() /*throw(...)*/ {
    if (pointer) { // skip if pointer is invalid
      if (ReferenceImpl(*pointer).removeReference()) {
        delete pointer;
      }
    }
  }
};

template<class TYPE>
class Relocateable<Reference<TYPE> > {
public:

  static const bool IS_RELOCATEABLE = Relocateable<void*>::IS_RELOCATEABLE;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
