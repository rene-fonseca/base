/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/features.h>
#include <base/concurrency/Process.h>

#if defined(__win32__)
#else // __unix__
  #include <sys/types.h>
  #include <sys/wait.h>
  #include <unistd.h>
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

void Process::nice(int value) throw(Exception) {
  if (::nice(value)) {
    throw Exception("Unable to change priority of process");
  }
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
  int status;
  ::waitpid(id, &status, 0);
}

void Process::kill() throw() {
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
