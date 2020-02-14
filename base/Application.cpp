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
#include <base/Application.h>
#include <base/TypeInfo.h>
#include <base/SystemLogger.h>
#include <base/concurrency/ThreadLocalContext.h>
#include <base/string/StringOutputStream.h>
#include <base/UnexpectedFailure.h>
#include <base/string/WideString.h>
#include <base/io/FileDescriptor.h>
#include <base/Profiler.h>
#include <base/build.h>

#include <stdlib.h>
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__EMCC)
#  include <emscripten.h>
#endif

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#  undef ERROR // protect against the evil programmers
#else // unix
#  if (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__S390)
#    define __thread // TAG: temp. fix for s390-ibm-linux-gnu
#  endif
#  if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__WASI) && \
      (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__EMCC)
#  include <sys/signal.h> // defines SIG_ERR on IRIX65
#  endif
#  include <signal.h>

#  if (defined(_COM_AZURE_DEV__BASE__HAVE_SIGACTION))
#    if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS)
#      include <sys/frame.h>
#    endif
#  endif
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

  // TAG: TODO we cannot get access to true exception type - best to hook throw after exception has been created
  void defaultExceptionHandler(const Exception* exception) noexcept
  {
    // C++: would be best to hook __cxa_throw to get stack trace at throw only
    if (exception) {
      Profiler::pushException(*exception); // TAG: get type directly from throw hook

      if (auto tls = Thread::getLocalContext()) {
        tls->stackTrace = StackTrace::getStack(1, 64); // TAG: can we skip stack so we only get the first exception constructor?
        // TAG: only if dumping and if exception isnt silenced
        if (Exception::getDumpExceptions()) {
          auto& ferr = StackTrace::getErrorStream();
          ferr << "EXCEPTION CONSTRUCTED BY: " << ENDL;
          ferr << FormattedStackTrace(tls->stackTrace,
            StackTrace::FLAG_DEFAULT |
            (ferr.isANSITerminal() ? StackTrace::FLAG_USE_COLORS : 0)
          ) << FLUSH;
        }
      }
    }
  }

// LLVM extern "C" void __cxa_throw();

class ApplicationImpl {
public:

  static void exceptionHandler(StackTrace* _stackTrace) noexcept
  {
    static bool firstTime = true;

    if (firstTime && _stackTrace) {
      StackTrace& stackTrace = *_stackTrace;
#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_GCC) // not in stack trace for LLVM
      // info before initial __cxa_throw is not useful
      void* address = (void*)&__cxa_throw;
      auto index = stackTrace.findLast(address);
      if (index >= 0) {
        stackTrace.stripUntil(index);
      }
#endif

      if (!stackTrace.isEmpty()) {
        auto& ferr = StackTrace::getErrorStream();
        ferr << FormattedStackTrace(stackTrace,
          StackTrace::FLAG_COMPACT | StackTrace::FLAG_DEFAULT |
          (ferr.isANSITerminal() ? StackTrace::FLAG_USE_COLORS : 0)
        ) << FLUSH;
      }
    }

    StringOutputStream stream;
    // const Type exceptionType = Exception::getExceptionType();
    if (firstTime /*|| exceptionType.isInitialized()*/) {
      firstTime = false;

      try {
        throw;
      } catch (Exception& e) {
        stream << "Internal error: Uncaught exception '"
               << TypeInfo::getTypename(e) << "' was raised";
        if (e.getType().isInitialized()) {
          stream << " by '" << TypeInfo::getTypename(e.getType()) << '\'';
        }
        const unsigned int cause = e.getCause();
        const unsigned int nativeError = e.getError();
        const char* message = e.getMessage();
        if (message || (cause != PrimitiveTraits<unsigned int>::MAXIMUM)) {
          stream << " with";
        }
        if (message) {
          stream << " message '" << message << '\'';
        }
        if (message && (cause != PrimitiveTraits<unsigned int>::MAXIMUM)) {
          stream << " and";
        }
        if (cause != PrimitiveTraits<unsigned int>::MAXIMUM) {
          stream << " cause " << cause;
        } else if (nativeError != 0) {
          stream << " due to native error " << nativeError;
          unsigned int error = OperatingSystem::getErrorCode(nativeError);
          if (error != OperatingSystem::UNSPECIFIED_ERROR) {
            stream << ' ' << '(' << OperatingSystem::getErrorMessage(error) << ')';
          }
        }
        stream << '.' << FLUSH;
      } catch (std::exception& e) {
        if (const String w = e.what()) {
          stream << "Internal error: Uncaught exception '" << Exception::getStdExceptionName(e)
                 << "' was raised with message '" << w << "'." << FLUSH;
        } else {
          stream << "Internal error: Uncaught exception '" << Exception::getStdExceptionName(e)
                 << "' was raised." << FLUSH;
        }
      } catch (const char*) {
        // TAG: we need safe virtual memory read to show string - we cant assume we can read anything
        stream << "Internal error: Uncaught string literal exception." << FLUSH;
      } catch (const wchar*) {
        // TAG: we need safe virtual memory read to show string - we cant assume we can read anything
        stream << "Internal error: Uncaught string literal exception." << FLUSH;
      } catch (...) {
        stream << "Internal error: Uncaught and unknown exception." << FLUSH;
      }
    } else {
      stream << "Internal error: No exception detected so must be explicit termination by throw; or std::terminate()." << FLUSH;
    }

    auto& ferr = StackTrace::getErrorStream();
    ferr << stream.getString() << ENDL; // TAG: use appropriate error stream
    
    if (auto tls = Thread::getLocalContext()) {
      if (!tls->stackTrace.isEmpty()) {
        ferr << FormattedStackTrace(tls->stackTrace,
          StackTrace::FLAG_DEFAULT |
          (ferr.isANSITerminal() ? StackTrace::FLAG_USE_COLORS : 0)
        ) << FLUSH;
      }
    }
    
#if defined(_COM_AZURE_DEV__BASE__ANY_DEBUG)
    // TAG: need runtime debug mode support (e.g. bool Trace::debug or with level support)
    Trace::message(stream.getString().native());
#endif
    exit(Application::EXIT_CODE_INTERNAL_ERROR); // TAG: is abort() better
  }

