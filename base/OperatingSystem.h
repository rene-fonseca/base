/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Object.h>
#include <base/NotSupported.h>
#include <base/OutOfRange.h>
#include <base/Literal.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Operating system.
  
  @short Operating system.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.2
*/

class OperatingSystem : public Object {
public:

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  /**
    The type of an ordinary resource handle (do NOT assume anything about this
    type).
  */
  typedef void* Handle;

  /** This constants indicates an invalid handle. */
  static const Handle INVALID_HANDLE;
#else
  /**
    The type of an ordinary resource handle (do NOT assume anything about this
    type).
  */
  typedef int Handle;
  
  /** This constant indicates an invalid handle. */
  static const Handle INVALID_HANDLE = -1;
#endif

  /** Common error codes. */
  enum ErrorCode {
    OK, /**< No error. */
    ACCESS_DENIED, /**< Access denied. */ // TAG: why not PERMISSION_DENIED
    BROKEN_STREAM, /**< Broken stream. */
    BUSY, /**< Busy. */
    CANCELED, /**< Operation was canceled. */
    DEAD_LOCK, /**< Dead lock detected. */
    END_OF_FILE, /**< End of file. */
    ENTRY_NOT_FOUND, /**< The path does not specify an existing entry within the filesystem. */
    FILE_EXISTS, /**< File (or any entry) already exists. */
    FILE_NOT_FOUND, /**< The path does not specify an existing file. */
    FILE_TOO_LARGE, /**< File size limit exceeded. */
    FOLDER_EXISTS, /**< Path specifies an existing folder. */
    FOLDER_NOT_EMPTY, /**< Folder not empty. */
    INTERRUPTED, /**< Interrupted by signal. */
    INVALID_ADDRESS, /**< Invalid address used in argument. */
    INVALID_ARGUMENT, /**< Invalid argument. */
    INVALID_IOCTL_REQUEST, /**< Invalid I/O control request. */
    INVALID_PATH, /**< Invalid filesystem path. */
    IO_ERROR, /**< I/O error. */
    LOCK_VIOLATION, /**< Object already locked by other process. */
    NO_SUCH_DEVICE, /**< Device does not exist. */
    NOT_A_FOLDER, /**< Not a folder. */
    NOT_A_HANDLE, /**< Invalid handle to system object. */
    NOT_PERMITTED, /**< Operation was not permitted. */
    NOT_SUPPORTED, /**< Operation is not supported. */
    OPERATION_IN_PROGRESS, /**< Operation already in progress. */
    OUT_OF_MEMORY, /**< Out of memory. */
    OUT_OF_SPACE, /**< Out of space on device. */
    PERMISSION_DENIED, /**< Permission denied. */
    REQUEST_REFUSED, /**< Request was refused. */
    RETRY, /**< Resource is currently unavailable. Try again later. */
    SHARING_VIOLATION, /**< Object in-use by other process. */
    STACK_OVERFLOW, /**< Stack overflow. */
    TIME_OUT, /**< Operation timed out. */
    TOO_MANY_LINKS, /**< Too many levels of symbolic links. */
    TOO_MANY_OPEN_FILES, /**< Too many open files (or other objects). */
    UNSPECIFIED_ERROR /**< Unspecified error (used if non of the above errors match the error). */
  };
  
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
    Returns a string representation of the specified error code.
  */
  static Literal getErrorMessage(unsigned int error) throw();
  
  /**
    Returns the error code for the specified native error code.
  */
  static unsigned int getErrorCode(unsigned int error) throw();
  
  /**
    Returns the value of the specified system variable.
  */
  static long getVariable(Variable variable) throw(NotSupported);

  /**
    Get resource limit.

    @param resource The resource limit.
    @param type Selects between soft and hard limit. The default is
    SOFT_LIMIT.
  */
  static int64 getResourceLimit(
    Resource resource, LimitType type = SOFT_LIMIT) throw();

  /**
    Set resource limit.
    
    @param resource The resource limit.
    @param limit The limit.
    @param type Selects between soft and hard limit. The default is
    SOFT_LIMIT.
  */
  static void setResourceLimit(
    Resource resource,
    int64 limit,
    LimitType type = SOFT_LIMIT) throw(OutOfRange);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
