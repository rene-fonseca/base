/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__PLATFORMS_FEATURES_H
#define _DK_SDU_MIP__BASE__PLATFORMS_FEATURES_H

#include <base/platforms/symbols.h> // must be first
#include <base/platforms/configuration.h>

#if defined(_DK_SDU_MIP__BASE__DEBUG)
  #define DEBUG // enable ASSERT and ASSERTION
#endif

#include <base/features.h>

// grants access to internal headers
#define _DK_SDU_MIP__BASE__INTERNAL_ACCESS

#if !defined(TRACE) && !defined(TRACE_MEMBER) // allow macros to be overridden
#if defined(_DK_SDU_MIP__BASE__TRACE)
  #include <base/Trace.h>

  #define TRACE(text) {Trace::message(text);}

  #if defined(__GNUC__)
    #define TRACE_MEMBER() {Trace::member(this, __PRETTY_FUNCTION__);}
  #else
    #define TRACE_MEMBER() {Trace::member(this, __func__);} // __func__ is ISO C99
  #endif
#else
  #define TRACE(text)
  #define TRACE_MEMBER()
#endif
#endif

#else
  #error Mulitiple inclusion of base/platforms/features header
#endif
