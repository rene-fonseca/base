/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

namespace {

  // TAG: this function prevents an empty archive (temporary fix)
  // TAG: binutils does not access empty archives on some platforms
  void denyEmtpyArchive() throw() {
  }
  
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
