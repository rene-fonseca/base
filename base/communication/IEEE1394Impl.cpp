/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/communication/IEEE1394Impl.h>

#if (!defined(_COM_AZURE_DEV__BASE__IEEE_1394_IMPL))
#  if 0 && (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
#    define _COM_AZURE_DEV__BASE__IEEE_1394_IMPL LinuxRawIEEE1394
#    define _COM_AZURE_DEV__BASE__IEEE_1394_IMPL_HEADER "base/communication/LinuxRawIEEE1394.cpp"
#  else
#    define _COM_AZURE_DEV__BASE__IEEE_1394_IMPL DummyIEEE1394
#    define _COM_AZURE_DEV__BASE__IEEE_1394_IMPL_HEADER "base/communication/DummyIEEE1394.h"
#  endif
#endif

#include _COM_AZURE_DEV__BASE__IEEE_1394_IMPL_HEADER

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

IEEE1394Impl* IEEE1394Impl::getDefault() noexcept
{
  return new _COM_AZURE_DEV__BASE__IEEE_1394_IMPL();
}

IEEE1394Impl::~IEEE1394Impl() {
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
