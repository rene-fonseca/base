/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>

#if !defined(_DK_SDU_MIP__BASE__CPP_BOOL)
#  error Compiler does not support the bool primitive
#endif

#if !defined(_DK_SDU_MIP__BASE__CPP_CONST)
#  error Compiler does not support the const qualifier
#endif

#if !defined(_DK_SDU_MIP__BASE__CPP_RESTRICT)
#  if defined(_DK_SDU_MIP__BASE__COMPILER_RESTRICT)
#    warning Using alternative restrict qualifier keyword
#  else
#    warning Compiler does not support restrict qualifier
#  endif
#endif

#if !defined(_DK_SDU_MIP__BASE__CPP_INLINE)
#  if defined(_DK_SDU_MIP__BASE__COMPILER_INLINE)
#    warning Using alternative inline specifier keyword
#  else
#    warning Compiler does not support inline specifier
#  endif
#endif

#if !defined(_DK_SDU_MIP__BASE__CPP_STATIC_CONST)
#  error Compiler does not support initialization of static constants inside class definition
#endif

#if !defined(_DK_SDU_MIP__BASE__CPP_ANONYMOUS)
#  warning Compiler does not support anonymous structures (and unions)
#endif

#if !defined(_DK_SDU_MIP__BASE__CPP_SPECIFICATION)
#  error Compiler does not support exception specifications
#endif

#if (_DK_SDU_MIP__BASE__COMPILER_ABI != _DK_SDU_MIP__BASE__ABI)
#  warning ABI of compiler is different from configured ABI
#endif

#if !defined(_DK_SDU_MIP__BASE__ARCH_ABI)
#  warning Unsupported architecture ABI
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
