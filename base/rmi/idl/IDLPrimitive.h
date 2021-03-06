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

#include <base/rmi/idl/IDLType.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Interface Definition Language (IDL) primitive type.
  
  @short IDL primitive type.
  @ingroup rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API IDLPrimitive : public IDLType {
public:
  
  /** The primitive type. */
  enum Type {
    TYPE_VOID,
    TYPE_CHARACTER,
    TYPE_WIDE_CHARACTER,
    TYPE_STRING,
    TYPE_WIDE_STRING,
    TYPE_SHORT,
    TYPE_UNSIGNED_SHORT,
    TYPE_INT,
    TYPE_UNSIGNED_INT,
    TYPE_LONG,
    TYPE_UNSIGNED_LONG,
    TYPE_LONG_LONG,
    TYPE_UNSIGNED_LONG_LONG
  };
private:
  
  /** The primitive type. */
  Type type = TYPE_VOID;
public:
  
  /**
    Initializes the IDL primitive type.
  */
  IDLPrimitive(const Type& type) noexcept;
  
  /**
    Returns the primitive type.
  */
  inline Type getType() const noexcept {
    return type;
  }
  
  /**
    Destroys the IDL primitive type.
  */
  ~IDLPrimitive() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
