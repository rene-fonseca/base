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

#include <base/features.h>

#if (!defined(_DK_SDU_MIP__BASE__REDIR_ISOC_LOG2L))
#  if (defined(_DK_SDU_MIP__BASE__REDIR_ISOC_LOG2))
#    define _DK_SDU_MIP__BASE__REDIR_ISOC_LOG2L _DK_SDU_MIP__BASE__REDIR_ISOC_LOG2
#  elif (defined(_DK_SDU_MIP__BASE__REDIR_ISOC_LOG2F))
#    define _DK_SDU_MIP__BASE__REDIR_ISOC_LOG2L _DK_SDU_MIP__BASE__REDIR_ISOC_LOG2F
#  else
#    define _DK_SDU_MIP__BASE__REDIR_ISOC_LOG2L isoc::log2
#  endif
#endif

#define _DK_SDU_MIP__BASE__REDIR_ISOC_FLOORL isoc::floor
#define _DK_SDU_MIP__BASE__REDIR_ISOC_EXPL isoc::exp