  static void terminationExceptionHandler() noexcept
  {
    static bool firstTime = true;
    if (firstTime) {
      firstTime = false;
      ferr << "Internal error: Application will be terminated." << ENDL;
      auto stackTrace = StackTrace::getStack(2);
      exceptionHandler(&stackTrace);
    } else {
      exceptionHandler(nullptr);
    }
  }

  static void unexpectedExceptionHandler() noexcept
  {
    static bool firstTime = true;
    if (firstTime) {
      firstTime = false;
      ferr << "Internal error: Unexpected exception." << ENDL;
      auto stackTrace = StackTrace::getStack(2);
      exceptionHandler(&stackTrace);
    } else {
      exceptionHandler(nullptr);
    }
  }

  /* The application signal handler. */
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#if 0 // disabled
  // TAG: we should destroy window in destructor
  StringOutputStream stream;
  stream << "messageHandler: message=" << message << " primary="
         << primaryParameter << " second=" << secondaryParameter << FLUSH;
  Trace::message(stream.getString().native());
  if (::InSendMessage()) {
    ::ReplyMessage(0);
  }
  if (Application::application) {
    Application::application->terminate();
  }
  return 0;
  switch () {
  case WM_CLOSE:
    return 0;
  case WM_DESTROY:
    ::PostQuitMessage(Application::application->exitCode);
  return 0;
  case WM_QUERYENDSESSION:
    return TRUE; // do nothing but allow session to be closed
  case WM_ENDSESSION: // TAG: I do NOT like this message
    // wait for termination
    return 0;
  }
  
  windowClass.lpszClassName = "https://dev.azure.com/renefonseca/base?message handler";
  OSVERSIONINFO versionInfo;
  versionInfo.dwOSVersionInfoSize = sizeof(versionInfo);
  ::GetVersionEx(&versionInfo); // never fails
  bool isWindows2000OrLater = (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) && (versionInfo.dwMajorVersion >= 5);
//   OSVERSIONINFO versionInfo;
//   versionInfo.dwOSVersionInfoSize = sizeof(versionInfo);
//   ::GetVersionEx(&versionInfo); // never fails
//   bool isWindows2000OrLater = (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) && (versionInfo.dwMajorVersion >= 5);
// we do not use message-only window 'cause it cannot be enumerated
  "https://dev.azure.com/renefonseca/base?message handler", // registered class name
    isWindows2000OrLater ? ((HWND)-3) : ((HWND)0), // no parent or owner window - (HWND(-3)) ~ HWND_MESSAGE
    ((HWND)0), // no parent or owner window
                                        //isWindows2000OrLater ? ((HWND)-3) : ((HWND)0), // (HWND(-3)) ~ HWND_MESSAGE
  DWORD dispatchResult = 0;
  LRESULT result2 = ::SendMessageTimeout(
    messageWindow,
    10000 + WM_QUIT,
    Application::EXIT_CODE_EXTERNAL,
    0,
    SMTO_NORMAL,
    3000,
    &dispatchResult
  );
#endif // disabled
  
  class AbortException : public Exception {
  public:

    AbortException(const char* message)
      : Exception(message)
    {
    }
  };

