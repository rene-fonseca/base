/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/OperatingSystem.h>
#include <base/SystemLogger.h>

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  #include <windows.h>
  #undef ERROR // protect against the evil programmers
#endif // flavour

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
  ASSERTION(sizeof(OperatingSystem::Handle) == sizeof(HANDLE));
#endif

#if (_DK_SDU_MIP__BASE__FLAVOUR == _DK_SDU_MIP__BASE__WIN32)
namespace {

  class IsCompatible {
  public:

    IsCompatible() throw() {
      OSVERSIONINFO versionInfo;
      versionInfo.dwOSVersionInfoSize = sizeof(versionInfo);
      ::GetVersionEx(&versionInfo); // never fails
      // only Windows NT 4.0 or later
      bool compatible = (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) && (versionInfo.dwMajorVersion >= 4);
      if (!compatible) {
        SystemLogger::write(SystemLogger::ERROR, "Opeating system is not supported.");
      }
    }
    
  };

  IsCompatible isCompatible();
};
#endif // flavour

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
