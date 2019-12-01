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
#include <base/concurrency/Process.h>
#include <base/Type.h>
#include <base/Application.h>
#include <base/Cast.h>
#include <base/string/WideString.h>
#include <base/Profiler.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#  include <psapi.h>
#else // unix
#  define __thread // TAG: temp. fix for s390-ibm-linux-gnu
#  include <sys/types.h>
#  include <sys/wait.h>
#  include <sys/time.h>
#  include <sys/resource.h> // getpriority, getrusage
#  include <unistd.h>
#  include <signal.h>
#  include <errno.h>
#  include <stdlib.h>

#  define _COM_AZURE_DEV__BASE__HAVE_GETRUSAGE
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
#  include <libproc.h>
#endif
#endif

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__UNIX)
  extern "C" char** environ;
#endif

// TAG: need to symbol to enable use of undocumented API (e.g. _COM_AZURE_DEV__BASE__UNDOCUMENTED)

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WINNT4) || (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__W2K)
namespace ntapi {
  
  typedef long NTSTATUS;
  typedef long KPRIORITY;
  
  struct ProcessBasicInformation {
    NTSTATUS exitStatus;
    void* /*PPEB*/ PEBBaseAddress;
    unsigned long* affinityMask;
    KPRIORITY basePriority;
    unsigned long* uniqueProcessId;
    unsigned long inheritedFromUniqueProcessId; // TAG: is the type correct
  };

  typedef NTSTATUS (__stdcall *PNtQueryInformationProcess)(HANDLE, unsigned int /*PROCESSINFOCLASS*/, void*, unsigned long, unsigned long*);
};
#endif

Process::ProcessHandle::~ProcessHandle() throw() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (isValid()) { // dont try to close if handle is invalidated
    ::CloseHandle(getHandle()); // should never fail
  }
#else // unix
  // nothing to do
#endif // flavor
}

Process::Process() throw() : id(Process::INVALID), handle(ProcessHandle::invalid) {
}

Process::Process(const Process& copy) throw() : id(copy.id), handle(copy.handle) {
}

Process Process::getProcess() throw() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return Process(::GetCurrentProcessId());
#else // unix
  return Process(::getpid());
#endif // flavor
}

Process Process::getParentProcess() throw() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WINNT4) || (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__W2K)
    // MT-safe 'cause DWORD is written atomically
    static bool isPIDCached = false;
    static unsigned long cachedPID = Process::INVALID;
    
    if (!isPIDCached) {
      ntapi::PNtQueryInformationProcess NtQueryInformationProcess =
        (ntapi::PNtQueryInformationProcess)::GetProcAddress(::GetModuleHandle("ntdll"), "NtQueryInformationProcess");
      if (NtQueryInformationProcess) {
        ntapi::ProcessBasicInformation information;
        if (!NtQueryInformationProcess(::GetCurrentProcess(),
                                       0, // process basic information class
                                       &information,
                                       sizeof(information),
                                       0)) {
          cachedPID = information.inheritedFromUniqueProcessId;
        }
      }
      isPIDCached = true; // cached pid is set to invalid by default
    }
    return Process(cachedPID);
  #else
    return Process(Process::INVALID); // win32 doesn't support this (WINNT 4)
  #endif
#else // unix
  return Process(::getppid());
#endif // flavor
}

Process Process::fork() throw(NotSupported, ProcessException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  throw NotSupported(Type::getType<Process>());
#else // unix
  pid_t result = ::fork(); // should we use fork1 on solaris
  if (result == (pid_t)-1) {
    throw ProcessException("Unable to fork process", Type::getType<Process>());
  }
  return Process(result);
#endif // flavor
}

#if !defined(BELOW_NORMAL_PRIORITY_CLASS) // should have been in winbase.h
  #define BELOW_NORMAL_PRIORITY_CLASS ((DWORD)0x00004000)
#endif

#if !defined(ABOVE_NORMAL_PRIORITY_CLASS) // should have been in winbase.h
  #define ABOVE_NORMAL_PRIORITY_CLASS ((DWORD)0x00008000)
#endif

