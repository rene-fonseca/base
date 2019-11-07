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
#include <base/concurrency/Thread.h>
#include <base/string/StringOutputStream.h>
#include <base/UnexpectedFailure.h>
#include <base/string/WideString.h>
#include <stdlib.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#  undef ERROR // protect against the evil programmers
#else // unix
#  if (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__S390)
#    define __thread // TAG: temp. fix for s390-ibm-linux-gnu
#  endif
#  include <sys/signal.h> // defines SIG_ERR on IRIX65
#  include <signal.h>

#  if (defined(_COM_AZURE_DEV__BASE__HAVE_SIGACTION))
#    include <ucontext.h> // TAG: is this required
#    if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS)
#      include <sys/frame.h>
#    endif
#  endif
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

  void defaultExceptionHandler(Exception* exception) noexcept
  {
    if (exception) {
      auto tls = Thread::getLocalContext();
      if (tls) {
        tls->stackTrace = StackFrame::getStack(64); // TAG: we can use static memory, make sure we do not reallocate
        // TAG: only if dumping and if exception isnt silenced
        if (Exception::getDumpExceptions()) {
          ferr << "EXCEPTION CONSTRUCTED BY: " << tls->stackTrace << ENDL;
        }
      }
    }
  }

class ApplicationImpl {
public:

  static void terminationExceptionHandler() noexcept
  {
    static bool firstTime = true;    
    StringOutputStream stream;
    const Type exceptionType = Exception::getExceptionType();
    if (firstTime || exceptionType.isInitialized()) {
      firstTime = false;

      auto tls = Thread::getLocalContext();
      if (tls) {
        if (!tls->stackTrace.isEmpty()) {
          ferr << "LAST EXCEPTION " << tls->stackTrace << ENDL;
        }
      }

      try {
        throw;
      } catch (Exception& e) {
        stream << "Internal error: uncaught exception '"
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
      } catch (...) {
        stream << "Internal error: uncaught and unsupported exception '"
               << TypeInfo::getTypename(exceptionType)
               << "' was raised." << FLUSH;
      }
    } else {
      stream << "Internal error: explicit termination." << FLUSH;
    }
    ferr << stream.getString() << ENDL; // TAG: use appropriate error stream
#if defined(_COM_AZURE_DEV__BASE__ANY_DEBUG)
    // TAG: need runtime debug mode support (e.g. bool Trace::debug or with level support)
    Trace::message(stream.getString().getElements());
#endif
    exit(Application::EXIT_CODE_INTERNAL_ERROR); // TAG: is abort() better
  }
  
  static void unexpectedExceptionHandler() noexcept
  {
    StringOutputStream stream;
    const Type exceptionType = Exception::getExceptionType();
    if (exceptionType.isInitialized()) {
      try {
        throw;
      } catch (Exception& e) {
        stream << "Internal error: exception '"
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
            stream << ' ' << '(' << OperatingSystem::getErrorMessage(error) << ')'
                   << " and";
          }
        }
        stream << " in violation with exception specification." << FLUSH;
      } catch (...) {
        stream << "Internal error: unsupported exception '"
               << TypeInfo::getTypename(exceptionType)
               << "' was raised in violation with exception specification."
               << FLUSH;
      }
    } else {
      stream << "Internal error: explicit invocation of unexpected." << FLUSH;
    }
#if defined(_COM_AZURE_DEV__BASE__ANY_DEBUG)
    Trace::message(stream.getString().getElements());
