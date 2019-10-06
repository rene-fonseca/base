/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class IDLAttribute : public IDLObject {
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
