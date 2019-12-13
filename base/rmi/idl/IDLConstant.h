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
  Interface Definition Language (IDL) constant.
  
  @short IDL constant.
  @ingroup rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API IDLConstant : public IDLObject {
private:

  /** The fully qualified name of the constant. */
  String name;
public:
  
  /**
    Initializes the IDL constant.
  */
  IDLConstant(const String& name) noexcept;
  
  /**
    Returns the fully qualified name of the object.
  */
  inline const String& getName() const noexcept {
    return name;
  }
  
  /**
    Destroys the IDL constant.
  */
  ~IDLConstant() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
