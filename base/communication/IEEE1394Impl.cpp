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

// TAG: does not belong here
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  define _DK_SDU_MIP__BASE__IEEE_1394_IMPL UnibrainIEEE1394
#  include <base/communication/UnibrainIEEE1394.cpp>
#elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX)
#  define _DK_SDU_MIP__BASE__IEEE_1394_IMPL LinuxRawIEEE1394
#  include <base/communication/LinuxRawIEEE1394.cpp>
#else
#  define _DK_SDU_MIP__BASE__IEEE_1394_IMPL DummyIEEE1394
#  include <base/communication/DummyIEEE1394.h>
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

IEEE1394Impl* IEEE1394Impl::getDefault() throw() {
  return new _DK_SDU_MIP__BASE__IEEE_1394_IMPL();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
