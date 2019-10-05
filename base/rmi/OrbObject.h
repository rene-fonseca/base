/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_RMI__ORB_OBJECT_H
#define _DK_SDU_MIP__BASE_RMI__ORB_OBJECT_H

#include <base/Object.h>
#include <base/rmi/OrbException.h>
#include <base/string/String.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Object Request Broker object.
  
  @short Object Request Broker object.
  @ingroup rmi
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class OrbObject : public Object {
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

#endif
