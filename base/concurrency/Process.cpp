/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/concurrency/Process.h>
#include <base/Type.h>
#include <base/Application.h>
#include <base/NotImplemented.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
  #include <psapi.h>
#else // unix
  #include <sys/types.h>
  #include <sys/wait.h>
  #include <sys/time.h>
  #include <sys/resource.h> // getpriority, getrusage
  #include <unistd.h>
  #include <signal.h>
  #include <errno.h>
  #include <stdlib.h>

  #define _DK_SDU_MIP__BASE__HAVE_GETRUSAGE
#endif

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__UNIX)
  extern "C" char** environ;
#endif

// TAG: need to symbol to enable use of undocumented API (e.g. _DK_SDU_MIP__BASE__UNDOCUMENTED)

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__WINNT4) || (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__W2K)
namespace ntapi {
  
  typedef long NTSTATUS;
  typedef long KPRIORITY;
  
  struct ProcessBasicInformation {
    NTSTATUS exitStatus;
    void* /*PPEB*/ PEBBaseAddress;
    unsigned long* affinityMask;
    KPRIORITY basePriority;
    unsigned long* uniqueProcessId;
    unsigned long inheritedFromUniqueProcessId; // TAG: is the type correct
  };

  typedef NTSTATUS (__stdcall *PNtQueryInformationProcess)(HANDLE, unsigned int /*PROCESSINFOCLASS*/, void*, unsigned long, unsigned long*);
};
#endif

Process Process::getProcess() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return Process(::GetCurrentProcessId());
#else // unix
  return Process(::getpid());
#endif
}

Process Process::getParentProcess() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__WINNT4) || (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__W2K)
    // MT-safe 'cause DWORD is written atomically
    static bool isPIDCached = false;
    static unsigned long cachedPID = Process::INVALID;
    
    if (!isPIDCached) {
      ntapi::PNtQueryInformationProcess NtQueryInformationProcess =
        (ntapi::PNtQueryInformationProcess)::GetProcAddress(::GetModuleHandle("ntdll"), "NtQueryInformationProcess");
      if (NtQueryInformationProcess) {
        ntapi::ProcessBasicInformation information;
        if (!NtQueryInformationProcess(::GetCurrentProcess(),
                                       0, // process basic information class
                                       &information,
                                       sizeof(information),
                                       0)) {
          cachedPID = information.inheritedFromUniqueProcessId;
        }
      }
      isPIDCached = true; // cached pid is set to invalid by default
    }
    return Process(cachedPID);
  #else
    return Process(Process::INVALID); // win32 doesn't support this (WINNT 4)
  #endif
#else // unix
  return Process(::getppid());
#endif
}

Process Process::fork() throw(NotSupported, ProcessException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  throw NotSupported(Type::getType<Process>());
#else // unix
  pid_t result = ::fork(); // should we use fork1 on solaris
  if (result == (pid_t)-1) {
    throw ProcessException("Unable to fork child process", Type::getType<Process>());
  }
  return Process(result);
#endif
}

#if !defined(BELOW_NORMAL_PRIORITY_CLASS) // should have been in winbase.h
  #define BELOW_NORMAL_PRIORITY_CLASS ((DWORD)0x00004000)
#endif

#if !defined(ABOVE_NORMAL_PRIORITY_CLASS) // should have been in winbase.h
  #define ABOVE_NORMAL_PRIORITY_CLASS ((DWORD)0x00008000)
#endif

int Process::getPriority() throw(ProcessException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  DWORD priority = ::GetPriorityClass(::GetCurrentProcess());
  switch (priority) {
  case 0:
    throw ProcessException("Unable to get priority of process", Type::getType<Process>());
  case REALTIME_PRIORITY_CLASS:
    return 7 - 24;
  case HIGH_PRIORITY_CLASS:
    return 7 - 13;
  case ABOVE_NORMAL_PRIORITY_CLASS: // w2k or later
    return 7 - 10;
  case NORMAL_PRIORITY_CLASS:
    return 7 - 7;
  case BELOW_NORMAL_PRIORITY_CLASS: // w2k or later
    return 7 - 6;
  case IDLE_PRIORITY_CLASS:
    return 7 - 4;
  default:
    return 0; // unknown
  }
#else // unix
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__CYGWIN)
    #warning Process::getPriority() is not supported
    return 0; // TAG: api cygwin
  #else
    errno = 0;
    int priority = ::getpriority(PRIO_PROCESS, getpid());
    if ((priority == -1) && (errno != 0)) {
      throw ProcessException("Unable to get priority of process", Type::getType<Process>());
    }
    return priority;
  #endif
