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
#include <base/collection/Hash.h>
#include <base/mem/ReferenceCountedObject.h>
#include <base/mem/NullPointer.h>
#include <base/Base.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

template<class TYPE> class Reference;

/** Type that supports all references. */
typedef Reference<ReferenceCountedObject> AnyReference;

/** Registers the given object for async garbage collection. */
_COM_AZURE_DEV__BASE__API void garbageCollect(AnyReference& reference);

/**
  Registers the given object for async garbage collection. Returns false if the object must be explicitly destructed.
*/
_COM_AZURE_DEV__BASE__API bool garbageCollectOptional(ReferenceCountedObject* object) noexcept;

/**
  Registers the given object for async garbage collection or destroys the object. Type MUST have virtual destructor.
*/
_COM_AZURE_DEV__BASE__API void destroyReference(ReferenceCountedObject* object);

/**
  Automation pointer for reference counting objects. This class is responsible
  for counting the total number of references to an object. The pointer
  automatically deletes a reference counted object when the number of
  references reaches zero. This pointer can only be used to reference count
  objects of type ReferenceCountedObject but is faster than ReferenceCounter.
  The Reference class is relocateable.
 
  Note that is it safe to convert this pointer back to a Reference pointer.

  This is the recommended automation pointer. Only use other automation classes when needed.
 
  void MyClass::doit()
  {
    Reference<MyClass> myObject = this;
    useIt(myObject);
  }
 
  @short Automation pointer that counts the number of references to an object.
  @ingroup memory
  @see ReferenceCountedObject ReferenceCounter
  @version 1.0
*/

template<class TYPE>
class Reference {
private:
  
  /** Pointer to shared reference counted object. */
  TYPE* value = nullptr;
public:

  // we need to ignore if type is forward declared
  // static_assert(std::is_base_of<ReferenceCountedObject, TYPE>(), "Reference must be used with ReferenceCountedObject.");

  /**
    Initializes an automation pointer as invalid (i.e. nullptr).
  */
  inline Reference() noexcept
  {
  }

  /**
    Initializes an automation pointer with the specified pointer value. The
    automation pointer may be implicitly initialized.
    
    @code
    Reference<MyClass> object = new MyClass();
    @endcode
    
    @param value The desired pointer value.
  */
  inline Reference(TYPE* _value) noexcept
    : value(_value)
  {
    if (value) {
      ReferenceCountedObject::ReferenceImpl(*value).addReference();
    }
  }
  
  /**
    Initialization of automation pointer from other automation pointer.
  */
  inline Reference(const Reference& copy) noexcept
    : value(copy.value)
  {
    if (value) {
      ReferenceCountedObject::ReferenceImpl(*value).addReference();
    }
  }

  /**
    Move initialization of automation pointer from other automation pointer.
  */
  inline Reference(Reference&& move)
    : value(moveObject(move.value))
  {
#if 0
    if (value) {
      ReferenceCountedObject::ReferenceImpl(*value).addReference();
    }
    move.invalidate();
#else // no need to do counting
    move.value = nullptr;
#endif
  }

