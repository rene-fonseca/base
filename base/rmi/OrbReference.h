/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_RMI__ORB_REFERENCE_H
#define _DK_SDU_MIP__BASE_RMI__ORB_REFERENCE_H

#include <base/Object.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Direct reference to a skeleton object within a local address space.
  
  @short Orb skeleton reference.
  @ingroup rmi
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class OrbReference : public Object {
private:
  
  /** The id of the object. */
  uint64 id;
public:
  
  /**
    Initializes the object reference.
  */
  inline OrbReference(uint64 _id) throw(OrbException) : id(_id) {
  }
  
  /**
    Initializes reference by reference.
  */
  inline OrbReference(const OrbReference& copy) throw() : id(copy.id) {
  }

  /**
    Assignment of reference be reference.
  */
  inline OrbReference& operator=(const OrbReference& eq) throw() {
    id = eq.id;
    return *this;
  }

  /**
    Returns true if the objects are equal.
  */
  inline bool operator==(const OrbReference& eq) const throw() {
    return id == eq.id;
  }
  
  /**
    Returns true if the objects are non-equal.
  */
  inline bool operator!=(const OrbReference& eq) const throw() {
    return id != eq.id;
  }
  
  /**
    Returns the skeleton reference value.
  */
  inline uint64 getId() const throw() {
    return id;
  }
};

template<>
class Hash<OrbReference> {
public:
  
  /**
    Returns the hash value of the specified object.
  */
  inline unsigned long operator()(const OrbReference& value) throw() {
    return 257 * value.getId() + (value.getId() >> 32); // TAG: need better hash function
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
