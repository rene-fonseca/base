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

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Interface Definition Language (IDL) field of structure.
  
  @short IDL field of structure.
  @ingroup rmi
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API IDLField : public IDLObject {
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
  IDLField(const IDLType& type) throw();
  
  /**
    Returns the type of the field.
  */
  inline IDLType getType() const throw() {
    return type;
  }
  
  /**
    Destroys the IDL field.
  */
  ~IDLField() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
