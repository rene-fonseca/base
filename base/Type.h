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
#include <typeinfo> // header approved // required for typeid
// C++: if we could forward declare std::type_info we could move all implementation to cpp file except the actual typeid()
// C++: opaque void* __builtin_typeid() would work also

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

  /** Returns type used as uninitialized marker. */
  static const std::type_info* getUninitialized() noexcept;

  inline Type(const std::type_info* _type) noexcept
  {
    BASSERT(_type);
    type = _type ? _type : getUninitialized(); // prevent nullptr
  }
  
  template<class TYPE>
  class GetType {
  public:

    typedef TYPE BaseType;
    
    inline Type operator()(const TYPE& object) const noexcept
    {
      return Type::getType<TYPE>();
    }
  };

  template<class TYPE>
  class GetType<TYPE*> { // prevent pointer types
  public:

    typedef typename GetType<TYPE>::BaseType BaseType; // throw away pointer and resolve recursively

    inline Type operator()(const TYPE* const object) const noexcept
    {
      return Type::getType<TYPE>();
    }
  };
public:

  /** Convert native type to Type. */
  static inline Type makeType(const std::type_info* _type) noexcept
  {
    return Type(_type);
  }

  /** Returns Type. */
  template<class TYPE>
  static inline Type getTypeImpl() noexcept
  {
    return Type(&typeid(TYPE));
  }

  /**
    Returns the type object for the specified type.
  */
  template<class TYPE>
  static inline Type getType() noexcept
  {
    return getTypeImpl<typename GetType<TYPE>::BaseType>();
  }

  /**
    Returns the type object for the type of the specified object.
  */
  template<class TYPE>
  static inline Type getType(const TYPE& object) noexcept
  {
    return GetType<TYPE>()(object);
  }

  /**
    Initializes type object in an uninitialized state.
  */
  inline Type() noexcept
    : type(getUninitialized())
  {
  }

  /**
    Initializes type to the type of the specified object.
  */
  template<class TYPE>
  inline Type(const TYPE& object) noexcept
    : type(GetType<TYPE>()(object).type)
  {
  }

  /**
    Initializes type object from other type object.
  */
  inline Type(const Type& copy) noexcept
    : type(copy.type)
  {
  }

  /**
    Assignment of type object from other type object.
  */
  inline Type& operator=(const Type& assign) noexcept
  {
    type = assign.type;
    return *this;
  }

  /**
    Returns true if the type object has been initialized.
  */
  inline bool isInitialized() const noexcept
  {
    return *type != *getUninitialized();
  }

  /**
    Returns true if the type object hasn't been initialized.
  */
  inline bool isUninitialized() const noexcept
  {
    return *type == *getUninitialized();
  }

  /**
    Returns true if the types are identical.

    @return True if both types are uninitialized.
  */
  inline bool operator==(const Type& compare) const noexcept
  {
    return *type == *compare.type;
  }

  /**
    Returns true if the types are different.

    @return False if both types are uninitialized.
  */
  inline bool operator!=(const Type& compare) const noexcept
  {
    return *type != *compare.type;
  }

  /**
    Returns a compiler specific string identifying the type uniquely. The return
    value is unspecified for uninitialized type objects. The string could be a
    multibyte character string.

    @see TypeInfo
  */
  const char* getLocalName() const noexcept;

  /** Returns the native type_info. */
  inline operator const std::type_info*() const noexcept
  {
    return type;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
