/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/SystemInformation.h>
#include <base/Exception.h>
#include <base/SystemLogger.h>

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  include <windows.h>
#else
#  include <sys/utsname.h>
#endif // flavor

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

String SystemInformation::getOS() noexcept
{
  const char* os = nullptr;
  switch (_COM_AZURE_DEV__BASE__OS) {
  case _COM_AZURE_DEV__BASE__GNULINUX:
    os = "GNU/Linux";
    break;
  case _COM_AZURE_DEV__BASE__IRIX65:
    os = "IRIX";
    break;
  case _COM_AZURE_DEV__BASE__SOLARIS:
    os = "Solaris";
    break;
  case _COM_AZURE_DEV__BASE__CYGWIN:
    os = "Cygwin";
    break;
  case _COM_AZURE_DEV__BASE__WINNT4:
    os = "Windows";
    break;
  case _COM_AZURE_DEV__BASE__W2K:
    os = "Windows";
    break;
  case _COM_AZURE_DEV__BASE__WXP:
    os = "Windows";
    break;
  case _COM_AZURE_DEV__BASE__BEOS:
    os = "BEOS";
    break;
  case _COM_AZURE_DEV__BASE__AIX:
    os = "AIX";
    break;
  case _COM_AZURE_DEV__BASE__MACOS:
    os = "macOS";
    break;
  case _COM_AZURE_DEV__BASE__FREEBSD:
    os = "FreeBSD";
    break;
  case _COM_AZURE_DEV__BASE__WEBASSEMBLY:
    os = "WebAssembly";
    break;
  default:
    ;
  }
  return os;
}

String SystemInformation::getVendor() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return Literal("Microsoft");
#else
  struct utsname name;
  int result = ::uname(&name);
  if (result == 0) {
    return String(name.sysname);
    return String(name.nodename);
  } else {
    return Literal("Unspecified");
  }
#endif
}

String SystemInformation::getSystem() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  OSVERSIONINFO versionInfo;
  versionInfo.dwOSVersionInfoSize = sizeof(versionInfo);
  ::GetVersionEx(&versionInfo); // never fails
  if ((versionInfo.dwMajorVersion == 4) && (versionInfo.dwMinorVersion == 0)) {
    return Literal("Windows NT");
  } else if ((versionInfo.dwMajorVersion == 5) &&
             (versionInfo.dwMinorVersion == 0)) {
    return Literal("Windows 2000");
  } else if ((versionInfo.dwMajorVersion == 5) &&
             (versionInfo.dwMinorVersion == 1)) {
    return Literal("Windows XP");
  } else {
    return Literal("Windows later than XP");
  }
#else
  struct utsname name;
  int result = ::uname(&name);
  if (result != 0) {
    return String();
  }
  return String(name.sysname);
#endif // flavor
}

String SystemInformation::getRelease() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return Literal("Unspecified");
#else
  struct utsname name;
  int result = ::uname(&name);
  if (result != 0) {
    return String();
  }
  return String(name.release);
#endif
}

String SystemInformation::getVersion() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return Literal("Unspecified");
#else
  struct utsname name;
  int result = ::uname(&name);
  if (result != 0) {
    return String();
  }
  return String(name.version);
#endif
}

String SystemInformation::getMachine() noexcept
{
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
  return Literal("Unspecified");
#else
  struct utsname name;
  int result = ::uname(&name);
  if (result != 0) {
    return String();
  }
  return String(name.machine);
#endif
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
