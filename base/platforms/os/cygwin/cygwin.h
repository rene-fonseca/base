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

#undef _DK_SDU_MIP__BASE__COMPILER_ISOC_FABSL
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_FABSL isoc::fabs
#undef _DK_SDU_MIP__BASE__COMPILER_ISOC_CEILL
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_CEILL isoc::ceil
#undef _DK_SDU_MIP__BASE__COMPILER_ISOC_FLOORL
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_FLOORL isoc::floor
#undef _DK_SDU_MIP__BASE__COMPILER_ISOC_ROUNDL
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_ROUNDL isoc::round
#undef _DK_SDU_MIP__BASE__COMPILER_ISOC_TRUNCL
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_TRUNCL isoc::trunc
#undef _DK_SDU_MIP__BASE__COMPILER_ISOC_SQRTL
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_SQRTL isoc::sqrt
#undef _DK_SDU_MIP__BASE__COMPILER_ISOC_CBRTL
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_CBRTL isoc::cbrt
#undef _DK_SDU_MIP__BASE__COMPILER_ISOC_HYPOTL
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_HYPOTL isoc::hypot
#undef _DK_SDU_MIP__BASE__COMPILER_ISOC_EXPL
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_EXPL isoc::exp
#undef _DK_SDU_MIP__BASE__COMPILER_ISOC_LOGL
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_LOGL isoc::log
#undef _DK_SDU_MIP__BASE__COMPILER_ISOC_LOG2L
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_LOG2L isoc::log2
#undef _DK_SDU_MIP__BASE__COMPILER_ISOC_LOG10L
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_LOG10L isoc::log10
#undef _DK_SDU_MIP__BASE__COMPILER_ISOC_POWL
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_POWL isoc::pow
#undef _DK_SDU_MIP__BASE__COMPILER_ISOC_SINL
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_SINL isoc::sin
#undef _DK_SDU_MIP__BASE__COMPILER_ISOC_COSL
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_COSL isoc::cos
#undef _DK_SDU_MIP__BASE__COMPILER_ISOC_ASINL
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_ASINL isoc::asin
#undef _DK_SDU_MIP__BASE__COMPILER_ISOC_ACOSL
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_ACOSL isoc::acos
#undef _DK_SDU_MIP__BASE__COMPILER_ISOC_SINHL
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_SINHL isoc::sinh
#undef _DK_SDU_MIP__BASE__COMPILER_ISOC_COSHL
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_COSHL isoc::cosh
#undef _DK_SDU_MIP__BASE__COMPILER_ISOC_ASINHL
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_ASINHL isoc::asinh
#undef _DK_SDU_MIP__BASE__COMPILER_ISOC_ACOSHL
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_ACOSHL isoc::acosh
#undef _DK_SDU_MIP__BASE__COMPILER_ISOC_ATANL
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_ATANL isoc::atan
#undef _DK_SDU_MIP__BASE__COMPILER_ISOC_TANHL
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_TANHL isoc::tanh
#undef _DK_SDU_MIP__BASE__COMPILER_ISOC_ATANHL
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_ATANHL isoc::atanh
#undef _DK_SDU_MIP__BASE__COMPILER_ISOC_ATAN2L
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_ATAN2L isoc::atan2

#endif
