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
  Base class of Interface Definition Language (IDL) types.
  
  @short IDL type.
  @ingroup rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API IDLType : public IDLObject {
public:
  
  /**
    Initializes the IDL type.
  */
  IDLType() noexcept;
  
  /**
    Destroys the IDL type.
  */
  ~IDLType() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
