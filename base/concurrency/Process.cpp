/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/features.h>
#include <base/concurrency/Process.h>

#if defined(__win32__)
  #include <windows.h>
#else // __unix__
  #include <sys/types.h>
  #include <sys/wait.h>
  #include <unistd.h>
  #include <sys/time.h>
  #include <sys/resource.h>
  #include <errno.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

Process Process::getProcess() throw() {
#if defined(__win32__)
  return Process(GetCurrentProcessId());
#else // __unix__
  return Process(getpid());
#endif
}

Process Process::getParentProcess() throw() {
#if defined(__win32__)
#else // __unix__
  return Process(getppid());
#endif
}

Process Process::fork() throw(Exception) {
#if defined(__win32__)
#else // __unix__
  pid_t result = ::fork(); // should use fork1 on solaris
  if (result == (pid_t)-1) {
    throw Exception("Unable to fork child process");
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
#if defined(__win32__)
  switch (GetPriorityClass(GetCurrentProcess())) { // no need to close handle
  case 0:
    throw ProcessException("Unable to get priority of process");
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
#else // __unix__
  errno = 0;
  int priority = ::getpriority(PRIO_PROCESS, getpid());
  if ((priority == -1) && (errno != 0)) {
    throw ProcessException("Unable to get priority of process");
  }
  return priority;
#endif
}

void Process::setPriority(int priority) throw(ProcessException) {
#if defined(__win32__)
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
  if (!SetPriorityClass(GetCurrentProcess(), priorityClass)) {
    throw ProcessException("Unable to set priority of process");
  }
#else // __unix__
  if (::setpriority(PRIO_PROCESS, getpid(), priority)) {
    ProcessException("Unable to set priority");
  }
#endif
}

void Process::execute(const String& name) throw() {
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

void Process::wait() throw() {
#if defined(__win32__)
#else // __unix__
  int status;
  ::waitpid(id, &status, 0);
#endif
}

void Process::kill() throw() {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
