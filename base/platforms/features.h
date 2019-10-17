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

#include <base/platforms/symbols.h> // must be first
#include <base/configuration.h>

// _COM_AZURE_DEV__BASE__DEBUG influences state!

#if defined(_COM_AZURE_DEV__BASE__DEBUG)
#  define DEBUG // enable ASSERT and ASSERTION
#endif

#if defined(DEBUG) || defined(_DEBUG) || defined(_COM_AZURE_DEV__BASE__DEBUG)
#  define _COM_AZURE_DEV__BASE__ANY_DEBUG
#endif

#if defined(NDEBUG) && defined(_COM_AZURE_DEV__BASE__ANY_DEBUG)
#  error NDEBUG and _COM_AZURE_DEV__BASE__ANY_DEBUG are both defined
#endif

#include <base/features.h>

// grants access to internal headers
#define _COM_AZURE_DEV__BASE__INTERNAL_ACCESS

#if (_COM_AZURE_DEV__BASE__ABI == _COM_AZURE_DEV__BASE__ABI_V3MV)
#  define _COM_AZURE_DEV__BASE__EXCEPTION_V3MV
#endif

#if !defined(TRACE) && !defined(TRACE_MEMBER) // allow macros to be overridden
#if defined(_COM_AZURE_DEV__BASE__TRACE)
#  include <base/Trace.h>
 
#  define TRACE(text) {Trace::message(text);}

#  if defined(_COM_AZURE_DEV__BASE__COMPILER_FUNCTION)
#    define TRACE_MEMBER() {Trace::member(this, _COM_AZURE_DEV__BASE__COMPILER_FUNCTION);}
#  else
#    define TRACE_MEMBER() {Trace::member(this, __func__);} // __func__ is ISO C99
#  endif
#else
#  define TRACE(text)
#  define TRACE_MEMBER()
#endif
#endif
