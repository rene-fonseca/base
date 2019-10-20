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

#include <base/concurrency/AtomicCounter.h>
#include <base/Type.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

class _COM_AZURE_DEV__BASE__API InstanceCounters {
public:

  PreferredAtomicCounter constructions;
  PreferredAtomicCounter destructions;

  void checkInstanceCounters(const Type& type, MemorySize size);
};

/**
  Instance counter for counting number of constructions and destructions for a given type.
  Use DebugOnlyInstanceCounter if you do not want to impact release builds.

  @code
  class MyClass : public InstanceCounter<MyClass> {
  public:
  };

  MyObject o;
  MemorySize instances = getInstanceCount<MyObject>();
  getInstanceCount(o);
  @endcode

  @ingroup memory
*/

template<class TYPE>
class InstanceCounter {
public:

  class InstanceCountersForType : public InstanceCounters {
  public:

    ~InstanceCountersForType()
    {
      checkInstanceCounters(Type::getType<TYPE>(), sizeof(TYPE));
    }
  };

  static InstanceCountersForType& getInstanceCounters() noexcept
  {
    static InstanceCountersForType counters;
    return counters;
  }

  InstanceCounter() noexcept
  {
    ++getInstanceCounters().constructions;
  }

  InstanceCounter(const InstanceCounter&) noexcept
  {
    ++getInstanceCounters().constructions;
  }

  InstanceCounter(InstanceCounter&&) noexcept
  {
  }

  InstanceCounter& operator=(const InstanceCounter&) noexcept
  {
    ++getInstanceCounters().constructions;
    return *this;
  }

  InstanceCounter& operator=(InstanceCounter&&) noexcept
  {
    return *this;
  }

  /** Returns the current number of instances. */
  MemorySize getNumberOfInstances() noexcept
  {
    return getInstanceCounters().constructions - getInstanceCounters().destructions;
  }

  /** Returns the total number of constructed instances. */
  MemorySize getNumberOfConstructedInstances() noexcept
  {
    return getInstanceCounters().constructions;
  }

  /** Returns the total number of destructed instances. */
  MemorySize getNumberOfDestructedInstances() noexcept
  {
    return getInstanceCounters().constructions;
  }

  ~InstanceCounter()
  {
    ++getInstanceCounters().destructions;
  }
};

/** Instance counter which if only enabled for debug builds. */
#if defined(_COM_AZURE_DEV__BASE__ANY_DEBUG)
template<class TYPE>
class DebugOnlyInstanceCounter : public InstanceCounter<TYPE> {
public:
};
#else
template<class TYPE>
class DebugOnlyInstanceCounter {
public:
};
#endif

/** Returns the number of current instances for the given type. */
template<class TYPE>
inline MemorySize getInstanceCount() noexcept
{
  auto counters = InstanceCounter<TYPE>::getInstanceCounters();
  return counters.constructions - counters.destructions;
}

/** Returns the number of constructed instances for the given type. */
template<class TYPE>
inline MemorySize getInstanceConstructionCount() noexcept
{
  return InstanceCounter<TYPE>::getInstanceCounters().constructions;
}

/** Returns the number of destructed instances for the given type. */
template<class TYPE>
inline MemorySize getInstanceDestructionCount() noexcept
{
  return InstanceCounter<TYPE>::getInstanceCounters().destructions;
}

/** Returns the number of current instances for the given object type. */
template<class TYPE>
inline MemorySize getInstanceCountFor(const TYPE&) noexcept
{
  return getInstanceConstructionCount<TYPE>();
}

/** Returns the number of constructed instances for the given object type. */
template<class TYPE>
inline MemorySize getInstanceConstructionCountFor(const TYPE&) noexcept
{
  return getInstanceConstructionCount<TYPE>();
}

/** Returns the number of destructed instances for the given object type. */
template<class TYPE>
inline MemorySize getInstanceDestructionCountFor(const TYPE&) noexcept
{
  return getInstanceDestructionCount<TYPE>();
}

/** Returns true if the given object is instance counted. */
template<class TYPE>
inline bool isInstanceCounted(const TYPE* v) noexcept
{
  return dynamic_cast<const InstanceCounter<TYPE>*>(v);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
