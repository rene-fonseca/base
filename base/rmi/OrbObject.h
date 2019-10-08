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

#include <base/Object.h>
#include <base/rmi/OrbException.h>
#include <base/string/String.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Object Request Broker object.
  
  @short Object Request Broker object.
  @ingroup rmi
  @version 1.0
*/

class _DK_SDU_MIP__BASE__API OrbObject : public Object {
public:
  
  /**
    Initializes the orb object.
  */
  OrbObject() throw(OrbException);
  
  /**
    Returns the identifier of the object.
  */
  String getOrbIdentifier() const throw(OrbException);
  
  /**
    Destroys the orb object.
  */
  virtual ~OrbObject() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
