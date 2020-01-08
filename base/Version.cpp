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
#include <base/Version.h>
#include <base/config.h>

#define _COM_AZURE_DEV__BASE__RELEASE ""

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

const char Version::banner[] =
"The Base Framework release " _COM_AZURE_DEV__BASE__RELEASE "\n"
"A framework for developing platform independent applications\n"
"Copyright (C) 2000-2020 by Rene Moeller Fonseca\n\n"
"This framework is distributed in the hope that it will be useful,\n"
"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
"Please report bugs to https://dev.azure.com/renefonseca/base.\n\n"
"Version: " _COM_AZURE_DEV__BASE__VERSION "\n"
"Build date: " _COM_AZURE_DEV__BASE__BUILD_DATE "\n";

unsigned int Version::getMajorVersion() const noexcept
{
  return _COM_AZURE_DEV__BASE__MAJOR_VERSION;
}

unsigned int Version::getMinorVersion() const noexcept
{
  return _COM_AZURE_DEV__BASE__MINOR_VERSION;
}

unsigned int Version::getMicroVersion() const noexcept
{
  return _COM_AZURE_DEV__BASE__GIT_REVISION;
}

unsigned int Version::getRevision() const noexcept
{
  return _COM_AZURE_DEV__BASE__GIT_REVISION;
}

String Version::getRelease() const noexcept
{
  return Literal(_COM_AZURE_DEV__BASE__VERSION);
}

String Version::getVersion() const noexcept
{
  return Literal(_COM_AZURE_DEV__BASE__VERSION);
}

String Version::getCommit() const noexcept
{
  return Literal(_COM_AZURE_DEV__BASE__GIT_COMMIT);
}

String Version::getCommitShort() const noexcept
{
  return Literal(_COM_AZURE_DEV__BASE__GIT_COMMIT_SHORT);
}

int64 Version::getBuildDate() const noexcept
{
  return _COM_AZURE_DEV__BASE__BUILD_DATE_SECONDS;
}

String Version::getBranch() const noexcept
{
  return Literal(_COM_AZURE_DEV__BASE__GIT_BRANCH);
}

String Version::getRemoteUrl() const noexcept
{
  return Literal(_COM_AZURE_DEV__BASE__GIT_REMOTE_URL);
}

String Version::getBanner() const noexcept
{
  return Literal(banner);
}

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX) && \
    (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__X86)
// TAG: platform specific - and only shared library
// entry point of library when run as an ordinary application
extern "C" void libbase_entry() {
  // exit without segmentation fault
  /*volatile*/ asm(
      "movl   $1,%eax\n\t" // system call number (sys_exit)
      "movl   $1,%ebx\n\t" // exit code
      "int    $0x80\n\n" // call kernel
  );
}
#endif

MemorySize Version::getDynamicObjectSize() noexcept
{
  return sizeof(DynamicObject);
}

bool Version::getDefine_COM_AZURE_DEV__BASE__DEBUG() noexcept
{
#if defined(_COM_AZURE_DEV__BASE__DEBUG)
  return true;
#else
  return false;
#endif
}

bool Version::getDefine_DEBUG() noexcept
{
#if defined(_COM_AZURE_DEV__BASE__ANY_DEBUG)
  return true;
#else
  return false;
#endif
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
