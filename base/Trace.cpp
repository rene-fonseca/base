/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Trace.h>
#include <stdio.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void Trace::message(const char* message) {
  fprintf(stderr, "TRACE %s\n", message); // fprintf must be MT-safe
}

void Trace::member(const void* ptr, const char* message) {
  fprintf(stderr, "TRACE %08x >> %s\n", ptr, message); // fprintf must be MT-safe
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
