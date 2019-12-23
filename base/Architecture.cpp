/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Architecture.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

unsigned int Architecture::getTechonologies() noexcept
{
  unsigned int result = 0;
  return result;
}

Architecture::Major Architecture::getArchitecture() noexcept
{
#if (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__X86)
  return X86;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__SPARC32)
  return SPARC32;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__SPARC64)
  return SPARC64;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__MIPS)
  return MIPS;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__MIPS64)
  return MIPS64;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__ITANIUM)
  return ITANIUM;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__ALPHA)
  return ALPHA;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__POWERPC32)
  return POWER_PC_32;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__POWERPC64)
  return POWER_PC_64;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__S390X)
  return S390X;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__RS)
  return RS;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__X86_64)
  return X86_64;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__S390)
  return S390;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__ARM32)
  return ARM32;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__ARM64)
  return ARM64;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__WASM32)
  return WASM32;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__WASM64)
  return WASM64;
#else
#  error Unsupported architecture
#endif
}

Architecture::Minor Architecture::getMinorArchitecture() noexcept
{
  Architecture::Minor result = GENERIC;
  return result;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
