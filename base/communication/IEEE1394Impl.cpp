/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/communication/IEEE1394Impl.h>

#if (!defined(_DK_SDU_MIP__BASE__IEEE_1394_IMPL))
#  if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
#    define _DK_SDU_MIP__BASE__IEEE_1394_IMPL LinuxRawIEEE1394
#    define _DK_SDU_MIP__BASE__IEEE_1394_IMPL_HEADER "base/communication/LinuxRawIEEE1394.h"
#  else
#    define _DK_SDU_MIP__BASE__IEEE_1394_IMPL DummyIEEE1394
#    define _DK_SDU_MIP__BASE__IEEE_1394_IMPL_HEADER "base/communication/DummyIEEE1394.h"
#  endif
#endif

#include _DK_SDU_MIP__BASE__IEEE_1394_IMPL_HEADER

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

IEEE1394Impl* IEEE1394Impl::getDefault() throw() {
  return new _DK_SDU_MIP__BASE__IEEE_1394_IMPL();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
