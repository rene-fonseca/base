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

#if (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__X86)
#  if (_COM_AZURE_DEV__BASE__ARCH_MINOR >= _COM_AZURE_DEV__BASE__PENTIUM_4)
#    define _COM_AZURE_DEV__BASE__ARCH_MMX_EXTENSION 0100
#    define _COM_AZURE_DEV__BASE__ARCH_SSE_EXTENSION 0200
#  elif (_COM_AZURE_DEV__BASE__ARCH_MINOR >= _COM_AZURE_DEV__BASE__PENTIUM_III)
#    define _COM_AZURE_DEV__BASE__ARCH_MMX_EXTENSION 0100
#    define _COM_AZURE_DEV__BASE__ARCH_SSE_EXTENSION 0100
#  elif (_COM_AZURE_DEV__BASE__ARCH_MINOR >= _COM_AZURE_DEV__BASE__PENTIUM_II)
#    define _COM_AZURE_DEV__BASE__ARCH_MMX_EXTENSION 0100
#  endif
#endif // arch

#if (_COM_AZURE_DEV__BASE__POINTER_SIZE == 4)
#  define _COM_AZURE_DEV__BASE__ARCH_ABI _COM_AZURE_DEV__BASE__ABI_32
#elif (_COM_AZURE_DEV__BASE__POINTER_SIZE == 8)
#  define _COM_AZURE_DEV__BASE__ARCH_ABI _COM_AZURE_DEV__BASE__ABI_64
#endif // abi
