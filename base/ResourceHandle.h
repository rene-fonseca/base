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

#include <base/Profiler.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Resource handle meta information for profiling.
*/

class _COM_AZURE_DEV__BASE__API ResourceHandle : public ReferenceCountedObject {
private:

  /** Unique ID generator. */
  static PreferredAtomicCounter id; // TAG: share id with Thread?
  /** Current number of resources. */
  static PreferredAtomicCounter total;
private:

  /** Description for resource - set explicitly. */
  Reference<ReferenceCountedAllocator<char> > description; // do NOT use String here to avoid cyclic depends
  /** Thread created object. */
  unsigned int createdById = 0;
  /** Unique resource id. */
  unsigned int resourceId = 0;
  // could add flag to allow profiling for specific handle
public:
  
  /** Returns the number of open resources. */
  static inline unsigned int getResources() noexcept
  {
    return total;
  }
  
  /** Initializes resource meta info. */
  inline ResourceHandle() noexcept
  {
    if (Profiler::isEnabledDirect()) {
      createdById = Thread::getThreadSimpleId();
      resourceId = static_cast<unsigned int>(++id);
    }
    ++total;
  }

  /** Returns the description of the resource. */
  inline String getDescription() const noexcept
  {
    return description;
  }

  /** Sets the description of the resource. */
  inline void setDescription(const String& _description) noexcept
  {
    description = _description.getContainer();
  }

  /** Returns the ID of the thread that created the resource. */
  inline unsigned int getCreatedById() const noexcept
  {
    return createdById;
  }

  /** Returns the unique resource ID. */
  inline unsigned int getResourceId() const noexcept
  {
    return resourceId;
  }

  inline ~ResourceHandle() noexcept
  {
    --total;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
