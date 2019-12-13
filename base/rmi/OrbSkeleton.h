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

#include <base/mem/ReferenceCountedObject.h>
#include <base/rmi/OrbException.h>
#include <base/string/String.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Object Request Broker skeleton.
  
  @short Object Request Broker skeleton.
  @ingroup rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API OrbSkeleton : public Object, public virtual ReferenceCountedObject {
private:

  /** The fully qualified name of the skeleton. */
  String name;
  /** The version of the skeleton. */
  unsigned int version = 0;
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
  inline const String& getOrbName() const throw() {
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
  virtual ~OrbSkeleton() noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
