/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_PLATFORMS_OS_CYGWIN__CYGWIN_H
#define _DK_SDU_MIP__BASE_PLATFORMS_OS_CYGWIN__CYGWIN_H

#include <base/features.h>

#define _DK_SDU_MIP__BASE__REDIR_ISOC_FABSL isoc::fabs
#define _DK_SDU_MIP__BASE__REDIR_ISOC_CEILL isoc::ceil
#define _DK_SDU_MIP__BASE__REDIR_ISOC_FLOORL isoc::floor
#define _DK_SDU_MIP__BASE__REDIR_ISOC_ROUNDL isoc::round
#define _DK_SDU_MIP__BASE__REDIR_ISOC_TRUNCL isoc::trunc
#define _DK_SDU_MIP__BASE__REDIR_ISOC_SQRTL isoc::sqrt
#define _DK_SDU_MIP__BASE__REDIR_ISOC_CBRTL isoc::cbrt
#define _DK_SDU_MIP__BASE__REDIR_ISOC_HYPOTL isoc::hypot
#define _DK_SDU_MIP__BASE__REDIR_ISOC_EXPL isoc::exp
#define _DK_SDU_MIP__BASE__REDIR_ISOC_LOGL isoc::log
#define _DK_SDU_MIP__BASE__REDIR_ISOC_LOG2L isoc::log2
#define _DK_SDU_MIP__BASE__REDIR_ISOC_LOG10L isoc::log10
#define _DK_SDU_MIP__BASE__REDIR_ISOC_POWL isoc::pow
#define _DK_SDU_MIP__BASE__REDIR_ISOC_SINL isoc::sin
#define _DK_SDU_MIP__BASE__REDIR_ISOC_ASINL isoc::asin
#define _DK_SDU_MIP__BASE__REDIR_ISOC_COSL isoc::cos
#define _DK_SDU_MIP__BASE__REDIR_ISOC_ACOSL isoc::acos
#define _DK_SDU_MIP__BASE__REDIR_ISOC_TANL isoc::tan
#define _DK_SDU_MIP__BASE__REDIR_ISOC_ATANL isoc::atan
#define _DK_SDU_MIP__BASE__REDIR_ISOC_SINHL isoc::sinh
#define _DK_SDU_MIP__BASE__REDIR_ISOC_COSHL isoc::cosh
#define _DK_SDU_MIP__BASE__REDIR_ISOC_ASINHL isoc::asinh
#define _DK_SDU_MIP__BASE__REDIR_ISOC_ACOSHL isoc::acosh
#define _DK_SDU_MIP__BASE__REDIR_ISOC_TANHL isoc::tanh
#define _DK_SDU_MIP__BASE__REDIR_ISOC_ATANHL isoc::atanh
#define _DK_SDU_MIP__BASE__REDIR_ISOC_ATAN2L isoc::atan2

#endif
