/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Object.h>
#include <base/collection/HashTable.h>
#include <base/mathematics/AmbiguousRegistration.h>
#include <base/mem/Reference.h>
#include <base/rmi/OrbException.h>
#include <base/rmi/OrbReference.h>
#include <base/rmi/OrbSkeleton.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Object Request Broker skeleton manager responsible for maintaining the
  skeletons for the ORB and associated channel independent cached context
  information.
  
  @short Object Request Broker skeleton manager.
  @ingroup rmi
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class OrbSkeletonManager : public Object {
private:

  /** Mapping of fully qualified names to skeleton objects. */
  HashTable<String, Reference<OrbSkeleton> > names;
  /** Mapping of identifiers to skeleton objects. */
  HashTable<OrbReference, Reference<OrbSkeleton> > skeletons;
  // TAG: maintain channel independent identifier cache
public:
  
  /**
    Initializes the ORB skeleton.
    
    @param name The fully qualified name.
    @param version The version of the interface.
  */
  OrbSkeletonManager() throw(MemoryException);
  
  /**
    Returns the current number of registered skeletons.
  */
  inline unsigned int getNumberOfSkeletons() const throw() {
    return skeletons.getSize();
  }
  
  /**
    Returns true if the skeleton with the specified fully qualified name has been
    registered.
  */
  inline bool hasSkeleton(const String& name) const throw() {
    return names.isKey(name);
  }
  
  /**
    Returns true if the skeleton with the specified identifier has been
    registered.
  */
  inline bool hasSkeleton(const OrbReference& reference) const throw() {
    return skeletons.isKey(reference);
  }

  /**
    Returns the skeleton with the specified fully qualified name.
  */
  inline Reference<OrbSkeleton> getSkeleton(const String& name) const throw(InvalidKey) {
    return names[name];
  }
  
  /**
    Returns the skeleton with the specified local reference.
  */
  inline Reference<OrbSkeleton> getSkeleton(const OrbReference& reference) const throw(InvalidKey) {
    return skeletons[reference];
  }
  
  /**
    Registers the specified skeleton.
  */
  void add(Reference<OrbSkeleton> skeleton) throw(AmbiguousRegistration, MemoryException);
  
  /**
    Deregisters the specified skeleton.
  */
  void remove(Reference<OrbSkeleton> skeleton) throw(InvalidKey);

  /**
    Deregisters the specified skeleton.
  */
  void remove(const String& name) throw(InvalidKey);
  
  /**
    Destroys the ORB skeleton manager.
  */
  ~OrbSkeletonManager() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
