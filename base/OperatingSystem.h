/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__OPERATING_SYSTEM_H
#define _DK_SDU_MIP__BASE__OPERATING_SYSTEM_H

#include <base/Object.h>
#include <base/NotSupported.h>
#include <base/OutOfRange.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Operating system.
  
  @short Operating system
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.2
*/

class OperatingSystem : public Object {
public:

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  /** The type of an ordinary resource handle (do NOT assume anything about this type). */
  typedef void* Handle;

  /** This constants indicates an invalid handle. */
  static const Handle INVALID_HANDLE = static_cast<char*>(0) - 1;
#else
  /** The type of an ordinary resource handle (do NOT assume anything about this type). */
  typedef int Handle;
  
  /** This constant indicates an invalid handle. */
  static const Handle INVALID_HANDLE = -1;
#endif

  /** Operating system variables. */
  enum Variable {
    MAX_NUM_OF_ASYNC_IO_OPRS_PER_LIST,
    MAX_NUM_OF_PENDING_ASYNC_IO_OPRS,
    MAX_LEN_OF_ARGUMENTS,
    MAX_NUM_OF_SIMULTANEOUS_PROCESSES_PER_REAL_UID,
    MAX_LEN_OF_HOST_NAME,
    MAX_NUM_OF_IOVS,
    MAX_LEN_OF_LOGIN_NAME,
    MAX_NUM_OF_OPEN_MESSAGE_QUEUES,
    MAX_NUM_OF_OPEN_FILES,
    PAGE_SIZE,
    MAX_NUM_OF_THREAD_KEYS,
    MIN_SIZE_OF_THREAD_STACK,
    MAX_NUM_OF_THREADS,
    MAX_NUM_OF_SEMAPHORES,
    MAX_VALUE_OF_SEMAPHORE,
    MAX_NUM_OF_QUEUED_SIGNALS,
    MAX_NUM_OF_NESTED_SYMBOLIC_LINKS,
    MAX_NUM_OF_TIMERS,
    MAX_NUM_OF_SUPPLEMENTARY_GIDS,
    TICKS_PER_SECOND,
    SUPPORTS_JOB_CONTROL,
    NUM_OF_CONFIGURED_PROCESSORS,
    NUM_OF_ONLINE_PROCESSORS,
    SUPPORTS_ACCESSS_CONTROL_LISTS,
    SUPPORTS_AUDIT,
    SUPPORTS_ASYNC_IO,
    SUPPORTS_MAPPED_FILES,
    SUPPORTS_PROCESS_SHARED_SYNCHRONIZATION
  };

  enum Resource {
    RESOURCE_CORE, /**< Core file size. */
    RESOURCE_CPU, /**< CPU time. */
    RESOURCE_DATA, /**< Data segment size. */
    RESOURCE_FILE_SIZE, /**< File size. */
    RESOURCE_OPEN_FILES, /**< Number of open files. */
    RESOURCE_STACK, /**< Stack size. */
    RESOURCE_ADDRESS_SPACE /**< Address space size. */
  };

  /** The limit type. */
  enum LimitType {
    HARD_LIMIT, /**< Hard limit. */
    SOFT_LIMIT /**< Soft limit. */
  };
  
  /**
    Returns the value of the specified system variable.
  */
  static long getVariable(Variable variable) throw(NotSupported);

  /**
    Get resource limit.

    @param resource The resource limit.
    @param type Selects between soft and hard limit . The default is SOFT_LIMIT.
  */
  static int64 getResourceLimit(Resource resource, LimitType type = SOFT_LIMIT) throw();

  /**
    Set resource limit.
    
    @param resource The resource limit.
    @param limit The limit.
    @param type Selects between soft and hard limit . The default is SOFT_LIMIT.
  */
  static void setResourceLimit(Resource resource, int64 limit, LimitType type = SOFT_LIMIT) throw(OutOfRange);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
