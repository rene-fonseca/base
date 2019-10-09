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

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Interface Definition Language (IDL) constant.
  
  @short IDL constant.
  @ingroup rmi
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API IDLConstant : public IDLObject {
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
  inline const String& getName() const throw() {
    return name;
  }
  
  /**
    Destroys the IDL constant.
  */
  ~IDLConstant() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
