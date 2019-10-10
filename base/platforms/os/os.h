/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN)
  #include <base/platforms/os/cygwin/cygwin.h>
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WINNT4)
  #include <base/platforms/os/winnt4/winnt4.h>
#elif (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS)
  #include <base/platforms/os/solaris/solaris.h>
#endif
