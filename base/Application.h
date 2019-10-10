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
#include <base/collection/Array.h>
#include <base/collection/Map.h>
#include <base/OutOfDomain.h>
#include <base/SingletonException.h>
#include <base/concurrency/SpinLock.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  This is a singleton object for the context of the application. User-defined
  exit codes should start from EXIT_CODE_USER.  
  
  Example:
  @code
  class MyApplication : public Application {
  public:
  
    MyApplication(int numberOfArguments, const char* arguments[], const char* environment[]) throw()
      : Application("MyApplication", numberOfArguments, arguments, environment) {
    }

    void main() throw() {
      // ...
    }
  };

  STUB(MyApplication);
  @endcode
  
  @short Application.
  @version 1.3
*/

class _COM_AZURE_DEV__BASE__API Application : public Object {
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
    EXIT_CODE_ERROR = 123,
    /** This value specifies that the true exit code conflicts a reserved exit code in the current context. */
    EXIT_CODE_CONFLICT = 124,
    /** The exit code indicating an internal error (e.g. uncaught exception and violations of exception specification). */
    EXIT_CODE_INTERNAL_ERROR = 125,
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
  int exitCode = 0;
  /** Specifies whether the application should be terminated. */
  bool terminated = false;
  /** Specifies whether the application should reload its configuration. */
  bool hangingup = false;
  /** Synchronization object. */
  SpinLock lock;

  /** Common initialization method used by constructors. */
  void initialize() throw();
public:
  
  /**
    Returns the application object.
  */
  static inline Application* getApplication() throw() {
    return application;
  }
  
  template<class APPLICATION>
  static inline int stub(int numberOfArguments, const char* arguments[], const char* environment[]) throw() {
    try {
      APPLICATION application(numberOfArguments, arguments, environment);
      try {
        application.main();
      } catch (Exception& e) {
        return Application::getApplication()->exceptionHandler(e);
      } catch (...) {
        return Application::getApplication()->exceptionHandler();
      }
      return Application::getApplication()->getExitCode();
    } catch (...) {
      return Application::EXIT_CODE_INITIALIZATION;
    }
  }
  
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
    @param environment The "env" argument of the entry function main. This
    argument is not required.
  */
  Application(const String& name, int numberOfArguments, const char* arguments[], const char* environment[] = 0) throw(SingletonException, OutOfDomain);

  /**
    Returns the formal name of the application.
  */
  inline const String& getFormalName() const throw() {
    return formalName;
  }

  /**
    Returns the name/path of the application (argument 0).
  */
  inline const String& getName() const throw() {
    return path;
  }

  /**
    Returns the arguments passed to the application.
  */
  inline const Array<String>& getArguments() const throw() {
    return arguments;
  }

  /**
    Returns the environment variables.
  */
  inline const Map<String, String>& getEnvironment() const throw() {
    return environment;
  }

  /**
    Handler of uncaught exceptions. By default this handler writes the exception
    to stderr and sets the error code to EXIT_CODE_ERROR.
  */
  virtual int exceptionHandler(const Exception& e) throw();

  /**
    Handler of uncaught unknown exceptions. By default this handler writes an
    error message to stderr and sets the error code to EXIT_CODE_ERROR.
  */
  virtual int exceptionHandler() throw();

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
  inline bool isTerminated() const throw() {
    return terminated;
  }

  /**
    Invoked on application termination. Exits the application immediately by
    default (i.e. does not return).
  */
  virtual void onTermination() throw();

  /**
    Destroys the application object.
  */
  virtual ~Application() throw();
};

#define _COM_AZURE_DEV__BASE__STUB(APPLICATION) \
int main(int argc, const char* argv[], const char* env[]) throw() { \
  return com::azure::dev::base::Application::stub<APPLICATION>(argc, argv, env); \
}

#if (!defined(STUB))
#  define STUB _COM_AZURE_DEV__BASE__STUB
#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