int Process::getPriority() throw(ProcessException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DWORD priority = ::GetPriorityClass(::GetCurrentProcess());
  switch (priority) {
  case 0:
    throw ProcessException("Unable to get priority of process", Type::getType<Process>());
  case REALTIME_PRIORITY_CLASS:
    return 7 - 24;
  case HIGH_PRIORITY_CLASS:
    return 7 - 13;
  case ABOVE_NORMAL_PRIORITY_CLASS: // w2k or later
    return 7 - 10;
  case NORMAL_PRIORITY_CLASS:
    return 7 - 7;
  case BELOW_NORMAL_PRIORITY_CLASS: // w2k or later
    return 7 - 6;
  case IDLE_PRIORITY_CLASS:
    return 7 - 4;
  default:
    return 0; // unknown
  }
#else // unix
  #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN)
    #warning Process::getPriority() is not supported
    return 0; // TAG: api cygwin
  #else
    errno = 0;
    int priority = ::getpriority(PRIO_PROCESS, getpid());
    if ((priority == -1) && (errno != 0)) {
      throw ProcessException("Unable to get priority of process", Type::getType<Process>());
    }
    return priority;
  #endif
#endif // flavor
}

void Process::setPriority(int priority) throw(ProcessException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  DWORD priorityClass = 0;
  if (priority <= -20) {
    priorityClass = REALTIME_PRIORITY_CLASS;
  } else if (priority <= -10) {
    priorityClass = HIGH_PRIORITY_CLASS;
  } else if (priority <= -5) {
    priorityClass = ABOVE_NORMAL_PRIORITY_CLASS; // w2k or later
  } else if (priority < 5) {
    priorityClass = NORMAL_PRIORITY_CLASS;
  } else if (priority <= 10) {
    priorityClass = BELOW_NORMAL_PRIORITY_CLASS; // w2k or later
  } else {
    priorityClass = IDLE_PRIORITY_CLASS;
  }
  if (!::SetPriorityClass(::GetCurrentProcess(), priorityClass)) {
    throw ProcessException("Unable to set priority of process", Type::getType<Process>());
  }
#else // unix
  #if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN)
    #warning Process::setPriority() is not supported
  #else
    if (::setpriority(PRIO_PROCESS, ::getpid(), priority)) {
      ProcessException("Unable to set priority", Type::getType<Process>());
    }
  #endif
#endif // flavor
}

#if 0
// need mapping from error code to base error codes: ::GetLastError ... but also for unix

// need garbage collector for DLL attach/detach - TerminateThread does not call
// TAG: !!! Exit of process
BOOL GetExitCodeProcess(
  HANDLE hProcess,     // handle to the process
  LPDWORD lpExitCode   // termination status
);

// use with ExitProcess(...)
HANDLE CreateRemoteThread(
  HANDLE hProcess,                          // handle to process
  LPSECURITY_ATTRIBUTES lpThreadAttributes, // SD
  SIZE_T dwStackSize,                       // initial stack size
  LPTHREAD_START_ROUTINE lpStartAddress,    // thread function
  LPVOID lpParameter,                       // thread argument
  DWORD dwCreationFlags,                    // creation option
  LPDWORD lpThreadId                        // thread identifier
);
#endif

Process Process::execute(const String& command) throw(ProcessException) {
  // inherit handles, environment, use current working directory, and allow this app to wait for process to terminate
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  String commandLine = command;
  STARTUPINFO startInfo;
  PROCESS_INFORMATION processInformation;
  fill<uint8>(Cast::getAddress(startInfo), sizeof(startInfo), 0);
//   startInfo.cb = sizeof(startInfo);
//   startInfo.dwFlags = STARTF_USESTDHANDLES;
//   startInfo.hStdInput
//   startInfo.hStdOutput
//   startInfo.hStdError
  bassert(
    ::CreateProcess(0,
                    const_cast<wchar*>(ToWCharString(commandLine).native()), // command line (may need quotes)
                    nullptr, // process security attributes
                    nullptr, // primary thread security attributes
                    TRUE, // handles are inherited
                    0, // creation flags
                    nullptr, // use parent's environment
                    nullptr, // use parent's current directory
                    &startInfo, // STARTUPINFO
                    &processInformation // receives PROCESS_INFORMATION
    ) != 0,
    bindCause(ProcessException("Unable to execute command", Type::getType<Process>()), ::GetLastError())
  );
  
  ::CloseHandle(processInformation.hThread);
  Process result(processInformation.dwProcessId);
  result.handle = new ProcessHandle(processInformation.hProcess); // keep lock on process
  return result;
#else // unix
  // TAG: use spawn if available
  
  pid_t pid = 0;
  // int status = 0;

  pid = ::fork();
  if (pid == -1) {
    throw ProcessException("Unable to execute command", Type::getType<Process>());
  }
  if (pid == 0) { // is this the child
    // setup arguments list
    // first argument must be the module path
    char* argv[2] = {nullptr, nullptr};
    ::execve(command.getElements(), argv, environ);
    // we only get here if exec failed
    ::exit(Application::EXIT_CODE_INITIALIZATION); // child must never return from this method
  } else {
    return Process(pid);
  }
#endif // flavor
}

