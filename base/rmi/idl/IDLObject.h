/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_RMI_IDL__IDL_OBJECT_H
#define _DK_SDU_MIP__BASE_RMI_IDL__IDL_OBJECT_H

#include <base/rmi/OrbException.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Interface Definition Language (IDL) object.
  
  @short IDL object.
  @ingroup rmi
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class IDLObject : public Object {
public:
  
  /**
    Initializes the IDL object.
  */
  IDLObject() throw();  
  
  /**
    Destroys the IDL object.
  */
  ~IDLObject() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
