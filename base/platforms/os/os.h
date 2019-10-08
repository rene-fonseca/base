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

#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__CYGWIN)
  #include <base/platforms/os/cygwin/cygwin.h>
#elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__WINNT4)
  #include <base/platforms/os/winnt4/winnt4.h>
#elif (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS)
  #include <base/platforms/os/solaris/solaris.h>
#endif