Process& Process::operator=(const Process& assign) throw()
{
  if (&assign == this) {
    id = assign.id;
    handle = assign.handle;
  }
  return *this;
}

bool Process::isAlive() const throw(ProcessException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
//   HANDLE newHandle;
//   BOOL res = ::DuplicateHandle(
//     ::GetCurrentProcess(), // handle to source process
//     id, // handle to duplicate
//     ::GetCurrentProcess(), // handle to target process
//     newHandle, // the duplicate handle
//     SYNCHRONIZE, // requested access
//     FALSE, // handle inheritance option
//     0 // optional actions
//   );
  // use SYNCHRONIZE
  DWORD exitCode = 0;
  HANDLE handle = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, static_cast<DWORD>(id));
  bassert(handle != 0, ProcessException("Unable to query process", this));
  BOOL result = ::GetExitCodeProcess(handle, &exitCode);
  ::CloseHandle(handle);
  if (result != 0) {
    return exitCode == STILL_ACTIVE;
  } else {
    throw ProcessException(this); 
  }
#else // unix
  
  int status = 0;
  pid_t result = ::waitpid(id, &status, WNOHANG);
  bassert(result >= 0, ProcessException("Unable to query process", this));

  /**
    GCC 3.0.4 bug
    
    For some reason this exception cannot be caught
    
    bassert(result >= 0, ProcessException("Unable to query process", this));
  */
  if (result == (pid_t)id) {
    return false;
  } else if (result == 0) {
    return true;
  }
  throw ProcessException("Unable to query process", this);
  
  /*
    GCC 3.0.4 bug here which results in "Abort" when the exception is raised below.
    
    Linux/RedHat 7.2
    
    gcc -I../.. -pipe -x c++ -ansi -O3 -g3 -march=i686 -malign-double -fomit-frame-pointer -funroll-loops -fno-delete-null-pointer-checks -fno-math-errno -fexpensive-optimizations -c Process.cpp -MT Process.lo -MD -MP -MF .deps/Process.TPlo -o Process.o
    
    if (result == (pid_t)id) {
      return false;
    } else if (result == 0) {
      return true;
    }
    throw ProcessException("Unable to query process", this); // Aborts here
  */

  // TAG: need to protect against EINTR
#endif // flavor
}

#if 0
// struct proc_taskinfo taskInfo;
// int status = proc_pidinfo(id, PROC_PIDTASKINFO, 0, &taskInfo, sizeof(taskInfo));

Array<unsigned int> /*Process::*/getPIDs()
{
  Array<unsigned int> result;
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  result.setSize(4096);
  while (true) {
    int count = proc_listallpids(&result[0], result.getSize());
    if (count < 0) {
      return result;
    }
    if (count < result.getSize()) {
      result.setSize(count);
      break;
    }
    result.setSize(result.getSize() * 2);
  }
#else
  BASSERT(!"Not implemented.");
#endif
  return result;
}
#endif

String Process::getName() const throw(NotSupported, ProcessException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  HANDLE process = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, id);
  bassert(process, ProcessException(this));
  wchar buffer[MAX_PATH + 1];
  DWORD length = ::GetModuleFileNameEx(process, 0, buffer, getArraySize(buffer));
  bassert(length > 0, ProcessException(this));
  ::CloseHandle(process);
  return String(buffer, length - 1);
#else // unix
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  PrimitiveStackArray<char> buffer(PROC_PIDPATHINFO_MAXSIZE);
  int status = proc_pidpath(id, buffer, buffer.size());
  if (status < 0) {
    return String();
  }
  return String(static_cast<const char*>(buffer), status);
