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

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Process manipulation class.

  @short Process
  @ingroup concurrency
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Process : public Object {
public:

  /**
    Exception raised by the Process class.

    @ingroup exceptions concurrency
  */
  class ProcessException : public Exception {
  public:
    
    ProcessException() throw() {}
    ProcessException(const char* message) throw() : Exception(message) {}
    ProcessException(Type type) throw() : Exception(type) {}
    ProcessException(const char* message, Type type) throw() : Exception(message, type) {}
  };
  
  /**
    Returns this process.
  */
  static Process getProcess() throw();
  
  /**
    Returns parent process.
  */
  static Process getParentProcess() throw();
  
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
    Executes the specified application.

    @return The child process.
  */
  static Process execute(const String& app) throw(ProcessException);
private:

  /** The host local id of the process. */
  unsigned int id;
public:

  /**
    Initializes a process.

    @param id The id of the process.
  */
  Process(unsigned int id) throw();

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
  unsigned int getId() throw();

  /**
    Returns true if the process is alive.
  */
  bool isAlive() const throw(ProcessException);
  
  /**
    Waits for the process to terminate.

    @return The exit code (Application::EXIT_CODE_INVALID if exit code is not available).
  */
  int wait() const throw(ProcessException);

  /**
    Requests the process to terminate.
    
    @param force Specifies whether to force the process to terminate. The default is false.
  */
  void terminate(bool force = false) throw(ProcessException);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
