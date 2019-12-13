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

#include <base/rmi/idl/IDLNamedType.h>
#include <base/rmi/idl/IDLAttribute.h>
#include <base/collection/Array.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Interface Definition Language (IDL) exception.
  
  @short IDL exception.
  @ingroup rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API IDLException : public IDLNamedType {
private:
  
  /** The version of the exception. */
  unsigned int version = 0;
  /** The attributes of the exception. */
  Array<IDLAttribute> attributes;
public:
  
  /**
    Initializes the IDL exception.
  */
  IDLException(const String& name, unsigned int version) noexcept;

  /**
    Adds the specified atttribute.
  */
  void addAttribute(const IDLAttribute& attribute) noexcept;
  
  /**
    Returns the attributes of the exception.
  */
  Array<IDLAttribute> getAttributes() const throw();
  
  /**
    Returns true if the exceptions are compatible.
  */
  bool isCompatible(const IDLException& exception) const throw();
  
  /**
    Destroys the IDL exception.
  */
  ~IDLException() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
