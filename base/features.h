/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/platforms/symbols.h>
#include <base/configuration.h>
#include <base/platforms/arch/architecture.h>
#include <base/platforms/compiler/compiler.h>
#include <base/platforms/os/os.h>

#define _COM_AZURE_DEV__BASE__MAJOR_VERSION 0
#define _COM_AZURE_DEV__BASE__MINOR_VERSION 9
#define _COM_AZURE_DEV__BASE__MICRO_VERSION 1
#define _COM_AZURE_DEV__BASE__VERSION "0.9.1"

/** The ID of this module. Use for consumer when registering dependencies. */
#define _COM_AZURE_DEV__BASE__THIS_MODULE "com.azure.dev:renefonseca_base"

/** MODULE_REGISTER("your.module.id", _COM_AZURE_DEV__BASE__MODULE_INFO). See <base/Module.h>. */
#define _COM_AZURE_DEV__BASE__MODULE_INFO \
  {"PREFIX=com.azure.dev", \
   "NAME=base", \
   "VERSION=" _COM_AZURE_DEV__BASE__VERSION, \
   "URL=https://dev.azure.com/renefonseca/base/" \
   "LICENSE=GPL-2.0"}

#define _COM_AZURE_DEV__BASE__REQUIRE(major, minor, micro) \
  ((major <= _COM_AZURE_DEV__BASE__MAJOR_VERSION) && \
  (minor <= _COM_AZURE_DEV__BASE__MINOR_VERSION) && \
  (micro <= _COM_AZURE_DEV__BASE__MICRO_VERSION))

#if (defined(_COM_AZURE_DEV__BASE__NAMESPACE))
#  define _COM_AZURE_DEV__BASE__ENTER_NAMESPACE namespace base {
#  define _COM_AZURE_DEV__BASE__LEAVE_NAMESPACE }

// _COM_AZURE_DEV__BASE__DEBUG influences state!

#if 0 // could be dangerous
#if defined(_COM_AZURE_DEV__BASE__DEBUG)
#  define DEBUG // enable ASSERT and ASSERTION
#endif
#endif

#if defined(DEBUG) || defined(_DEBUG) || defined(_COM_AZURE_DEV__BASE__DEBUG)
#  define _COM_AZURE_DEV__BASE__ANY_DEBUG
#endif

#if !defined(_COM_AZURE_DEV__BASE__TESTS) // for now we include tests in release builds
#  define _COM_AZURE_DEV__BASE__TESTS
#endif

#if defined(NDEBUG) && defined(_COM_AZURE_DEV__BASE__ANY_DEBUG)
#  error NDEBUG and _COM_AZURE_DEV__BASE__ANY_DEBUG are both defined
#endif

/**
  This is the root namespace of <strong>The Base Framework</strong>. All the
  symbols defined by the framework are contained within this particular
  namespace. You should not add your own symbols to this namespace. Ignoring
  this rule will put your application at risk of clashing with a future release
  of this framework. You should therefore consider this namespace reserved
  solely for this project just like the 'std' namespace is allocated for the
  <strong>Standard C++ Library</strong> (and <strong>C++</strong>
  language).

  @short The root namespace of the framework
*/

namespace base {

  /** Assert helpers. */
  class _COM_AZURE_DEV__BASE__API Assert {
  public:

    /** Print assert to error stream. */
    static void setWriteAssertsToErrorStream(bool useErrorStream) noexcept;
    
    /** Print stack trace on assert. */
    static void setWriteStackTraceForAsserts(bool writeStackTrace) noexcept;

    /** Handle assertion. */
    static bool handle(const char* message);

    /** Handle assertion. */
    static bool handle(const char* expression, const char* filename, const char* line);
  };

  class _COM_AZURE_DEV__BASE__API DummySymbol {
  public:

    inline DummySymbol(void*)
    {
    }
  };
}

namespace com {
  namespace azure {
    namespace dev {
      namespace base {
        using namespace ::base;
      }
    }
  }
}

#else
#  define _COM_AZURE_DEV__BASE__ENTER_NAMESPACE
#  define _COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
#endif

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  define _COM_AZURE_DEV__BASE__DUMMY_SYMBOL
#elif (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_LLVM)
#  define _COM_AZURE_DEV__BASE__DUMMY_SYMBOL \
  namespace { int _DUMMY_SYMBOL = 0; base::DummySymbol _dummySymbol(&_DUMMY_SYMBOL); }
#else
#  define _COM_AZURE_DEV__BASE__DUMMY_SYMBOL extern int _DUMMY_SYMBOL;
#endif

#define _COM_AZURE_DEV__BASE__ID_PREFIX L"https://dev.azure.com/renefonseca/base"

#define _COM_AZURE_DEV__BASE__STRINGIFY(value) #value
#define _COM_AZURE_DEV__BASE__INDIRECT_STRINGIFY(value) _COM_AZURE_DEV__BASE__STRINGIFY(value)

#if (!defined(STRINGIFY))
#  define STRINGIFY(value) #value
#endif

#define _COM_AZURE_DEV__BASE__CONCATENATE_IMPL(a, b) a ## b
#define _COM_AZURE_DEV__BASE__CONCATENATE(a, b) _COM_AZURE_DEV__BASE__CONCATENATE_IMPL(a, b)

