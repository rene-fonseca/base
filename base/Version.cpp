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
#include <base/string/FormatOutputStream.h>
#include <base/SystemInformation.h>
#include <base/Architecture.h>

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

String getEndianness(unsigned int endianness)
{
  switch (endianness) {
  case Architecture::BE:
    return "Big-endian";
  case Architecture::LE:
    return "Little-endian";
  default:
    return "UNKNOWN";
  }
}

String getFloatType(unsigned int type)
{
  switch (type) {
  case _COM_AZURE_DEV__BASE__IEEE_754_SINGLE_PRECISION:
    return "IEEE 754 single precision";
  case _COM_AZURE_DEV__BASE__IEEE_754_DOUBLE_PRECISION:
    return "IEEE 754 double precision";
  case _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96:
    return "IEEE extended double precision 96 bits";
  case _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16:
    return "IEEE extended double precision 96 bits with 16 bytes alignment";
  case _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128:
    return "IEEE extended double precision 128 bits";
  case _COM_AZURE_DEV__BASE__IEEE_QUADRUPLE_PRECISION:
    return "IEEE quadruple precision";
  case _COM_AZURE_DEV__BASE__IBM_EXTENDED_PRECISION:
    return "IEEE extended precision";
  default:
    return "UNKNOWN";
  }
}

void Version::printSystem()
{
  Version v;
  fout << "BASE framework " << v.getVersion() << " r" << v.getRevision() << " " << v.getCommitShort() << EOL
       << EOL

       << "OS: " << SystemInformation::getOS() << EOL
       << "OS Vendor: " << SystemInformation::getVendor() << EOL
       << "OS System: " << SystemInformation::getSystem() << EOL
       << "OS Release: " << SystemInformation::getRelease() << EOL
       << "OS Version: " << SystemInformation::getVersion() << EOL
       << "OS Machine: " << SystemInformation::getMachine() << EOL
       << "OS Architecture: " << Architecture::getArchitectureAsString() << EOL
       << EOL

       << "Endianess: " << getEndianness(Architecture::NATIVE) << EOL
       << "Float endianess: " << getEndianness(Architecture::FLOAT_NATIVE) << EOL
       << "Memory: " << (sizeof(void*) * 8) << " bit" << EOL
       << "char size: " << sizeof(char) << " bytes" << EOL
       << "wchar size: " << sizeof(wchar) << " bytes" << EOL
       << "short size: " << sizeof(short) << " bytes" << EOL
       << "int size: " << sizeof(int) << " bytes" << EOL
       << "long size: " << sizeof(long) << " bytes" << EOL
       << "long long size: " << sizeof(long long) << " bytes" << EOL
       << EOL

       << "float: " << getFloatType(_COM_AZURE_DEV__BASE__FLOAT) << " bytes" << EOL
       << "double: " << getFloatType(_COM_AZURE_DEV__BASE__DOUBLE) << " bytes" << EOL
       << "long double: " << getFloatType(_COM_AZURE_DEV__BASE__LONG_DOUBLE) << " bytes" << EOL
       << "float128: " << getFloatType(_COM_AZURE_DEV__BASE__FLOAT128) << " bytes" << EOL
       << ENDL;

  // TAG: add memory and CPU info
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
