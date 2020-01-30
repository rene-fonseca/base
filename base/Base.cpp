/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Base.h>
#include <base/concurrency/AtomicCounter.h>
#include <base/string/FormatOutputStream.h>
#include <base/string/ANSIEscapeSequence.h>
#include <base/StackFrame.h>
#include <base/io/FileDescriptor.h>
#include <base/UnitTest.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#else // unix
#  include <csignal>
#  include <sys/types.h>
#  if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS)
#    include <unistd.h>
#  elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREERTOS)
#  elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__ZEPHYR)
#  elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
#  elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__IOS)
#  else
#    include <sys/ptrace.h>
#  endif
#  if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
#    include <sys/sysctl.h>
#    include <unistd.h>
#  endif
#endif // flavor
#include <stdio.h>
#include <stdarg.h>
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include<io.h>
#else
#  include <unistd.h>
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

void GlobalPrint::printf(const char* text, ...) noexcept
{
#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
  puts(text);
  return;
#endif

#if (_COM_AZURE_DEV__BASE__FLAVOR != _COM_AZURE_DEV__BASE__WIN32)
  char buffer[1024+1];
  va_list args;
  va_start(args, text);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
  int result = vsnprintf(buffer, getArraySize(buffer), text, args);
#pragma GCC diagnostic pop
  va_end (args);
  if (INLINE_ASSERT(result >= 0)) {
    return;
  }
  if (result < getArraySize(buffer)) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    _write(1, buffer, result);
#else
    write(1, buffer, result);
#endif
  }
#endif
}

GlobalPrint::GlobalPrint(const char* _text) noexcept
  : text(Debug::getRelativePath(_text))
{
  printf("GLOBAL CONSTRUCT %s\n", text);
}

GlobalPrint::~GlobalPrint() noexcept
{
  printf("GLOBAL DESTRUCT %s\n", text);
}

_COM_AZURE_DEV__BASE__GLOBAL_PRINT();

namespace {

  PreferredAtomicCounter assertCounter;
  PreferredAtomicCounter uniqueIdCounter;

  inline char* concat(char* dest, const char* end, const char* src) noexcept
  {
    while (*src && (dest != end)) {
      *dest++ = *src++;
    }
    return dest;
  }

  char* writeColor(char* dest, const char* end, ANSIEscapeSequence::Color color) noexcept
  {
    char buffer[sizeof("\033[3xm") + 1];
    char* d = buffer;
    *d++ = '\033';
    *d++ = '[';
    *d++ = '3';
    *d++ = color + '0';
    *d++ = 'm';
    *d = 0;
    return concat(dest, end, buffer);
  }
}

namespace {

  bool writeAssertsToErrorStream = true;
  bool writeStackTraceForAsserts = true;
}

void Assert::setWriteAssertsToErrorStream(bool useErrorStream) noexcept
{
  writeAssertsToErrorStream = useErrorStream;
}

void Assert::setWriteStackTraceForAsserts(bool writeStackTrace) noexcept
{
  writeStackTraceForAsserts = writeStackTrace;
}

bool Assert::handle(const char* message)
{
  // do not use high level features for string conversion to avoid recursive asserts

  if (!message) {
    message = "Assertion handler called without message";
  }
  MemorySize count = ++assertCounter;;

  char digitBuffer[32]; // enough for digits - reverse order
  char* digit = digitBuffer;
  if (count == 0) {
    *digit++ = '0';
  }
  while (count != 0) {
    *digit++ = static_cast<char>((count % 10) + '0');
    count /= 10;
  }

  // convert to string
  char buffer[1024];
  char* dest = buffer;
  const char* end = buffer + (sizeof(buffer) - 3 - 1); // keep room for terminator and ellipsis
  dest = concat(dest, end, message);
  dest = concat(dest, end, " [");
  while ((digit != digitBuffer) && (dest != end)) {
    *dest++ = *--digit;
  }
  dest = concat(dest, end, "]");
  if (dest == end) { // indicate missing info with ellipsis
    dest = concat(dest, end + 3, "...");
  }
  *dest++ = '\0'; // always room for terminator
  
  Trace::message(buffer);

  if (writeAssertsToErrorStream && Runtime::isGlobalStateInGoodCondition()) {
    // TAG: we should suppress recursive assert
    auto& ferr = StackFrame::getErrorStream();
    try {
      ferr << static_cast<const char*>(buffer) << ENDL;
    } catch (...) { // ignore
    }
  }

  if (writeStackTraceForAsserts) {
    StackFrame::dump();
  }

  Debug::breakpoint();
  return false;
}

