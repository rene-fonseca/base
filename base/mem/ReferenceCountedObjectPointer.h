/***************************************************************************
    copyright            : (C) 2000 by René Møller Fonseca
    email                : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTED_OBJECT_POINTER_H
#define _DK_SDU_MIP__BASE_MEM__REFERENCE_COUNTED_OBJECT_POINTER_H

#include <base/mem/ReferenceCountedObject.h>
#include <base/mem/NullPointer.h>
#include <base/Base.h>

/**
  This class is only introduced to give the general
  ReferenceCountedObjectPointer template class access to the
  ReferenceCountedObject. You should not used this class directly.

  @see ReferenceCountedObjectPointer
  @author René Møller Fonseca
  @version 1.03
*/

class ReferenceCountedObjectPointerImpl {
private:

  /** Type of pointer to reference counted object. */
  typedef const ReferenceCountedObject* Pointer;
  /** Pointer to shared reference counted object. */
  Pointer ptr; // protect pointer value from the evil programmers
public:

  /**
    Initializes an automation pointer as invalid (null).
  */
  inline ReferenceCountedObjectPointerImpl() throw() {}

  /**
    Initializes automation pointer with the specified pointer value.
    Implicit initialization is allowed.

    @param value The desired pointer value.
  */
  inline ReferenceCountedObjectPointerImpl(Pointer value) : ptr(value) {
    if (ptr) {
      ++ptr->references; // add reference
    }
  }

  /**
    Initialization of automation pointer from other automation pointer.
  */
  inline ReferenceCountedObjectPointerImpl(const ReferenceCountedObjectPointerImpl& copy) : ptr(copy.ptr) {
    if (ptr) {
      ++ptr->references; // add reference
    }
  }

  /**
    Returns the pointer value of this automation pointer.
  */
  inline Pointer getValue() const throw() {return ptr;}

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
    Returns true if the reference counted object is referenced by more than
    one automation pointer.

    @return False if the pointer is invalid (i.e. not pointing to an object).
  */
  inline bool isMultiReferenced() const throw() {
    return (ptr) && (ptr->references > 1); // false if NULL pointer
  }

  /**
    Returns true if the automation pointer is valid (i.e. it is pointing to an
    object).
  */
  inline bool isValid() const throw() {return ptr != 0;}

  /**
    Destroys this automation pointer.
  */
  inline ~ReferenceCountedObjectPointerImpl() {
    if (ptr) { // skip if NULL pointer
      if (--ptr->references == 0) { // remove reference
        delete ptr; // could throw exception if object is destroyed unsuccessfully
        ptr = 0;
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
  @author René Møller Fonseca
  @version 1.04
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

  /**
    Initializes an automation pointer as invalid (null).
  */
  inline ReferenceCountedObjectPointer() throw() {}

  /**
    Initializes an automation pointer with the specified pointer value. The
    automation pointer may be implicitly initialized.

    <pre>
    ReferenceCountedObjectPointer<MyClass> rcop = new MyClass();
    </pre>

    @param value The desired pointer value.
  */
  inline ReferenceCountedObjectPointer(Pointer value) throw() :
    ReferenceCountedObjectPointerImpl(value) {}

  /**
    Initialization of automation pointer from other automation pointer.
  */
  inline ReferenceCountedObjectPointer(const ReferenceCountedObjectPointer& copy) :
    ReferenceCountedObjectPointerImpl(copy) {}

  /**
    Initialization of automation pointer from other automation pointer using
    compile time polymorphism.
  */
  template<class POLY>
  inline ReferenceCountedObjectPointer(const ReferenceCountedObjectPointer<POLY>& copy) : ReferenceCountedObjectPointerImpl(down_cast<Pointer>(copy.getValue())) {}

  /**
    Assignment of automation pointer to this automation pointer.
  */
  inline ReferenceCountedObjectPointer& operator=(const ReferenceCountedObjectPointer& eq) {
    if (&eq != this) { // protect against self assignment
      setValue(eq.getValue());
    }
    return *this;
  }

  /**
    Assignment of automation pointer to this automation pointer using compile
    time polymorphism.
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
    Returns the pointer value of this automation pointer. Be careful when
    utilizing this member function. Its completely up to you to ensure that
    the reference counting rules aren't violated.
  */
  inline Pointer getValue() const throw() {
    return static_cast<Pointer>(const_cast<ReferenceCountedObject*>(
      ReferenceCountedObjectPointerImpl::getValue()));
  }

  /**
    Returns true if the reference counted object is referenced by more than
    one automation pointer.

    @return False if the pointer is invalid (i.e. not pointing to an object).
  */
  inline bool isMultiReferenced() const throw() {
    return ReferenceCountedObjectPointerImpl::isMultiReferenced();
  }

  /**
    Makes a new copy of the reference counted object if referenced by more than
    one automation pointer. This member function is invocated by some classes
    before a object is modified. The reference counted object must implement
    the default copy constructor for this to work.
  */
  inline void copyOnWrite() throw() {
    if (isMultiReferenced()) { // do we have the object for our self
      setValue(new Value(*getValue())); // make a copy of the object
    }
  }

  /**
    Returns true if the automation pointer is valid (i.e. it is pointing to an
    object).
  */
  inline bool isValid() const throw() {
    return ReferenceCountedObjectPointerImpl::isValid();
  }

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
