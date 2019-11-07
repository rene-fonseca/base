/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/CastException.h>
#include <base/Functor.h>
#include <base/concurrency/AtomicCounter.h>
#include <base/collection/Hash.h>
#include <base/mem/NullPointer.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Automation pointer for reference counting objects. This class is responsible
  for counting the total number of references to an object. The pointer
  automatically deletes a reference counted object when the number of
  references reaches zero. If possible, you should use the
  CombinedReferenceCounter in preference to this class. ReferenceCounter is a
  bit slower than CombinedReferenceCounter but works for any class being
  reference counted. The ReferenceCounter class is relocateable.

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
class ReferenceCounter {
public:

  /** The type of the reference counter. */
  typedef PreferredAtomicCounter Counter; // out of memory before counter overflow
private:
  
  /** Pointer to shared reference counted object. */
  TYPE* value = nullptr; // protect pointer value from the evil programmers
  /** Holds the total number of references. */
  Counter* references = nullptr;
public:

  /**
    Initializes an automation pointer as invalid (i.e. nullptr).
  */
  inline ReferenceCounter() noexcept
  {
  }

  /**
    Initializes an automation pointer with the specified pointer value. The
    automation pointer may be implicitly initialized.
    
    @code
    ReferenceCounter<MyClass> object = new MyClass();
    @endcode
    
    @param value The desired pointer value.
  */
  inline ReferenceCounter(TYPE* _value) noexcept
    : value(_value),
      references(_value ? new Counter(1) : nullptr)
  {
  }
  
  /**
    Initialization of automation pointer from other automation pointer.
  */
  inline ReferenceCounter(const ReferenceCounter& copy) noexcept
    : value(copy.value), references(copy.references)
  {
    if (value) {
      ++*references;
    }
  }

  /**
    Initialization of automation pointer from other automation pointer using
    compile time polymorphism.
  */
  template<class POLY>
  inline ReferenceCounter(const ReferenceCounter<POLY>& copy) noexcept
    : value(copy.getValue()),
      references(copy.getReferences())
  {
    if (value) {
      ++*references;
    }
  }

  /**
    Returns true if the object may be cast to the specified type.
  */
  template<class POLY>
  inline bool isType() const noexcept
  {
    return dynamic_cast<const POLY*>(value);
  }

  /**
    Dynamic cast to the specified type. Raises CastException if the reference
    is invalid or the reference cannot be cast to the specified type.
  */
  template<class POLY>
  inline ReferenceCounter<POLY> castChecked() const throw(CastException)
  {
    POLY* temp = dynamic_cast<POLY*>(value);
    if (!temp) {
      throw CastException(this);
    }
    return temp;
  }

  /**
    Dynamic cast to the specified type.
  */
  template<class POLY>
  inline ReferenceCounter<POLY> cast() const noexcept
  {
    return dynamic_cast<POLY*>(value);
  }

  /**
    Assignment of automation pointer to this automation pointer.
  */
  inline ReferenceCounter& operator=(const ReferenceCounter& eq)
  {
    if (&eq != this) { // protect against self assignment
      if (value) {
        TYPE* oldValue = value;
        value = nullptr;
        Counter* oldReferences = references;
        references = nullptr;
        if (!--*oldReferences) {
          delete oldReferences; // less likely to throw
          delete oldValue; // could throw
        }
      }
      if (eq.value) {
        value = eq.value;
        references = eq.references;
        ++*references; // add reference
      }
    }
    return *this;
  }
  
  /**
    Assignment of automation pointer to this automation pointer using compile
    time polymorphism.
  */
  template<class POLY>
  inline ReferenceCounter& operator=(const ReferenceCounter<POLY>& eq)
  {
    if (&eq != this) { // protect against self assignment
      if (value) {
        TYPE* oldValue = value;
        value = nullptr;
        Counter* oldReferences = references;
        references = nullptr;
        if (!--*oldReferences) {
          delete oldReferences; // less likely to throw
          delete oldValue; // could throw
        }
      }
      if (eq.value) {
        value = eq.value;
        references = eq.references;
        ++*references; // add reference
      }
    }
    return *this;
  }

