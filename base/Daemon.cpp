/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/Daemon.h>
#include <base/ResourceException.h>
#include <base/SystemLogger.h>
#include <base/Application.h>
#include <base/concurrency/Thread.h>
#include <base/Trace.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#  undef ERROR // protect against the evil programmers
#else // unix
#  include <sys/types.h>
#  include <unistd.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

_COM_AZURE_DEV__BASE__GLOBAL_PRINT();

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)

class DaemonImpl {
public:
  
  static bool dispatched; // specifies whether the service entry function has been called
  static SERVICE_STATUS serviceStatus; // the current status of the service
  static SERVICE_STATUS_HANDLE serviceStatusHandle; // handle to the service status
  static Thread* daemonThread; // the daemon context
  static Thread* parentThread; // the daemon context
  static Runnable* runnable; // the active object

  static bool run() noexcept {
    try {
      BASSERT(Thread::getThread() == 0); // make sure this is a new context - ThreadKey is initialized to 0
      DaemonImpl::daemonThread = new Thread(DaemonImpl::parentThread);
      Thread::ThreadLocal threadLocal(DaemonImpl::daemonThread);
      DaemonImpl::runnable->run();
      DaemonImpl::daemonThread->setTerminationState(Thread::TERMINATED); // do not access thread state here after
    } catch (...) {
      DaemonImpl::daemonThread->setTerminationState(Thread::EXCEPTION); // do not access thread state here after
      return false;
    }
    return true;
  }
  
}; // end of DaemonImpl

bool DaemonImpl::dispatched = false; // specifies whether the service entry function has been called
SERVICE_STATUS DaemonImpl::serviceStatus; // the current status of the service
SERVICE_STATUS_HANDLE DaemonImpl::serviceStatusHandle; // handle to the service status
Thread* DaemonImpl::daemonThread = nullptr; // the daemon context
Thread* DaemonImpl::parentThread = nullptr; // the daemon context
Runnable* DaemonImpl::runnable = nullptr; // the active object

namespace win32 {
  
  const DWORD SERVICE_CONTROL_USERDEFINED_HANGUP = 128;

  /* Sets the current status of the service and reports it to the Service Control Manager. */
  bool reportStatusToControlManager(DWORD currentState, DWORD exitCode, DWORD waitHint) noexcept {
    static DWORD checkPoint = 1; // TAG: do we need to reset the checkpoint at some time
    bool result = true;
    
    if (currentState == SERVICE_START_PENDING) {
      DaemonImpl::serviceStatus.dwControlsAccepted = 0;
    } else {
       // TAG: SERVICE_ACCEPT_PAUSE_CONTINUE
      DaemonImpl::serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    }

    DaemonImpl::serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    DaemonImpl::serviceStatus.dwServiceSpecificExitCode = 0;
    DaemonImpl::serviceStatus.dwCurrentState = currentState;
    DaemonImpl::serviceStatus.dwWin32ExitCode = exitCode;
    DaemonImpl::serviceStatus.dwWaitHint = waitHint;

    if ((currentState == SERVICE_RUNNING) || (currentState == SERVICE_STOPPED)) {
      DaemonImpl::serviceStatus.dwCheckPoint = 0;
    } else {
      DaemonImpl::serviceStatus.dwCheckPoint = checkPoint++;
    }

    // report the status of the service to the service control manager.
    if (!(result = ::SetServiceStatus(DaemonImpl::serviceStatusHandle, &DaemonImpl::serviceStatus))) {
      SystemLogger::write(SystemLogger::ERROR, "Unable to report status of service to the service control manager.");
    }
    return result;
  }

