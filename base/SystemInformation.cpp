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
#include <base/SystemInformation.h>
#include <base/Exception.h>
#include <base/SystemLogger.h>

#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
#  include <windows.h>
#else
#  include <sys/utsname.h>
#endif // flavor

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

String SystemInformation::getVendor() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return MESSAGE("Microsoft");
#else
  struct utsname name;
  int result = ::uname(&name);
  if (result == 0) {
    return String(name.sysname);
    return String(name.nodename);
  } else {
    return MESSAGE("Unspecified");
  }
#endif
}

String SystemInformation::getSystem() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  OSVERSIONINFO versionInfo;
  versionInfo.dwOSVersionInfoSize = sizeof(versionInfo);
  ::GetVersionEx(&versionInfo); // never fails
  if ((versionInfo.dwMajorVersion == 4) && (versionInfo.dwMinorVersion == 0)) {
    return MESSAGE("Windows NT");
  } else if ((versionInfo.dwMajorVersion == 5) &&
             (versionInfo.dwMinorVersion == 0)) {
    return MESSAGE("Windows 2000");
  } else if ((versionInfo.dwMajorVersion == 5) &&
             (versionInfo.dwMinorVersion == 1)) {
    return MESSAGE("Windows XP");
  } else {
    return MESSAGE("Windows later than XP");
  }
#else
  struct utsname name;
  int result = ::uname(&name);
  return String(name.sysname);
#endif // flavor
}

String SystemInformation::getRelease() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return MESSAGE("Unspecified");
#else
  struct utsname name;
  int result = ::uname(&name);
  return String(name.release);
#endif
}

String SystemInformation::getVersion() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return MESSAGE("Unspecified");
#else
  struct utsname name;
  int result = ::uname(&name);
  return String(name.version);
#endif
}

String SystemInformation::getMachine() throw() {
#if (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32)
  return MESSAGE("Unspecified");
#else
  struct utsname name;
  int result = ::uname(&name);
  return String(name.machine);
#endif
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
