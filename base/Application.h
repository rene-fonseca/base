/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__APPLICATION_H
#define _DK_SDU_MIP__BASE__APPLICATION_H

#include <base/Object.h>
#include <base/string/String.h>
#include <base/collection/Array.h>
#include <base/collection/Map.h>
#include <base/OutOfDomain.h>
#include <base/SingletonException.h>
#include <base/concurrency/SpinLock.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This is a singleton object for the context of this application. User-defined
  exit codes should start from EXIT_CODE_USER.

  Example:
  <pre>
  class MyApplication : public Application {
  public:
  
    MyApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw()
      : Application(MESSAGE("MyApplication"), numberOfArguments, arguments, environment) {
    }

    void main() throw() {
      ...
    }
  };
  
  int main(int argc, const char* argv[], const char *env[]) {
    MyApplication application(argc, argv, env);
    try {
      application.main();
    } catch(Exception& e) {
      return Application::getApplication()->exceptionHandler(e);
    } catch(...) {
      return Application::getApplication()->exceptionHandler();
    }
    return Application::getApplication()->getExitCode();
  }
  </pre>

  @short Application context wrapper.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.2
*/

class Application : public Object {
  friend class ApplicationImpl;
public:

  /** Application exit code. Exit codes should always be in the range [0; 127]. */
  enum ExitCode {
    /** This value specifies an invalid exit code which is used when the true exit code isn't available. */
    EXIT_CODE_INVALID = -1,
    /** The normal exit code of the application (indicating no errors). */
    EXIT_CODE_NORMAL = 0,
    /** The first user defined exit code. */
    EXIT_CODE_USER = 1,
    /** The default exit code indicating an unspecified error. */
    EXIT_CODE_ERROR = 125,
    /** The exit code returned by the application on initialization error. */
    EXIT_CODE_INITIALIZATION = 126,
    /** The exit code used when the application exit code is determined externally. */
    EXIT_CODE_EXTERNAL = 127
  };
private:

  /** The application object. */
  static Application* application;
  /** The formal name of the application. */
  String formalName;
  /** The path used to start the application. */
  String path;
  /** Container for the arguments passed to the application. */
  Array<String> arguments;
  /** Container for the environment variables. */
  Map<String, String> environment;
  /** The exit code. */
  int exitCode;
  /** Specifies whether the application should be terminated. */
  bool terminated;
  /** Specifies whether the application should reload its configuration. */
  bool hangingup;
  /** Synchronization object. */
  SpinLock lock;

  /** Common initialization method used by constructors. */
  void initialize() throw();
public:

  /**
    Returns the application object.
  */
  inline static Application* getApplication() throw() {return application;}

  /**
    Initializes application with no arguments and no environment variables.

    @param name The formal name.
  */
  Application(const String& name) throw(SingletonException); 

  /**
    Initializes application.

    @param name The formal name.
    @param numberOfArguments The "argc" argument of the entry function main.
    @param arguments The "argv" argument of the entry function main.
    @param environment The "env" argument of the entry function main. This argument is not required.
  */
  Application(const String& name, int numberOfArguments, const char* arguments[], const char* environment[] = 0) throw(SingletonException, OutOfDomain);

  /**
    Returns the formal name of the application.
  */
  inline const String& getFormalName() const throw() {return formalName;}

  /**
    Returns the name/path of the application (argument 0).
  */
  inline const String& getName() const throw() {return path;}

  /**
    Returns the arguments passed to the application.
  */
  inline const Array<String>& getArguments() const throw() {return arguments;}

  /**
    Returns the environment variables.
  */
  inline const Map<String, String>& getEnvironment() const throw() {return environment;}

  /**
    Handler of uncaught exceptions. By default this handler writes the exception
    to stderr and returns error code 1.
  */
  virtual int exceptionHandler(const Exception& e) const throw();

  /**
    Handler of uncaught unknown exceptions. By default this handler writes an
    error message to stderr and returns error code 1.
  */
  virtual int exceptionHandler() const throw();

  /**
    Signals the application to reload its configuration.
  */
  void hangup() throw();

  /**
    Signals the application to terminate itself.
  */
  void terminate() throw();

  /**
    Returns true if the application has been signaled to hangup. The hangup flag
    is automatically reset.
  */
  bool isHangingup() throw();

  /**
    Returns the exit code.
  */
  inline int getExitCode() const throw() {
    return exitCode;
  }
  
  /**
    Sets the exit code (the default is EXIT_CODE_NORMAL).

    @param value The desired exit code. The value should be in the range from 0 to 127.
  */
  inline void setExitCode(int value) throw() {
    exitCode = value;
  }
  
  /**
    Returns true if the application has been signaled to terminate.
  */
  inline bool isTerminated() const throw() {return terminated;}

  /**
    Invoked on application termination. Exits the application immediately by
    default (i.e. does not return).
  */
  virtual void onTermination() throw();

  /**
    Destroys the application object.
  */
  ~Application() throw();
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