  /* This function is called by the SCM whenever ControlService() is called on this service. */
  void WINAPI serviceControlHandler(DWORD controlCode) {
    // this function invoked in the context of the thread that called StartServiceCtrlDispatcher
    switch (controlCode) {
    case SERVICE_CONTROL_SHUTDOWN: // stop the service
    case SERVICE_CONTROL_STOP: // stop the service
      reportStatusToControlManager(SERVICE_STOP_PENDING, NO_ERROR, 5000);
      try {
        Application::getApplication()->terminate();
        Trace::message("Waiting for daemon thread to terminate.");
        DaemonImpl::daemonThread->join();
        reportStatusToControlManager(SERVICE_STOPPED, NO_ERROR, 0);
        Trace::message("Destroying daemon thread.");
        delete DaemonImpl::daemonThread;
        Trace::message("Daemon thread has been destroyed.");
        reportStatusToControlManager(SERVICE_STOPPED, NO_ERROR, 0);
      } catch (...) {
        reportStatusToControlManager(SERVICE_STOPPED, ERROR_EXCEPTION_IN_SERVICE, 0);
      }
      break;
    case SERVICE_CONTROL_PAUSE:
    case SERVICE_CONTROL_CONTINUE:
      break;
    case SERVICE_CONTROL_USERDEFINED_HANGUP:
      try {
        Application::getApplication()->hangup();
      } catch (...) {
        reportStatusToControlManager(SERVICE_RUNNING, ERROR_EXCEPTION_IN_SERVICE, 0);
      }
      break;
    case SERVICE_CONTROL_INTERROGATE: // report status to control manager
    default: // ignore control code
      reportStatusToControlManager(DaemonImpl::serviceStatus.dwCurrentState, NO_ERROR, 0);
      break;
    }
  }

  /* Initialize service. */
  void WINAPI serviceEntry(DWORD numberOfArguments, LPWSTR* argumentVector)
  {
    DaemonImpl::dispatched = true;
    // register the service control handler
    DaemonImpl::serviceStatusHandle = ::RegisterServiceCtrlHandlerW(
      ToWCharString(Application::getApplication()->getFormalName()),
      serviceControlHandler
    );

    //Array<String> arguments;
    //for (unsigned int i = 1; i < numberOfArguments; ++i) { // ignore first arg which is name of service
    //  arguments.append(arguments[i]);
    //}
    //Application::getApplication()->arguments = arguments;
/*
  if (environment) {
    for (; *environment != 0; ++environment) {
      String temp(*environment);
      int index = temp.indexOf('=');
      if (index != -1) { // ignore the environment string if it doesn't contain
        this->environment[temp.substring(0, index)] = temp.substring(index)
      }
    }
  }
*/
    if (!DaemonImpl::serviceStatusHandle) {
      return;
    }

    // report the status to the service control manager
    if (!reportStatusToControlManager(SERVICE_START_PENDING, NO_ERROR, 5000)) {
      // try to report the stopped status to the service control manager
      reportStatusToControlManager(SERVICE_STOPPED, GetLastError(), 0);
      return;
    }

    reportStatusToControlManager(SERVICE_RUNNING, NO_ERROR, 0);
    if (DaemonImpl::run()) {
      reportStatusToControlManager(SERVICE_STOPPED, NO_ERROR, 0);
    } else {
      reportStatusToControlManager(SERVICE_STOPPED, ERROR_EXCEPTION_IN_SERVICE, 0);
    }
  }

};

