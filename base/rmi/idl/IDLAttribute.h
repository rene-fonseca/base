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
#include <base/rmi/idl/IDLPrimitive.h>
#include <base/rmi/idl/IDLEnumeration.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Interface Definition Language (IDL) attribute of an exception.
  
  @short IDL attribute of an exception.
  @ingroup rmi
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API IDLAttribute : public IDLObject {
private:

  /** Type of the field. */
  IDLPrimitive primitive;
  /** Type of the field. */
  IDLEnumeration enumeration;
public:
  
  /**
    Initializes the IDL attribute as a primitive type.
    
    @param type The type of the attribute.
  */
  IDLAttribute(const IDLPrimitive& type) throw();
  
  /**
    Initializes the IDL attribute as an enumeration.
    
    @param type The type of the attribute.
  */
  IDLAttribute(const IDLEnumeration& type) throw();
  
  /**
    Destroys the IDL attribute.
  */
  ~IDLAttribute() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
