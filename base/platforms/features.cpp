/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>

#if ((_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__UNIX) || \
     (_DK_SDU_MIP__BASE__FLAVOR == _DK_SDU_MIP__BASE__WIN32))
#else
#  error Invalid flavor (FLAVOR) specification
#endif

#if ((_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__UNSPECIFIED) || \
     (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__GNULINUX) || \
     (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__IRIX65) || \
     (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__SOLARIS) || \
     (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__CYGWIN) || \
     (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__WINNT4) || \
     (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__W2K) || \
     (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__WXP) || \
     (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__BEOS) || \
     (_DK_SDU_MIP__BASE__OS == _DK_SDU_MIP__BASE__AIX))
#else
#  error Invalid operating system (OS) specification
#endif

#if ((_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__UNSPECIFIED) || \
     (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__X86) || \
     (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__SPARC) || \
     (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__MIPS) || \
     (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__ITANIUM) || \
     (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__ALPHA) || \
     (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__PPC) || \
     (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__RS) || \
     (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__X86_64) || \
     (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__S390))
#else
#  error Invalid architecture (ARCH) specification
#endif

#if (defined(_DK_SDU_MIP__BASE__EXPERIMENTAL))
#  warning Experimental components have been enabled
#endif

#if (!defined(_DK_SDU_MIP__BASE__CPP_BOOL))
#  error Compiler does not support the bool primitive
#endif

#if (!defined(_DK_SDU_MIP__BASE__CPP_CONST))
#  error Compiler does not support the const qualifier
#endif

#if (!defined(_DK_SDU_MIP__BASE__CPP_RESTRICT) && !defined(restrict))
#  if defined(_DK_SDU_MIP__BASE__COMPILER_RESTRICT)
#    warning Using alternative restrict qualifier keyword
#  else
#    warning Compiler does not support restrict qualifier
#  endif
#endif

#if (!defined(_DK_SDU_MIP__BASE__CPP_INLINE) && !defined(inline))
#  if defined(_DK_SDU_MIP__BASE__COMPILER_INLINE)
#    warning Using alternative inline specifier keyword
#  else
#    warning Compiler does not support inline specifier
#  endif
#endif

#if (!defined(_DK_SDU_MIP__BASE__CPP_STATIC_CONST))
#  error Compiler does not support initialization of static constants inside class definition
#endif

#if (!defined(_DK_SDU_MIP__BASE__CPP_ANONYMOUS))
#  warning Compiler does not support anonymous structures (and unions)
#endif

#if (!defined(_DK_SDU_MIP__BASE__CPP_SPECIFICATION))
#  error Compiler does not support exception specifications
#endif

#if (_DK_SDU_MIP__BASE__COMPILER_ABI != _DK_SDU_MIP__BASE__ABI)
#  warning ABI of compiler is different from configured ABI
#endif

#if (!defined(_DK_SDU_MIP__BASE__ARCH_ABI))
#  warning Unsupported architecture ABI
#endif

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
