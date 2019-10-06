/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#if (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__X86)
#  if (_DK_SDU_MIP__BASE__ARCH_MINOR >= _DK_SDU_MIP__BASE__PENTIUM_4)
#    define _DK_SDU_MIP__BASE__ARCH_MMX_EXTENSION 0100
#    define _DK_SDU_MIP__BASE__ARCH_SSE_EXTENSION 0200
#  elif (_DK_SDU_MIP__BASE__ARCH_MINOR >= _DK_SDU_MIP__BASE__PENTIUM_III)
#    define _DK_SDU_MIP__BASE__ARCH_MMX_EXTENSION 0100
#    define _DK_SDU_MIP__BASE__ARCH_SSE_EXTENSION 0100
#  elif (_DK_SDU_MIP__BASE__ARCH_MINOR >= _DK_SDU_MIP__BASE__PENTIUM_II)
#    define _DK_SDU_MIP__BASE__ARCH_MMX_EXTENSION 0100
#  endif
#endif // arch

#if (_DK_SDU_MIP__BASE__POINTER_SIZE == 4)
#  define _DK_SDU_MIP__BASE__ARCH_ABI _DK_SDU_MIP__BASE__ABI_32
#elif (_DK_SDU_MIP__BASE__POINTER_SIZE == 8)
#  define _DK_SDU_MIP__BASE__ARCH_ABI _DK_SDU_MIP__BASE__ABI_64
#endif // abi
