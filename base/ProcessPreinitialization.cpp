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
#include <base/ProcessPreinitialization.h>
#include <base/Application.h>
#include <base/OperatingSystem.h>
#include <base/string/ASCIITraits.h>
#include <base/Trace.h>
#include <base/concurrency/Thread.h>
#include <base/Literal.h>
#include <stdlib.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#else
#  define __thread // TAG: temp. fix for s390-ibm-linux-gnu
#  include <time.h> // time_t
#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__WASI)
#  include <pthread.h> // pthread_t
#endif
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

_COM_AZURE_DEV__BASE__GLOBAL_PRINT();

namespace internal {
  
  namespace specific {
    
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    
    // handle to the global process heap
    OperatingSystem::Handle processHeap = nullptr;
    
    // the original unhandled exception filter
    LPTOP_LEVEL_EXCEPTION_FILTER originalExceptionFilter = nullptr;

    LONG WINAPI exceptionFilter(EXCEPTION_POINTERS* exception)
    {
      char errorMessage[sizeof("Internal error: System exception 0x################ (access violation) while reading from 0x################ at 0x################")]; // worst case
      char* dest = errorMessage;
      
      const Literal literal("Internal error: System exception 0x");
      copy<char>(dest, literal.getValue(), literal.getLength());
      dest += literal.getLength();
      
      const DWORD exceptionCode = exception->ExceptionRecord->ExceptionCode;

      enum {
        DESCRIPTION_ACCESS_VIOLATION,
        DESCRIPTION_INVALID_PAGE,
        DESCRIPTION_GUARD_PAGE,
        DESCRIPTION_MISALIGNMENT,
        DESCRIPTION_STACK_OVERFLOW,
        DESCRIPTION_DIVISION_BY_ZERO,
        DESCRIPTION_FLOATING_POINT,
        DESCRIPTION_ILLEGAL_INSTRUCTION,
        DESCRIPTION_INVALID_HANDLE,
        DESCRIPTION_NONE
      };
      
      // make sure size of errorMessage is ok
      static const Literal EXCEPTION_DESCRIPTIONS[] = {
        Literal("access violation"),
        Literal("invalid page"),
        Literal("guard page"),
        Literal("misalignment"),
        Literal("stack overflow"),
        Literal("division by zero"),
        Literal("floating-point exception"),
        Literal("illegal instruction"),
        Literal("invalid handle"),
        Literal("")
      };

      unsigned int description = 0;
      switch (exceptionCode) {
      case EXCEPTION_ACCESS_VIOLATION:
        description = DESCRIPTION_ACCESS_VIOLATION;
        break;
      case EXCEPTION_IN_PAGE_ERROR:
        description = DESCRIPTION_INVALID_PAGE;        
        break;
      case EXCEPTION_GUARD_PAGE:
        description = DESCRIPTION_GUARD_PAGE;        
        break;
      case EXCEPTION_DATATYPE_MISALIGNMENT:
        description = DESCRIPTION_MISALIGNMENT;
        break;
      case EXCEPTION_STACK_OVERFLOW:
        description = DESCRIPTION_STACK_OVERFLOW;
        break;
      case EXCEPTION_INT_DIVIDE_BY_ZERO:
        description = DESCRIPTION_DIVISION_BY_ZERO;
        break;
      case EXCEPTION_FLT_DENORMAL_OPERAND:
      case EXCEPTION_FLT_INEXACT_RESULT:
      case EXCEPTION_FLT_DIVIDE_BY_ZERO:
      case EXCEPTION_FLT_INVALID_OPERATION:
      case EXCEPTION_FLT_OVERFLOW:
      case EXCEPTION_FLT_STACK_CHECK:
      case EXCEPTION_FLT_UNDERFLOW:
        description = DESCRIPTION_FLOATING_POINT;
        break;
      case EXCEPTION_ILLEGAL_INSTRUCTION:
      case EXCEPTION_PRIV_INSTRUCTION:
        description = DESCRIPTION_ILLEGAL_INSTRUCTION;
        break;
      case EXCEPTION_INVALID_HANDLE:
        description = DESCRIPTION_INVALID_HANDLE;
        break;
      case CONTROL_C_EXIT: // TAG: not an error
        description = DESCRIPTION_NONE;        
        break;
      case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
      case EXCEPTION_INT_OVERFLOW:
      default:
        description = DESCRIPTION_NONE;
      }

      DWORD temp = exceptionCode;
      for (unsigned int i = 0; i < (sizeof(DWORD) * 2); ++i) {
        dest[sizeof(DWORD) * 2 - 1 - i] =
          ASCIITraits::valueToDigit(temp & 0x0f);
        temp >>= 4; // bits per digit
      }
      dest += sizeof(DWORD) * 2;

      const Literal desc = EXCEPTION_DESCRIPTIONS[description];
      if (desc.getLength()) {
        *dest++ = ' ';
        *dest++ = '(';
        copy<char>(dest, desc.getValue(), desc.getLength());
        dest += desc.getLength();
        *dest++ = ')';
      }
      
      if ((exceptionCode == EXCEPTION_ACCESS_VIOLATION) &&
          (exception->ExceptionRecord->NumberParameters == 2)) {
        MemorySize dataAddress =
          exception->ExceptionRecord->ExceptionInformation[1];
        if (exception->ExceptionRecord->ExceptionInformation[0] == 0) {
          const Literal literal(" while reading from 0x");
          copy<char>(dest, literal.getValue(), literal.getLength());
          dest += literal.getLength();
          for (unsigned int i = 0; i < (sizeof(MemorySize) * 2); ++i) {
            dest[sizeof(MemorySize) * 2 - 1 - i] =
              ASCIITraits::valueToDigit(dataAddress & 0x0f);
            dataAddress >>= 4; // bits per digit
          }
          dest += sizeof(MemorySize) * 2;
        } else if (exception->ExceptionRecord->ExceptionInformation[0] == 1) {
          const Literal literal(" while writing to 0x");
          copy<char>(dest, literal.getValue(), literal.getLength());
          dest += literal.getLength();
          for (unsigned int i = 0; i < (sizeof(MemorySize) * 2); ++i) {
            dest[sizeof(MemorySize) * 2 - 1 - i] =
              ASCIITraits::valueToDigit(dataAddress & 0x0f);
            dataAddress >>= 4; // bits per digit
          }
          dest += sizeof(MemorySize) * 2;
        }
      }
      
      copy<char>(dest, " at 0x", sizeof(" at 0x") - 1);
      dest += sizeof(" at 0x") - 1;
      MemorySize exceptionAddress =
        Cast::getOffset(exception->ExceptionRecord->ExceptionAddress);
      for (unsigned int i = 0; i < (sizeof(MemorySize) * 2); ++i) {
        dest[sizeof(MemorySize) * 2 - 1 - i] =
          ASCIITraits::valueToDigit(exceptionAddress & 0x0f);
        exceptionAddress >>= 4; // bits per digit
      }
      dest += sizeof(MemorySize) * 2;
      *dest = 0; // terminate string
      
      Trace::message(errorMessage);
      // TAG: use system log (for errorMessage only)
      
      const CONTEXT* context = exception->ContextRecord;
#if (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__X86)
      // TAG: move context dump to Architecture class?
      // TAG: reimplement without dependency on FormatOutputStream?
      ferr << NativeString(errorMessage) << EOL << ENDL;
      
      ferr << "eax:" << HEX << ZEROPAD << PREFIX << setWidth(10)
           << context->Eax << indent(4)
           << "ebx:" << HEX << ZEROPAD << PREFIX << setWidth(10)
           << context->Ebx << indent(4)
           << "ecx:" << HEX << ZEROPAD << PREFIX << setWidth(10)
           << context->Ecx << indent(4)
           << "edx:" << HEX << ZEROPAD << PREFIX << setWidth(10)
           << context->Edx << EOL
        
           << "edi:" << HEX << ZEROPAD << PREFIX << setWidth(10)
           << context->Edi << indent(4)
           << "esi:" << HEX << ZEROPAD << PREFIX << setWidth(10)
           << context->Esi << indent(4)
           << "ebp:" << HEX << ZEROPAD << PREFIX << setWidth(10)
           << context->Ebp << indent(4)
           << "esp:" << HEX << ZEROPAD << PREFIX << setWidth(10)
           << context->Esp << EOL
        
           << "eflags:" << HEX << ZEROPAD << PREFIX << setWidth(10)
           << context->Edi << indent(1)
           << "eip:" << HEX << ZEROPAD << PREFIX << setWidth(10)
           << context->Eip << indent(4)
           << "cs:" << HEX << ZEROPAD << PREFIX << setWidth(6)
           << static_cast<uint16>(context->SegCs) << indent(9)
           << "ss:" << HEX << ZEROPAD << PREFIX << setWidth(6)
           << static_cast<uint16>(context->SegSs) << EOL
        
           << "ds:" << HEX << ZEROPAD << PREFIX << setWidth(6)
           << static_cast<uint16>(context->SegDs) << indent(9)
           << "es:" << HEX << ZEROPAD << PREFIX << setWidth(6)
           << static_cast<uint16>(context->SegEs) << indent(9)
           << "fs:" << HEX << ZEROPAD << PREFIX << setWidth(6)
           << static_cast<uint16>(context->SegFs) << indent(9)
           << "gs:" << HEX << ZEROPAD << PREFIX << setWidth(6)
           << static_cast<uint16>(context->SegGs) << EOL
           << EOL;

      // TAG: what about fop
      ferr << "fctrl:" << HEX << ZEROPAD << PREFIX << setWidth(6)
           << static_cast<uint16>(context->FloatSave.ControlWord) << indent(6)
           << "fstat:" << HEX << ZEROPAD << PREFIX << setWidth(6)
           << static_cast<uint16>(context->FloatSave.StatusWord) << indent(6)
           << "ftag:" << HEX << ZEROPAD << PREFIX << setWidth(6)
           << static_cast<uint16>(context->FloatSave.TagWord) << EOL
        
           << "fiseg:" << HEX << ZEROPAD << PREFIX << setWidth(6)
           << static_cast<uint16>(context->FloatSave.ErrorSelector)
           << indent(6)
           << "fioff:" << HEX << ZEROPAD << PREFIX << setWidth(10)
           << context->FloatSave.ErrorOffset << indent(2)
           << "foseg:" << HEX << ZEROPAD << PREFIX << setWidth(6)
           << static_cast<uint16>(context->FloatSave.DataSelector)
           << indent(6)
           << "fooff:" << HEX << ZEROPAD << PREFIX << setWidth(10)
           << context->FloatSave.DataOffset << EOL
           << EOL;
      
      for (unsigned int i = 0; i < 8; ++i) {
        ferr << "st(" << i << "):0x";
        for (unsigned int j = 0; j < 10; ++j) {
          ferr << HEX << ZEROPAD << NOPREFIX << setWidth(2)
               << context->FloatSave.RegisterArea[i * 10 + 9 - j];
        }
        ferr << EOL;
      }
      ferr << EOL;

      ferr << "stack content:" << EOL;
      const uint32* stack =
        Cast::getPointer<const uint32*>((context->Esp - 2 * 4 * 4) & ~3);
      for (unsigned int i = 0; i < 5; ++i) { // 5 rows
        ferr << ZEROPAD << setWidth(10) << stack << ':';
        for (unsigned int j = 0; j < 4; ++j) { // 4 columns
          ferr << SP << HEX << ZEROPAD << PREFIX << setWidth(10) << *stack++;
        }
        ferr << EOL;
      }
      ferr << ENDL;
      
      // assembly:
      // TAG: write current fucntion/method symbol
#endif
      // TAG: terminate process (or call originalExceptionFilter(exception))
      TerminateProcess(
        GetCurrentProcess(),
        Application::EXIT_CODE_INTERNAL_ERROR
      );
      return EXCEPTION_EXECUTE_HANDLER;
    }
#endif // flavor
  }; // end of namespace - specific
}; // end of namespace - internal

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)

