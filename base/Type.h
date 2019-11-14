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

#include <base/features.h>
#include <typeinfo>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This class is used to identify any type within the application.

  @short Type identity
  @see TypeInfo
  @version 1.0.2
*/

class _COM_AZURE_DEV__BASE__API Type {
  friend class Exception;
private:

  const std::type_info* type = nullptr;

  class _COM_AZURE_DEV__BASE__API Uninitialized {
  };

  inline Type(const std::type_info* _type) throw() {
    BASSERT(_type);
    type = _type ? _type : &typeid(Uninitialized); // prevent nullptr
  }
  
  template<class TYPE>
  class GetType {
  public:

    typedef TYPE BaseType;
    
    inline Type operator()(const TYPE& object) const throw() {
      return Type(&typeid(object));
    }
  };
  
  template<>
  class GetType<void*> { // prevent pointer types
  public:

    typedef typename void BaseType; // throw away pointer and resolve recursively

    inline Type operator()(const void* const object) const throw() {
      return Type();
    }
  };

  template<class TYPE>
  class GetType<TYPE*> { // prevent pointer types
  public:

    typedef typename GetType<TYPE>::BaseType BaseType; // throw away pointer and resolve recursively

    inline Type operator()(const TYPE* const object) const throw() {
      return GetType<TYPE>()(*object);
    }
  };
public:

  /** Convert native type to Type. */
  static inline Type makeType(const std::type_info* _type) throw() {
    return Type(_type);
  }

  /**
    Returns the type object for the specified type.
  */
  template<class TYPE>
  static inline Type getType() throw() {
    return Type(&typeid(typename GetType<TYPE>::BaseType));
  }

  /**
    Returns the type object for the type of the specified object.
  */
  template<class TYPE>
  static inline Type getType(const TYPE& object) throw() {
    return GetType<TYPE>()(object);
  }

  /**
    Initializes type object in an uninitialized state.
  */
  inline Type() throw() : type(&typeid(Uninitialized)) {
  }

  /**
    Initializes type to the type of the specified object.
  */
  template<class TYPE>
  inline Type(const TYPE& object) throw() : type(GetType<TYPE>()(object).type) {
  }

  /**
    Initializes type object from other type object.
  */
  inline Type(const Type& copy) throw() : type(copy.type) {
  }

  /**
    Assignment of type object from other type object.
  */
  inline Type& operator=(const Type& assign) throw() {
    type = assign.type;
    return *this;
  }

  /**
    Returns true if the type object has been initialized.
  */
  inline bool isInitialized() const throw() {
    return *type != typeid(Uninitialized);
  }

  /**
    Returns true if the type object hasn't been initialized.
  */
  inline bool isUninitialized() const throw() {
    return *type == typeid(Uninitialized);
  }

  /**
    Returns true if the types are identical.

    @return True if both types are uninitialized.
  */
  inline bool operator==(const Type& eq) const throw() {
    return *type == *eq.type;
  }

  /**
    Returns true if the types are different.

    @return False if both types are uninitialized.
  */
  inline bool operator!=(const Type& eq) const throw() {
    return *type != *eq.type;
  }

  /**
    Returns a compiler specific string identifying the type uniquely. The return
    value is unspecified for uninitialized type objects. The string could be a
    multibyte character string.

    @see TypeInfo
  */
  inline const char* getLocalName() const throw() {
    return type->name();
  }

  /** Returns the native type_info. */
  inline operator const std::type_info*() const noexcept {
    return type;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
