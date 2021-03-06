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

#include <base/Functor.h>
#include <base/collection/Hash.h>
#include <base/mem/NullPointer.h>
#include <base/CastException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Automation pointer which provides polymorphic behavior for non-polymorphic
  objects (direct objects). This is accomplished by copying the object to the
  heap using the default copy constructor and referencing the object through a
  pointer. This automation pointer is intended for objects such as Socket which
  use internal reference counting and thus normally are accessed through their
  value and not through some pointer. The Indirect automation pointer is
  relocateable. Accessing the object through this automation object results in
  an overhead because an additional pointer must be dereferenced. However, the
  overhead can be avoided by doing a dynamic cast and then copying the "new"
  object onto the stack. This automation pointer only makes sense if the type
  in question is itself some pointer.
  
  @code
  class MyClass : public Object {
  private:

    Array<Indirect> sockets;
  public:

    void add(Indirect<Socket> socket) {
      sockets.append(socket);
    }
    
    void add(ServerSocket socket) {
      sockets.append(socket);
    }
    
    void add(StreamSocket socket) {
      sockets.append(socket);
    }
    
    void add(Socket socket) {
      sockets.append(socket);
    }
  };
  @endcode
  
  @short Automation pointer for direct values.
  @ingroup memory
  @see Reference
  @version 1.0
*/

template<class TYPE>
class Indirect {
private:
  
  /** The value. */
  TYPE* value = nullptr;
public:
  
  /**
    Initializes an automation pointer as invalid (i.e. nullptr).
  */
  inline Indirect() noexcept
  {
  }
  
  /**
    Initializes reference with the specified pointer value. The automation
    pointer may be implicitly initialized.
    
    @param value The desired pointer value.
  */
  inline Indirect(const TYPE& _value)
    : value(new TYPE(_value))
  {
  }
  
  /**
    Initialization of automation pointer by automation pointer.
  */
  inline Indirect(const Indirect& copy)
    : value(new TYPE(*copy.value))
  {
  }
  
  /**
    Dynamic cast to the specified type.

    @return nullptr if cast is not possible.
  */
  template<class POLY>
  inline Indirect<POLY> cast()
  {
    POLY* temp = dynamic_cast<POLY*>(value);
    if (!temp) {
      _throw CastException(this);
    }
    return *temp;
  }

  /**
    Initialization of automation pointer from other automation pointer using
    compile time polymorphism.
  */
#if 0 // TAG: fixme
  template<class POLY>
  inline Indirect(const Indirect<POLY>& copy)
    : value(new TYPE(*copy.cast<TYPE>().value))
  {
  }
#endif
  
  /**
    Assignment of automation pointer by automation pointer.
  */
  inline Indirect& operator=(const Indirect& assign)
  {
    if (&assign != this) {
      TYPE* temp = value;
      value = new TYPE(*assign.value);
      if (temp) {
        delete temp;
      }
    }
    return *this;
  }
  
  /**
    Assignment of automation pointer by automation pointer using compile time
    polymorphism.
  */
  template<class POLY>
  inline Indirect& operator=(const Indirect<POLY>& assign) noexcept
  {
    // make sure CastException is not possible
    TYPE* unused = static_cast<POLY*>(nullptr);
    if (unused) { // avoid compiler warning
    }
#if 0 // TAG: fixme
    Indirect indirect = assign.cast<TYPE>();
#else
    Indirect indirect;
#endif
    TYPE* temp = value;
    value = new TYPE(*indirect.value);
    if (temp) {
      delete temp;
    }
    return *this;
  }
  
  /**
    Returns true if the automation pointer is valid (i.e. it is pointing to an
    object).
  */
  inline bool isValid() const noexcept
  {
    return value;
  }
  
  /**
    Returns true if the references are equal.
  */
  inline bool operator==(const Indirect& compare) const noexcept
  {
    return false;
  }
  
  /**
    Returns true if the references are non-equal.
  */
  inline bool operator!=(const Indirect& compare) const noexcept
  {
    return true;
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
    Returns the value.
  */
  inline const TYPE& getValue() const
  {
    if (!value) {
      _throw NullPointer(this);
    }
    return *value;
  }
  
  /**
    Casts the object to the specified object type. Raises CastException if the
    cast failed.
  */
  template<class POLY>
  inline POLY getValue() const
  {
    const POLY* result = dynamic_cast<POLY*>(value);
    if (!result) {
      _throw CastException(this);
    }
    return *result;
  }
  
  /**
    Dereferences the automation pointer.
  */
  inline TYPE* operator->()
  {
    if (!value) {
      _throw NullPointer(this);
    }
    return value;
  }
  
  /**
    Dereferences the automation pointer.
  */
  inline const TYPE* operator->() const
  {
    if (!value) {
      _throw NullPointer(this);
    }
    return value;
  }
  
  /**
    Returns true if the automation pointer is valid.
  */
  inline operator bool() const noexcept
  {
    return value;
  }
  
  /**
    Returns the hash value of the pointer.
  */
  inline unsigned long getHash() const noexcept
  {
    Hash<TYPE> hash;
    return value ? hash(*value) : 0;
  }
  
  /**
    Destroys the automation pointer.
  */
  inline ~Indirect()
  {
    if (value) {
      delete value;
    }
  }
};

template<class TYPE>
class Indirect<TYPE*> {
};

template<class TYPE>
class IsRelocateable<Indirect<TYPE> > : public IsRelocateable<TYPE*> {
};

template<class TYPE>
class Hash<Indirect<TYPE> > {
public:

  inline unsigned long operator()(const Indirect<TYPE>& value) noexcept
  {
    return value.getHash();
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
