/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/features.h>
#include <base/Application.h>
#include <base/Type.h>
#include <base/TypeInfo.h>
#include <base/SystemLogger.h>
#include <base/concurrency/Thread.h>
#include <stdlib.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#else // unix
  #include <sys/signal.h> // defines SIG_ERR on IRIX65
  #include <signal.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class ApplicationImpl {
public:

  static void terminationExceptionHandler() {
    ferr << MESSAGE("Internal error: Exception handling abandoned") << ENDL;
    exit(Application::EXIT_CODE_ERROR); // TAG: is abort() better
  }

  static void unexpectedExceptionHandler() {
//    ferr << MESSAGE("Internal error: Violation of exception specification") << ENDL;
    try {
      throw;
    } catch(Exception& e) {
      ferr << MESSAGE("Internal error: Violation of exception specification with") << EOL
           << MESSAGE("  ") << getTypename(e) << MESSAGE(": ") << e.getMessage() << ENDL;
    } catch(...) {
      ferr << MESSAGE("Internal error: Exception violates exception specification") << ENDL;
    }
    exit(Application::EXIT_CODE_ERROR); // TAG: is abort() or terminate() better
  }

  /* The application signal handler. */
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)

  static BOOL WINAPI signalHandler(DWORD signal) throw() {
    switch (signal) {
    case CTRL_CLOSE_EVENT: // console is closing
    case CTRL_BREAK_EVENT: // Ctrl+Break
    case CTRL_C_EVENT: // Ctrl+C
      SystemLogger::write(SystemLogger::INFORMATION, "Terminate signal.");
      if (Application::application) {
        Application::application->terminate();
        return TRUE;
      }
    }
    return FALSE;
  }

#else // unix

  static void signalHandler(int signal) throw() {
    switch (signal) {
    case SIGHUP: // hangup
      if (Thread::getThread()->isMainThread()) {
        SystemLogger::write(SystemLogger::INFORMATION, MESSAGE("Hangup signal."));
        if (Application::application) {
          Application::application->hangup();
        }
      }
      break;
    case SIGQUIT: // quit signal from keyboard
      if (Thread::getThread()->isMainThread()) {
        SystemLogger::write(SystemLogger::INFORMATION, MESSAGE("Quit signal."));
        if (Application::application) {
          Application::application->terminate();
        }
      }
      break;
    case SIGINT: // interrrupt from keyboard
      if (Thread::getThread()->isMainThread()) {
        SystemLogger::write(SystemLogger::INFORMATION, MESSAGE("Interrupt signal."));
        if (Application::application) {
          Application::application->terminate();
        }
      }
      break;
    case SIGABRT: // abort
      if (Thread::getThread()->isMainThread()) {
        SystemLogger::write(SystemLogger::INFORMATION, MESSAGE("Abort signal."));
      }
      break;
    case SIGTERM: // terminate
      if (Thread::getThread()->isMainThread()) {
        SystemLogger::write(SystemLogger::INFORMATION, MESSAGE("Terminate signal."));
        if (Application::application) {
          Application::application->terminate();
        }
      }
      break;
    case SIGCHLD: // child state changed
      break;
    case SIGPWR: // power fail or restart
      if (Thread::getThread()->isMainThread()) {
        SystemLogger::write(SystemLogger::INFORMATION, MESSAGE("Power signal."));
        if (Application::application) {
          Application::application->terminate();
        }
      }
      break;
    }
  }

#endif

}; // end of ApplicationImpl



Application* Application::application = 0; // initialize application as uninitialized

void Application::initialize() throw() {
  static unsigned int singleton = 0;
  assert(singleton == 0, SingletonException("Application has been instantiated"));
  ++singleton;

  // install signal handler
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)ApplicationImpl::signalHandler, TRUE)) {
    throw Exception("Unable to install signal handler");
  }
#else // unix
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__IRIX65)
    if (!((bsd_signal(SIGHUP, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (bsd_signal(SIGTERM, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (bsd_signal(SIGCHLD, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (bsd_signal(SIGQUIT, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (bsd_signal(SIGINT, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (bsd_signal(SIGABRT, ApplicationImpl::signalHandler) != SIG_ERR))) {
      throw Exception("Unable to install signal handler");
    }
  #else
    /* use sigaction */
    if (!((signal(SIGHUP, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (signal(SIGTERM, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (signal(SIGCHLD, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (signal(SIGQUIT, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (signal(SIGINT, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (signal(SIGABRT, ApplicationImpl::signalHandler) != SIG_ERR))) {
      throw Exception("Unable to install signal handler");
    }
  #endif
#endif

  // install exception handlers
  std::set_terminate(ApplicationImpl::terminationExceptionHandler);
  std::set_unexpected(ApplicationImpl::unexpectedExceptionHandler);
}

Application::Application(const String& name) throw(SingletonException) :
  exitCode(EXIT_CODE_NORMAL), formalName(name), terminated(false), hangingup(false) {
  initialize();
  application = this;
}

Application::Application(const String& name, int numberOfArguments, const char* arguments[], const char* environment[]) throw(SingletonException, OutOfDomain) :
  exitCode(EXIT_CODE_NORMAL), formalName(name), terminated(false), hangingup(false) {
  initialize();

  assert((numberOfArguments > 0) && (arguments), OutOfDomain());
  path = arguments[0];
  for (unsigned int i = 1; i < numberOfArguments; ++i) {
    this->arguments.append(arguments[i]);
  }

  if (environment) {
    for (; *environment != 0; ++environment) {
      String temp(*environment);
      int index = temp.indexOf('=');
      if (index != -1) { // ignore the environment string if it doesn't contain '='
        this->environment[temp.substring(0, index - 1)] = temp.substring(index + 1);
      }
    }
  }

  application = this;
}

int Application::exceptionHandler(const Exception& e) const throw() {
  ferr << getTypename(e) << MESSAGE(": ") << e.getMessage() << ENDL;
  return Application::EXIT_CODE_ERROR;
}

int Application::exceptionHandler() const throw() {
  ferr << MESSAGE("Internal error: Unspecified exception") << ENDL;
  return Application::EXIT_CODE_ERROR;
}

void Application::hangup() throw() {
  lock.exclusiveLock();
  hangingup = true;
  lock.releaseLock();
}

void Application::terminate() throw() {
  terminated = true;
  onTermination();
}

bool Application::isHangingup() throw() {
  bool result;
  lock.exclusiveLock();
  result = hangingup;
  hangingup = false;
  lock.releaseLock();
  return result;
}

void Application::onTermination() throw() {
  exit(exitCode);
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
