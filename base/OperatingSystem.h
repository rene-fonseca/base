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
  
  @short Operating system
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class OperatingSystem : public Object {
public:

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  /** The type of an ordinary resource handle (do NOT assume anything about this type). */
  typedef void* Handle;

  /** This constants indicates an invalid handle. */
  static const Handle INVALID_HANDLE = static_cast<char*>(0) - 1;
#else
  /** The type of an ordinary resource handle (do NOT assume anything about this type). */
  typedef int Handle;
  
  /** This constant indicates an invalid handle. */
  static const Handle INVALID_HANDLE = -1;
#endif
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
