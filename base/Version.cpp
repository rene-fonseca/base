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
"Copyright (C) 2000-2003 by Rene Moeller Fonseca\n\n"
"This framework is distributed in the hope that it will be useful,\n"
"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
"Please report bugs to https://dev.azure.com/renefonseca/base.\n\n"
"build data: " _COM_AZURE_DEV__BASE__BUILD_DATE "\n";

unsigned int Version::getMajorVersion() const throw() {
  return _COM_AZURE_DEV__BASE__MAJOR_VERSION;
}

unsigned int Version::getMinorVersion() const throw() {
  return _COM_AZURE_DEV__BASE__MINOR_VERSION;
}

unsigned int Version::getMicroVersion() const throw() {
  return _COM_AZURE_DEV__BASE__GIT_REVISION; // TAG: FIXME
}

unsigned int Version::getRevision() const throw() {
  return _COM_AZURE_DEV__BASE__GIT_REVISION;
}

String Version::getRelease() const throw() {
  return Literal(_COM_AZURE_DEV__BASE__VERSION);
}

String Version::getVersion() const throw() {
  return Literal(_COM_AZURE_DEV__BASE__VERSION);
}

String Version::getCommit() const throw() {
  return Literal(_COM_AZURE_DEV__BASE__GIT_COMMIT_SHORT);
}

int64 Version::getBuildDate() const throw() {
  return _COM_AZURE_DEV__BASE__BUILD_DATE_SECONDS;
}

String Version::getBanner() const throw() {
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

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
