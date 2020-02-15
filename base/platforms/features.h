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
#include <base/features.h>

// grants access to internal headers
#define _COM_AZURE_DEV__BASE__INTERNAL_ACCESS

#if (_COM_AZURE_DEV__BASE__ABI == _COM_AZURE_DEV__BASE__ABI_V3MV)
#  define _COM_AZURE_DEV__BASE__EXCEPTION_V3MV
#endif

#if !defined(TRACE) // allow macros to be overridden
#if defined(_COM_AZURE_DEV__BASE__TRACE)
#  include <base/Trace.h>
#  define TRACE(text) {Trace::message(text);}
#else
#  define TRACE(text)
#endif
#endif
