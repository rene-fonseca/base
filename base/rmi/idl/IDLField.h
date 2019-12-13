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

#include <base/rmi/idl/IDLObject.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Interface Definition Language (IDL) field of structure.
  
  @short IDL field of structure.
  @ingroup rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API IDLField : public IDLObject {
private:

  /** Type of the field. */
  // IDLPrimitive
  // IDLEnumeration
  // IDLStructure
  // IDLInterface
public:
  
  /**
    Initializes the IDL field.
    
    @param type The type of the field.
  */
  IDLField(const IDLType& type) noexcept;
  
  /**
    Returns the type of the field.
  */
  inline IDLType getType() const noexcept {
    return type;
  }
  
  /**
    Destroys the IDL field.
  */
  ~IDLField() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