ProcessPreinitialization::ProcessPreinitialization() noexcept
{
  // OS version compatibility check
  OSVERSIONINFO versionInfo;
  versionInfo.dwOSVersionInfoSize = sizeof(versionInfo);
  ::GetVersionEx(&versionInfo); // never fails
  // only Windows NT 4.0 or later
  // TAG: check w2k and
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WXP)
  bool compatible = (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) &&
    (versionInfo.dwMajorVersion >= 5) && (versionInfo.dwMinorVersion >= 1);
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__W2K)
  bool compatible = (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) &&
    (versionInfo.dwMajorVersion >= 5);
#else
  bool compatible = (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) &&
    (versionInfo.dwMajorVersion >= 4);
#endif
  if (!compatible) {
    Trace::message("Operating system is not supported.");
    ::MessageBox(0, L"Operating system is not supported.", 0, MB_OK);
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
  internal::specific::originalExceptionFilter =
    ::SetUnhandledExceptionFilter(internal::specific::exceptionFilter);
  //::SetErrorMode(SEM_NOGPFAULTERRORBOX);    
}

ProcessPreinitialization::~ProcessPreinitialization() noexcept
{
  // restore the original unhandled exception filter
  ::SetUnhandledExceptionFilter(internal::specific::originalExceptionFilter);
  if (::WSACleanup() != 0) {
    // we couldn't care less if this fails
  }
}

#else // unix
ProcessPreinitialization::ProcessPreinitialization() noexcept
{
}

ProcessPreinitialization::~ProcessPreinitialization() noexcept
{
}

#endif // flavor

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
