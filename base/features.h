/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__FEATURES_H
#define _DK_SDU_MIP__BASE__FEATURES_H

#include <base/platforms/symbols.h> // must be first
#include <base/configuration.h>
#include <base/platforms/platform.h>

#define _DK_SDU_MIP__BASE__MAJOR_VERSION 0
#define _DK_SDU_MIP__BASE__MINOR_VERSION 9
#define _DK_SDU_MIP__BASE__MICRO_VERSION 1
#define _DK_SDU_MIP__BASE__VERSION "0.9.1"

#define _DK_SDU_MIP__BASE__REQUIRE(major, minor, micro) \
  ((major <= _DK_SDU_MIP__BASE__MAJOR_VERSION) && \
  (minor <= _DK_SDU_MIP__BASE__MINOR_VERSION) && \
  (micro <= _DK_SDU_MIP__BASE__MICRO_VERSION))

#if !defined(_DK_SDU_MIP__BASE__CPP_BOOL)
  #error Compiler does not support the bool primitive
#endif

#if !defined(_DK_SDU_MIP__BASE__CPP_CONST)
  #error Compiler does not support the const keyword  
#endif

#if !defined(_DK_SDU_MIP__BASE__CPP_RESTRICT)
#endif

#if !defined(_DK_SDU_MIP__BASE__CPP_INLINE)
  #error Compiler does not support inlining of functions
#endif

#if !defined(_DK_SDU_MIP__BASE__CPP_STATIC_CONST)
  #error Static const with initializer inside class definition is not supported by compiler
#endif

#if !defined(_DK_SDU_MIP__BASE__CPP_ANONYMOUS)
//  #error Anonymous structure (and union) is not supported by compiler
#endif

#if (_DK_SDU_MIP__BASE__LONG_SIZE < _DK_SDU_MIP__BASE__POINTER_SIZE)
  #error Primitive long cannot hold result of pointer arithmetic
#endif

#if (_DK_SDU_MIP__BASE__POINTER_SIZE == 4)
  #define _DK_SDU_MIP__BASE__ABI32
#elif (_DK_SDU_MIP__BASE__POINTER_SIZE == 8)
  #define _DK_SDU_MIP__BASE__ABI64
#else
  #error Unsupported pointer size
#endif

#if defined(_DK_SDU_MIP__BASE__NAMESPACE)
  #define _DK_SDU_MIP__BASE__ENTER_NAMESPACE namespace base {
  #define _DK_SDU_MIP__BASE__LEAVE_NAMESPACE }

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

#if defined(_DK_SDU_MIP__BASE__TRACE)
  #define TRACE(text) {Trace::message(text);}
  #define TRACE_MEMBER() {Trace::member(this, __PRETTY_FUNCTION__);}
#else
  #define TRACE(text)
  #define TRACE_MEMBER()
#endif

#if defined(_DK_SDU_MIP__BASE__DEBUG)
  #define _DK_SDU_MIP__BASE__STRINGIFICATION(VALUE) #VALUE
  #define _DK_SDU_MIP__BASE__INDIRECT_STRINGIFICATION(VALUE) _DK_SDU_MIP__BASE__STRINGIFICATION(VALUE)
  #define ASSERT(expression) {if (!(expression)) {ferr << "Assertion failure of (" #expression ") at " __FILE__ ":" _DK_SDU_MIP__BASE__INDIRECT_STRINGIFICATION(__LINE__) << ENDL;}}
#else
  #define ASSERT(expression)
#endif

#endif
