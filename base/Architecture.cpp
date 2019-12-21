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
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__SPARC)
  return SPARC;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__MIPS)
  return MIPS;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__MIPS64)
  return MIPS64;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__ITANIUM)
  return ITANIUM;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__ALPHA)
  return ALPHA;
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__PPC)
  return POWER_PC;
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

// TAG: need support for multiple processors (and hyper-threading)
// struct ProcessorInformation {major, minor, #logical processors, technologies};

// Array is no good in Architecture header
// Array<Architecture::Minor> Architecture::getProcessors() noexcept
// {
//   Array<Minor> result;
//   return result;
// }

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