#else
  return String();
#endif
#endif
}

void Process::lock() throw(ProcessException)
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!handle->isValid()) {
    HANDLE result = ::OpenProcess(SYNCHRONIZE, FALSE, static_cast<DWORD>(id));
    bassert(result != 0, ProcessException("Unable to acquire lock", this));
    handle = new ProcessHandle(result);
  }
#else // unix
#endif // flavor
}

int Process::wait(unsigned int microseconds) throw()
{
  Profiler::WaitTask profile("Process::wait()");
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!handle->isValid()) {
    lock();
  }
  if (::WaitForSingleObject(handle->getHandle(), (microseconds+999)/1000) == WAIT_TIMEOUT) {
    return Application::EXIT_CODE_INVALID;
  }
  DWORD exitCode = 0;
  ::GetExitCodeProcess(handle->getHandle(), &exitCode);
  return (exitCode != static_cast<unsigned int>(Application::EXIT_CODE_INVALID)) ? exitCode : Application::EXIT_CODE_CONFLICT;
#else // unix
// #  warning Process::wait(unsigned int microseconds) not implemented
  return Application::EXIT_CODE_INVALID;
#endif // flavor
}

int Process::wait() throw(ProcessException)
{
  Profiler::WaitTask profile("Process::wait()");
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!handle->isValid()) {
    lock();
  }
  DWORD exitCode = 0;
  ::WaitForSingleObject(handle->getHandle(), INFINITE);
  ::GetExitCodeProcess(handle->getHandle(), &exitCode);
  return exitCode;
#else // unix
  int status = 0;
  pid_t result = ::waitpid((pid_t)id, &status, 0);
  if (result != (pid_t)id) {
    if (errno == EINTR) {
      return Application::EXIT_CODE_INVALID;
    } else {
      throw ProcessException("Unable to wait for process", this);
    }
  }
  if (WIFEXITED(status)) {
    return WEXITSTATUS(status);
  } else {
    return Application::EXIT_CODE_INVALID;
  }
#endif // flavor
}

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
class KillImpl {
public:
  
  enum State {WAITING, FAILED, SUCCEEDED};
private:
  
  Process process;
  State state;
public:
  
  inline KillImpl(Process _process) throw() : process(_process), state(WAITING) {
  }

  inline Process getProcess() const throw() {
    return process;
  }
  
  inline void onFailure() throw() {
    state = FAILED;
  }
  
  inline void onSuccess() throw() {
    state = SUCCEEDED;
  }
  
  inline bool succeeded() const throw() {
    return state == SUCCEEDED;
  }
  
  static BOOL CALLBACK windowHandler(HWND window, LPARAM parameter) {
    KillImpl* kill = (KillImpl*)(parameter);
    
    DWORD processId = 0;
    /*DWORD threadId =*/ ::GetWindowThreadProcessId(window, &processId);
    
    if (processId == kill->getProcess().getId()) {
      static const wchar messageHandlerIdentity[] = _COM_AZURE_DEV__BASE__ID_PREFIX L"?message handler";
      wchar className[sizeof(messageHandlerIdentity) + 1];
      int length = ::GetClassName(window, className, getArraySize(className)); // excludes terminator
//      if ((length == (sizeof(messageHandlerIdentity) - 1)) &&
//          (compare(className, messageHandlerIdentity, sizeof(messageHandlerIdentity) - 1) == 0)) {
      DWORD_PTR dispatchResult = 0;
      // TAG: WM_QUIT is normally not posted - is this ok
      DWORD processId = 0;
      DWORD threadId = ::GetWindowThreadProcessId(window, &processId);
      fout << "threadId=" << threadId << " processId=" << processId << ENDL;
      
      Trace::message("sending WM_CLOSE now");
      LRESULT result = ::SendMessageTimeout(window, WM_CLOSE, 0, 0, SMTO_NORMAL, 3000, &dispatchResult);
      Trace::message("sending WM_DESTROY now");
      result = ::SendMessageTimeout(window, WM_DESTROY, 0, 0, SMTO_NORMAL, 3000, &dispatchResult);
      Trace::message("sending WM_QUIT now");
      result = ::SendMessageTimeout(window, WM_QUIT, Application::EXIT_CODE_EXTERNAL, 0, SMTO_NORMAL, 3000, &dispatchResult);
      fout << "result=" << result << ENDL;
      result = 1;
      if (result == 0) {
        kill->onFailure();
      } else {
        kill->onSuccess();
      }
      return TRUE; // stop enumeration
//      }
    }
    return TRUE; // continue enumeration
  }
  