  /**
    Initialization of automation pointer from other automation pointer using
    compile time polymorphism.
  */
  template<class POLY>
  inline Reference(const Reference<POLY>& copy) noexcept
    : value(copy.getValue())
  {
    if (value) {
      ReferenceCountedObject::ReferenceImpl(*value).addReference();
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
  inline Reference<POLY> castChecked() const
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
  inline Reference<POLY> cast() const noexcept
  {
    return dynamic_cast<POLY*>(value);
  }

  /**
    Assignment of automation pointer to this automation pointer.
  */
  inline Reference& operator=(const Reference& copy)
  {
    setValue(copy.value); // no need to protect against self assignment
    return *this;
  }

  /**
    Assignment of automation pointer to this automation pointer.
  */
  inline Reference& operator=(Reference&& move)
  {
    if (this != &move) { // no self assignment since we invalidate copy
#if 0
      setValue(move.value);
      move.invalidate();
#else
      setValue(nullptr); // make sure we release existing object
      value = move.value; // no need to do counting
      move.value = nullptr;
#endif
    }
    return *this;
  }

  /**
    Assignment of automation pointer to this automation pointer using compile
    time polymorphism.
  */
  template<class POLY>
  inline Reference& operator=(const Reference<POLY>& assign)
  {
    setValue(assign.getValue()); // no need to protect against self assignment
    return *this;
  }

  /**
    Assignment of normal pointer to this automation pointer.
  */
  inline Reference& operator=(TYPE* assign)
  {
    setValue(assign);
    return *this;
  }

  /**
    Returns true if the reference is nullptr.
  */
  inline bool operator==(NullPtr) const noexcept
  {
    return value == nullptr;
  }

  /**
    Returns true if the references are equal.
  */
  inline bool operator==(const Reference& compare) const noexcept
  {
    return value == compare.value;
  }

  /**
    Returns true if the references are non-equal.
  */
  inline bool operator!=(const Reference& compare) const noexcept
  {
    return value != compare.value;
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
    Sets the pointer value of this automation pointer.
  */
  inline void setValue(TYPE* _value)
  {
    if (_value) { // skip if pointer is invalid
      ReferenceCountedObject::ReferenceImpl(*_value).addReference();
    }
    TYPE* oldValue = value;
    value = _value;
    if (oldValue) { // skip if pointer is invalid
      if (ReferenceCountedObject::ReferenceImpl(*oldValue).removeReference()) {
        if (std::has_virtual_destructor<TYPE>()) {
          destroyReference(oldValue); // could throw
        } else {
          if (!garbageCollectOptional(oldValue)) {
            delete oldValue; // could throw
          }
        }
      }
    }
  }

  /**
    Returns true if the reference counted object is referenced by more than
    one automation pointer.

    @return False if the pointer is invalid (i.e. not pointing to an object).
  */
  inline bool isMultiReferenced() const noexcept
  {
    return value &&
      ReferenceCountedObject::ReferenceImpl(*value).isMultiReferenced();
  }

  /** Returns the number of references. Returns 0 if pointer is nullptr. */
  inline MemorySize getNumberOfReferences() const noexcept
  {
    return value ? ReferenceCountedObject::ReferenceImpl(*value).getNumberOfReferences() : 0;
  }

  /** Returns a new object by using default construction. */
  static inline Reference make()
  {
    return new TYPE();
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
  inline TYPE& operator*()
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
  inline const TYPE& operator*() const
  {
    if (!value) {
      throw NullPointer(this);
    }
    return *value;
  }

  /**
    Returns the reference counted object.
  */
  inline TYPE* operator->()
  {
    if (!value) {
      throw NullPointer(this);
    }
    return value;
  }
  
  /**
    Returns the reference counted object.
  */
  inline const TYPE* operator->() const
  {
    if (!value) {
      throw NullPointer(this);
    }
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
  
  /** Garbage collects object. Object will be set to nullptr. */
  inline void garbageCollect()
  {
    base::garbageCollect(*this);
  }
  
  /**
    Destroys the automation pointer.
  */
  inline ~Reference()
  {
    setValue(nullptr);
  }

  static inline void swapper(Reference& a, Reference& b) noexcept
  {
    base::swapper(a.value, b.value); // self swap allowed - no reference counting required
  }
};

template<class TYPE>
inline void swapper(Reference<TYPE>& a, Reference<TYPE>& b)
{
  Reference<TYPE>::swapper(a, b);
}

/** Requests automatic garbage collection on destruction. */
class GarbageCollect {
private:

  AnyReference object;
public:
  
  GarbageCollect(AnyReference _object) noexcept
    : object(_object) {
  }

  inline ~GarbageCollect() noexcept
  {
    if (object) {
      object.garbageCollect();
    }
  }
};

template<class TYPE>
class IsRelocateable<Reference<TYPE> > : public IsRelocateable<TYPE*> {
};

// static_assert(IsRelocateable<AnyReference>());

template<class TYPE>
class Hash<Reference<TYPE> > {
public:

  inline unsigned long operator()(const Reference<TYPE>& value) noexcept
  {
    Hash<void*> hash;
    return hash(value.getValue());
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
