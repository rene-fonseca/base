/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Application.h>
#include <base/TypeInfo.h>
#include <base/SystemLogger.h>
#include <base/concurrency/Thread.h>
#include <base/string/StringOutputStream.h>
#include <stdlib.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
  #undef ERROR // protect against the evil programmers
#else // unix
  #include <sys/signal.h> // defines SIG_ERR on IRIX65
  #include <signal.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class ApplicationInitialization {
public:

  static void terminationExceptionHandler() throw() {
    // TAG: need some way to extract the raised exception object
    Trace::message("Exception was raised during application initialization or cleanup.");
    SystemLogger::write(SystemLogger::ERROR, MESSAGE("Exception was raised during application initialization or cleanup."));
    // ferr must not be used
    exit(Application::EXIT_CODE_INITIALIZATION);
  }

  static void unexpectedExceptionHandler() throw() {
    // TAG: does not work!
    // TAG: make sure base library is initialized
    StringOutputStream stream;
    try {
      throw;
    } catch(Exception& e) {
      stream << MESSAGE("Internal error: exception '") << TypeInfo::getTypename(e) << MESSAGE("' was raised");
      if (e.getType().isInitialized()) {
        stream << MESSAGE(" by '") << TypeInfo::getTypename(e.getType()) << '\'';
      }
      if (e.getMessage()) {
        stream << MESSAGE(" with message '") << e.getMessage() << '\'';
      }
      stream << MESSAGE(" in violation with exception specification during application initialization or cleanup.") << FLUSH;
    } catch(...) {
      stream << MESSAGE("Internal error: unsupported exception was raised in violation with exception specification during application initialization or cleanup.") << FLUSH;
    }
    Trace::message(stream.getString().getElements());
    SystemLogger::write(SystemLogger::ERROR, stream.getString().getElements());
    // TAG: force stack to be unfolded and exit
    exit(Application::EXIT_CODE_INITIALIZATION); // TAG: is abort() or terminate() better
  }
  
  ApplicationInitialization() throw() {
    // install exception handlers
    std::set_terminate(ApplicationInitialization::terminationExceptionHandler);
    std::set_unexpected(ApplicationInitialization::unexpectedExceptionHandler);
  }

  // TAG: restore previous exception handlers in destructor
};

ApplicationInitialization applicationInitialization;

Application* Application::application = 0; // initialize application as uninitialized

class ApplicationImpl {
public:

  static void terminationExceptionHandler() {
    StringOutputStream stream;
    stream << MESSAGE("Internal error: exception handling abandoned.") << FLUSH;
    Trace::message(stream.getString().getElements());
    // TAG: only if ferr is valid
    ferr << stream.getString() << ENDL;
    exit(Application::EXIT_CODE_ERROR); // TAG: is abort() better
  }

  static void unexpectedExceptionHandler() {
    StringOutputStream stream;
    try {
      throw;
    } catch(Exception& e) {
      stream << MESSAGE("Internal error: exception '") << TypeInfo::getTypename(e) << MESSAGE("' was raised");
      if (e.getType().isInitialized()) {
        stream << MESSAGE(" by '") << TypeInfo::getTypename(e.getType()) << '\'';
      }
      if (e.getMessage()) {
        stream << MESSAGE(" with message '") << e.getMessage() << '\'';
      }
      stream << MESSAGE(" in violation with exception specification.") << FLUSH;
    } catch(...) {
      stream << MESSAGE("Internal error: unsupported exception was raised in violation with exception specification.") << FLUSH;
    }
    Trace::message(stream.getString().getElements());
    // TAG: either use SystemLogger or ferr but not both
    SystemLogger::write(SystemLogger::ERROR, stream.getString().getElements());
    // TAG: only if ferr is valid
    ferr << stream.getString() << ENDL;
    exit(Application::EXIT_CODE_ERROR); // TAG: is abort() or terminate() better
  }

  /* The application signal handler. */
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)

  static BOOL WINAPI signalHandler(DWORD signal) throw() {
    switch (signal) {
    case CTRL_LOGOFF_EVENT:
    case CTRL_SHUTDOWN_EVENT:
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



void Application::initialize() throw() {
  static unsigned int singleton = 0;
  assert(singleton == 0, SingletonException("Application has been instantiated"));
  ++singleton;

  // install signal handler
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  if (!::SetConsoleCtrlHandler((PHANDLER_ROUTINE)ApplicationImpl::signalHandler, TRUE)) {
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
  ferr << MESSAGE("Exception: ") << TypeInfo::getTypename(e) << MESSAGE(": ") << e.getMessage() << ENDL;
  return Application::EXIT_CODE_ERROR;
}

int Application::exceptionHandler() const throw() {
  ferr << MESSAGE("Internal error: unspecified exception.") << ENDL;
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
