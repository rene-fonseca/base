/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

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
  This is a singleton object for the context of this application.

  @short Application context wrapper.
  @author René Møller Fonseca
*/

class Application : public Object {
friend class ApplicationImpl;
friend class DaemonImpl;
public:

  /** The normal exit code of the application (indicating no errors). */
  static const int EXIT_CODE_NORMAL = 0;
  /** The exit code returned by the application on errors. */
  static const int EXIT_CODE_ERROR = 1;
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
  inline static const Application* getApplication() throw() {return application;}

  /**
    Initializes application with no arguments and no environment variables.

    @param name The formal name.
  */
  Application(const String& name) throw(SingletonException);

  /**
    Initializes application.

    Example:
    <pre>
    int main(int argc, const char* argv[], const char *envp[]) {
      Application app(argc, argv, envp);
      try {
        return entry();
      } catch(Exception& e) {
        return Application::getApplication()->exceptionHandler(e);
      } catch(...) {
        return Application::getApplication()->exceptionHandler();
      }
    }
    </pre>

    @param name The formal name.
    @param numberOfArguments The "argc" argument of the entry function main.
    @param arguments The "argv" argument of the entry function main.
    @param environment The "envp" argument of the entry function main. This argument is not required.
  */
  Application(const String& name, int numberOfArguments, const char* arguments[], const char* environment[] = 0) throw(SingletonException, OutOfDomain);

  /**
    Returns the formal name of the application.
  */
  inline String getFormalName() const throw() {return formalName;}

  /**
    Returns the name/path of the application.
  */
  inline String getName() const throw() {return path;}

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
    Returns true if the application has been signaled to hangup. The hangup flag is automatically reset.
  */
  bool isHangingup() throw();

  /**
    Returns true if the application has been signaled to terminate.
  */
  inline bool isTerminated() const throw() {return terminated;}
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
