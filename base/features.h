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

#define _COM_AZURE_DEV__BASE__REQUIRE(major, minor, micro) \
  ((major <= _COM_AZURE_DEV__BASE__MAJOR_VERSION) && \
  (minor <= _COM_AZURE_DEV__BASE__MINOR_VERSION) && \
  (micro <= _COM_AZURE_DEV__BASE__MICRO_VERSION))

#if (defined(_COM_AZURE_DEV__BASE__NAMESPACE))
#  define _COM_AZURE_DEV__BASE__ENTER_NAMESPACE namespace base {
#  define _COM_AZURE_DEV__BASE__LEAVE_NAMESPACE }

/**
  This is the root namespace of <strong>The Base Framework</strong>. All the
  symbols defined by the framework are contained within this particular
  namespace. You should not add your own symbols to this namespace. Ignoring
  this rule will put your application at risk of clashing with a future release
  of this framework. You should therefore consider this namespace reserved
  solely for this project just like the 'std' namespace is allocated for the
  <strong>Standard Template Library</strong> (and <strong>C++</strong>
  language).

  @short The root namespace of the framework
*/

namespace base {
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

// allow macros to be overridden
#if defined(_COM_AZURE_DEV__BASE__ANY_DEBUG)
#  include <base/Trace.h>
#  if (!defined(ASSERT))
#    define ASSERT(expression) {if (!(expression)) {Trace::message("Assertion failure of (" #expression ") at " __FILE__ ":" _COM_AZURE_DEV__BASE__INDIRECT_STRINGIFY(__LINE__));}}
#  endif
#  if (!defined(ASSERTION))
#    define ASSERTION(expression) namespace {Assertion assertion(expression, "Assertion failure of (" #expression ") at " __FILE__ ":" _COM_AZURE_DEV__BASE__INDIRECT_STRINGIFY(__LINE__));}
#  endif
#else
#  if (!defined(ASSERT))
#    define ASSERT(expression)
#  endif
#  if (!defined(ASSERTION))
#    define ASSERTION(expression)
#  endif
#endif

#if (!defined(GET_SOURCE_LOCATION))
#  define GET_SOURCE_LOCATION() \
  FormatOutputStream::Debug( \
    MESSAGE(__FILE__ ":" _COM_AZURE_DEV__BASE__INDIRECT_STRINGIFY(__LINE__)) \
  )
#endif

#if (!defined(WRITE_SOURCE_LOCATION))
#  define WRITE_SOURCE_LOCATION() {fout << GET_SOURCE_LOCATION() << ENDL;}
#endif