Daemon::Daemon(Runnable* runnable) {
  static unsigned int singleton = 0;
  bassert(singleton == 0, SingletonException("Daemon has been instantiated."));
  ++singleton;
  if (!runnable) {
    _throw OutOfDomain();
  }
  bassert(Application::getApplication(), Exception("Application has not been institiated."));
  DaemonImpl::runnable = runnable;
  DaemonImpl::parentThread = Thread::getThread();

  // TAG: need to detect whether the application is started as a service or a normal application
  // TAG: check whether a console is attached

  const wchar* EMPTY = L"";
  SERVICE_TABLE_ENTRY dispatchTable[] = {
    {const_cast<wchar*>(EMPTY), &win32::serviceEntry}, // name is ignored 'cause using SERVICE_WIN32_OWN_PROCESS
    {NULL, NULL} // termination entry
  };
  
  if (!::StartServiceCtrlDispatcher(dispatchTable)) {
    if (DaemonImpl::dispatched) {
      SystemLogger::write(SystemLogger::ERROR, "Unable to connect to service control manager.");
    } else {
      runnable->run(); // run as console application
    }
  }
}
/*
void Daemon::start(const Array<String>& arguments) {
  SC_HANDLE manager = ::OpenSCManager(0, 0, 0);
  bassert(manager, DaemonException(this));
  SC_HANDLE service = ::OpenService(manager, application->getFormalName(), SERVICE_START);
  bassert(service, DaemonException(this));
  const char* argumentBuffer[arguments.getSize()];
  for (unsigned int i = arguments.getSize(); i > 0;) {
    --i;
    argumentBuffer[i] = arguments[i];
  }
  BOOL result = ::StartService(service, arguments.getSize(), &argumentBuffer);
  ::CloseServiceHandle(service);
  ::CloseServiceHandle(manager);
}

void Daemon::stop() {
  SC_HANDLE manager = ::OpenSCManager(0, 0, 0);
  bassert(manager, DaemonException(this));
  SC_HANDLE service = ::OpenService(manager, 0, SERVICE_STOP);
  if (service) {
    SERVICE_STATUS status;
    BOOL result = ::ControlService(service, SERVICE_CONTROL_STOP, &status);
    ::CloseServiceHandle(service);
  }
  ::CloseServiceHandle(manager);
}

void Daemon::uninstall() {
  SC_HANDLE manager = ::OpenSCManager(0, 0, 0);
  bassert(manager, DaemonException(this));
  SC_HANDLE service = ::OpenService(manager, 0, SERVICE_DELETE);
  bassert(service, DaemonException(this));
  ::DeleteService(service);
  ::CloseServiceHandle(service);
  ::CloseServiceHandle(manager);
}
*/
/* Installs service. */
void Daemon::install()
{
  TCHAR path[512];
  if (::GetModuleFileName(0, path, getArraySize(path)) == 0) {
    ferr << "Unable to install service as "
         << Application::getApplication()->getFormalName() << ENDL;
    return;
  }

  SC_HANDLE manager = ::OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);

  if (manager) {
    SC_HANDLE service = ::CreateService(
      manager,
      ToWCharString(Application::getApplication()->getFormalName()), // name of service
      ToWCharString(Application::getApplication()->getFormalName()), // name to display
      SERVICE_ALL_ACCESS, // desired access
      SERVICE_WIN32_OWN_PROCESS, // service type
      SERVICE_DEMAND_START, // start type
      SERVICE_ERROR_IGNORE, // error control type
      path, // service's binary
      0, // no load ordering group
      0, // no tag identifier
      TEXT(""), // dependencies
      0, // LocalSystem account
      0 // no password
    );

    if (service) {
      fout << "Service has been installed as "
           << Application::getApplication()->getFormalName() << ENDL;
      ::CloseServiceHandle(service);
    } else {
      ferr << "Failed to install service" << ENDL;
    }
    ::CloseServiceHandle(manager);
  } else {
    ferr << "Unable to open service control manager." << ENDL;
  }
}

#else // unix

Daemon::Daemon(Runnable* runnable)
{
  static unsigned int singleton = 0;
  bassert(singleton == 0, SingletonException("Daemon has been instantiated."));
  ++singleton;
  if (!runnable) {
    _throw OutOfDomain();
  }
  bassert(Application::getApplication(), Exception("Application has not been institiated."));

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  _throw ResourceException("Unable to daemonize process.");
#else
  switch (fork()) {
  case 0:
    setsid();
    runnable->run();
    break;
  case -1:
    SystemLogger::write(SystemLogger::ERROR, "Unable to daemonize process.");
    _throw ResourceException("Unable to daemonize process.");
    break;
  }
#endif
}

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
