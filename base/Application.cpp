/***************************************************************************
    copyright   : (C) 2001 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/features.h>
#include <base/Application.h>
#include <base/Type.h>
//#include <base/Trace.h>
//#include <base/SystemLogger.h>
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

Application::Application(const String& name, int argc, const char* argv[], const char* envp[]) throw(SingletonException, OutOfDomain) : formalName(name) {
  static unsigned int singleton = 0;
  assert(singleton == 0, SingletonException("Application has been instantiated"));
  ++singleton;

  // install signal handler
#if defined(__win32__)
  if (!SetConsoleCtrlHandler(ApplicationImpl::signalHandler, TRUE)) {
    throw Exception("Unable to install signal handler");
  }
#else // __unix__
  struct sigaction action;
  action.sa_handler = ApplicationImpl::signalHandler;
  action.sa_mask = 0;
  action.sa_flags = SA_RESTART;
  assert(!sigaction(SIGHUP, &action, 0), Exception("Unable to install signal handler"));
  assert(!sigaction(SIGTERM, &action, 0), Exception("Unable to install signal handler"));
  assert(!sigaction(SIGCHLD, &action, 0), Exception("Unable to install signal handler"));
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
  lock.exclusiveLock();
  hangingup = true;
  lock.releaseLock();
}

void Application::terminate() throw() {
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
