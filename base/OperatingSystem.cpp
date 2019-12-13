/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/OperatingSystem.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
// #  include <windows.h>
#  include <Ws2tcpip.h>
#  include <lmaccess.h>
#else // unix
#  include <limits.h>
#  include <unistd.h>
#  include <errno.h>
#  include <sys/resource.h>
//#  include <sys/types.h>
//#  include <sys/sysctl.h>

#  if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__IRIX65)
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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
const OperatingSystem::Handle OperatingSystem::INVALID_HANDLE = static_cast<char*>(nullptr) - 1;
#endif

long OperatingSystem::getVariable(Variable variable) throw(NotSupported) {
  // TAG: need to cache all cacheable variables
  
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
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
        if (information.dwActiveProcessorMask & (1ULL << bit)) {
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
  throw NotSupported(
    "System variable not supported",
    Type::getType<OperatingSystem>()
  );
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
  throw NotSupported(
    "System variable not supported",
    Type::getType<OperatingSystem>()
  );
#endif // flavor
}

int64 OperatingSystem::getResourceLimit(
  Resource resource, LimitType type) throw() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return 0;
  // throw NotSupported(Type::getType<OperatingSystem>());
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  return 0;
#else // unix
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
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
#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__OPENBSD)
    RLIMIT_AS
#endif
  };
#if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM) && \
    !(_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREEBSD) && \
    !(_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__OPENBSD)
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
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  throw NotSupported(Type::getType<OperatingSystem>());
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  throw NotSupported(Type::getType<OperatingSystem>());
#else // unix
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
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
#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__OPENBSD)
    RLIMIT_AS
#endif
  };
