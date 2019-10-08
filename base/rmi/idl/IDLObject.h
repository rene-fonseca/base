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

#include <base/rmi/OrbException.h>
#include <base/Object.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Interface Definition Language (IDL) object.
  
  @short IDL object.
  @ingroup rmi
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API IDLObject : public Object {
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
