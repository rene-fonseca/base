/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__OPERATING_SYSTEM_H
#define _DK_SDU_MIP__BASE__OPERATING_SYSTEM_H

#include <base/Object.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Operating system.

  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class OperatingSystem : public Object {
public:

  /** The type of an ordinary resource handle (do NOT assume anything about this type). */
#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  typedef unsigned long Handle;
#else
  typedef int Handle;
#endif
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
