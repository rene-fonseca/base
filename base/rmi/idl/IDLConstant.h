/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_RMI_IDL__IDL_CONSTANT_H
#define _DK_SDU_MIP__BASE_RMI_IDL__IDL_CONSTANT_H

#include <base/rmi/idl/IDLObject.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Interface Definition Language (IDL) constant.
  
  @short IDL constant.
  @ingroup rmi
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class IDLConstant : public IDLObject {
private:

  /** The fully qualified name of the constant. */
  String name;
public:
  
  /**
    Initializes the IDL constant.
  */
  IDLConstant(const String& name) throw();
  
  /**
    Returns the fully qualified name of the object.
  */
  inline String getName() const throw() {
    return name;
  }
  
  /**
    Destroys the IDL constant.
  */
  ~IDLConstant() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
