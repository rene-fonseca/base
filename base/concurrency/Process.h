/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_CONCURRENCY__PROCESS_H
#define _DK_SDU_MIP__BASE_CONCURRENCY__PROCESS_H

#include <base/Object.h>
#include <base/string/String.h>
#include <base/Exception.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Process.

  @author René Møller Fonseca
  @version 1.0
*/

class Process : public Object {
public: 

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
  static Process fork() throw(Exception);

  /**
    Sets the priority of the process.
  */
  static void nice(int value) throw(Exception);

  /**
    Executes the specified application.
  */
  static void execute(const String& app) throw();
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
    Waits for the process to terminate.
  */
  void wait() throw();

  /**
    Sends a signal to the process.
  */
  void kill() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