#endif
}

void Process::setPriority(int priority) throw(ProcessException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  DWORD priorityClass;
  if (priority <= -20) {
    priorityClass = REALTIME_PRIORITY_CLASS;
  } else if (priority <= -10) {
    priorityClass = HIGH_PRIORITY_CLASS;
  } else if (priority <= -5) {
    priorityClass = ABOVE_NORMAL_PRIORITY_CLASS; // w2k or later
  } else if (priority < 5) {
    priorityClass = NORMAL_PRIORITY_CLASS;
  } else if (priority <= 10) {
    priorityClass = BELOW_NORMAL_PRIORITY_CLASS; // w2k or later
  } else {
    priorityClass = IDLE_PRIORITY_CLASS;
  }
  if (!::SetPriorityClass(::GetCurrentProcess(), priorityClass)) {
    throw ProcessException("Unable to set priority of process", Type::getType<Process>());
  }
#else // unix
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__CYGWIN)
    #warning Process::setPriority() is not supported
  #else
    if (::setpriority(PRIO_PROCESS, ::getpid(), priority)) {
      ProcessException("Unable to set priority", Type::getType<Process>());
    }
  #endif
#endif
}

Process Process::execute(const String& command) throw(ProcessException) {
  // inherit handles, environment, use current working directory, and allow this app to wait for process to terminate
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  String commandLine = command;
  STARTUPINFO startInfo;
  PROCESS_INFORMATION processInformation;
  clear(startInfo);
  startInfo.cb = sizeof(startInfo);
  startInfo.dwFlags = STARTF_USESTDHANDLES;
  BOOL result = ::CreateProcess(0,
                                commandLine.getElements(), // command line (may need quotes)
                                0, // process security attributes
                                0, // primary thread security attributes
                                TRUE, // handles are inherited
                                0, // creation flags
                                0, // use parent's environment
                                0, // use parent's current directory
                                &startInfo, // STARTUPINFO
                                &processInformation // receives PROCESS_INFORMATION
  );
  if (result != 0) {
    throw ProcessException("Unable to execute command", Type::getType<Process>());
  }
  // TAG: return special Process object - close handles in processInformation later
#else
  // TAG: use spawn if available
  
  pid_t pid;
  int status;

  pid = ::fork();
  if (pid == -1) {
    throw ProcessException("Unable to execute command", Type::getType<Process>());
  }
  if (pid == 0) { // is this the child
    // setup arguments list
    // first argument must be the module path
    char* argv[1];
    ::execve(command.getElements(), argv, environ);
    // we only get here if exec failed
    ::exit(Application::EXIT_CODE_INITIALIZATION); // child must never return from this method
  } else {
    return Process(pid);
  }
#endif // flavor
}

Process::Process(unsigned long _id) throw() : id(_id) {
}

Process::Process(const Process& copy) throw() : id(copy.id) {
}

Process& Process::operator=(const Process& eq) throw() {
  if (&eq == this) {
    id = eq.id;
  }
  return *this;
}

bool Process::isAlive() const throw(ProcessException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
//   HANDLE newHandle;
//   BOOL res = ::DuplicateHandle(
//     ::GetCurrentProcess(), // handle to source process
//     id, // handle to duplicate
//     ::GetCurrentProcess(), // handle to target process
//     newHandle, // the duplicate handle
//     SYNCHRONIZE, // requested access
//     FALSE, // handle inheritance option
//     0 // optional actions
//   );
  // use SYNCHRONIZE
  DWORD exitCode;
  HANDLE handle = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, static_cast<DWORD>(id));
  assert(handle != 0, ProcessException("Unable to query process", this));
  BOOL result = ::GetExitCodeProcess(handle, &exitCode);
  ::CloseHandle(handle);
  if (result != 0) {
    return exitCode == STILL_ACTIVE;
  } else {
    throw ProcessException(this); 
  }
