/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/features.h>
#include <base/Application.h>
#include <base/Type.h>
#include <base/SystemLogger.h>
#include <stdlib.h>

#if defined(__win32__)
  #include <windows.h>
#else // __unix__
  #include <signal.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

class ApplicationImpl {
public:

  /* The application signal handler. */
#if defined(__win32__)

  static BOOL WINAPI signalHandler(DWORD signal) throw() {
    switch (signal) {
    case CTRL_CLOSE_EVENT: // console is closing
    case CTRL_BREAK_EVENT: // Ctrl+Break
    case CTRL_C_EVENT: // Ctrl+C
      if (Application::application) {
	Application::application->terminate();
	return TRUE;
      }
    }
    return FALSE;
  }

#else // __unix__

  static void signalHandler(int signal) throw() {
    switch (signal) {
    case SIGHUP: // reload
      if (Application::application) {
	Application::application->hangup();
      }
      break;
    case SIGTERM: // terminate
      if (Application::application) {
	Application::application->terminate();
      }
      break;
    case SIGCHLD: // child state changed
      break;
    }
  }

#endif

}; // end of ApplicationImpl

Application* Application::application = 0; // initialize application as uninitialized

Application::Application(const String& name, int argc, const char* argv[], const char* envp[]) throw(SingletonException, OutOfDomain) : formalName(name), terminated(false), hangingup(false) {
  static unsigned int singleton = 0;
  assert(singleton == 0, SingletonException("Application has been instantiated"));
  ++singleton;

  // install signal handler
#if defined(__win32__)
  if (!SetConsoleCtrlHandler(ApplicationImpl::signalHandler, TRUE)) {
    throw Exception("Unable to install signal handler");
  }
#else // __unix__
  assert(signal(SIGHUP, ApplicationImpl::signalHandler) != SIG_ERR, Exception("Unable to install signal handler"));
  assert(signal(SIGTERM, ApplicationImpl::signalHandler) != SIG_ERR, Exception("Unable to install signal handler"));
  assert(signal(SIGCHLD, ApplicationImpl::signalHandler) != SIG_ERR, Exception("Unable to install signal handler"));
#endif

  assert((argc > 0) && (argv), OutOfDomain());
  path = argv[0];
  for (unsigned int i = 1; i < argc; ++i) {
    arguments.append(argv[i]);
  }

  for (; *envp != 0; ++envp) {
    String temp(*envp);
    int index = temp.indexOf('=');
    if (index != -1) { // ignore the environment string if it doesn't contain '='
      environment[temp.substring(0, index - 1)] = temp.substring(index + 1);
    }
  }

  application = this;
}

int Application::exceptionHandler(Exception& e) const throw() {
  ferr << getTypename(e) << ": " << e.getMessage() << ENDL;
  return 1;
}

int Application::exceptionHandler() const throw() {
  ferr << "Unspecified exception" << ENDL;
  return 1;
}

void Application::hangup() throw() {
  SystemLogger::write(SystemLogger::INFORMATION, "HANGUP SIGNAL");
  lock.exclusiveLock();
  hangingup = true;
  lock.releaseLock();
}

void Application::terminate() throw() {
  SystemLogger::write(SystemLogger::INFORMATION, "TERMINATE SIGNAL");
  terminated = true;
}

bool Application::isHangingup() throw() {
  bool result;
  lock.exclusiveLock();
  result = hangingup;
  hangingup = false;
  lock.releaseLock();
  return result;
}

/*
int run() throw() {
  try {
    runnable->run();
  } catch(Exception& e) {
    return Application::getApplication()->exceptionHandler(e);
  } catch(...) {
    return Application::getApplication()->exceptionHandler();
  }
  return 0;
}
*/
_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