bool Assert::handle(const char* expression, const char* filename, const char* line)
{
  // do not use high level features for string conversion to avoid recursive asserts
  if (!expression) {
    expression = "<UNKNOWN>";
  }

  filename = Debug::getRelativePath(filename);

  MemorySize count = ++assertCounter;;
  char digitBuffer[32]; // enough for digits - reverse order
  char* digit = digitBuffer;
  if (count == 0) {
    *digit++ = '0';
  }
  while (count != 0) {
    *digit++ = static_cast<char>((count % 10) + '0');
    count /= 10;
  }

  char buffer[1024];
  
  for (int i = 0; i < (writeAssertsToErrorStream ? 2 : 1); ++i) {
    const bool useANSI = (i > 0) && FileDescriptor::getStandardError().isANSITerminal();
    char* dest = buffer;
    const char* end = buffer + (sizeof(buffer) - 3 - 1); // keep room for terminator and ellipsis
    if (useANSI) {
      dest = writeColor(dest, end, ANSIEscapeSequence::RED);
    }
    dest = concat(dest, end, "Assert for expression (");
    if (useANSI) {
      dest = writeColor(dest, end, ANSIEscapeSequence::BLUE);
      dest = concat(dest, end, "\033[1m"); // bold code
    }
    dest = concat(dest, end, expression);
    if (useANSI) {
      dest = writeColor(dest, end, ANSIEscapeSequence::RED);
      dest = concat(dest, end, "\033[22m"); // unbold code
    }
    dest = concat(dest, end, ")");
    if (filename && *filename && line && *line) {
      dest = concat(dest, end, " failed at ");
      if (useANSI) {
        dest = writeColor(dest, end, ANSIEscapeSequence::YELLOW);
        dest = concat(dest, end, "\033[1m"); // bold code
      }
      dest = concat(dest, end, filename);
      dest = concat(dest, end, ":");
      dest = concat(dest, end, line);
      if (useANSI) {
        dest = concat(dest, end, "\033[22m"); // unbold code
        dest = writeColor(dest, end, ANSIEscapeSequence::RED);
      }
    }
    
    dest = concat(dest, end, " [");
    char* _digit = digit;
    while ((_digit != digitBuffer) && (dest != end)) {
      *dest++ = *--_digit;
    }
    dest = concat(dest, end, "]");

    if (useANSI) {
      dest = concat(dest, end, "\033[0m"); // normal code
    }

    if (dest == end) { // indicate missing info with ellipsis
      dest = concat(dest, end + 3, "...");
    }
    *dest++ = '\0'; // always room for terminator

    if (i == 0) {
      Trace::message(buffer);
      continue;
    }

    if (writeAssertsToErrorStream && Runtime::isGlobalStateInGoodCondition()) {
      // TAG: we should suppress recursive assert
      auto& ferr = StackFrame::getErrorStream();
      try {
        ferr << static_cast<const char*>(buffer) << ENDL;
      } catch (...) { // ignore
      }
    }
    break;
  }

  if (writeStackTraceForAsserts) {
    StackFrame::dump();
  }
  Debug::breakpoint();
  return false;
}

void* Runtime::getNullPointer() noexcept
{
  static void* value = nullptr;
  return value;
}

namespace {

  bool useBreakpoint = true;
}

void Debug::setUseBreakpoint(bool _useBreakpoint) noexcept
{
  useBreakpoint = _useBreakpoint;
}

namespace {

  String getRootPathImpl() noexcept
  {
    String result;
    const char* path = _COM_AZURE_DEV__BASE__SOURCE_FILE;
    const char* end = path;
    for (; *end; ++end) {
    }
    unsigned int count = 0;
    while (end != path) {
      --end;
      if ((*end == '/') || (*end == '\\')) {
        ++count;
        if (count > 1) {
          ++end;
          break;
        }
      }
    }
    return String(path, end - path);
  }
}

const char* Debug::getRootPath() noexcept
{
  static String root = getRootPathImpl();
  return root.native();
}

const char* Debug::getRelativePath(const char* path) noexcept
{
  if (!path) {
    return nullptr;
  }
  auto isSeparator = [](char ch) {return (ch == '/') || (ch == '\\');};
  
  const char* root = getRootPath();
  while (*path && *root) {
    const char* end = root;
    for (; *end && !isSeparator(*end); ++end) {
    }
    const char* src = path;
    for (; *root && (root != end) && (*src == *root); ++src, ++root) {
    }
    if (root != end) {
      return path;
    }
    path = src;
    if (*path) {
      ++path;
    }
    if (*root) {
      ++root;
    }
  }
  return path;
}

bool Debug::isDebuggerAttached() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return IsDebuggerPresent();
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
  return ptrace(PTRACE_TRACEME, 0, NULL, 0) == -1; // detect debugger - need a better way
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS)
  // Initialize the flags so that, if sysctl fails for some bizarre
  // reason, we get a predictable result.
  struct kinfo_proc info;
  info.kp_proc.p_flag = 0;

  // Initialize mib, which tells sysctl the info we want, in this case
  // we're looking for information about a specific process ID.
  int mib[4] = {0};
  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC;
  mib[2] = KERN_PROC_PID;
  mib[3] = getpid();

  size_t size = size = sizeof(info);
  int status = sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &size, NULL, 0);
  if (status != 0) {
    return false;
  }
  return ((info.kp_proc.p_flag & P_TRACED) != 0);
#else
  return false;
#endif
}

void Debug::breakpoint() noexcept
{
  if (useBreakpoint) {
    if (!isDebuggerAttached()) {
      return;
    }
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    DebugBreak();
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WASI)
#else
    // TAG: skip for qemu
    std::raise(SIGINT);
#endif
  }
}

MemorySize Debug::allocateUniqueId() noexcept
{
  return ++uniqueIdCounter;
}

#if defined(_COM_AZURE_DEV__BASE__DEBUG)
void _COM_AZURE_DEV__BASE__BUILD_DEBUG() noexcept
{
  Debug::breakpoint(); // do not call
}
#else
void _COM_AZURE_DEV__BASE__BUILD_RELEASE() noexcept
{
  Debug::breakpoint(); // do not call
}
#endif

void _COM_AZURE_DEV__BASE__CONCATENATE(_COM_AZURE_DEV__BASE__VERSION_, _COM_AZURE_DEV__BASE__MAJOR_VERSION)() noexcept
{
  Debug::breakpoint(); // do not call
}

// Generate symbols to cause linker failure on mismatching shared/static builds
#if defined(_COM_AZURE_DEV__BASE__SHARED_LIBRARY_BUILD)
void _COM_AZURE_DEV__BASE__BUILD_SHARED() noexcept
{
  Debug::breakpoint(); // do not call
}
#else
void _COM_AZURE_DEV__BASE__BUILD_STATIC() noexcept
{
  Debug::breakpoint(); // do not call
}
#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
