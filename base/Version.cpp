/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>
#include <base/Version.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

const char Version::banner[] =
"The Base Framework release "_DK_SDU_MIP__BASE__RELEASE"\n"
"A framework for developing platform independent applications\n"
"Copyright (C) 2000, 2001, 2002 by Rene Moeller Fonseca\n\n"
"This framework is distributed in the hope that it will be useful,\n"
"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n"
"Please report bugs to <fonseca@mip.sdu.dk>.\n\n"
"configure: "_DK_SDU_MIP__BASE__CONFIGURE"\n"
"build data: "_DK_SDU_MIP__BASE__BUILD_DATE"\n"
"build user: "_DK_SDU_MIP__BASE__BUILD_USER"\n"
"build host: "_DK_SDU_MIP__BASE__BUILD_HOST"\n"
"build system: "_DK_SDU_MIP__BASE__BUILD_SYSTEM"\n"
"target system: "_DK_SDU_MIP__BASE__TARGET_SYSTEM"\n";

unsigned int Version::getMajorVersion() const throw() {
  return _DK_SDU_MIP__BASE__MAJOR_VERSION;
}

unsigned int Version::getMinorVersion() const throw() {
  return _DK_SDU_MIP__BASE__MINOR_VERSION;
}

unsigned int Version::getMicroVersion() const throw() {
  return _DK_SDU_MIP__BASE__MICRO_VERSION;
}

String Version::getRelease() const throw() {
  return MESSAGE(_DK_SDU_MIP__BASE__RELEASE);
}

String Version::getVersion() const throw() {
  return MESSAGE(_DK_SDU_MIP__BASE__VERSION);
}

String Version::getBanner() const throw() {
  return MESSAGE(banner);
}

#if (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX) && \
    (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__X86)
// TAG: platform specific - and only shared library
// entry point of library when run as an ordinary application
extern "C" void libbase_entry() {
  // exit without segmentation fault
  volatile asm(
      "movl   $1,%eax\n\t" // system call number (sys_exit)
      "movl   $1,%ebx\n\t" // exit code
      "int    $0x80\n\n" // call kernel
  );
}
#endif

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
