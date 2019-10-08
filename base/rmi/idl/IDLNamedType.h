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
  Base class of Interface Definition Language (IDL) named types.
  
  @short IDL named type.
  @ingroup rmi
  @version 1.0
*/

class IDLNamedType : public IDLObject {
private:
  
  /** The fully qualified name of the type. */
  String name;
public:
  
  /**
    Initializes the IDL type.
  */
  IDLNamedType(const String& name) throw();
  
  /**
    Returns the fully qualified name of the type.
  */
  inline String getName() const throw() {
    return name;
  }
  
  /**
    Destroys the IDL type.
  */
  ~IDLNamedType() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
