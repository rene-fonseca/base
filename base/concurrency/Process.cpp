/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/concurrency/Process.h>
#include <base/TypeInfo.h>
#include <base/Application.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
#include <windows.h>
#else // unix
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#endif

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__UNIX)
  extern "C" char** environ;
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Process Process::getProcess() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  return Process(::GetCurrentProcessId());
#else // unix
  return Process(getpid());
#endif
}

Process Process::getParentProcess() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
#else // unix
  return Process(::getppid());
#endif
}

Process Process::fork() throw(NotSupported, ProcessException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  throw NotSupported(TypeInfo::getTypename<Process>());
#else // unix
  pid_t result = ::fork(); // should use fork1 on solaris
  if (result == (pid_t)-1) {
    throw ProcessException("Unable to fork child process", TypeInfo::getTypename<Process>());
  }
  return Process(result);
#endif
}

#if !defined(BELOW_NORMAL_PRIORITY_CLASS) // should have been in winbase.h
  #define BELOW_NORMAL_PRIORITY_CLASS 0x00004000
#endif

#if !defined(ABOVE_NORMAL_PRIORITY_CLASS) // should have been in winbase.h
  #define ABOVE_NORMAL_PRIORITY_CLASS 0x00008000
#endif

int Process::getPriority() throw(ProcessException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  switch (::GetPriorityClass(::GetCurrentProcess())) { // no need to close handle
  case 0:
    throw ProcessException("Unable to get priority of process", TypeInfo::getTypename<Process>());
  case REALTIME_PRIORITY_CLASS:
    return -20;
  case HIGH_PRIORITY_CLASS:
    return -10;
  case ABOVE_NORMAL_PRIORITY_CLASS: // w2k or later
    return -5;
  case NORMAL_PRIORITY_CLASS:
    return 0;
  case BELOW_NORMAL_PRIORITY_CLASS: // w2k or later
    return 5;
  case IDLE_PRIORITY_CLASS:
    return 19;
  }
#else // unix
  errno = 0;
  int priority = ::getpriority(PRIO_PROCESS, getpid());
  if ((priority == -1) && (errno != 0)) {
    throw ProcessException("Unable to get priority of process", TypeInfo::getTypename<Process>());
  }
  return priority;
#endif
}

void Process::setPriority(int priority) throw(ProcessException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
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
    throw ProcessException("Unable to set priority of process", TypeInfo::getTypename<Process>());
  }
#else // unix
  if (::setpriority(PRIO_PROCESS, getpid(), priority)) {
    ProcessException("Unable to set priority", TypeInfo::getTypename<Process>());
  }
#endif
}

Process Process::execute(const String& command) throw(ProcessException) {
  // inherit handles, environment, use current working directory, and allow this app to wait for process to terminate
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
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
    throw ProcessException("Unable to execute command", TypeInfo::getTypename<Process>());
  }
  // TAG: return special Process object - close handles in processInformation later
#else
  pid_t pid;
  int status;

  pid = ::fork();
  if (pid == -1) {
    throw ProcessException("Unable to execute command", TypeInfo::getTypename<Process>());
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

Process::Process(unsigned int id) throw() : id(id) {
}

Process::Process(const Process& copy) throw() : id(copy.id) {
}

unsigned int Process::getId() throw() {
  return id;
}

Process& Process::operator=(const Process& eq) throw() {
  if (&eq == this) {
    id = eq.id;
  }
  return *this;
}

bool Process::isAlive() const throw(ProcessException) {
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
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
  if (result == (pid_t)id) {
    return false;
  } else if (result == 0) {
    return true;
  } else {
    throw ProcessException("Unable to query process", this);
  }
  // TAG: need to protect against EINTR
#endif // flavor
}

int Process::wait() const  throw(ProcessException) {
  // TAG: need timeout support
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
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
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
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

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
