/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/features.h>

#if ((_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__UNIX) || \
     (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32))
#else
#  error Invalid flavor (FLAVOR) specification
#endif

#if ((_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__UNSPECIFIED) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__IRIX65) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__SOLARIS) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__CYGWIN) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WINNT4) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__W2K) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WXP) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__BEOS) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__AIX) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__FREEBSD) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__OPENBSD) || \
     (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__WEBASSEMBLY))
#else
#  error Invalid operating system (OS) specification
#endif

#if ((_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__UNSPECIFIED) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__X86) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__SPARC) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__MIPS) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__ITANIUM) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__ALPHA) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__PPC) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__RS) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__X86_64) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__S390) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__ARM) || \
     (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__ARM64))
#else
#  error Invalid architecture (ARCH) specification
#endif

#if (!defined(_COM_AZURE_DEV__BASE__CPP_BOOL))
#  error Compiler does not support the bool primitive
#endif

#if (!defined(_COM_AZURE_DEV__BASE__CPP_CONST))
#  error Compiler does not support the const qualifier
#endif

#if (!defined(_COM_AZURE_DEV__BASE__CPP_RESTRICT) && !defined(restrict))
#  if defined(_COM_AZURE_DEV__BASE__COMPILER_RESTRICT)
#    warning Using alternative restrict qualifier keyword
#  else
#    warning Compiler does not support restrict qualifier
#  endif
#endif

#if (!defined(_COM_AZURE_DEV__BASE__CPP_INLINE) && !defined(inline))
#  if defined(_COM_AZURE_DEV__BASE__COMPILER_INLINE)
#    warning Using alternative inline specifier keyword
#  else
#    warning Compiler does not support inline specifier
#  endif
#endif

#if (!defined(_COM_AZURE_DEV__BASE__CPP_STATIC_CONST))
#  error Compiler does not support initialization of static constants inside class definition
#endif

#if (!defined(_COM_AZURE_DEV__BASE__CPP_ANONYMOUS))
#  warning Compiler does not support anonymous structures (and unions)
#endif

#if (!defined(_COM_AZURE_DEV__BASE__CPP_SPECIFICATION))
#  error Compiler does not support exception specifications
#endif

#if 0
#if (_COM_AZURE_DEV__BASE__COMPILER_ABI != _COM_AZURE_DEV__BASE__ABI)
#  warning ABI of compiler is different from configured ABI
#endif

#if (!defined(_COM_AZURE_DEV__BASE__ARCH_ABI))
#  warning Unsupported architecture ABI
#endif
#endif

_COM_AZURE_DEV__BASE__DUMMY_SYMBOL

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
