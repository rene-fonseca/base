/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_RMI__ORB_SKELETON_H
#define _DK_SDU_MIP__BASE_RMI__ORB_SKELETON_H

#include <base/mem/ReferenceCountedObject.h>
#include <base/rmi/OrbException.h>
#include <base/string/String.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Object Request Broker skeleton.
  
  @short Object Request Broker skeleton.
  @ingroup rmi
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class OrbSkeleton : public Object, public virtual ReferenceCountedObject {
private:

  /** The fully qualified name of the skeleton. */
  String name;
  /** The version of the skeleton. */
  unsigned int version;
public:
  
  /**
    Initializes the ORB skeleton.
    
    @param name The fully qualified name.
    @param version The version of the interface.
  */
  OrbSkeleton(const String& name, unsigned int version) throw(OrbException);
  
  /**
    Returns the fully qualified name of the skeleton.
  */
  inline String getOrbName() const throw() {
    return name;
  }
  
  /**
    Returns the version of the skeleton.
  */
  inline unsigned int getVersion() const throw() {
    return version;
  }
  
  /**
    Destroys the ORB skeleton.
  */
  virtual ~OrbSkeleton() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