#endif
    // TAG: either use SystemLogger or ferr but not both
    SystemLogger::write(SystemLogger::ERROR, stream.getString());
    // TAG: only if ferr is valid
    ferr << stream.getString() << ENDL;
    exit(Application::EXIT_CODE_INTERNAL_ERROR); // TAG: is abort() or terminate() better
  }

  /* The application signal handler. */
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#if 0 // disabled
  // TAG: we should destroy window in destructor
  StringOutputStream stream;
  stream << "messageHandler: message=" << message << " primary="
         << primaryParameter << " second=" << secondaryParameter << FLUSH;
  Trace::message(stream.getString().getElements());
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

  static LRESULT CALLBACK messageHandler(
    HWND window,
    UINT message,
    WPARAM primaryParameter,
    LPARAM secondaryParameter) throw() {
    // TAG: we should destroy window in destructor
    //StringOutputStream stream;
    //stream << "messageHandler: message=" << message << " primary="
    //       << primaryParameter << " second=" << secondaryParameter << FLUSH;
    //Trace::message(stream.getString().getElements());
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

#  if (defined(_COM_AZURE_DEV__BASE__HAVE_SIGACTION))
  static void actionHandler(
    int signal, siginfo_t* info, void* opaque) throw() {
    const char* error = nullptr;

    const ucontext_t* context = Cast::pointer<const ucontext_t*>(opaque);
    const mcontext_t* m = &context->uc_mcontext;

    const void* instructionAddress = nullptr;
#if ((_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS) && \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__SPARC))
    instructionAddress = (void*)(m->gregs[REG_PC]);
#endif
    
    switch (signal) {
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
      ferr << "Internal error: floating-point exception";
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
      ferr << "Internal error: bus error ";
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
      ferr << "Internal error: illegal instruction";
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
    
#if ((_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS) && \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__SPARC))
    
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
    
#if (_COM_AZURE_DEV__BASE__ARCH_MINOR == _COM_AZURE_DEV__BASE__SPARCV9)
#endif // SPARCV9
    
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
  
  static void signalHandler(int signal) throw() {
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
      if (Thread::getThread()->isMainThread()) {
        SystemLogger::write(SystemLogger::INFORMATION, "Quit signal.");
        if (Application::application) {
          Application::application->terminate();
        }
      }
      break;
    case SIGINT: // interrrupt from keyboard
      if (Thread::getThread()->isMainThread()) {
        SystemLogger::write(SystemLogger::INFORMATION, "Interrupt signal.");
        if (Application::application) {
          Application::application->terminate();
        }
      }
      break;
    case SIGSEGV:
      SystemLogger::write(SystemLogger::INFORMATION, "Segmentation fault.");
      throw MemoryException("Invalid memory access"); // TAG: remove
      abort();
    case SIGILL:
      SystemLogger::write(SystemLogger::INFORMATION, "Invalid instruction.");
      throw Exception("Invalid instruction");
    case SIGFPE:
      SystemLogger::write(
        SystemLogger::INFORMATION,
        "Floating point exception."
      );
      throw Exception("Floating point exception");
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



void Application::initialize() throw() {
  static unsigned int singleton = 0;
  if (singleton != 0) {
    throw SingletonException("Application has been initialized", this);
  }
  ++singleton;

  // install signal handler
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  if (!::SetConsoleCtrlHandler((PHANDLER_ROUTINE)ApplicationImpl::signalHandler, TRUE)) {
    throw UnexpectedFailure("Unable to install signal handler", this);
  }

  WNDCLASSEX windowClass;
  clear(windowClass);
  windowClass.cbSize = sizeof(windowClass);
  windowClass.lpfnWndProc = /*(WNDPROC)*/ApplicationImpl::messageHandler;
  windowClass.lpszClassName = L"https://dev.azure.com/renefonseca/base";
  ATOM result = ::RegisterClassEx(&windowClass);
  ASSERT(result != 0);

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
#  if (defined(_COM_AZURE_DEV__BASE__HAVE_SIGACTION))
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
      throw UnexpectedFailure("Unable to register signal handler", this);
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
      throw UnexpectedFailure("Unable to register signal handler", this);
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
      throw UnexpectedFailure("Unable to register signal handler", this);
    }
#  endif
#endif

  // install exception handlers
  std::set_terminate(ApplicationImpl::terminationExceptionHandler);
  std::set_unexpected(ApplicationImpl::unexpectedExceptionHandler);
}

Application* Application::getApplication() noexcept
{
  return application;
}

Application::Application(const String& _formalName) throw(SingletonException)
  : formalName(_formalName),
    exitCode(EXIT_CODE_NORMAL),
    terminated(false),
    hangingup(false)
{
  initialize();
  application = this;
}

Application::Application(
  const String& _formalName,
  int numberOfArguments,
  const char* arguments[],
  const char* environment[]) throw(SingletonException, OutOfDomain)
  : formalName(_formalName),
    exitCode(EXIT_CODE_NORMAL),
    terminated(false),
    hangingup(false) {
  initialize();
  
  if (!((numberOfArguments > 0) && arguments)) {
    throw OutOfDomain(this);
  }
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  wchar buffer[MAX_PATH + 1]; // what if path starts with "\\?\"
  DWORD length = ::GetModuleFileName(0, buffer, MAX_PATH /*lengthOf(buffer)*/);
  ASSERT(length > 0);
  if (length > 0) { // TAG: need inline assert
    path = toUTF8(WideString(buffer, length));
  }
#else
  path = arguments[0]; // TAG: fixme
#endif
  for (int i = 1; i < numberOfArguments; ++i) {
    this->arguments.append(arguments[i]);
  }

  if (environment) {
    for (; *environment != 0; ++environment) {
      const String temp(*environment);
      const int index = temp.indexOf('=');
      if (index != -1) { // ignore the environment string if it doesn't contain '='
        this->environment.add(temp.substring(0, index - 1), temp.substring(index + 1));
      }
    }
  }

  application = this;
}

int Application::exceptionHandler(const Exception& e) throw() {
  ferr << e << ENDL;
  setExitCode(Application::EXIT_CODE_ERROR);
  return Application::EXIT_CODE_ERROR;
}

int Application::exceptionHandler() throw() {
  ferr << "Internal error: unspecified exception." << ENDL;
  setExitCode(Application::EXIT_CODE_ERROR);
  return Application::EXIT_CODE_ERROR;
}

void Application::hangup() throw() {
  MutualExclusion::Sync _guard(lock);
  hangingup = true;
}

void Application::terminate() throw() {
  terminated = true;
  onTermination();
}

bool Application::isHangingup() throw() {
  bool result = false;
  MutualExclusion::Sync _guard(lock);
  result = hangingup;
  hangingup = false;
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
  application = 0;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