#else // unix
  
  int status;
  pid_t result = ::waitpid(id, &status, WNOHANG);
  assert(result >= 0, ProcessException("Unable to query process", this));

  /**
    GCC 3.0.4 bug
    
    For some reason this exception cannot be caught
    
    assert(result >= 0, ProcessException("Unable to query process", this));
  */
  if (result == (pid_t)id) {
    return false;
  } else if (result == 0) {
    return true;
  }

  /*
    GCC 3.0.4 bug here which results in "Abort" when the exception is raised below.
    
    Linux/RedHat 7.2
    
    gcc -I../.. -pipe -x c++ -ansi -O3 -g3 -march=i686 -malign-double -fomit-frame-pointer -funroll-loops -fno-delete-null-pointer-checks -fno-math-errno -fexpensive-optimizations -c Process.cpp -MT Process.lo -MD -MP -MF .deps/Process.TPlo -o Process.o
    
    if (result == (pid_t)id) {
      return false;
    } else if (result == 0) {
      return true;
    }
    throw ProcessException("Unable to query process", this); // Aborts here
  */

  // TAG: need to protect against EINTR
#endif // flavor
}

String Process::getName() const throw(NotSupported, ProcessException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HANDLE process = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, id);
  assert(process, ProcessException(this));
  char buffer[MAX_PATH + 1];
  DWORD length = ::GetModuleFileNameEx(process, 0, buffer, sizeof(buffer));
  assert(length > 0, ProcessException(this));
  ::CloseHandle(process);
  return String(buffer, length - 1);
#else // unix
  throw NotSupported(this);
#endif
}

int Process::wait() const  throw(ProcessException) {
  // TAG: need timeout support
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  HANDLE handle = ::OpenProcess(SYNCHRONIZE, FALSE, static_cast<DWORD>(id));
  assert(handle != 0, ProcessException("Unable to wait for process", this));
  ::WaitForSingleObject(handle, INFINITE);
  ::CloseHandle(handle);
#else // unix
  int status;
  pid_t result = ::waitpid((pid_t)id, &status, 0);
  if (result != (pid_t)id) {
    if (errno == EINTR) {
      return Application::EXIT_CODE_INVALID;
    } else {
      throw ProcessException("Unable to wait for process", this);
    }
  }
  if (WIFEXITED(status)) {
    return WEXITSTATUS(status);
  } else {
    return Application::EXIT_CODE_INVALID;
  }
#endif // flavor
}

// TAG: need process group support
void Process::terminate(bool force) throw(ProcessException) {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (force) {
    HANDLE handle = ::OpenProcess(PROCESS_TERMINATE, FALSE, (DWORD)id);
    assert(handle != 0, ProcessException("Unable to terminate process", this));
    ::TerminateProcess(handle, Application::EXIT_CODE_EXTERNAL);
    ::CloseHandle(handle);
  } else {
    // TAG: if service then ask to stop
    // TAG: WM_QUIT is normally not posted - is this ok - do we need to post to specific window handle
    BOOL result = ::PostThreadMessage(static_cast<DWORD>(id), WM_QUIT, Application::EXIT_CODE_EXTERNAL, 0);
    ASSERT(result != 0);
    // TAG: throw ProcessException(this)
  }
#else
  int result = ::kill(pid_t(id), force ? (SIGKILL) : (SIGTERM));
  if (!result) {
    switch (errno) {
    case EPERM:
      // TAG: I think we need an AccessDenied exception which is not a specialization of IOException
      // throw Permission(this);
    case ESRCH:
    default:
      throw ProcessException(this);
    }
  }
#endif // flavor
}

Process::Times Process::getTimes() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  Process::Times result;
  ::GetProcessTimes(::GetCurrentProcess(), 0, 0, (FILETIME*)&result.system, (FILETIME*)&result.user);
  return result;
#else // unix
  #if defined(_DK_SDU_MIP__BASE__HAVE_GETRUSAGE)
    struct rusage usage;
    ::getrusage(RUSAGE_SELF, &usage); // does not fail
    Process::Times result;
    result.user = usage.ru_utime.tv_sec * 1000000000ULL + usage.ru_utime.tv_usec * 1000ULL;
    result.system = usage.ru_stime.tv_sec * 1000000000ULL + usage.ru_stime.tv_usec * 1000ULL;
    return result;
  #else
    static unsigned long ticksPerSecond = 0; // 0 ~ not cached
    struct tms;
    ::times(&tms);
    if (ticksPerSecond == 0) { // get if not cached
      ticksPerSecond = ::sysconf(_SC_CLK_TCK);
    }
    Process::Times result;
    result.user = tms.tms_utime * 1000000000ULL/ticksPerSecond;
    result.system = tms.tms_stime * 1000000000ULL/ticksPerSecond;
    return result;
  #endif
#endif // flavor
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
