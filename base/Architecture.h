/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__ARCHITECTURE_H
#define _DK_SDU_MIP__BASE__ARCHITECTURE_H

#include <base/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Architecture information.
  
  @short Architecture information.
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

class Architecture {
public:

  enum ByteOrder {
    BE = 0,
    LE = 1,
#if (_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__BIG_ENDIAN)
    NATIVE = BE
#elif (_DK_SDU_MIP__BASE__BYTE_ORDER == _DK_SDU_MIP__BASE__LITTLE_ENDIAN)
    NATIVE = LE
#else
    NATIVE = 0
#endif
  };

  /**
    Returns true if the native byte order is big endian.
  */
  static bool isBigEndian() throw() {
    return NATIVE == BE;
  }

  /**
    Returns true if the native byte order is little endian.
  */
  static bool isLittleEndian() throw() {
    return NATIVE == LE;
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
