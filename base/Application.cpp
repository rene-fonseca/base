/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/Application.h>
#include <base/TypeInfo.h>
#include <base/SystemLogger.h>
#include <base/concurrency/Thread.h>
#include <base/string/StringOutputStream.h>
#include <base/UnexpectedFailure.h>
#include <stdlib.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
  #undef ERROR // protect against the evil programmers
#else // unix
  #include <sys/signal.h> // defines SIG_ERR on IRIX65
  #include <signal.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class ApplicationImpl {
public:

  static void terminationExceptionHandler() {
    StringOutputStream stream;
    const Type exceptionType = Exception::getExceptionType();
    if (exceptionType.isInitialized()) {
      try {
        throw;
      } catch(Exception& e) {
        stream << MESSAGE("Internal error: uncaught exception '") << TypeInfo::getTypename(e) << MESSAGE("' was raised");
        if (e.getType().isInitialized()) {
          stream << MESSAGE(" by '") << TypeInfo::getTypename(e.getType()) << '\'';
        }
        if (e.getMessage()) {
          stream << MESSAGE(" with message '") << e.getMessage() << '\'';
        }
        stream << FLUSH;
      } catch(...) {
        stream << MESSAGE("Internal error: uncaugth and unsupported exception '") << TypeInfo::getTypename(exceptionType) << MESSAGE("' was raised.") << FLUSH;
      }
    } else {
      stream << MESSAGE("Internal error: explicit termination.") << FLUSH;
    }
    ferr << stream.getString() << ENDL; // TAG: use appropriate error stream
#if defined(_DK_SDU_MIP__BASE__DEBUG)
    // TAG: need runtime debug mode support (e.g. bool Trace::debug or with level support)
    Trace::message(stream.getString().getElements());
#endif
    exit(Application::EXIT_CODE_ERROR); // TAG: is abort() better
  }

  static void unexpectedExceptionHandler() {
    StringOutputStream stream;
    const Type exceptionType = Exception::getExceptionType();
    if (exceptionType.isInitialized()) {
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
        stream << MESSAGE("Internal error: unsupported exception '") << TypeInfo::getTypename(exceptionType) << MESSAGE("' was raised in violation with exception specification.") << FLUSH;
      }
    } else {
      stream << MESSAGE("Internal error: explicit invocation of unexpected.") << FLUSH;
    }
#if defined(_DK_SDU_MIP__BASE__DEBUG)
    Trace::message(stream.getString().getElements());
#endif
    // TAG: either use SystemLogger or ferr but not both
    SystemLogger::write(SystemLogger::ERROR, stream.getString().getElements());
    // TAG: only if ferr is valid
    ferr << stream.getString() << ENDL;
    exit(Application::EXIT_CODE_ERROR); // TAG: is abort() or terminate() better
  }

  /* The application signal handler. */
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)

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

  static LRESULT CALLBACK messageHandler(HWND window, UINT message, WPARAM primaryParameter, LPARAM secondaryParameter) {
    // TAG: we should destroy window in destructor
    StringOutputStream stream;
    stream << MESSAGE("messageHandler: message=") << message << " primary=" << primaryParameter << " second=" << secondaryParameter << FLUSH;
    Trace::message(stream.getString().getElements());
    switch (message) {
    case WM_QUIT:
      Trace::message("Quit");
      if (Application::application) {
        Application::application->terminate();
      }
      return 0;
    default:
      // TAG: what should we do here
      return ::DefWindowProc(window, message, primaryParameter, secondaryParameter);
    }
    return 0;
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
#if defined(SIGPWR)
    case SIGPWR: // power fail or restart
      if (Thread::getThread()->isMainThread()) {
        SystemLogger::write(SystemLogger::INFORMATION, MESSAGE("Power signal."));
        if (Application::application) {
          Application::application->terminate();
        }
      }
      break;
#endif
    }
  }

#endif

}; // end of ApplicationImpl



void Application::initialize() throw() {
  static unsigned int singleton = 0;
  assert(singleton == 0, SingletonException("Application has been instantiated", this));
  ++singleton;

  // install signal handler
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  if (!::SetConsoleCtrlHandler((PHANDLER_ROUTINE)ApplicationImpl::signalHandler, TRUE)) {
    throw UnexpectedFailure("Unable to install signal handler", this);
  }

  WNDCLASSEX windowClass;
  clear(windowClass);
  windowClass.cbSize = sizeof(windowClass);
  windowClass.lpfnWndProc = /*(WNDPROC)*/ApplicationImpl::messageHandler;
  windowClass.lpszClassName = "mip.sdu.dk/~fonseca/base";
  ATOM result = ::RegisterClassEx(&windowClass);
  ASSERT(result != 0);

  OSVERSIONINFO versionInfo;
  versionInfo.dwOSVersionInfoSize = sizeof(versionInfo);
  ::GetVersionEx(&versionInfo); // never fails
  bool isWindows2000OrLater = (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) && (versionInfo.dwMajorVersion >= 5);

  HWND messageWindow = ::CreateWindowEx(
    0, // no extended window style
    "mip.sdu.dk/~fonseca/base", // registered class name
    0, // no window name // TAG: should I use ""
    WS_DISABLED, // window style // TAG: or just 0
    0, // horizontal position of window
    0, // vertical position of window
    0, // window width
    0, // window height
    isWindows2000OrLater ? ((HWND)-3) : ((HWND)0), // no parent or owner window - (HWND(-3)) ~ HWND_MESSAGE
    HMENU(0), // use class menu
    HINSTANCE(0), // ignored for Windows NT or later
    0 // no window-creation data
  );
  
#else // unix
  #if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__IRIX65)
    if (!((bsd_signal(SIGHUP, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (bsd_signal(SIGTERM, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (bsd_signal(SIGCHLD, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (bsd_signal(SIGQUIT, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (bsd_signal(SIGINT, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (bsd_signal(SIGABRT, ApplicationImpl::signalHandler) != SIG_ERR))) {
      throw UnexpectedFailure("Unable to install signal handler", this);
    }
  #else
    /* use sigaction */
    if (!((signal(SIGHUP, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (signal(SIGTERM, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (signal(SIGCHLD, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (signal(SIGQUIT, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (signal(SIGINT, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (signal(SIGABRT, ApplicationImpl::signalHandler) != SIG_ERR))) {
      throw UnexpectedFailure("Unable to install signal handler", this);
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

namespace internal {
  
  void terminationExceptionHandler() throw();

  void unexpectedExceptionHandler() throw();
};

Application::~Application() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
//   if (::DestroyWindow(windowHandle) == 0) {
//     // failed but we do not care
//   }
//   if (::UnregisterClass("mip.sdu.dk/~fonseca/base", HINSTANCE(0)) == 0) { // TAG: should be done automatically
//     // failed but we do not care
//   }
#endif // flavor
  
  std::set_terminate(internal::terminationExceptionHandler);
  std::set_unexpected(internal::unexpectedExceptionHandler);
  application = 0;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
