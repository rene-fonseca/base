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
#  include <sys/resource.h>

#  if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__IRIX65)
#    define _SC_NPROCESSORS_CONF _SC_NPROC_CONF
#    define _SC_NPROCESSORS_ONLN _SC_NPROC_ONLN
#  endif

#  if (!defined(_SC_IOV_MAX))
#    define _SC_IOV_MAX -1
#  endif

#  if (!defined(_SC_HOST_NAME_MAX))
#    define _SC_HOST_NAME_MAX -1
#  endif

#  if (!defined(_SC_SYMLOOP_MAX))
#    define _SC_SYMLOOP_MAX -1
#  endif

#  if (!defined(_SC_ACL))
#    define _SC_ACL -1
#  endif

#  if (!defined(_SC_AUDIT))
#    define _SC_AUDIT -1
#  endif

#undef _POSIX_AIO_LISTIO_MAX
#define _POSIX_AIO_LISTIO_MAX 2
#undef _POSIX_AIO_MAX
#define _POSIX_AIO_MAX 1
#undef _POSIX_ARG_MAX
#define _POSIX_ARG_MAX 4096
#undef _POSIX_CHILD_MAX
#define _POSIX_CHILD_MAX 6
#undef _POSIX_DELAYTIMER_MAX
#define _POSIX_DELAYTIMER_MAX 32
#undef _POSIX_HOST_NAME_MAX
#define _POSIX_HOST_NAME_MAX 255
#undef _POSIX_LINK_MAX
#define _POSIX_LINK_MAX 8
#undef _POSIX_LOGIN_NAME_MAX
#define _POSIX_LOGIN_NAME_MAX 9
#undef _POSIX_MAX_CANON
#define _POSIX_MAX_CANON 255
#undef _POSIX_MAX_INPUT
#define _POSIX_MAX_INPUT 255
#undef _POSIX_MQ_OPEN_MAX
#define _POSIX_MQ_OPEN_MAX 8
#undef _POSIX_MQ_PRIO_MAX
#define _POSIX_MQ_PRIO_MAX 32
#undef _POSIX_NAME_MAX
#define _POSIX_NAME_MAX 14
#undef _POSIX_NGROUPS_MAX
#define _POSIX_NGROUPS_MAX 8
#undef _POSIX_OPEN_MAX
#define _POSIX_OPEN_MAX 20
#undef _POSIX_PATH_MAX
#define _POSIX_PATH_MAX 256
#undef _POSIX_PIPE_BUF
#define _POSIX_PIPE_BUF 512
#undef _POSIX_RE_DUP_MAX
#define _POSIX_RE_DUP_MAX 255
#undef _POSIX_RTSIG_MAX
#define _POSIX_RTSIG_MAX 8
#undef _POSIX_SEM_NSEMS_MAX
#define _POSIX_SEM_NSEMS_MAX 256
#undef _POSIX_SEM_VALUE_MAX
#define _POSIX_SEM_VALUE_MAX 32767
#undef _POSIX_SIGQUEUE_MAX
#define _POSIX_SIGQUEUE_MAX 32
#undef _POSIX_SSIZE_MAX
#define _POSIX_SSIZE_MAX 32767
#undef _POSIX_STREAM_MAX
#define _POSIX_STREAM_MAX 8
#undef _POSIX_SS_REPL_MAX
#define _POSIX_SS_REPL_MAX 4
#undef _POSIX_SYMLINK_MAX
#define _POSIX_SYMLINK_MAX 255
#undef _POSIX_SYMLOOP_MAX
#define _POSIX_SYMLOOP_MAX 8
#undef _POSIX_THREAD_DESTRUCTOR_ITERATIONS
#define _POSIX_THREAD_DESTRUCTOR_ITERATIONS 4
#undef _POSIX_THREAD_KEYS_MAX
#define _POSIX_THREAD_KEYS_MAX 128
#undef _POSIX_THREAD_THREADS_MAX
#define _POSIX_THREAD_THREADS_MAX 64
#undef _POSIX_TIMER_MAX
#define _POSIX_TIMER_MAX 32
#undef _POSIX_TRACE_EVENT_NAME_MAX
#define _POSIX_TRACE_EVENT_NAME_MAX 30
#undef _POSIX_TRACE_NAME_MAX
#define _POSIX_TRACE_NAME_MAX 8
#undef _POSIX_TRACE_SYS_MAX
#define _POSIX_TRACE_SYS_MAX 8
#undef _POSIX_TRACE_USER_EVENT_MAX
#define _POSIX_TRACE_USER_EVENT_MAX 32
#undef _POSIX_TTY_NAME_MAX
#define _POSIX_TTY_NAME_MAX 9
#undef _POSIX_TZNAME_MAX
#define _POSIX_TZNAME_MAX 6
#undef _POSIX2_BC_BASE_MAX
#define _POSIX2_BC_BASE_MAX 99
#undef _POSIX2_BC_DIM_MAX
#define _POSIX2_BC_DIM_MAX 2048
#undef _POSIX2_BC_SCALE_MAX
#define _POSIX2_BC_SCALE_MAX 99
#undef _POSIX2_BC_STRING_MAX
#define _POSIX2_BC_STRING_MAX 1000
#undef _POSIX2_CHARCLASS_NAME_MAX
#define _POSIX2_CHARCLASS_NAME_MAX 14
#undef _POSIX2_COLL_WEIGHTS_MAX
#define _POSIX2_COLL_WEIGHTS_MAX 2
#undef _POSIX2_EXPR_NEST_MAX
#define _POSIX2_EXPR_NEST_MAX 32
#undef _POSIX2_LINE_MAX
#define _POSIX2_LINE_MAX 2048
#undef _POSIX2_RE_DUP_MAX
#define _POSIX2_RE_DUP_MAX 255
#undef _XOPEN_IOV_MAX
#define _XOPEN_IOV_MAX 16
#undef _XOPEN_NAME_MAX
#define _XOPEN_NAME_MAX 255
#undef _XOPEN_PATH_MAX
#define _XOPEN_PATH_MAX 1024

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
    _SC_PAGESIZE, // PAGE_SIZE
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

