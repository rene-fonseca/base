/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__TRACE_H
#define _DK_SDU_MIP__BASE__TRACE_H

#include <base/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class is used for dumping debugging information to standard error in a
  very low-level way.

  @author René Møller Fonseca
*/

class Trace {
public:

  /**
    Write message to standard error.
  */
  static void message(const char* message);

  /**
    Write pointer and message to standard error.
  */
  static void member(const void* ptr, const char* message);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
