/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__TYPE_H
#define _DK_SDU_MIP__BASE__TYPE_H

#include <base/features.h>
#include <typeinfo>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class is used to identify any type within the application.

  @short Type identity
  @see TypeInfo
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Type {
private:

  const std::type_info* type;

  class Unintialized {};

  inline Type(const std::type_info* _type) throw() : type(_type) {}
  
  template<class TYPE>
  class GetType {
  public:

    typedef TYPE BaseType;
    
    inline Type operator()(const TYPE& object) const throw() {
      return Type(&typeid(object));
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

  /**
    Returns the type object for the specified type.
  */
  template<class TYPE>
  static inline Type getType() throw() {
    return Type(&typeid(GetType<TYPE>::BaseType));
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
  inline Type() throw() : type(&typeid(Unintialized)) {}

  /**
    Initialize type to the type of the specified object.
  */
  template<class TYPE>
  inline Type(const TYPE& object) throw() : type(GetType<TYPE>()(object).type) {
  }

  /**
    Initialized type object from other type object.
  */
  inline Type(const Type& copy) throw() : type(copy.type) {}

  /**
    Assignment of type object from other type object.
  */
  inline Type& operator=(const Type& eq) throw() {
    type = eq.type;
    return *this;
  }

  /**
    Returns true if the type object has been initialized.
  */
  inline bool isInitialized() const throw() {
    return *type != typeid(Unintialized);
  }

  /**
    Returns true if the type object hasn't been initialized.
  */
  inline bool isUninitialized() const throw() {
    return *type == typeid(Unintialized);
  }

  /**
    Returns true if the types are identical.
  */
  bool operator==(const Type& eq) const throw() {
    return *type == *eq.type;
  }

  /**
    Returns true if the types are different.
  */
  bool operator!=(const Type& eq) const throw() {
    return *type != *eq.type;
  }

  /**
    Returns a compiler specific string identifying the type uniquely. The return
    value is unspecified for uninitialized type objects. The string could be a
    multibyte character string.

    @see TypeInfo
  */
  const char* getLocalName() const throw() {
    return type->name();
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