  inline bool signal() throw() {
    /*BOOL ignore =*/ ::EnumWindows(windowHandler, (LPARAM)this);
    return state == SUCCEEDED;
  }
};
#endif // flavor

// TAG: need process group support
bool Process::terminate(bool force) throw(ProcessException) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (force) {
    // TAG: ask nicely first
    HANDLE handle = ::OpenProcess(PROCESS_TERMINATE, FALSE, static_cast<DWORD>(id));
    bassert(handle != 0, ProcessException("Unable to terminate process", this));
    ::TerminateProcess(handle, Application::EXIT_CODE_EXTERNAL);
    ::CloseHandle(handle);
    return true;
  } else {
    // TAG: if service then ask to stop

    HANDLE handle = ::OpenProcess(SYNCHRONIZE, FALSE, static_cast<DWORD>(id));
    bassert(handle != 0, ProcessException("Unable to terminate process", this));
    KillImpl kill(id);
    kill.signal();
    ::CloseHandle(handle);
    return kill.succeeded();
    
    // TAG: WM_QUIT is normally not posted - is this ok - do we need to post to specific window handle
    // BOOL result = ::PostThreadMessage(static_cast<DWORD>(id), WM_QUIT, Application::EXIT_CODE_EXTERNAL, 0);
    // BASSERT(result != 0);
    // TAG: throw ProcessException(this)
  }
#else
  int result = ::kill(pid_t(id), force ? (SIGKILL) : (SIGTERM));
  if (!result) {
    switch (errno) {
    case EPERM:
      // TAG: I think we need an AccessDenied exception which is not a specialization of IOException
      // throw Permission(this);
    case ESRCH:
    default:
      throw ProcessException(this);
    }
  }
  return result == 0;
#endif // flavor
}

Process::Times Process::getTimes() throw() {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!handle->isValid()) {
    lock();
  }
  FILETIME creationTime;
  FILETIME exitTime;
  FILETIME systemTime;
  FILETIME userTime;
  ::GetProcessTimes(handle->getHandle(), &creationTime, &exitTime, &systemTime, &userTime);
  Process::Times result;
  result.user = Cast::impersonate<uint64>(userTime) * 100;
  result.system = Cast::impersonate<uint64>(systemTime) * 100;
  return result;
#else // unix
  // TAG: fixme
  #if defined(_COM_AZURE_DEV__BASE__HAVE_GETRUSAGE)
    struct rusage usage;
    ::getrusage(RUSAGE_SELF, &usage); // does not fail
    Process::Times result;
    result.user = usage.ru_utime.tv_sec * 1000000000ULL + usage.ru_utime.tv_usec * 1000ULL;
    result.system = usage.ru_stime.tv_sec * 1000000000ULL + usage.ru_stime.tv_usec * 1000ULL;
    return result;
  #else
    static unsigned long ticksPerSecond = 0; // 0 ~ not cached
    struct tms;
    ::times(&tms);
    if (ticksPerSecond == 0) { // get if not cached
      ticksPerSecond = ::sysconf(_SC_CLK_TCK);
    }
    Process::Times result;
    result.user = tms.tms_utime * 1000000000ULL/ticksPerSecond;
    result.system = tms.tms_stime * 1000000000ULL/ticksPerSecond;
    return result;
  #endif
#endif // flavor
}

extern "C" int main();

// TAG: need method to dump stack trace and registers of context
// class Context (architure)

