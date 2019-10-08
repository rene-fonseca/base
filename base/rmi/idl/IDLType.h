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
  Base class of Interface Definition Language (IDL) types.
  
  @short IDL type.
  @ingroup rmi
  @version 1.0
*/

class IDLType : public IDLObject {
public:
  
  /**
    Initializes the IDL type.
  */
  IDLType() throw();
  
  /**
    Destroys the IDL type.
  */
  ~IDLType() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
