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

  static PreferredAtomicCounter id; // TAG: share id with Thread?
public:

  /** Description for resource. */
  String name;
  /** Thread created object. */
  unsigned int createdById = 0;
  /** Unique resource id. */
  unsigned int resourceId = 0;

  /** Initializes resource meta info. */
  inline ResourceHandle() noexcept
  {
    if (Profiler::isEnabledDirect()) {
      name = Thread::getThreadName(); // TAG: this may change over time - what should we do - can we lookup thread name instead
      // TAG: manage lot for Thread
      createdById = Thread::getThreadSimpleId();
      resourceId = static_cast<unsigned int>(++id);
    }
  }

  /** Returns the ID of the thread that created the resource. */
  inline unsigned int getCreatedById() const noexcept
  {
    return createdById;
  }

  /** Returns the name of the thread that created the resource. */
  inline const String& getCreateByName() const noexcept
  {
    return name;
  }

  /** Returns the unique resource ID. */
  inline unsigned int getResourceId() const noexcept
  {
    return resourceId;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