  static BOOL WINAPI signalHandler(DWORD signal) noexcept
  {
    Profiler::pushSignal("signalHandler()");

    switch (signal) {
    case CTRL_LOGOFF_EVENT:
    case CTRL_SHUTDOWN_EVENT:
    case CTRL_CLOSE_EVENT: // console is closing
    case CTRL_BREAK_EVENT: // Ctrl+Break
    case CTRL_C_EVENT: // Ctrl+C
      static bool firstTime = true;
      if (firstTime) {
        firstTime = false;
        if (FileDescriptor::getStandardError().isTerminal()) {
          ferr << "Aborted by user." << ENDL;

#if 0 // not useful for Win32
          StackTrace stackTrace = StackTrace::getStack(0);
          auto& ferr = StackTrace::getErrorStream();
          ferr << FormattedStackTrace(stackTrace,
            StackTrace::FLAG_DEFAULT |
            (ferr.isANSITerminal() ? StackTrace::FLAG_USE_COLORS : 0)
          ) << FLUSH;
#endif

          // _throw AbortException("Aborted by user.");
        }
      }
      SystemLogger::write(SystemLogger::INFORMATION, "Terminate signal.");
      if (Application::application) {
        Application::application->terminate();
        return TRUE;
      }
    }
    return FALSE;
  }