#if (0 && (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32))
void dumpDebugInfo(const BYTE* caller, void* instance) throw() {
  const char* name = ::GetModuleFilename(module) ;
  
  // offset 0 is DOS header
  PIMAGE_DOS_HEADER fileBase =  ::MapFileInMemory(name);
  
  // retrieve NT header
  PIMAGE_NT_HEADER NT_Header = fileBase + filebase->e_lfanew;
  
  // Get debug header
  // Borland:
  PIMAGE_SECTION_HEADER debugHeader =
    SectionHeaderFromName( ".debug" ) ;
  PIMAGE_DEBUG_DIRECTORY debugDir =
    fileBase + debugHeader->PointerToRawData ;
  // Microsoft
  debugHeader = SectionHeaderFromName( ".rdata" ) ;
  debugDir = fileBase + 
    debugHeader->PointerToRawData + 
    debugDirRVA - debugHeader->VirtualAddress ;

  // Get COFF debug directory and COFF debug header
  Look for an entry in the debug directory table 
  with Type == IMAGE_DEBUG_TYPE_COFF ;
  PIMAGE_COFF_SYMBOLS_HEADER COFFdebug  = 
    fileBase + debugDir->PointerToRawData ;

  // Get symbol table, symbol count, string table;
  // the string table starts right after the symbol table
  PIMAGE_SYMBOL COFFsymbol = 
    filebase + NT_Header->FileHeader.PointerToSymbolTable ;
  int COFFcount = 
    NT_Header->FileHeader.NumberOfSymbols ;
  const char* stringTable = COFFsymbolTable + COFFsymbolCount ;
  
  // Dump info about caller
  int RVA = caller - (BYTE*)hInstance ;
  if(COFFcount && COFFsymbolTable) {
    // Lookup source file name
    Search in COFFsymbolTable a ".text"
    (or "CODE") section which includes RVA:
    that gives the filename ;
    
    // Lookup function name
    Search in COFFsymbolTable the function symbol 
    with the biggest address <= RVA ;
    Lookup the function name in stringTable
    given the function symbol ;

    // Lookup line number
    PIMAGE_LINENUMBER lineTable = 
      COFFDebugInfo + 
      COFFDebugInfo->LvaToFirstLinenumber ;
    Search in lineTable the line number 
    with the biggest address <= RVA ;

    Dump file/function/line info ;
    }
  else
    only module name and caller address available ;
  }
#endif  

// TAG: add method Thread::getInvocationLevel() const throw();
// TAG: get current stack usage
// TAG: get stack available

FormatOutputStream& operator<<(
  FormatOutputStream& stream,
  const Process::Layout& value) throw(IOException)
{
  return stream;
  
  // modules
  // stack
  // threads
  // segments: text, data, stack, ...
  // ...
  stream << "Process layout of " << Process::getProcess().getId() << EOL;

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  MEMORY_BASIC_INFORMATION information;
  ::VirtualQuery(::GetModuleHandle(0), &information, sizeof(information));
  
  stream << "  base address: " << information.BaseAddress << EOL
         << "  allocation base: " << information.AllocationBase << EOL
         << "  region size: " << information.RegionSize << EOL
         << "  protection: " << HEX << information.AllocationProtect << EOL
         << "  type: " << HEX << information.Type << EOL;

  CONTEXT context;
  unsigned short ss = 0;
  unsigned int current = 0;
#if 0 // TAG: FIXME
  asm (
    "movl %%ss,%0; \n\t"
    : "=m" (ss) // output
  );
#endif

  LDT_ENTRY ldt;
  /* BOOL xxx =*/ ::GetThreadSelectorEntry(::GetCurrentThread(), ss, &ldt);
  current = ldt.BaseLow | (ldt.HighWord.Bytes.BaseMid << 16) | (ldt.HighWord.Bytes.BaseHi << 24);

  //::GetThreadContext(::GetCurrentThread(), &context);
  stream << "  esp: " << HEX << current << EOL;

  ::VirtualQuery((void*)context.Rsp, &information, sizeof(information));
  stream << "  stack base address: " << information.BaseAddress << EOL
         << "  stack allocation base: " << information.AllocationBase << EOL
         << "  stack region size: " << information.RegionSize << EOL
         << "  stack protection: " << HEX << information.AllocationProtect << EOL
         << "  stack type: " << HEX << information.Type << EOL;
  
#elif ((_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX) && (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__X86))
  // text, data, bss begin at 0x08048000 for IA-32
  // last address is 0xbfffffff for IA-32
  
  stream << "  begin of text segment: " << 0x08048000 << EOL
         << "  end of data segment: " << sbrk(0) << EOL
         << "  end of stack segment: " << 0xbfffffff << EOL;
#endif // flavor
  
  stream << ENDL;
  return stream;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
