/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/ProcessPreinitialization.h>
#include <base/Application.h>
#include <base/OperatingSystem.h>
#include <base/string/ASCIITraits.h>
#include <base/Trace.h>
#include <base/concurrency/Thread.h>
#include <stdlib.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
#else
  #include <time.h> // time_t
  #include <pthread.h> // pthread_t
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

namespace internal {
  
  namespace specific {
    
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
    
    // handle to the global process heap
    OperatingSystem::Handle processHeap;
    
    // the original unhandled exception filter
    LPTOP_LEVEL_EXCEPTION_FILTER originalExceptionFilter;
    
    LONG __stdcall exceptionFilter(EXCEPTION_POINTERS* exception) {
      char errorMessage[sizeof("System exception 0x################ at 0x################")]; // worst case
      char* dest = errorMessage;
      copy<char>(dest, "System exception 0x", sizeof("System exception 0x") - 1);
      dest += sizeof("System exception 0x") - 1;
      
      DWORD exceptionCode = exception->ExceptionRecord->ExceptionCode;
      for (unsigned int i = 0; i < (sizeof(DWORD) * 2); ++i) {
        dest[sizeof(DWORD)*2 - 1 - i] = ASCIITraits::valueToDigit(exceptionCode & 0x0f);
        exceptionCode >>= 4; // bits per digit
      }
      dest += sizeof(DWORD) * 2;
      
      copy<char>(dest, " at 0x", sizeof(" at 0x") - 1);
      dest += sizeof(" at 0x") - 1;
      unsigned long exceptionAddress = reinterpret_cast<unsigned long>(exception->ExceptionRecord->ExceptionAddress);
      for (unsigned int i = 0; i < (sizeof(unsigned long) * 2); ++i) {
        dest[sizeof(unsigned long)*2 - 1 - i] = ASCIITraits::valueToDigit(exceptionAddress & 0x0f);
        exceptionAddress >>= 4; // bits per digit
      }
      dest += sizeof(unsigned long) * 2;
      *dest = 0; // terminate string
      Trace::message(errorMessage);
      return EXCEPTION_EXECUTE_HANDLER; // terminate process (or call originalExceptionFilter(exception))
    }
#endif // flavor
  }; // end of namespace - specific
}; // end of namespace - internal

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)

ProcessPreinitialization::ProcessPreinitialization() throw() {
  // OS version compatibility check
  OSVERSIONINFO versionInfo;
  versionInfo.dwOSVersionInfoSize = sizeof(versionInfo);
  ::GetVersionEx(&versionInfo); // never fails
  // only Windows NT 4.0 or later
  // TAG: check w2k and
#if !defined(_DK_SDU_MIP__BASE__WINXP)
  #define _DK_SDU_MIP__BASE__WINXP (_DK_SDU_MIP__BASE__W2K + 1)
  #warning _DK_SDU_MIP__BASE__WINXP is not defined
#endif
#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__WINXP)
  bool compatible = (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) && (versionInfo.dwMajorVersion >= 5) && (versionInfo.dwMinorVersion >= 1);
#elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__W2K)
  bool compatible = (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) && (versionInfo.dwMajorVersion >= 5);
#else
  bool compatible = (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) && (versionInfo.dwMajorVersion >= 4);
#endif
  if (!compatible) {
    Trace::message("Operating system is not supported.");
    ::MessageBox(0, "Operating system is not supported.", 0, MB_OK);
    exit(Application::EXIT_CODE_INITIALIZATION);
  }
  
  // check types
  if (!((sizeof(OperatingSystem::Handle) == sizeof(HANDLE)) && // OperatingSystem
        (sizeof(OperatingSystem::Handle) == sizeof(SOCKET)) && // Socket
        (sizeof(long long) == sizeof(FILETIME)) && // Date, File
        (sizeof(long long) == sizeof(LARGE_INTEGER)) && // Timer
        (sizeof(unsigned long) == sizeof(DWORD)) &&
        (sizeof(unsigned long) >= sizeof(DWORD)) && // Process
        (sizeof(Thread::Identifier) >= sizeof(DWORD)))) { // Thread
    Trace::message("Type mismatch detected.");
    exit(Application::EXIT_CODE_INITIALIZATION);
  }
  // memory
  internal::specific::processHeap = ::GetProcessHeap(); // never fails
  
  // request access to the Windows Sockets interface
  WORD versionRequested;
  WSADATA wsaData;
  versionRequested = MAKEWORD(2, 2);
  if (::WSAStartup(versionRequested, &wsaData)) {
    Trace::message("Unable to initialize Windows Sockets.");
    exit(Application::EXIT_CODE_INITIALIZATION);
  }
  if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
    ::WSACleanup();
    Trace::message("Unable to initialize Windows Sockets.");
    exit(Application::EXIT_CODE_INITIALIZATION);
  }
  
  // install structured exception handler (nothing to do with c++ exception handling)
  internal::specific::originalExceptionFilter = ::SetUnhandledExceptionFilter(internal::specific::exceptionFilter);
  //::SetErrorMode(SEM_NOGPFAULTERRORBOX);    
}

ProcessPreinitialization::~ProcessPreinitialization() throw() {
  // restore the original unhandled exception filter
  ::SetUnhandledExceptionFilter(internal::specific::originalExceptionFilter);
  if (::WSACleanup() != 0) {
    // we couldn't care less if this fails
  }
}

#else // unix

ProcessPreinitialization::ProcessPreinitialization() throw() {
  // pthread_t is an arithmetic type according to The Single UNIX Specification, Version 2
  if (!((sizeof(Thread::Identifier) >= sizeof(pthread_t)) && // Thread (pthread support)
        (sizeof(int) >= sizeof(time_t)) && // Date
        (sizeof(unsigned long) >= sizeof(pid_t)))) { // Process
    Trace::message("Type mismatch detected."); // no stream initialized at this point
    exit(Application::EXIT_CODE_INITIALIZATION);
  }
}

ProcessPreinitialization::~ProcessPreinitialization() throw() {
}

#endif // flavor

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