  static LRESULT CALLBACK messageHandler(
    HWND window,
    UINT message,
    WPARAM primaryParameter,
    LPARAM secondaryParameter) noexcept
  {
    Profiler::pushSignal("messageHandler()");

    // TAG: we should destroy window in destructor
    //StringOutputStream stream;
    //stream << "messageHandler: message=" << message << " primary="
    //       << primaryParameter << " second=" << secondaryParameter << FLUSH;
    //Trace::message(stream.getString().native());
    switch (message) {
    case WM_QUIT:
      Trace::message("Quit.");
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

#  if (defined(_COM_AZURE_DEV__BASE__HAVE_SIGACTION))
  static void actionHandler(int signal, siginfo_t* info, void* opaque) noexcept
  {
    Profiler::pushSignal("actionHandler()");

    const char* error = nullptr;
    auto& ferr = StackTrace::getErrorStream();
    
#if 0
    const ucontext_t* context = Cast::pointer<const ucontext_t*>(opaque);
    const mcontext_t* m = &context->uc_mcontext;
#endif

    const void* instructionAddress = nullptr;
#if ((_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS) && \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__SPARC32))
    instructionAddress = (void*)(m->gregs[REG_PC]);
#endif
    
    switch (signal) {
    case SIGQUIT: // quit signal from keyboard
      static bool firstTime = true;
      if (firstTime) {
        firstTime = false;
        if (FileDescriptor::getStandardError().isTerminal()) {
          ferr << "Aborted by user." << ENDL;
        }
      }
      if (Thread::getThread()->isMainThread()) {
        SystemLogger::write(SystemLogger::INFORMATION, "Quit signal.");
        if (Application::application) {
          Application::application->terminate();
        }
      }
      break;
    case SIGINT: // interrrupt from keyboard
      if (Thread::getThread()->isMainThread()) {
        SystemLogger::write(SystemLogger::ERROR, "Interrupt signal.");
        if (Application::application) {
          Application::application->terminate();
        }
      }
      break;
    case SIGFPE:
      switch (info->si_code) {
      case FPE_INTDIV:
        error = "integer divide by zero";
        break;
      case FPE_INTOVF:
        error = "integer overflow";
        break;
      case FPE_FLTDIV:
        error = "floating-point divide by zero";
        break;
      case FPE_FLTOVF:
        error = "floating-point overflow";
        break;
      case FPE_FLTUND:
        error = "floating-point underflow";
        break;
      case FPE_FLTRES:
        error = "floating-point inexact result";
        break;
      case FPE_FLTINV:
        error = "invalid floating-point operation";
        break;
      case FPE_FLTSUB:
        error = "subscript out of range";
        break;
      default:
        break;
      }
      ferr << "Internal error: Floating-point exception";
      if (error) {
        ferr << SP << '(' << error << ')';
      }
      ferr << " at address " << setWidth(2 + 2 * sizeof(void*))
           << instructionAddress << '.' << ENDL;
      break;
    case SIGBUS:
      switch (info->si_code) {
      case BUS_ADRALN:
        error = "address alignment";
        break;
      case BUS_ADRERR:
        error = "invalid physical address";
        break;
      case BUS_OBJERR:
        error = "hardware error";
        break;
      default:
        break;
      }
      ferr << "Internal error: Bus error ";
      if (error) {
        ferr << '(' << error << ')';
      }
      ferr << ENDL;
      break;
    case SIGILL:
      switch (info->si_code) {
      case ILL_ILLOPC:
        error = "illegal opcode";
        break;
      case ILL_ILLOPN:
        error = "illegal operand";
        break;
      case ILL_ILLADR:
        error = "illegal addressing mode";
        break;
      case ILL_ILLTRP:
        error = "illegal trap";
        break;
      case ILL_PRVOPC:
        error = "privileged opcode";
        break;
      case ILL_PRVREG:
        error = "privileged register";
        break;
      case ILL_BADSTK:
        error = "stack error";
        break;
      default:
        break;
      }
      ferr << "Internal error: Illegal instruction";
      if (error) {
        ferr << SP << '(' << error << ')';
      }
      ferr << " at address " << setWidth(2 + 2 * sizeof(void*))
           << info->si_addr << '.' << ENDL;
      break;
    case SIGSEGV:
      switch (info->si_code) {
      case SEGV_MAPERR:
        ferr << "Internal error: Segmentation fault due to " \
                "unmapped memory for memory reference "
             << setWidth(2 + 2 * sizeof(void*)) << info->si_addr
             << " at address " << setWidth(2 + 2 * sizeof(void*))
             << instructionAddress << '.' << ENDL;
        break;
      case SEGV_ACCERR:
        ferr << "Internal error: Segmentation fault due to " \
                "invalid permissions for memory reference "
             << setWidth(2 + 2 * sizeof(void*)) << info->si_addr
             << " at address " << setWidth(2 + 2 * sizeof(void*))
             << instructionAddress << '.' << ENDL;
        break;
      default:
        /*
        if ((info->si_addr == nullptr) && (instructionAddress == nullptr)) {
          // this would normally be dereference of nullptr - what should we write
        }
        */
        ferr << "Internal error: Segmentation fault for memory reference "
             << setWidth(2 + 2 * sizeof(void*)) << info->si_addr
             << " at address " << setWidth(2 + 2 * sizeof(void*))
             << instructionAddress << '.' << ENDL;
        break;
      }
      break;
    default:
      ferr << "Internal error: System exception " << signal << '.' << ENDL;
      exit(Application::EXIT_CODE_INTERNAL_ERROR); // TAG: need other function?
    }

    if (StackTrace::doesSupportStackTrace()) {
      ferr << FormattedStackTrace(StackTrace::getStack(1), StackTrace::FLAG_DEFAULT | StackTrace::FLAG_COMPACT |
        (ferr.isANSITerminal() ? StackTrace::FLAG_USE_COLORS : 0)) << ENDL;
    }
    
#if ((_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS) && \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__SPARC32))
    
    static const Literal REG_NAMES[8 + 8 + 8 + 8 + 8 + 32] = {
      Literal("g0"), Literal("g1"), Literal("g2"), Literal("g3"),
      Literal("g4"), Literal("g5"), Literal("g6"), Literal("g7"),
      Literal("o0"), Literal("o1"), Literal("o2"), Literal("o3"),
      Literal("o4"), Literal("o5"), Literal("sp"), Literal("o7"),
      Literal("l0"), Literal("l1"), Literal("l2"), Literal("l3"),
      Literal("l4"), Literal("l5"), Literal("l6"), Literal("l7"),
      Literal("i0"), Literal("i1"), Literal("i2"), Literal("i3"),
      Literal("i4"), Literal("i5"), Literal("fp"), Literal("i7"),
      Literal("y"), Literal("psr"), Literal("wim"), Literal("tbr"),
      Literal("pc"), Literal("npc"), Literal("fpsr"), Literal("cpsr"),
      Literal("f0"), Literal("f1"), Literal("f2"), Literal("f3"),
      Literal("f4"), Literal("f5"), Literal("f6"), Literal("f7"),
      Literal("f8"), Literal("f9"), Literal("f10"), Literal("f11"),
      Literal("f12"), Literal("f13"), Literal("f14"), Literal("f15"),
      Literal("f16"), Literal("f17"), Literal("f18"), Literal("f19"),
      Literal("f20"), Literal("f21"), Literal("f22"), Literal("f23"),
      Literal("f24"), Literal("f25"), Literal("f26"), Literal("f27"),
      Literal("f28"), Literal("f29"), Literal("f30"), Literal("f31")
    };
    
    ferr << EOL;
    ferr << RIGHT << setWidth(4) << REG_NAMES[36] << ':'
         << HEX << ZEROPAD << PREFIX << setWidth(10)
         << m->gregs[REG_PC] << indent(4)
         << RIGHT << setWidth(4) << REG_NAMES[37] << ':'
         << HEX << ZEROPAD << PREFIX << setWidth(10)
         << m->gregs[REG_nPC] << indent(4)
         << RIGHT << setWidth(4) << REG_NAMES[32] << ':'
         << HEX << ZEROPAD << PREFIX << setWidth(10)
         << m->gregs[REG_Y] << EOL
      
         << RIGHT << setWidth(4) << REG_NAMES[0] << ':'
         << HEX << ZEROPAD << PREFIX << setWidth(10)
         << m->gregs[0] << indent(4)
         << RIGHT << setWidth(4) << REG_NAMES[1] << ':'
         << HEX << ZEROPAD << PREFIX << setWidth(10)
         << m->gregs[REG_G1] << indent(4)
         << RIGHT << setWidth(4) << REG_NAMES[2] << ':'
         << HEX << ZEROPAD << PREFIX << setWidth(10)
         << m->gregs[REG_G2] << indent(4)
         << RIGHT << setWidth(4) << REG_NAMES[3] << ':'
         << HEX << ZEROPAD << PREFIX << setWidth(10)
         << m->gregs[REG_G3] << EOL
      
         << RIGHT << setWidth(4) << REG_NAMES[4] << ':'
         << HEX << ZEROPAD << PREFIX << setWidth(10)
         << m->gregs[REG_G4] << indent(4)
         << RIGHT << setWidth(4) << REG_NAMES[5] << ':'
         << HEX << ZEROPAD << PREFIX << setWidth(10)
         << m->gregs[REG_G5] << indent(4)
         << RIGHT << setWidth(4) << REG_NAMES[6] << ':'
         << HEX << ZEROPAD << PREFIX << setWidth(10)
         << m->gregs[REG_G6] << indent(4)
         << RIGHT << setWidth(4) << REG_NAMES[7] << ':'
         << HEX << ZEROPAD << PREFIX << setWidth(10)
         << m->gregs[REG_G7] << EOL
      
         << RIGHT << setWidth(4) << REG_NAMES[8] << ':'
         << HEX << ZEROPAD << PREFIX << setWidth(10)
         << m->gregs[REG_O0] << indent(4)
         << RIGHT << setWidth(4) << REG_NAMES[9] << ':'
         << HEX << ZEROPAD << PREFIX << setWidth(10)
         << m->gregs[REG_O1] << indent(4)
         << RIGHT << setWidth(4) << REG_NAMES[10] << ':'
         << HEX << ZEROPAD << PREFIX << setWidth(10)
         << m->gregs[REG_O2] << indent(4)
         << RIGHT << setWidth(4) << REG_NAMES[11] << ':'
         << HEX << ZEROPAD << PREFIX << setWidth(10)
         << m->gregs[REG_O3] << EOL
      
         << RIGHT << setWidth(4) << REG_NAMES[12] << ':'
         << HEX << ZEROPAD << PREFIX << setWidth(10)
         << m->gregs[REG_O4] << indent(4)
         << RIGHT << setWidth(4) << REG_NAMES[13] << ':'
         << HEX << ZEROPAD << PREFIX << setWidth(10)
         << m->gregs[REG_O5] << indent(4)
         << RIGHT << setWidth(4) << REG_NAMES[14] << ':'
         << HEX << ZEROPAD << PREFIX << setWidth(10)
         << m->gregs[REG_O6] << indent(4)
         << RIGHT << setWidth(4) << REG_NAMES[15] << ':'
         << HEX << ZEROPAD << PREFIX << setWidth(10)
         << m->gregs[REG_O7] << EOL
         << EOL;
    
    void* framePointer = nullptr;
    // TAG: check if valid pointer - what constraints
    if (framePointer) {
      const uint32* window = Cast::pointer<const uint32*>(framePointer);
      for (unsigned int i = 0; i < 16; ++i) {
        ferr << RIGHT << setWidth(4) << REG_NAMES[16+i] << ':'
             << HEX << ZEROPAD << PREFIX << setWidth(10)
             << window[i];
        if ((i % 4) == 3) {
          ferr << EOL;
        } else {
          ferr << indent(4);
        }
      }
      ferr << EOL;
    }
    
    ferr << RIGHT << setWidth(4) << "fpsr" << ':'
         << HEX << ZEROPAD << PREFIX << setWidth(10)
         << m->fpregs.fpu_fsr << indent(4)
         << EOL;
    
    for (unsigned int i = 0; i < 32; ++i) {
      ferr << RIGHT << setWidth(4) << REG_NAMES[40 + i] << ':'
           << HEX << ZEROPAD << PREFIX << setWidth(10)
           << m->fpregs.fpu_fr.fpu_regs[i];
      if ((i % 4) == 3) {
        ferr << EOL;
      } else {
        ferr << indent(4);
      }
    }
        
    ferr << EOL << "stack frames:" << EOL;

    const struct frame* f = (const struct frame*)(framePointer);
    while (f && ((((uint32)f) & 7) == 0)) {
      ferr << (void*)(f->fr_savfp) << SP << (void*)(f->fr_savpc) << SP << ENDL;
      f = f->fr_savfp;
      // TAG: must be ascending
    }
    // TAG: must end with 0
    f = Cast::pointer<const struct frame*>(framePointer);
    
    ferr << EOL << "stack content:" << EOL;
    
    ferr << FLUSH;
    
#endif // Solaris && SPARC
    exit(Application::EXIT_CODE_INTERNAL_ERROR); // TAG: need other function?
  }
#  endif
  
  static void signalHandler(int signal) noexcept
  {
    Profiler::pushSignal("signalHandler()");

    switch (signal) {
    case SIGHUP: // hangup
      if (Thread::getThread()->isMainThread()) {
        SystemLogger::write(SystemLogger::INFORMATION, "Hangup signal.");
        if (Application::application) {
          Application::application->hangup();
        }
      }
      break;
    case SIGQUIT: // quit signal from keyboard
      static bool firstTime = true;
      if (firstTime) {
        firstTime = false;
        if (FileDescriptor::getStandardError().isTerminal()) {
          ferr << "Aborted by user." << ENDL;
        }
      }
      if (Thread::getThread()->isMainThread()) {
        SystemLogger::write(SystemLogger::INFORMATION, "Quit signal.");
        if (Application::application) {
          Application::application->terminate();
        }
      }
      break;
    case SIGINT: // interrrupt from keyboard
      if (Thread::getThread()->isMainThread()) {
        SystemLogger::write(SystemLogger::ERROR, "Interrupt signal.");
        if (Application::application) {
          Application::application->terminate();
        }
      }
      break;
    case SIGSEGV:
      SystemLogger::write(SystemLogger::ERROR, "Segmentation fault.");
      if (FileDescriptor::getStandardError().isTerminal()) {
        ferr << "Error: Segmentation fault." << ENDL;
      }
      abort();
    case SIGILL:
      SystemLogger::write(SystemLogger::ERROR, "Invalid instruction.");
      if (FileDescriptor::getStandardError().isTerminal()) {
        ferr << "Error: Invalid instruction." << ENDL;
      }
      abort();
    case SIGFPE:
      SystemLogger::write(SystemLogger::ERROR, "Floating point exception.");
      if (FileDescriptor::getStandardError().isTerminal()) {
        ferr << "Error: Floating point exception." << ENDL;
      }
      abort();
    case SIGABRT: // abort
      if (Thread::getThread()->isMainThread()) {
        SystemLogger::write(SystemLogger::INFORMATION, "Abort signal.");
      }
      break;
    case SIGPIPE: // broken pipe
      break;
    case SIGTERM: // terminate
      if (Thread::getThread()->isMainThread()) {
        SystemLogger::write(SystemLogger::INFORMATION, "Terminate signal.");
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
        SystemLogger::write(SystemLogger::INFORMATION, "Power signal.");
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



Application* Application::getApplication() noexcept
{
  return application;
}

MutualExclusion& Application::getLock() noexcept
{
  return lock;
}

namespace internal
{
  
  int numberOfArguments = 0;
  const char** arguments = nullptr;
  const char** environment = nullptr;
  // Map<String, String> environment;
}

void Application::setArgumentsAndEnvironment(int _numberOfArguments, const char* _arguments[], const char* _environment[])
{
  if (!((_numberOfArguments > 0) && _arguments)) {
    _throw OutOfDomain("Invalid arguments.");
  }

  internal::numberOfArguments = _numberOfArguments;
  internal::arguments = _arguments;
  internal::environment = _environment;

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  size_t count = 0;
  size_t size = 0;
  __wasi_errno_t e = __wasi_environ_sizes_get(&count, &size);
  if ((e == 0) && (count > 0) && (size > 0)) {
    char* buffer = new char[size]; // leak for now
    char** environ = new char*[count]; // leak for now
    e = __wasi_environ_get(environ, buffer);
    if (e == 0) {
/*
      for (size_t i = 0; i < count; ++i) {
        printf("ENVIRONMENT VARIABLE: %s\n", environ[i]);    
      }
*/
      if (!internal::environment) {
        internal::environment = (const char**)environ;
      }
    }
  }
#endif
  // TAG: allow initialization for shared library also - move to static data
}

namespace {

  PreferredAtomicCounter applicationSingleton;
}

Application::Application(const String& _formalName)
  : formalName(_formalName),
    exitCode(EXIT_CODE_NORMAL),
    terminated(false),
    hangingup(false)
{
  if (applicationSingleton) {
    _throw SingletonException("Application has been initialized.", this);
  }
  ++applicationSingleton;

  // install signal handler
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!::SetConsoleCtrlHandler((PHANDLER_ROUTINE)ApplicationImpl::signalHandler, TRUE)) {
    _throw UnexpectedFailure("Unable to install signal handler.", this);
  }

  WNDCLASSEX windowClass;
  clear(windowClass);
  windowClass.cbSize = sizeof(windowClass);
  windowClass.lpfnWndProc = /*(WNDPROC)*/ApplicationImpl::messageHandler;
  windowClass.lpszClassName = L"https://dev.azure.com/renefonseca/base";
  ATOM result = ::RegisterClassEx(&windowClass);
  BASSERT(result != 0);

  OSVERSIONINFO versionInfo;
  versionInfo.dwOSVersionInfoSize = sizeof(versionInfo);
  ::GetVersionEx(&versionInfo); // never fails
  bool isWindows2000OrLater =
    (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) &&
    (versionInfo.dwMajorVersion >= 5);

  HWND messageWindow = ::CreateWindowEx(
    0, // no extended window style
    L"https://dev.azure.com/renefonseca/base", // registered class name
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
#  if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI) || \
      (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__EMCC)
#  elif (defined(_COM_AZURE_DEV__BASE__HAVE_SIGACTION))
  static const int SIGNALS[] = {
    SIGHUP, SIGPIPE, SIGTERM, SIGCHLD, SIGQUIT, SIGINT, SIGABRT, SIGSEGV,
    SIGILL, SIGFPE, SIGBUS
  };
  
  struct sigaction action;
  clear(action);
  action.sa_flags = SA_SIGINFO;
  action.sa_sigaction = ApplicationImpl::actionHandler;
  for (unsigned int i = 0; i < getArraySize(SIGNALS); ++i) {
    if (sigaction(SIGNALS[i], &action, 0) != 0) {
      _throw UnexpectedFailure("Unable to register signal handler.", this);
    }
  }
#  elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__IRIX65)
    if (!((bsd_signal(SIGHUP, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (bsd_signal(SIGPIPE, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (bsd_signal(SIGTERM, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (bsd_signal(SIGCHLD, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (bsd_signal(SIGQUIT, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (bsd_signal(SIGINT, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (bsd_signal(SIGSEGV, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (bsd_signal(SIGILL, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (bsd_signal(SIGFPE, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (bsd_signal(SIGABRT, ApplicationImpl::signalHandler) != SIG_ERR))) {
      _throw UnexpectedFailure("Unable to register signal handler.", this);
    }
#  else
    if (!((signal(SIGHUP, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (signal(SIGPIPE, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (signal(SIGTERM, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (signal(SIGCHLD, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (signal(SIGQUIT, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (signal(SIGINT, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (signal(SIGSEGV, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (signal(SIGILL, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (signal(SIGFPE, ApplicationImpl::signalHandler) != SIG_ERR) &&
          (signal(SIGABRT, ApplicationImpl::signalHandler) != SIG_ERR))) {
      _throw UnexpectedFailure("Unable to register signal handler.", this);
    }
#  endif
#endif

  // install exception handlers
  std::set_terminate(ApplicationImpl::terminationExceptionHandler);
  std::set_unexpected(ApplicationImpl::unexpectedExceptionHandler);

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  {
    // what if path starts with "\\?\"
    PrimitiveStackArray<wchar> buffer(1024);
    while (buffer.size() < (64 * 1024)) {
      DWORD length = GetModuleFileNameW((HMODULE)NULL, buffer, buffer.size());
      if (length == 0) {
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
          buffer.resize(buffer.size() * 2);
          continue;
        }
        break;
      }
      path = String(buffer, length);
      break;
    }
  }
#else
  if (internal::arguments) {
    path = internal::arguments[0];
  }
#endif
  
  if (internal::arguments) {
    for (int i = 1; i < internal::numberOfArguments; ++i) {
      this->arguments.append(internal::arguments[i]);
    }
  }
  
  if (internal::environment) {
    // TAG: fallback to char** _environ;
    for (; *internal::environment != nullptr; ++internal::environment) {
      const String temp(*internal::environment);
      const MemoryDiff index = temp.indexOf('=');
      if (index != -1) { // ignore the environment string if it doesn't contain '='
        this->environment.add(temp.substring(0, index), temp.substring(index + 1));
      }
    }
  }

  internal::numberOfArguments = 0;
  internal::arguments = nullptr;
  internal::environment = nullptr;

  application = this;
}

int Application::exceptionHandler(const Exception& e) noexcept
{
  auto& ferr = StackTrace::getErrorStream();
  ferr << e << ENDL;

  if (auto tls = Thread::getLocalContext()) {
    if (!tls->stackTrace.isEmpty()) {
      ferr << FormattedStackTrace(tls->stackTrace,
        StackTrace::FLAG_DEFAULT |
        (ferr.isANSITerminal() ? StackTrace::FLAG_USE_COLORS : 0)
      ) << FLUSH;
    }
  }

  setExitCode(Application::EXIT_CODE_ERROR);
  return Application::EXIT_CODE_ERROR;
}

int Application::exceptionHandler() noexcept
{
  setExitCode(Application::EXIT_CODE_ERROR);
  // TAG: if UI app - show dialog
  // TAG: show thread info
  auto& ferr = StackTrace::getErrorStream();

  try {
    throw;
  } catch (Exception& e) {
    BASSERT(!"Unpexpected Exception.");
    exceptionHandler(e);
    return Application::EXIT_CODE_ERROR;
  } catch (std::exception& e) {
    if (const String w = e.what()) {
      ferr << "Internal error: Uncaught exception '" << Exception::getStdExceptionName(e)
           << "' was raised with message '" << w << "'." << ENDL;
    } else {
      ferr << "Internal error: Uncaught exception '" << Exception::getStdExceptionName(e)
           << "' was raised." << ENDL;
    }
  } catch (const char*) {
    // TAG: we need safe virtual memory read to show string - we cant assume we can read anything
    ferr << "Internal error: String literal exception." << ENDL;
  } catch (const wchar*) {
    // TAG: we need safe virtual memory read to show string - we cant assume we can read anything
    ferr << "Internal error: String literal exception." << ENDL;
  } catch (...) {
    ferr << "Internal error: Unspecified exception." << ENDL;
  }

  if (auto tls = Thread::getLocalContext()) { // this is not relevant since we didnt get here by throwing base::Exception
    if (!tls->stackTrace.isEmpty()) {
      ferr << FormattedStackTrace(tls->stackTrace,
        StackTrace::FLAG_DEFAULT |
        (ferr.isANSITerminal() ? StackTrace::FLAG_USE_COLORS : 0)
      ) << FLUSH;
    }
  }

  return Application::EXIT_CODE_ERROR;
}

void Application::error(const String& text, int exitCode)
{
  // use color?
  ferr << "Error: " << text << ENDL;
  if (exitCode != EXIT_CODE_INVALID) {
    setExitCode(exitCode);
  }
}

void Application::hangup() noexcept
{
  MutualExclusion::Sync _guard(lock);
  hangingup = true;
}

void Application::terminate() noexcept
{
  terminated = true;
  onTermination();
}

bool Application::isHangingup() noexcept
{
  bool result = false;
  MutualExclusion::Sync _guard(lock);
  result = hangingup;
  hangingup = false;
  return result;
}

void Application::onTermination() noexcept
{
  exit(exitCode);
}

namespace internal {
  
  void terminationExceptionHandler() noexcept;

  void unexpectedExceptionHandler() noexcept;
};

Application::~Application() noexcept {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
//   if (::DestroyWindow(windowHandle) == 0) {
//     // failed but we do not care
//   }
//   if (::UnregisterClass("https://dev.azure.com/renefonseca/base", HINSTANCE(0)) == 0) { // TAG: should be done automatically
//     // failed but we do not care
//   }
#endif // flavor
  
  std::set_terminate(internal::terminationExceptionHandler);
  std::set_unexpected(internal::unexpectedExceptionHandler);
  application = nullptr;

  --applicationSingleton;
  BASSERT(!applicationSingleton);
}

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__EMCC)
namespace {

  class ForkMainThread : public Thread {
  public:

    void run()
    {
      Profiler::Task profile("Application::main()");
      Application::getApplication()->main();
      // emscripten_cancel_main_loop(); // skipped on exception
      emscripten_force_exit(0);
    }
  };

  ForkMainThread forkMainThread;

  void mainHandler()
  {
    if (forkMainThread.getState() > Thread::ALIVE) {
      emscripten_cancel_main_loop();
      // emscripten_set_main_loop(mainHandler, 0, 0);
      // emscripten_cancel_main_loop();
      emscripten_force_exit(0);
      // exit(0);
    }
  }
}
#endif

int Application::start(Application* application)
{
  if (!application) {
    return Application::EXIT_CODE_INITIALIZATION;
  }
  try {
#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__EMCC)
    Profiler::Task profile("Application::main()");
    application->main();
#else
    // main thread must NOT be blocked
    forkMainThread.start(); // do NOT wait for thread to complete
    // https://emscripten.org/docs/porting/asyncify.html
    emscripten_set_main_loop(mainHandler, 5, 1);
    // emscripten_set_main_loop(mainHandler, 0, 0);
    // TAG: need proper way to return so we clean up state
#endif
    return application->getExitCode();
  } catch (Exception& e) {
    return application->exceptionHandler(e);
  } catch (...) {
    return application->exceptionHandler();
  }
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