#define  _COM_AZURE_DEV__BASE__SOURCE_FILE __FILE__
// #define  _COM_AZURE_DEV__BASE__SOURCE_FILE __FILE_NAME__ // new for LLVM

/** Makes an identifier unique with a cpp. Avoid in header. */
#define _COM_AZURE_DEV__BASE__MAKE_IDENTIFIER(prefix) \
  _COM_AZURE_DEV__BASE__CONCATENATE(prefix, __LINE__) // we can consider __COUNTER__ but it has some implications too

/** Assert. */
#define _COM_AZURE_DEV__BASE__ASSERT(expression) \
  {if (!(expression)) {Assert::handle(#expression, _COM_AZURE_DEV__BASE__SOURCE_FILE, _COM_AZURE_DEV__BASE__INDIRECT_STRINGIFY(__LINE__));}}

/** Assert within an expression. */
#define _COM_AZURE_DEV__BASE__INLINE_ASSERT(expression) \
  ((!(expression) ? Assert::handle(#expression, _COM_AZURE_DEV__BASE__SOURCE_FILE, _COM_AZURE_DEV__BASE__INDIRECT_STRINGIFY(__LINE__)) : false), (expression))

/** Assert during initialization. */
#define _COM_AZURE_DEV__BASE__ASSERTION(expression) \
  namespace {Assertion _COM_AZURE_DEV__BASE__MAKE_IDENTIFIER(assertion)(expression, #expression, _COM_AZURE_DEV__BASE__SOURCE_FILE, _COM_AZURE_DEV__BASE__INDIRECT_STRINGIFY(__LINE__));}

// allow shorthand macros to be overridden
#if defined(_COM_AZURE_DEV__BASE__ANY_DEBUG)
#  if (!defined(BASSERT)) // BASE specific ASSERT
#    define BASSERT(expression) _COM_AZURE_DEV__BASE__ASSERT(expression)
#  endif
#  if (!defined(ASSERT)) // could conflict with other ASSERT
#    define ASSERT(expression) _COM_AZURE_DEV__BASE__ASSERT(expression)
#  endif
#  if (!defined(INLINE_ASSERT))
#    define INLINE_ASSERT(expression) _COM_AZURE_DEV__BASE__INLINE_ASSERT(expression)
#  endif
#  if (!defined(BASSERTION)) // BASE specific ASSERTION
#    define BASSERTION(expression) _COM_AZURE_DEV__BASE__ASSERTION(expression)
#  endif
#  if (!defined(ASSERTION))
#    define ASSERTION(expression) _COM_AZURE_DEV__BASE__ASSERTION(expression)
#  endif
#else
#  if (!defined(BASSERT))
#    define BASSERT(expression)
#  endif
#  if (!defined(ASSERT))
#    define ASSERT(expression)
#  endif
#  if (!defined(INLINE_ASSERT))
#    define INLINE_ASSERT(expression) (expression)
#  endif
#  if (!defined(BASSERTION))
#    define BASSERTION(expression)
#  endif
#  if (!defined(ASSERTION))
#    define ASSERTION(expression)
#  endif
#endif

/** Use INVALID_CONTROL_FLOW() to indicate invalid control flow. */
#if defined(_COM_AZURE_DEV__BASE__ANY_DEBUG)
#  define INVALID_CONTROL_FLOW() _COM_AZURE_DEV__BASE__ASSERT(!"Invalid control flow.")
#else
#  define INVALID_CONTROL_FLOW() if (true) {}
#endif

#if (!defined(GET_SOURCE_LOCATION))
#  define GET_SOURCE_LOCATION() \
  FormatOutputStream::SourceCodeLocation( \
    MESSAGE(_COM_AZURE_DEV__BASE__SOURCE_FILE ":" _COM_AZURE_DEV__BASE__INDIRECT_STRINGIFY(__LINE__)) \
  )
#endif

#if (!defined(WRITE_SOURCE_LOCATION))
#  define WRITE_SOURCE_LOCATION() {fout << GET_SOURCE_LOCATION() << ENDL;}
#endif

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/** Used for debugging global initialization issues. */
class GlobalPrint {
private:

  const char* text = nullptr;
public:

  /** Print to stdout. */
  static void printf(const char* text, ...) noexcept;

  GlobalPrint(const char* text) noexcept;

  ~GlobalPrint() noexcept;
};

#if 0
#  define _COM_AZURE_DEV__BASE__GLOBAL_PRINT() \
  namespace {base::GlobalPrint _COM_AZURE_DEV__BASE__MAKE_IDENTIFIER(globalPrint)(__FILE__);}

#  define _COM_AZURE_DEV__BASE__PRINT(...) base::GlobalPrint::printf(__VA_ARGS__)
#else
#  define _COM_AZURE_DEV__BASE__GLOBAL_PRINT() namespace {}
#  define _COM_AZURE_DEV__BASE__PRINT(message, ...)
#endif

/** Specified that the method is not implemented but could potentially be so. */
#define _COM_AZURE_DEV__BASE__NOT_IMPLEMENTED() \
  Runtime::notImplemented(_COM_AZURE_DEV__BASE__PRETTY_FUNCTION)

/** Specified that the method is not supported. */
#define _COM_AZURE_DEV__BASE__NOT_SUPPORTED() \
  Runtime::notSupported(_COM_AZURE_DEV__BASE__PRETTY_FUNCTION)

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