#if defined(_COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM) && \
    !(_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREEBSD) && \
    !(_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__OPENBSD)
  bassert(
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
  bassert(
    ::setrlimit64(RESOURCES[resource], &rlimit) == 0,
    OutOfRange(Type::getType<OperatingSystem>())
  );
#  else
  bassert(
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
  bassert(
    ::setrlimit(RESOURCES[resource], &rlimit) == 0,
    OutOfRange(Type::getType<OperatingSystem>())
  );
#  endif // LFS
#endif // flavor
}

Literal OperatingSystem::getErrorMessage(unsigned int error) throw() {
  static const Literal
    ERROR_MESSAGES[OperatingSystem::UNSPECIFIED_ERROR + 1] = {
    MESSAGE("Ok"),
    MESSAGE("Access denied"),
    MESSAGE("Broken stream"),
    MESSAGE("Busy"),
    MESSAGE("Operation canceled"),
    MESSAGE("Dead lock"),
    MESSAGE("End of file"),
    MESSAGE("Path does not specify an existing entry within the filesystem"),
    MESSAGE("Filesystem entry already exists"),
    MESSAGE("File does not exist"),
    MESSAGE("File size limit exceeded"),
    MESSAGE("Path specifies an existing folder"),
    MESSAGE("Folder not empty"),
    MESSAGE("Operation interrupted by signal"),
    MESSAGE("Invalid address"),
    MESSAGE("Invalid argument"),
    MESSAGE("Invalid I/O control request"),
    MESSAGE("Invalid filesystem path"),
    MESSAGE("I/O error"),
    MESSAGE("Object locked by other process"),
    MESSAGE("Device does not exist"),
    MESSAGE("Not a folder"),
    MESSAGE("Invalid handle"),
    MESSAGE("Operation not permitted"),
    MESSAGE("Operation not supported"),
    MESSAGE("Operation already in progress"),
    MESSAGE("Out of memory"),
    MESSAGE("Out of space on device"),
    MESSAGE("Permission denied"),
    MESSAGE("Request refused"),
    MESSAGE("Resource is currently unavailable"),
    MESSAGE("Sharing violation"),
    MESSAGE("Stack overflow"),
    MESSAGE("Time out"),
    MESSAGE("Too many levels of symbolic links"),
    MESSAGE("Too many open files"),
    MESSAGE("Unspecified error")
  };
  if (error > OperatingSystem::UNSPECIFIED_ERROR) {
    error = OperatingSystem::UNSPECIFIED_ERROR;
  }
  return ERROR_MESSAGES[error];
}

unsigned int OperatingSystem::getErrorCode(unsigned int error) throw() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  switch (error) {
  case ERROR_SUCCESS:
    return OperatingSystem::OK;
  case ERROR_FILE_NOT_FOUND:
    return OperatingSystem::FILE_NOT_FOUND;
  case ERROR_PATH_NOT_FOUND:
    return OperatingSystem::ENTRY_NOT_FOUND;
  case ERROR_TOO_MANY_OPEN_FILES:
    return OperatingSystem::TOO_MANY_OPEN_FILES;
  case ERROR_ACCESS_DENIED:
    return OperatingSystem::PERMISSION_DENIED;
  case ERROR_INVALID_HANDLE:
    return OperatingSystem::NOT_A_HANDLE;
  case ERROR_NOT_ENOUGH_MEMORY:
    return OperatingSystem::OUT_OF_MEMORY;
  case ERROR_OUTOFMEMORY:
    return OperatingSystem::OUT_OF_MEMORY;
  case ERROR_SHARING_VIOLATION:
    return OperatingSystem::SHARING_VIOLATION;
  case ERROR_LOCK_VIOLATION:
    return OperatingSystem::LOCK_VIOLATION;
  case ERROR_HANDLE_EOF:
    return OperatingSystem::END_OF_FILE;
  case ERROR_HANDLE_DISK_FULL:
    return OperatingSystem::OUT_OF_SPACE;
  case ERROR_NOT_SUPPORTED:
    return OperatingSystem::NOT_SUPPORTED;
  case ERROR_FILE_EXISTS:
    return OperatingSystem::FILE_EXISTS;
  case ERROR_INVALID_PARAMETER:
    return OperatingSystem::INVALID_ARGUMENT;
  case ERROR_BROKEN_PIPE:
    return OperatingSystem::BROKEN_STREAM;
  case ERROR_INVALID_CATEGORY:
    return OperatingSystem::INVALID_IOCTL_REQUEST;
  case WAIT_TIMEOUT:
    return OperatingSystem::TIME_OUT;
  case ERROR_INVALID_ADDRESS:
    return OperatingSystem::INVALID_ADDRESS;
  case ERROR_EA_ACCESS_DENIED:
    return OperatingSystem::ACCESS_DENIED;
  case ERROR_OPERATION_ABORTED:
    return OperatingSystem::CANCELED;
  case ERROR_NOACCESS:
    return OperatingSystem::ACCESS_DENIED;
  case ERROR_STACK_OVERFLOW:
    return OperatingSystem::STACK_OVERFLOW;
  case WSAEINTR:
    return OperatingSystem::INTERRUPTED;
  case WSAEBADF:
    return OperatingSystem::NOT_A_HANDLE;
  case WSAEFAULT:
    return OperatingSystem::INVALID_ADDRESS;
  case WSAEINVAL:
    return OperatingSystem::INVALID_ARGUMENT;
  case WSAEMFILE:
    return OperatingSystem::TOO_MANY_OPEN_FILES;
  case WSAEWOULDBLOCK:
    return OperatingSystem::RETRY;
  case WSAEINPROGRESS:
    return OperatingSystem::OPERATION_IN_PROGRESS;
  case WSAEALREADY:
    return OperatingSystem::OPERATION_IN_PROGRESS;
  case WSAENOTSOCK:
    return OperatingSystem::NOT_A_HANDLE;
  case WSAETIMEDOUT:
    return OperatingSystem::TIME_OUT;
  case WSAECONNREFUSED:
    return OperatingSystem::REQUEST_REFUSED;
  default:
    return OperatingSystem::UNSPECIFIED_ERROR;
  }
#else // unix
  switch (error) {
  case 0:
    return OperatingSystem::OK;
  case EACCES:
    return OperatingSystem::PERMISSION_DENIED;
  case EAGAIN:
    return OperatingSystem::RETRY;
  case EBADF:
    return OperatingSystem::NOT_A_HANDLE;
  case EBUSY:
    return OperatingSystem::BUSY;
#if (defined(ECANCELED)) // TAG: fix cygwin
  case ECANCELED:
    return OperatingSystem::CANCELED;
#endif
  case ECONNREFUSED:
    return OperatingSystem::REQUEST_REFUSED;
  case EDEADLK:
    return OperatingSystem::DEAD_LOCK;
  case EEXIST:
    return OperatingSystem::FILE_EXISTS;
  case EFAULT:
    return OperatingSystem::INVALID_ADDRESS;
  case EFBIG:
    return OperatingSystem::FILE_TOO_LARGE;
  case EINPROGRESS:
    return OperatingSystem::OPERATION_IN_PROGRESS;
  case EINTR:
    return OperatingSystem::INTERRUPTED;
  case EINVAL:
    return OperatingSystem::INVALID_ARGUMENT;
  case EIO:
    return OperatingSystem::IO_ERROR;
  case EISDIR:
    return OperatingSystem::FOLDER_EXISTS;
  case ELOOP:
    return OperatingSystem::TOO_MANY_LINKS;
  case EMFILE:
    return OperatingSystem::TOO_MANY_OPEN_FILES;
  case EMLINK:
    return OperatingSystem::TOO_MANY_LINKS;
  case ENAMETOOLONG:
    return OperatingSystem::INVALID_PATH;
  case ENFILE:
    return OperatingSystem::TOO_MANY_OPEN_FILES;
  case ENODEV:
    return OperatingSystem::NO_SUCH_DEVICE;
  case ENOENT:
    return OperatingSystem::FILE_NOT_FOUND;
  case ENOMEM:
    return OperatingSystem::OUT_OF_MEMORY;
  case ENOSPC:
    return OperatingSystem::OUT_OF_SPACE;
  case ENOTDIR:
    return OperatingSystem::NOT_A_FOLDER;
  case ENOTEMPTY:
    return OperatingSystem::FOLDER_NOT_EMPTY;
  case ENOTSUP:
    return OperatingSystem::NOT_SUPPORTED;
  case ENOTTY:
    return OperatingSystem::INVALID_IOCTL_REQUEST;
  case EPERM:
    return OperatingSystem::NOT_PERMITTED;
  case EPIPE:
    return OperatingSystem::BROKEN_STREAM;
  default:
    return OperatingSystem::UNSPECIFIED_ERROR;
  }
#endif
}

class MemoryInfo {
public:
  
  MemorySize used = 0;
  MemorySize available = 0;
};

MemoryInfo getMemory()
{
  MemoryInfo result;
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#else
#endif
  return result;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
