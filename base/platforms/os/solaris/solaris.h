/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_PLATFORMS_OS_CYGWIN__CYGWIN_H
#define _DK_SDU_MIP__BASE_PLATFORMS_OS_CYGWIN__CYGWIN_H

#include <base/features.h>

#if !defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_FABSF)
#  if defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_FABS)
#    define _DK_SDU_MIP__BASE__COMPILER_ISOC_FABSF _DK_SDU_MIP__BASE__COMPILER_ISOC_FABS
#  else
#    define _DK_SDU_MIP__BASE__COMPILER_ISOC_FABSF isoc::fabs
#  endif
#endif

#if !defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_FABSL)
#  if defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_FABS)
#    define _DK_SDU_MIP__BASE__COMPILER_ISOC_FABSL _DK_SDU_MIP__BASE__COMPILER_ISOC_FABS
#  else
#    define _DK_SDU_MIP__BASE__COMPILER_ISOC_FABSL isoc::fabs
#  endif
#endif

#if !defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_COSF)
#  if defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_COS)
#    define _DK_SDU_MIP__BASE__COMPILER_ISOC_COSF _DK_SDU_MIP__BASE__COMPILER_ISOC_COS
#  else
#    define _DK_SDU_MIP__BASE__COMPILER_ISOC_COSF isoc::cos
#  endif
#endif

#if !defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_COSL)
#  if defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_COS)
#    define _DK_SDU_MIP__BASE__COMPILER_ISOC_COSL _DK_SDU_MIP__BASE__COMPILER_ISOC_COS
#  else
#    define _DK_SDU_MIP__BASE__COMPILER_ISOC_COSL isoc::cos
#  endif
#endif

#if !defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_SINF)
#  if defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_SIN)
#    define _DK_SDU_MIP__BASE__COMPILER_ISOC_SINF _DK_SDU_MIP__BASE__COMPILER_ISOC_SIN
#  else
#    define _DK_SDU_MIP__BASE__COMPILER_ISOC_SINF isoc::sin
#  endif
#endif

#if !defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_SINL)
#  if defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_SIN)
#    define _DK_SDU_MIP__BASE__COMPILER_ISOC_SINL _DK_SDU_MIP__BASE__COMPILER_ISOC_SIN
#  else
#    define _DK_SDU_MIP__BASE__COMPILER_ISOC_SINL isoc::sin
#  endif
#endif

#if !defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_TANF)
#  if defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_TAN)
#    define _DK_SDU_MIP__BASE__COMPILER_ISOC_TANF _DK_SDU_MIP__BASE__COMPILER_ISOC_TAN
#  else
#    define _DK_SDU_MIP__BASE__COMPILER_ISOC_TANF isoc::tan
#  endif
#endif

#if !defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_TANL)
#  if defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_TAN)
#    define _DK_SDU_MIP__BASE__COMPILER_ISOC_TANL _DK_SDU_MIP__BASE__COMPILER_ISOC_TAN
#  else
#    define _DK_SDU_MIP__BASE__COMPILER_ISOC_TANL isoc::tan
#  endif
#endif

#if !defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_ATAN2F)
#  if defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_ATAN2)
#    define _DK_SDU_MIP__BASE__COMPILER_ISOC_ATAN2F _DK_SDU_MIP__BASE__COMPILER_ISOC_ATAN2
#  else
#    define _DK_SDU_MIP__BASE__COMPILER_ISOC_ATAN2F isoc::atan2
#  endif
#endif

#if !defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_ATAN2L)
#  if defined(_DK_SDU_MIP__BASE__COMPILER_ISOC_ATAN2)
#    define _DK_SDU_MIP__BASE__COMPILER_ISOC_ATAN2L _DK_SDU_MIP__BASE__COMPILER_ISOC_ATAN2
#  else
#    define _DK_SDU_MIP__BASE__COMPILER_ISOC_ATAN2L isoc::atan2
#  endif     
#endif

#endif
