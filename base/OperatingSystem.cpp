/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/OperatingSystem.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <windows.h>
#  include <lmaccess.h>
#  include <ws2tcpip.h>
#else // unix
#  include <limits.h>
#  include <unistd.h>
#  include <errno.h>

#  if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__IRIX65)
#    define _SC_NPROCESSORS_CONF _SC_NPROC_CONF
#    define _SC_NPROCESSORS_ONLN _SC_NPROC_ONLN
#  endif

#  if (!defined(_SC_HOST_NAME_MAX))
#    define _SC_HOST_NAME_MAX -1
#  endif

#  if (!defined(_SC_SYMLOOP_MAX))
#    define _SC_SYMLOOP_MAX -1
#  endif

#  if (!defined(_SC_ACL_MAX))
#    define _SC_ACL -1
#  endif

#  if (!defined(_SC_AUDIT_MAX))
#    define _SC_AUDIT -1
#  endif

#if (!defined(_POSIX_HOST_NAME_MAX))
#  define _POSIX_HOST_NAME_MAX 255
#endif

#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

long OperatingSystem::getVariable(Variable variable) throw(NotSupported) {
  // TAG: need to cache all cacheable variables
  
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  switch (variable) {
  case MAX_NUM_OF_ASYNC_IO_OPRS_PER_LIST:
    return PrimitiveTraits<unsigned long>::MAXIMUM; // unknown
  case MAX_NUM_OF_PENDING_ASYNC_IO_OPRS:
    return PrimitiveTraits<unsigned long>::MAXIMUM; // unknown
  case MAX_LEN_OF_ARGUMENTS:
    return PrimitiveTraits<unsigned long>::MAXIMUM; // unknown
  case MAX_NUM_OF_SIMULTANEOUS_PROCESSES_PER_REAL_UID:
    return PrimitiveTraits<unsigned long>::MAXIMUM; // unknown
  case MAX_LEN_OF_HOST_NAME:
    return NI_MAXHOST; // TAG: does this include terminating null
  case MAX_NUM_OF_IOVS:
    return PrimitiveTraits<unsigned long>::MAXIMUM; // unknown
  case MAX_LEN_OF_LOGIN_NAME:
    return UNLEN;
  case MAX_NUM_OF_OPEN_MESSAGE_QUEUES:
    return PrimitiveTraits<unsigned long>::MAXIMUM; // unknown
  case MAX_NUM_OF_OPEN_FILES:
    return PrimitiveTraits<unsigned long>::MAXIMUM; // unknown
  case PAGE_SIZE:
    {
      SYSTEM_INFO information;
      ::GetSystemInfo(&information);
      return information.dwPageSize;
    }
  case MAX_NUM_OF_THREAD_KEYS:
    return TLS_MINIMUM_AVAILABLE;
  case MIN_SIZE_OF_THREAD_STACK:
    return 0; // unknown
  case MAX_NUM_OF_THREADS:
    return PrimitiveTraits<unsigned long>::MAXIMUM; // unknown
  case MAX_NUM_OF_SEMAPHORES:
    return PrimitiveTraits<unsigned long>::MAXIMUM; // unknown
  case MAX_VALUE_OF_SEMAPHORE:
    return PrimitiveTraits<unsigned long>::MAXIMUM; // unknown
  case MAX_NUM_OF_QUEUED_SIGNALS:
    return PrimitiveTraits<unsigned long>::MAXIMUM; // unknown
  case MAX_NUM_OF_NESTED_SYMBOLIC_LINKS:
    return PrimitiveTraits<unsigned long>::MAXIMUM; // unknown
  case MAX_NUM_OF_TIMERS:
    return PrimitiveTraits<unsigned long>::MAXIMUM; // unknown
  case MAX_NUM_OF_SUPPLEMENTARY_GIDS:
    return PrimitiveTraits<unsigned long>::MAXIMUM; // unknown
  case TICKS_PER_SECOND:
    {
      LARGE_INTEGER frequency; // ticks per second
      ::QueryPerformanceFrequency(&frequency); // ignore any error
      return static_cast<unsigned long>(frequency.QuadPart);
    }
  case SUPPORTS_JOB_CONTROL:
    return 0; // TAG: w2k has job control support
  case NUM_OF_CONFIGURED_PROCESSORS:
    {
      SYSTEM_INFO information;
      ::GetSystemInfo(&information);
      return information.dwNumberOfProcessors;
    }
  case NUM_OF_ONLINE_PROCESSORS:
    {
      SYSTEM_INFO information;
      ::GetSystemInfo(&information);
      unsigned long result = 0;
      unsigned int count = information.dwNumberOfProcessors;
      for (unsigned int bit = 0; (count > 0) && (bit < sizeof(DWORD) * 8); ++bit, --count) {
        if (information.dwActiveProcessorMask & (1 << bit)) {
          ++result;
        }
      }
      return result;
    }
  case MAX_LEN_OF_PASSWORD:
    return MAX_PASSWD_LEN; // lmaccess
  case SUPPORTS_ACCESSS_CONTROL_LISTS:
    return 1;
  case SUPPORTS_AUDIT:
    return 1;
  case SUPPORTS_ASYNC_IO:
    return 1;
  case SUPPORTS_MAPPED_FILES:
    return 1;
  case SUPPORTS_PROCESS_SHARED_SYNCHRONIZATION:
    return 1;
  }
  throw NotSupported("System variable not supported", Type::getType<OperatingSystem>());
#else // unix
  static int SYSTEM_VARIABLES[SUPPORTS_PROCESS_SHARED_SYNCHRONIZATION + 1] = {
    _SC_AIO_LISTIO_MAX, // MAX_NUM_OF_ASYNC_IO_OPRS_PER_LIST
    _SC_AIO_MAX, // MAX_NUM_OF_PENDING_ASYNC_IO_OPRS
    _SC_ARG_MAX, // MAX_LEN_OF_ARGUMENTS
    _SC_CHILD_MAX, // MAX_NUM_OF_SIMULTANEOUS_PROCESSES_PER_REAL_UID
    _SC_HOST_NAME_MAX, // MAX_LEN_OF_HOST_NAME
    _SC_IOV_MAX, // MAX_NUM_OF_IOVS
    _SC_LOGIN_NAME_MAX, // MAX_LEN_OF_LOGIN_NAME
    _SC_MQ_OPEN_MAX, // MAX_NUM_OF_OPEN_MESSAGE_QUEUES
    _SC_OPEN_MAX, // MAX_NUM_OF_OPEN_FILES
    _SC_PAGE_SIZE, // PAGE_SIZE
    _SC_THREAD_KEYS_MAX, // MAX_NUM_OF_THREAD_KEYS
    _SC_THREAD_STACK_MIN, // MIN_SIZE_OF_THREAD_STACK
    _SC_THREAD_THREADS_MAX, // MAX_NUM_OF_THREADS
    _SC_SEM_NSEMS_MAX, // MAX_NUM_OF_SEMAPHORES
    _SC_SEM_VALUE_MAX, // MAX_VALUE_OF_SEMAPHORE
    _SC_SIGQUEUE_MAX, // MAX_NUM_OF_QUEUED_SIGNALS
    _SC_SYMLOOP_MAX, // MAX_NUM_OF_NESTED_SYMBOLIC_LINKS
    _SC_TIMER_MAX, // MAX_NUM_OF_TIMERS
    _SC_NGROUPS_MAX, // MAX_NUM_OF_SUPPLEMENTARY_GIDS
    _SC_CLK_TCK, // TICKS_PER_SECOND
    _SC_JOB_CONTROL, // SUPPORTS_JOB_CONTROL
    _SC_NPROCESSORS_CONF, // NUM_OF_CONFIGURED_PROCESSORS
    _SC_NPROCESSORS_ONLN, // NUM_OF_ONLINE_PROCESSORS
    _SC_PASS_MAX, // MAX_LEN_OF_PASSWORD
    _SC_ACL, // SUPPORTS_ACCESSS_CONTROL_LISTS
    _SC_AUDIT, // SUPPORTS_AUDIT
    _SC_ASYNCHRONOUS_IO, // SUPPORTS_ASYNC_IO
    _SC_MAPPED_FILES, // SUPPORTS_MAPPED_FILES
    _SC_THREAD_PROCESS_SHARED // SUPPORTS_PROCESS_SHARED_SYNCHRONIZATION
  };
  
  int systemVariable = SYSTEM_VARIABLES[variable];
  if (systemVariable != -1) {
    errno = 0;
    long result = ::sysconf(systemVariable);
    if (!((result == -1) && (errno != 0))) {
      return result;
    }
  }
  
  switch (variable) {
  case MAX_NUM_OF_ASYNC_IO_OPRS_PER_LIST:
    return _POSIX_AIO_LISTIO_MAX;
  case MAX_NUM_OF_PENDING_ASYNC_IO_OPRS:
    return _POSIX_AIO_MAX;
  case MAX_LEN_OF_ARGUMENTS:
    return _POSIX_ARG_MAX;
  case MAX_NUM_OF_SIMULTANEOUS_PROCESSES_PER_REAL_UID:
    return _POSIX_CHILD_MAX;
  case MAX_LEN_OF_HOST_NAME:
    return _POSIX_HOST_NAME_MAX;
  case MAX_NUM_OF_IOVS:
    return _XOPEN_IOV_MAX;
  case MAX_LEN_OF_LOGIN_NAME:
    return _POSIX_LOGIN_NAME_MAX - 1; // without terminating null
  case MAX_NUM_OF_OPEN_MESSAGE_QUEUES:
    return _POSIX_MQ_OPEN_MAX;
  case MAX_NUM_OF_OPEN_FILES:
    return _POSIX_OPEN_MAX;
  case PAGE_SIZE:
    break;
  case MAX_NUM_OF_THREAD_KEYS:
    return _POSIX_THREAD_KEYS_MAX;
  case MIN_SIZE_OF_THREAD_STACK:
    return 0;
  case MAX_NUM_OF_THREADS:
    return _POSIX_THREAD_THREADS_MAX;
  case MAX_NUM_OF_SEMAPHORES:
    return _POSIX_SEM_NSEMS_MAX;
  case MAX_VALUE_OF_SEMAPHORE:
    return _POSIX_SEM_VALUE_MAX;
  case MAX_NUM_OF_QUEUED_SIGNALS:
    return _POSIX_SIGQUEUE_MAX;
  case MAX_NUM_OF_NESTED_SYMBOLIC_LINKS:
    return _POSIX_LINK_MAX;
  case MAX_NUM_OF_TIMERS:
    return _POSIX_TIMER_MAX;
  case MAX_NUM_OF_SUPPLEMENTARY_GIDS:
    return _POSIX_NGROUPS_MAX;
  case TICKS_PER_SECOND:
    break;
  case SUPPORTS_JOB_CONTROL:
    return 1; // TAG: what about this
  case NUM_OF_CONFIGURED_PROCESSORS:
    break;
  case NUM_OF_ONLINE_PROCESSORS:
    break;
  case MAX_LEN_OF_PASSWORD:
    break;
  case SUPPORTS_ACCESSS_CONTROL_LISTS:
    return 0; // TAG: what about this
  case SUPPORTS_AUDIT:
    return 0; // TAG: what about this
  case SUPPORTS_ASYNC_IO:
    return (_POSIX_AIO_MAX > 0) ? 1 : 0;
  case SUPPORTS_MAPPED_FILES:
    return 1; // TAG: what about this
  case SUPPORTS_PROCESS_SHARED_SYNCHRONIZATION:
    return 0; // TAG: what about this
  }
  throw NotSupported("System variable not supported", Type::getType<OperatingSystem>());
#endif // flavor
}

/*
Other variables:

_POSIX_LINK_MAX
_POSIX_NAME_MAX
_POSIX_SYMLINK_MAX
_POSIX_PIPE_BUF
_POSIX_PATH_MAX
_POSIX_SYMLINK_MAX

*/

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
