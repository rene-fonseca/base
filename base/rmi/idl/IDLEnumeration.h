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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Interface Definition Language (IDL) enumeration.
  
  @short IDL enumeration.
  @ingroup rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API IDLEnumeration : public IDLNamedType {
public:
  
  /**
    Initializes the IDL enumeration.
  */
  IDLEnumeration(const String& name) throw();
  
  /**
    Destroys the IDL enumeration.
  */
  ~IDLEnumeration() throw();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
