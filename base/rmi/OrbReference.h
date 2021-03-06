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

#include <base/Object.h>
#include <base/Primitives.h>
#include <base/rmi/OrbException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Direct reference to a skeleton object within a local address space.
  
  @short Orb skeleton reference.
  @ingroup rmi
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API OrbReference : public Object {
private:
  
  /** The id of the object. */
  uint64 id = 0;
public:
  
  /**
    Initializes the object reference.
  */
  inline OrbReference(uint64 _id) : id(_id)
  {
  }
  
  /**
    Initializes reference by reference.
  */
  inline OrbReference(const OrbReference& copy) noexcept : id(copy.id)
  {
  }

  /**
    Assignment of reference be reference.
  */
  inline OrbReference& operator=(const OrbReference& assign) noexcept
  {
    id = assign.id;
    return *this;
  }

  /**
    Returns true if the objects are equal.
  */
  inline bool operator==(const OrbReference& compare) const noexcept
  {
    return id == compare.id;
  }
  
  /**
    Returns true if the objects are non-equal.
  */
  inline bool operator!=(const OrbReference& compare) const noexcept
  {
    return id != compare.id;
  }
  
  /**
    Returns the skeleton reference value.
  */
  inline uint64 getId() const noexcept
  {
    return id;
  }
};

template<>
class Hash<OrbReference> {
public:
  
  /**
    Returns the hash value of the specified object.
  */
  inline unsigned long operator()(const OrbReference& value) noexcept
  {
    return 257 * value.getId() + (value.getId() >> 32); // TAG: need better hash function
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
