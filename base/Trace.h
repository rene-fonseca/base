/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__TRACE_H
#define _DK_SDU_MIP__BASE__TRACE_H

#include <base/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  This class is used for dumping debugging information to standard output in a
  very low-level way.

  @author René Møller Fonseca
*/

class Trace {
public:

  /**
    Write message to standard output.
  */
  static void message(const char* message);

  /**
    Write pointer and message to standard output.
  */
  static void member(void* ptr, const char* message);
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
