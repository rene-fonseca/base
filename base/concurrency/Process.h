/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_CONCURRENCY__PROCESS_H
#define _DK_SDU_MIP__BASE_CONCURRENCY__PROCESS_H

#include <base/Object.h>
#include <base/string/String.h>
#include <base/Exception.h>
#include <base/NotSupported.h>
#include <base/io/Handle.h>
#include <base/mem/Reference.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Process manipulation class.

  @short Process
  @ingroup concurrency
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.2
*/

class Process : public Object {
public:

  /** Value which specifies an invalid process id. */
  static const unsigned long INVALID = PrimitiveTraits<unsigned long>::MAXIMUM;

  /* Structure holding the user and system times. */
  struct Times {
    /** The user mode time in nanoseconds. */
    uint64 user;
    /** The system mode time in nanoseconds. */
    uint64 system;
  };

  /**
    Exception raised by the Process class.

    @short Exception raised by Process.
    @ingroup exceptions concurrency
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.1
  */
  class ProcessException : public Exception {
  public:
    
    inline ProcessException() throw() {
    }
    
    inline ProcessException(const char* message) throw() : Exception(message) {
    }
    
    inline ProcessException(Type type) throw() : Exception(type) {
    }
    
    inline ProcessException(const char* message, Type type) throw()
      : Exception(message, type) {
    }
  };

  // TAG: do we need a base class for this kind of class
  class Layout {
  public:
  };
  
  /**
    Returns the layout of the process.
  */
  static inline Layout getLayout() throw() {
    return Layout();
  }
  
  /**
    Returns this process.
  */
  static Process getProcess() throw();
  
  /**
    Returns parent process. A process object with id 0 is returned when no
    parent is available.

    @return Invalid process if not supported.
  */
  static Process getParentProcess() throw();
  
  /**
    Returns the number of configured processors in the system. It is legal for
    the operating system to lie about the true number of processors.
    
    @return Returns 0 if the value cannot be determined.
  */
  static inline unsigned long getNumberOfConfiguredProcessors() throw() {
    return OperatingSystem::getVariable(
      OperatingSystem::NUM_OF_CONFIGURED_PROCESSORS
    );
  }
  
  /**
    Returns the number of online processors in the system. It is legal for the
    operating system to lie about the true number of processors.
    
    @return Returns 0 if the value cannot be determined.
  */
  static inline unsigned long getNumberOfOnlineProcessors() throw() {
    return OperatingSystem::getVariable(
      OperatingSystem::NUM_OF_ONLINE_PROCESSORS
    );
  }
  
  /**
    Returns true if the system supports job control.
  */
  static inline bool supportsJobControl() throw() {
    return OperatingSystem::getVariable(OperatingSystem::SUPPORTS_JOB_CONTROL) != 0;
  }
  
  /**
    Forks a child process.

    @return The parent returns the child process. The child returns a process with id 0.
  */
  static Process fork() throw(NotSupported, ProcessException);

  /**
    Returns the priority of the process.
  */
  static int getPriority() throw(ProcessException);

  /**
    Sets the priority of the process.
  */
  static void setPriority(int priority) throw(ProcessException);
 
  /**
    Executes the specified application (quotes are required if module contains
    a space). The return process has been locked.
    
    @return The child process.
  */
  static Process execute(const String& app) throw(ProcessException);
private:

  class ProcessHandle : public Handle {
    friend class Initialization;
    friend class Process;
  private:

    /** Invalid handle. */
    static Handle* invalid;
    
    /** Initializes process handle. */
    inline ProcessHandle(OperatingSystem::Handle handle) throw()
      : Handle(handle) {
    }
    
    /** Releases the resources used by the process. */
    ~ProcessHandle() throw();
  };
  
  /** The host local id of the process. */
  unsigned long id;
  /** Handle to the process. */
  Reference<Handle> handle;
public:

  /**
    Initializes process as invalid.
  */
  Process() throw();
  
  /**
    Initializes a process.

    @param id The id of the process.
  */
  Process(unsigned long id) throw();

  /**
    Initializes process from other process.
  */
  Process(const Process& copy) throw();

  /**
    Assignment of process with process.
  */
  Process& operator=(const Process& eq) throw();

  /**
    Returns the id of the process.
  */
  inline unsigned long getId() throw() {
    return id;
  }

  /**
    Returns true if the process id is invalid.
  */
  inline bool isValid() const throw() {
    return id != INVALID;
  }
  
  /**
    Returns true if the process is alive. On some platforms this will only work
    if the process is a child process of the current process.
  */
  bool isAlive() const throw(ProcessException);
  
  /**
    Returns the name (path) of the process.
  */
  String getName() const throw(NotSupported, ProcessException);

  /**
    Acquires a lock of the process which allows synchronization using the wait
    methods.
  */
  void lock() throw(ProcessException);

  /**
    Waits for the process to terminate. EXIT_CODE_CONFLICT is returned is the
    true exit code was EXIT_CODE_INVALID.
    
    @param microseconds The timeout period.
    
    @return The exit code (EXIT_CODE_INVALID if exit code is not available or
    the timeout period expired).
  */
  int wait(unsigned int microseconds) throw();
  
  /**
    Waits for the process to terminate.

    @return The exit code (EXIT_CODE_INVALID if exit code is not available).
  */
  int wait() throw(ProcessException);
  
  /**
    Requests the process to terminate.
    
    @param force Specifies whether to force the process to terminate. The
    default is false.

    @return True if the signal was delivered.
  */
  bool terminate(bool force = false) throw(ProcessException);

  /**
    Returns the current processing times (both user and system times).
  */
  Times getTimes() throw();
};

/**
  @example testsuite/Process.cpp
*/

inline Process::Process(unsigned long _id) throw()
  : id(_id), handle(ProcessHandle::invalid) {
}

FormatOutputStream& operator<<(
  FormatOutputStream& stream, const Process::Layout& value) throw(IOException);

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