  /**
    Assignment of normal pointer to this automation pointer.
  */
  inline ReferenceCounter& operator=(TYPE* eq)
  {
    setValue(eq);
    return *this;
  }

  /**
    Returns true if the references are equal.
  */
  inline bool operator==(const ReferenceCounter& eq) const noexcept
  {
    return value == eq.value;
  }

  /**
    Returns true if the references are non-equal.
  */
  inline bool operator!=(const ReferenceCounter& eq) const noexcept
  {
    return value != eq.value;
  }
  
  /**
    Returns the pointer value of this automation pointer. Be careful when
    utilizing this member function. Its completely up to you to ensure that
    the reference counting rules aren't violated. Invocation of this method
    should be avoided.
  */
  inline TYPE* getValue() const noexcept
  {
    return value;
  }
  
  /**
    Returns the reference counter. Do NOT modify.
  */
  inline Counter* getReferences() const noexcept
  {
    return references;
  }

  /**
    Sets the pointer value of this automation pointer.
  */
  inline void setValue(TYPE* _value)
  {
    if (value) {
      TYPE* oldValue = value;
      value = nullptr;
      Counter* oldReferences = references;
      references = nullptr;
      if (!--*oldReferences) {
        delete oldReferences; // less likely to throw
        delete oldValue; // could throw
      }
    }
    if (_value) {
      references = new Counter(1); // one reference
      value = _value;
    }
  }

  /**
    Returns true if the reference counted object is referenced by more than
    one automation pointer.

    @return False if the pointer is invalid (i.e. not pointing to an object).
  */
  inline bool isMultiReferenced() const noexcept
  {
    return value && (*references > 1);
  }

  /** Returns the number of references. Returns 0 if pointer is nullptr. */
  inline MemorySize getNumberOfReferences() const noexcept
  {
    return value ? static_cast<MemorySize>(*references) : 0;
  }

  /**
    Makes a new copy of the reference counted object if referenced by more than
    one automation pointer. This member function is invocated by some classes
    before a object is modified. The reference counted object must implement
    the default copy constructor for this to work.
  */
  inline void copyOnWrite()
  {
    if (isMultiReferenced()) { // do we have the object for our self
      setValue(new TYPE(*value)); // may throw
    }
  }

  /**
    Invalidates the reference.
  */
  inline void invalidate()
  {
    setValue(nullptr);
  }
  
  /**
    Returns true if the automation pointer is valid (i.e. it is pointing to an
    object).
  */
  inline bool isValid() const noexcept
  {
    return value != nullptr;
  }

  /**
    Returns the reference counted object.
    
    @deprecated
  */
  inline TYPE& operator*() throw(NullPointer)
  {
    if (!value) {
      throw NullPointer(this);
    }
    return *value;
  }
  
  /**
    Returns the reference counted object.

    @deprecated
  */
  inline const TYPE& operator*() const throw(NullPointer)
  {
    if (!value) {
      throw NullPointer(this);
    }
    return *value;
  }

  /**
    Returns the reference counted object.
  */
  inline TYPE* operator->() noexcept
  {
    return value;
  }
  
  /**
    Returns the reference counted object.
  */
  inline const TYPE* operator->() const noexcept
  {
    return value;
  }

  /**
    Returns true if the automation pointer is valid (i.e. it is pointing to an
    object).
  */
  inline operator bool() const noexcept
  {
    return value != nullptr;
  }
  
  /**
    Destroys the automation pointer.
  */
  inline ~ReferenceCounter()
  {
    setValue(nullptr);
  }
};

template<class TYPE>
class Relocateable<ReferenceCounter<TYPE> > {
public:

  static const bool IS_RELOCATEABLE = Relocateable<void*>::IS_RELOCATEABLE;
};

template<class TYPE>
class Hash<ReferenceCounter<TYPE> > {
public:

  inline unsigned long operator()(const ReferenceCounter<TYPE>& value) noexcept
  {
    Hash<void*> hash;
    return hash(value.getValue());
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
