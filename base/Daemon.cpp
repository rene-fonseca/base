/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/features.h>
#include <base/Daemon.h>
#include <base/ResourceException.h>
#include <base/SystemLogger.h>
#include <base/Application.h>
#include <base/concurrency/Thread.h>

#if defined(__win32__)
  #include <windows.h>
#else // __unix__
  #include <sys/types.h>
  #include <unistd.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if defined(__win32__)

class DaemonImpl {
public:

  static const DWORD SERVICE_CONTROL_USERDEFINED_HANGUP = 128;

  /* The current status of the service. */
  static SERVICE_STATUS serviceStatus;
  /* Handle to the service status. */
  static SERVICE_STATUS_HANDLE serviceStatusHandle;
  /* The daemon context. */
  static Thread* context;
  /* The active object. */
  static Runnable* runnable;

  /* Sets the current status of the service and reports it to the Service Control Manager. */
  static bool reportStatusToControlManager(DWORD currentState, DWORD exitCode, DWORD waitHint) throw() {
    static DWORD checkPoint = 1;
    bool result = true;

    if (currentState == SERVICE_START_PENDING) {
      serviceStatus.dwControlsAccepted = 0;
    } else {
      serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    }

    serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    serviceStatus.dwServiceSpecificExitCode = 0;
    serviceStatus.dwCurrentState = currentState;
    serviceStatus.dwWin32ExitCode = exitCode;
    serviceStatus.dwWaitHint = waitHint;

    if ((currentState == SERVICE_RUNNING) || (currentState == SERVICE_STOPPED)) {
      serviceStatus.dwCheckPoint = 0;
    } else {
      serviceStatus.dwCheckPoint = checkPoint++;
    }

    // report the status of the service to the service control manager.
    if (!(result = SetServiceStatus(serviceStatusHandle, &serviceStatus))) {
      SystemLogger::write(SystemLogger::ERROR, "Unable to report status of service to the service control manager");
    }
    return result;
  }

  /* This function is called by the SCM whenever ControlService() is called on this service. */
  static void WINAPI serviceControlHandler(DWORD controlCode) throw() {
    // this function is invoked in the context of the thread that called StartServiceCtrlDispatcher?
    switch (controlCode) {
    case SERVICE_CONTROL_STOP: // stop the service
      reportStatusToControlManager(SERVICE_STOP_PENDING, NO_ERROR, 5000);
      Application::application->terminate();
      context->terminate(); // send termination signal to daemon thread
      reportStatusToControlManager(SERVICE_STOPPED, NO_ERROR, 0);
      break;
    case SERVICE_CONTROL_SHUTDOWN: // stop the service
      reportStatusToControlManager(SERVICE_STOP_PENDING, NO_ERROR, 5000);
      Application::application->terminate();
      context->terminate(); // send termination signal to deamon thread
      reportStatusToControlManager(SERVICE_STOPPED, NO_ERROR, 0);
      break;
    case SERVICE_CONTROL_INTERROGATE: // report status to control manager
    case SERVICE_CONTROL_PAUSE:
    case SERVICE_CONTROL_CONTINUE:
    case SERVICE_CONTROL_USERDEFINED_HANGUP:
      Application::application->hangup();
    default: // ignore control code
      reportStatusToControlManager(serviceStatus.dwCurrentState, NO_ERROR, 0);
      break;
    }
  }

  /* Initialize service. */
  static void WINAPI serviceEntry(DWORD argc, LPTSTR* argv) throw() {
    ASSERT(Thread::getThread() == 0); // make sure this is a new context
    // register the service control handler
    serviceStatusHandle = RegisterServiceCtrlHandler(Application::getApplication()->getFormalName(), serviceControlHandler);
    if (!serviceStatusHandle) {
      return;
    }

    // report the status to the service control manager
    if (!reportStatusToControlManager(SERVICE_START_PENDING, NO_ERROR, 5000)) {
      // try to report the stopped status to the service control manager
      reportStatusToControlManager(SERVICE_STOPPED, GetLastError(), 0);
      return;
    }

    context = new Thread(runnable);
    context->start();
    reportStatusToControlManager(SERVICE_RUNNING, NO_ERROR, 0);
  }

}; // end of DaemonImpl

SERVICE_STATUS DaemonImpl::serviceStatus;
SERVICE_STATUS_HANDLE DaemonImpl::serviceStatusHandle;
Thread* DaemonImpl::context = 0;
Runnable* DaemonImpl::runnable = 0;

Daemon::Daemon(Runnable* runnable) throw(SingletonException, ResourceException) {
  static unsigned int singleton = 0;
  assert(singleton == 0, SingletonException("Daemon has been instantiated"));
  ++singleton;
  DaemonImpl::runnable = runnable;

  SERVICE_TABLE_ENTRY dispatchTable[] = {
    {"", DaemonImpl::serviceEntry}, // name is ignored 'cause using SERVICE_WIN32_OWN_PROCESS
    {NULL, NULL}
  };

  if (!StartServiceCtrlDispatcher(dispatchTable)) {
    SystemLogger::write(SystemLogger::ERROR, "Unable to connect to service control manager.");
    //throw ResourceException("Unable to daemonize process");
    DaemonImpl::runnable->run();
  }
  if (DaemonImpl::context) {
    SystemLogger::write(SystemLogger::INFORMATION, "Waiting for daemon context to complete.");
    DaemonImpl::context->join();
    //delete DaemonImpl::context; // release daemon thread
  }
}

/* Installs service. */
void Daemon::install() {
  TCHAR path[512];
  if (GetModuleFileName(NULL, path, 512) == 0) {
    ferr << "Unable to install service as " << Application::getApplication()->getFormalName() << ENDL;
    return;
  }

  SC_HANDLE manager = OpenSCManager(
    NULL, // local machine
    NULL, // default database
    SC_MANAGER_ALL_ACCESS // access required
  );

  if (manager) {
    SC_HANDLE service = CreateService(
      manager,
      Application::getApplication()->getFormalName(), // name of service
      Application::getApplication()->getFormalName(), // name to display
      SERVICE_ALL_ACCESS, // desired access
      SERVICE_WIN32_OWN_PROCESS, // service type
      SERVICE_DEMAND_START, // start type
      SERVICE_ERROR_NORMAL, // error control type
      path, // service's binary
      NULL, // no load ordering group
      NULL, // no tag identifier
      TEXT(""), // dependencies
      NULL, // LocalSystem account
      NULL // no password
    );                      

    if (service) {
      fout << "Service has been installed as " << Application::getApplication()->getFormalName() << ENDL;
      CloseServiceHandle(service);
    } else {
      ferr << "Failed to install service" << ENDL;
    }
    CloseServiceHandle(manager);
  } else {
    ferr << "Unable to open service control manager" << ENDL;
  }
}

#else // __unix__

Daemon::Daemon(Runnable* runnable) throw(SingletonException, ResourceException) {
  static unsigned int singleton = 0;
  assert(singleton == 0, SingletonException("Daemon has been instantiated"));
  ++singleton;

  switch(fork()) {
  case 0:
    setsid();
    runnable->run();
  case -1:
    SystemLogger::write(SystemLogger::ERROR, "Unable to daemonize process.");
    throw ResourceException("Unable to daemonize process");
  }
}

#endif

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
