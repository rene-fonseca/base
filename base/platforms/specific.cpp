/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>

// include platform specific source files

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <base/platforms/win32/AsyncReadFileContext.cpp>
#  include <base/platforms/win32/AsyncReadStreamContext.cpp>
#  include <base/platforms/win32/AsyncWriteFileContext.cpp>
#  include <base/platforms/win32/AsyncWriteStreamContext.cpp>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

namespace {

  // TAG: this function prevents an empty archive (temporary fix)
  // TAG: binutils does not access empty archives on some platforms
  void denyEmtpyArchive() throw() {}
  
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
