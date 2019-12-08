/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Object.h>
#include <base/string/String.h>
#include <base/Exception.h>
#include <base/NotSupported.h>
#include <base/io/Handle.h>
#include <base/mem/Reference.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Process manipulation class.

  @short Process.
  @ingroup concurrency
  @version 1.2
*/

class _COM_AZURE_DEV__BASE__API Process : public Object {
public:

  /** Value which specifies an invalid process id. */
  static const unsigned long INVALID = PrimitiveTraits<unsigned long>::MAXIMUM;

  /* Structure holding the user and system times. */
  class _COM_AZURE_DEV__BASE__API Times {
  public:

    /** The user mode time in nanoseconds. */
    uint64 user = 0;
    /** The system mode time in nanoseconds. */
    uint64 system = 0;

    inline Times() noexcept {
    }

    inline Times(uint64 _user, uint64 _system) noexcept
      : user(_user), system(_system)
    {
    }

    /** Returns the total processing time in nanoseconds. */
    inline uint64 getTotal() const noexcept
    {
      return user + system;
    }

    /** Returns the total processing time in microseconds. */
    inline uint64 getTotal_US() const noexcept
    {
      return (user + system + 500) / 1000;
    }
  };

  /**
    Exception raised by the Process class.

    @short Exception raised by Process.
    @ingroup exceptions concurrency
    @version 1.1
  */
  class _COM_AZURE_DEV__BASE__API ProcessException : public Exception {
  public:
    
    inline ProcessException() noexcept
    {
    }
    
    inline ProcessException(const char* message) noexcept
      : Exception(message)
    {
    }
    
    inline ProcessException(const Type& type) noexcept
      : Exception(type)
    {
    }
    
    inline ProcessException(const char* message, const Type& type) noexcept
      : Exception(message, type)
    {
    }
    
    _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
  };

  // TAG: do we need a base class for this kind of class
  class _COM_AZURE_DEV__BASE__API Layout {
  public:
  };
  
  /**
    Returns the layout of the process.
  */
  static inline Layout getLayout() noexcept
  {
    return Layout();
  }
  
  /**
    Returns this process.
  */
  static Process getProcess() noexcept;
  
  /**
    Returns parent process. A process object with id 0 is returned when no
    parent is available.

    @return Invalid process if not supported.
  */
  static Process getParentProcess() noexcept;
  
  /**
    Returns the number of configured processors in the system. It is legal for
    the operating system to lie about the true number of processors.
    
    @return Returns 0 if the value cannot be determined.
  */
  static inline unsigned long getNumberOfConfiguredProcessors() noexcept
  {
    return OperatingSystem::getVariable(
      OperatingSystem::NUM_OF_CONFIGURED_PROCESSORS
    );
  }
  
  /**
    Returns the number of online processors in the system. It is legal for the
    operating system to lie about the true number of processors.
    
    @return Returns 0 if the value cannot be determined.
  */
  static inline unsigned long getNumberOfOnlineProcessors() noexcept
  {
    return OperatingSystem::getVariable(
      OperatingSystem::NUM_OF_ONLINE_PROCESSORS
    );
  }
  
  /**
    Returns true if the system supports job control.
  */
  static inline bool supportsJobControl() noexcept {
    return OperatingSystem::getVariable(OperatingSystem::SUPPORTS_JOB_CONTROL) != 0;
  }
  
  /**
    Forks a child process.

    @return The parent returns the child process. The child returns a process with id 0.
  */
  static Process fork();

  /**
    Returns the priority of the process.
  */
  static int getPriority();

  /**
    Sets the priority of the process.
  */
  static void setPriority(int priority);
 
  /**
    Executes the specified application (quotes are required if module contains
    a space). The return process has been locked.
    
    @return The child process.
  */
  static Process execute(const String& app);
public:

  class _COM_AZURE_DEV__BASE__API ProcessHandle : public Handle {
    friend class Initialization;
    friend class Process;
  private:

    /** Invalid handle. */
    static Handle* invalid;
    
    /** Initializes process handle. */
    inline ProcessHandle(OperatingSystem::Handle handle) noexcept
      : Handle(handle)
    {
    }
    
    /** Releases the resources used by the process. */
    ~ProcessHandle() noexcept;
  };
private:

  /** The host local id of the process. */
  unsigned long id = 0;
  /** Handle to the process. */
  Reference<Handle> handle;
public:

  /**
    Initializes process as invalid.
  */
  Process() noexcept;
  
  /**
    Initializes a process.

    @param id The id of the process.
  */
  Process(unsigned long id) noexcept;

  /**
    Initializes process from other process.
  */
  Process(const Process& copy) noexcept;

  /**
    Assignment of process with process.
  */
  Process& operator=(const Process& assign) noexcept;

  /**
    Returns the id of the process.
  */
  inline unsigned long getId() noexcept
  {
    return id;
  }

  /**
    Returns true if the process id is invalid.
  */
  inline bool isValid() const noexcept
  {
    return id != INVALID;
  }
  
  /**
    Returns true if the process is alive. On some platforms this will only work
    if the process is a child process of the current process.
  */
  bool isAlive() const;
  
  /**
    Returns the name (path) of the process.
  */
  String getName() const;

  /**
    Acquires a lock of the process which allows synchronization using the wait
    methods.
  */
  void lock();

  /**
    Waits for the process to terminate. EXIT_CODE_CONFLICT is returned is the
    true exit code was EXIT_CODE_INVALID.
    
    @param microseconds The timeout period.
    
    @return The exit code (EXIT_CODE_INVALID if exit code is not available or
    the timeout period expired).
  */
  int wait(unsigned int microseconds) noexcept;
  
  /**
    Waits for the process to terminate.

    @return The exit code (EXIT_CODE_INVALID if exit code is not available).
  */
  int wait();
  
  /**
    Requests the process to terminate.
    
    @param force Specifies whether to force the process to terminate. The
    default is false.

    @return True if the signal was delivered.
  */
  bool terminate(bool force = false);

  /**
    Returns the current processing times (both user and system times).
  */
  Times getTimes() noexcept;
};

inline Process::Process(unsigned long _id) noexcept
  : id(_id),
    handle(ProcessHandle::invalid)
{
}

_COM_AZURE_DEV__BASE__API FormatOutputStream& operator<<(
  FormatOutputStream& stream, const Process::Layout& value);

/** Add resource times. */
inline Process::Times operator+(const Process::Times& a, const Process::Times& b) noexcept
{
  return Process::Times(a.user + b.user, a.system + b.system);
}

/** Subtract resource times. */
inline Process::Times operator-(const Process::Times& a, const Process::Times& b) noexcept
{
  return Process::Times(a.user - b.user, a.system - b.system);
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
