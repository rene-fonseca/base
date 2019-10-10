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

#if (!defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_LOG2L))
#  if (defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_LOG2))
#    define _COM_AZURE_DEV__BASE__REDIR_ISOC_LOG2L _COM_AZURE_DEV__BASE__REDIR_ISOC_LOG2
#  elif (defined(_COM_AZURE_DEV__BASE__REDIR_ISOC_LOG2F))
#    define _COM_AZURE_DEV__BASE__REDIR_ISOC_LOG2L _COM_AZURE_DEV__BASE__REDIR_ISOC_LOG2F
#  else
#    define _COM_AZURE_DEV__BASE__REDIR_ISOC_LOG2L isoc::log2
#  endif
#endif

#define _COM_AZURE_DEV__BASE__REDIR_ISOC_FLOORL isoc::floor
#define _COM_AZURE_DEV__BASE__REDIR_ISOC_EXPL isoc::exp