int64 OperatingSystem::getResourceLimit(Resource resource, LimitType type) throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  throw NotSupported(Type::getType<OperatingSystem>());
#else // unix
#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
  static const __rlimit_resource_t RESOURCES[] =
#else
  static const int RESOURCES[] =
#endif
  {
    RLIMIT_CORE,
    RLIMIT_CPU,
    RLIMIT_DATA,
    RLIMIT_FSIZE,
    RLIMIT_NOFILE,
    RLIMIT_STACK,
    RLIMIT_AS
  };
#if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
  struct rlimit64 limit;
  ::getrlimit64(RESOURCES[resource], &limit); // must not fail
#else
  struct rlimit limit;
  ::getrlimit(RESOURCES[resource], &limit); // must not fail
#endif // LFS
  rlim_t value = (type == SOFT_LIMIT) ? limit.rlim_cur : limit.rlim_max;
  if (value == RLIM_INFINITY) {
    return -1;
  } else if (value == RLIM_SAVED_MAX) {
    return limit.rlim_max;
  } else if (value == RLIM_SAVED_CUR) {
    return limit.rlim_cur;
  } else {
    return value;
  }
#endif // flavor
}

void OperatingSystem::setResourceLimit(Resource resource, int64 limit, LimitType type) throw(OutOfRange) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  throw NotSupported(Type::getType<OperatingSystem>());
#else // unix
#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
  static const __rlimit_resource_t RESOURCES[] =
#else
  static const int RESOURCES[] =
#endif
  {
    RLIMIT_CORE,
    RLIMIT_CPU,
    RLIMIT_DATA,
    RLIMIT_FSIZE,
    RLIMIT_NOFILE,
    RLIMIT_STACK,
    RLIMIT_AS
  };
#  if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
  assert(
    (limit >= -1) &&
    (limit != RLIM_INFINITY) &&
    (limit != RLIM_SAVED_MAX) &&
    (limit != RLIM_SAVED_CUR),
    OutOfRange(Type::getType<OperatingSystem>())
  );
  struct rlimit64 rlimit;
  ::getrlimit64(RESOURCES[resource], &rlimit); // must not fail
  if (type == SOFT_LIMIT) {
    rlimit.rlim_cur = (limit == -1) ? RLIM_INFINITY : limit;
  } else {
    rlimit.rlim_max = (limit == -1) ? RLIM_INFINITY : limit;
  }
  assert(
    ::setrlimit64(RESOURCES[resource], &rlimit) == 0,
    OutOfRange(Type::getType<OperatingSystem>())
  );
#  else
  assert(
    (limit <= PrimitiveTraits<rlim_t>::MAXIMUM) &&
    (limit >= -1) &&
    (limit != RLIM_INFINITY) &&
    (limit != RLIM_SAVED_MAX) &&
    (limit != RLIM_SAVED_CUR),
    OutOfRange(Type::getType<OperatingSystem>())
  );
  struct rlimit rlimit;
  ::getrlimit(RESOURCES[resource], &rlimit); // must not fail
  if (type == SOFT_LIMIT) {
    rlimit.rlim_cur = (limit == -1) ? RLIM_INFINITY : limit;
  } else {
    rlimit.rlim_max = (limit == -1) ? RLIM_INFINITY : limit;
  }
  assert(
    ::setrlimit(RESOURCES[resource], &rlimit) == 0,
    OutOfRange(Type::getType<OperatingSystem>())
  );
#  endif // LFS
#endif // flavor
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
