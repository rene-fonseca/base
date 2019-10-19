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

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#else // unix
#  include <csignal>
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

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

  static bool useBreakpoint = true; // TAG: look up application option
  if (useBreakpoint) {
    Debug::breakpoint();
  }

  static bool writeToStdout = false; // TAG: look up application option
  if (writeToStdout && Runtime::isGlobalStateInGoodCondition()) {
    // TAG: we should suppress recursive assert
    try {
      fout << buffer << ENDL;
    } catch (...) { // ignore
    }
  }

  return false;
}

bool Assert::handle(const char* expression, const char* filename, const char* line)
{
  // do not use high level features for string conversion to avoid recursive asserts
  if (!expression) {
    expression = "<UNKNOWN>";
  }
  
  char buffer[1024];
  
  char* dest = buffer;
  const char* end = buffer + (sizeof(buffer) - 3 - 1); // keep room for terminator and ellipsis
  dest = concat(dest, end, "Assert for expression (");
  dest = concat(dest, end, expression);
  dest = concat(dest, end, ")");
  if (filename && line) {
    dest = concat(dest, end, " failed at ");
    dest = concat(dest, end, filename);
    dest = concat(dest, end, ":");
    dest = concat(dest, end, line);
  }
  if (dest == end) { // indicate missing info with ellipsis
    dest = concat(dest, end + 3, "...");
  }
  *dest++ = '\0'; // always room for terminator

  return handle(buffer);
}

void Debug::breakpoint() noexcept
{
  static bool useBreakpoint = true;
  if (useBreakpoint) {
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
    DebugBreak();
#else
    std::raise(SIGINT);
#endif
  }
}

unsigned int Debug::allocateUniqueId() noexcept
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
