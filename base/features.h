/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__FEATURES_H
#define _DK_SDU_MIP__BASE__FEATURES_H

// order: const -> detect -> arch -> compiler -> os -> framework -> application

#include <base/platforms/symbols.h>
#include <base/platforms/platform.h>
#include <base/configuration.h>
#include <base/platforms/arch/architecture.h>
#include <base/platforms/compiler/compiler.h>
#include <base/platforms/os/os.h>

#define _DK_SDU_MIP__BASE__MAJOR_VERSION 0
#define _DK_SDU_MIP__BASE__MINOR_VERSION 9
#define _DK_SDU_MIP__BASE__MICRO_VERSION 1
#define _DK_SDU_MIP__BASE__VERSION "0.9.1"
#define _DK_SDU_MIP__BASE__RELEASE "1.0 prerelease 1"

#define _DK_SDU_MIP__BASE__REQUIRE(major, minor, micro) \
  ((major <= _DK_SDU_MIP__BASE__MAJOR_VERSION) && \
  (minor <= _DK_SDU_MIP__BASE__MINOR_VERSION) && \
  (micro <= _DK_SDU_MIP__BASE__MICRO_VERSION))

// TAG: temporary fix until all symbols have been replaced
#define _DK_SDU_MIP__BASE__FLAVOUR _DK_SDU_MIP__BASE__FLAVOR

#if defined(_DK_SDU_MIP__BASE__NAMESPACE)
  #define _DK_SDU_MIP__BASE__ENTER_NAMESPACE namespace base {
  #define _DK_SDU_MIP__BASE__LEAVE_NAMESPACE };

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
};

#else
  #define _DK_SDU_MIP__BASE__ENTER_NAMESPACE
  #define _DK_SDU_MIP__BASE__LEAVE_NAMESPACE
#endif

// allow macros to be overridden
#if defined(DEBUG) && !defined(ASSERT) && !defined(ASSERTION)
  #include <base/Trace.h>
  #define _DK_SDU_MIP__BASE__STRINGIFICATION(VALUE) #VALUE
  #define _DK_SDU_MIP__BASE__INDIRECT_STRINGIFICATION(VALUE) _DK_SDU_MIP__BASE__STRINGIFICATION(VALUE)
  #define ASSERT(expression) {if (!(expression)) {Trace::message("Assertion failure of (" #expression ") at " __FILE__ ":" _DK_SDU_MIP__BASE__INDIRECT_STRINGIFICATION(__LINE__));}}
  #define ASSERTION(expression) namespace {Assertion assertion(expression, "Assertion failure of (" #expression ") at " __FILE__ ":" _DK_SDU_MIP__BASE__INDIRECT_STRINGIFICATION(__LINE__));}
#else
  #define ASSERT(expression)
  #define ASSERTION(expression)
#endif

#endif
