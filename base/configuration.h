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

#include <base/platforms/symbols.h>

#if defined(__doxygen__)
#  define _COM_AZURE_DEV__BASE__COMPILER _COM_AZURE_DEV__BASE__COMPILER_DOXYGEN
#elif defined(__clang__)
#  define _COM_AZURE_DEV__BASE__COMPILER _COM_AZURE_DEV__BASE__COMPILER_LLVM
#elif defined(_MSC_VER)
#  define _COM_AZURE_DEV__BASE__COMPILER _COM_AZURE_DEV__BASE__COMPILER_MSC
#elif defined(__GNUG__)
#  define _COM_AZURE_DEV__BASE__COMPILER _COM_AZURE_DEV__BASE__COMPILER_GCC
#else
#  error Unsupported compiler.
#endif

#define _COM_AZURE_DEV__BASE__DEFAULT_MEMORY

#if defined(_WIN32)
#  define _COM_AZURE_DEV__BASE__FLAVOR _COM_AZURE_DEV__BASE__WIN32
#else
#  define _COM_AZURE_DEV__BASE__FLAVOR _COM_AZURE_DEV__BASE__UNIX
#endif

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  define _COM_AZURE_DEV__BASE__OS _COM_AZURE_DEV__BASE__WXP
#elif defined(__APPLE__)
#if __is_target_os(ios)
#  define _COM_AZURE_DEV__BASE__OS _COM_AZURE_DEV__BASE__IOS
#else
#  define _COM_AZURE_DEV__BASE__OS _COM_AZURE_DEV__BASE__MACOS
#endif
#elif defined(__linux__)
#  define _COM_AZURE_DEV__BASE__OS _COM_AZURE_DEV__BASE__GNULINUX
#elif defined(__sun__)
#  define _COM_AZURE_DEV__BASE__OS _COM_AZURE_DEV__BASE__SOLARIS
#  undef _XOPEN_SOURCE
#  define _XOPEN_SOURCE 600 // SUSv3/POSIX.1-2001 // see man standards
#  undef _XOPEN_SOURCE_EXTENDED
#  define _XOPEN_SOURCE_EXTENDED
#elif defined(__FreeBSD__)
#  define _COM_AZURE_DEV__BASE__OS _COM_AZURE_DEV__BASE__FREEBSD
#elif defined(__OpenBSD__)
#  define _COM_AZURE_DEV__BASE__OS _COM_AZURE_DEV__BASE__OPENBSD
#elif defined(__wasi__)
#  define _COM_AZURE_DEV__BASE__OS _COM_AZURE_DEV__BASE__WASI
#elif defined(__EMSCRIPTEN__)
#  define _COM_AZURE_DEV__BASE__OS _COM_AZURE_DEV__BASE__EMCC
#else
#  error Unsupported OS.
#endif

#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC)
#if defined(_M_AMD64)
#  define _COM_AZURE_DEV__BASE__ARCH _COM_AZURE_DEV__BASE__X86_64
#elif defined(_M_ARM64)
#  define _COM_AZURE_DEV__BASE__ARCH _COM_AZURE_DEV__BASE__ARM64
#elif defined(_M_ARM)
#  define _COM_AZURE_DEV__BASE__ARCH _COM_AZURE_DEV__BASE__ARM32
#elif defined(_M_IX86)
#  define _COM_AZURE_DEV__BASE__ARCH _COM_AZURE_DEV__BASE__X86
#else
#  error Unsupported arch.
#endif
#elif defined(__aarch64__)
#  define _COM_AZURE_DEV__BASE__ARCH _COM_AZURE_DEV__BASE__ARM64
#elif defined(__arm__)
#  define _COM_AZURE_DEV__BASE__ARCH _COM_AZURE_DEV__BASE__ARM32
#elif defined(__sparc__) && defined(__arch64__)
#  define _COM_AZURE_DEV__BASE__ARCH _COM_AZURE_DEV__BASE__SPARC64
#elif defined(__sparc__)
#  define _COM_AZURE_DEV__BASE__ARCH _COM_AZURE_DEV__BASE__SPARC32
#elif defined(__powerpc__)
#  define _COM_AZURE_DEV__BASE__ARCH _COM_AZURE_DEV__BASE__POWERPC32
#elif defined(__powerpc64__)
#  define _COM_AZURE_DEV__BASE__ARCH _COM_AZURE_DEV__BASE__POWERPC64
#elif defined(__s390x__)
#  define _COM_AZURE_DEV__BASE__ARCH _COM_AZURE_DEV__BASE__S390X
#elif defined(__mips__)
#  define _COM_AZURE_DEV__BASE__ARCH _COM_AZURE_DEV__BASE__MIPS
#elif defined(__amd64__)
#  define _COM_AZURE_DEV__BASE__ARCH _COM_AZURE_DEV__BASE__X86_64
#elif defined(__i386__)
#  define _COM_AZURE_DEV__BASE__ARCH _COM_AZURE_DEV__BASE__X86
#elif defined(__wasi__)
#  define _COM_AZURE_DEV__BASE__ARCH _COM_AZURE_DEV__BASE__WASM32
#elif defined(__EMSCRIPTEN__)
#  define _COM_AZURE_DEV__BASE__ARCH _COM_AZURE_DEV__BASE__WASM32
#else
#  error Unsupported arch.
#endif

#if (_COM_AZURE_DEV__BASE__ARCH != _COM_AZURE_DEV__BASE__X86) && \
    (_COM_AZURE_DEV__BASE__ARCH != _COM_AZURE_DEV__BASE__SPARC32) && \
    (_COM_AZURE_DEV__BASE__ARCH != _COM_AZURE_DEV__BASE__POWERPC32) && \
    (_COM_AZURE_DEV__BASE__ARCH != _COM_AZURE_DEV__BASE__MIPS) && \
    (_COM_AZURE_DEV__BASE__ARCH != _COM_AZURE_DEV__BASE__ARM32) && \
    (_COM_AZURE_DEV__BASE__ARCH != _COM_AZURE_DEV__BASE__WASM32) && \
    (defined(__SIZEOF_POINTER__) && (__SIZEOF_POINTER__ == 8))
#  define _COM_AZURE_DEV__BASE__ARCH_64BIT 1
#else
#  define _COM_AZURE_DEV__BASE__ARCH_64BIT 0
#endif

#define _COM_AZURE_DEV__BASE__EXTENSIONS
#define _COM_AZURE_DEV__BASE__NAMESPACE
#define _COM_AZURE_DEV__BASE__CPP_BOOL
#define _COM_AZURE_DEV__BASE__CPP_CONST
#define _COM_AZURE_DEV__BASE__CPP_INLINE
#define _COM_AZURE_DEV__BASE__CPP_STATIC_CONST
#define _COM_AZURE_DEV__BASE__CPP_ANONYMOUS
#define _COM_AZURE_DEV__BASE__CPP_SPECIFICATION

#define _COM_AZURE_DEV__BASE__CHAR_SIZE 1
#define _COM_AZURE_DEV__BASE__SHORT_SIZE 2
#define _COM_AZURE_DEV__BASE__INT_SIZE 4
#if (_COM_AZURE_DEV__BASE__FLAVOR != _COM_AZURE_DEV__BASE__WIN32) && \
    (defined(__SIZEOF_LONG__) && (__SIZEOF_LONG__ == 8))
#  define _COM_AZURE_DEV__BASE__LONG_SIZE 8
#else
#  define _COM_AZURE_DEV__BASE__LONG_SIZE 4
#endif
#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32) || \
    (defined(__SIZEOF_LONG_LONG__) && (__SIZEOF_LONG_LONG__ == 8))
#  define _COM_AZURE_DEV__BASE__LONG_LONG_SIZE 8
#else
#  define _COM_AZURE_DEV__BASE__LONG_LONG_SIZE 4
#endif

#if _COM_AZURE_DEV__BASE__ARCH_64BIT
#  define _COM_AZURE_DEV__BASE__POINTER_SIZE 8
#else
#  define _COM_AZURE_DEV__BASE__POINTER_SIZE 4
#endif

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  define _COM_AZURE_DEV__BASE__WCHAR_SIZE 2
#else
#  define _COM_AZURE_DEV__BASE__WCHAR_SIZE 4
#endif

#define _COM_AZURE_DEV__BASE__BYTE_ORDER _COM_AZURE_DEV__BASE__LITTLE_ENDIAN
#define _COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER _COM_AZURE_DEV__BASE__LITTLE_ENDIAN

#if defined(__SIZEOF_FLOAT__)
#  if (__SIZEOF_FLOAT__ == 4)
#    if defined(__FLT_MANT_DIG__) && (__FLT_MANT_DIG__ == 24)
#      define _COM_AZURE_DEV__BASE__FLOAT _COM_AZURE_DEV__BASE__IEEE_754_SINGLE_PRECISION
#    endif
#  elif (__SIZEOF_FLOAT__ == 8)
#    if defined(__FLT_MANT_DIG__) && (__FLT_MANT_DIG__ == 53)
#      define _COM_AZURE_DEV__BASE__FLOAT _COM_AZURE_DEV__BASE__IEEE_754_DOUBLE_PRECISION
#    endif
#  elif (__SIZEOF_FLOAT__ == 16)
#   if defined(__FLT_MANT_DIG__) && (__FLT_MANT_DIG__ == 113)
#     define _COM_AZURE_DEV__BASE__FLOAT _COM_AZURE_DEV__BASE__IEEE_QUADRUPLE_PRECISION
#   elif defined(__FLT_MANT_DIG__) && (__FLT_MANT_DIG__ == 53)
#     define _COM_AZURE_DEV__BASE__FLOAT _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16
#   else
#     error Unsupported float type
#   endif
#  else
#    error Unsupported float type
#  endif
#endif
#if !defined(_COM_AZURE_DEV__BASE__FLOAT)
#  define _COM_AZURE_DEV__BASE__FLOAT _COM_AZURE_DEV__BASE__IEEE_754_SINGLE_PRECISION
#endif

#if defined(__SIZEOF_DOUBLE__)
#  if (__SIZEOF_DOUBLE__ == 4)
#    if defined(__DBL_MANT_DIG__) && (__DBL_MANT_DIG__ == 24)
#      define _COM_AZURE_DEV__BASE__DOUBLE _COM_AZURE_DEV__BASE__IEEE_754_SINGLE_PRECISION
#    endif
#  elif (__SIZEOF_DOUBLE__ == 8)
#    if defined(__DBL_MANT_DIG__) && (__DBL_MANT_DIG__ == 53)
#      define _COM_AZURE_DEV__BASE__DOUBLE _COM_AZURE_DEV__BASE__IEEE_754_DOUBLE_PRECISION
#    endif
#  elif (__SIZEOF_DOUBLE__ == 16)
#   if defined(__DBL_MANT_DIG__) && (__DBL_MANT_DIG__ == 113)
#     define _COM_AZURE_DEV__BASE__DOUBLE _COM_AZURE_DEV__BASE__IEEE_QUADRUPLE_PRECISION
#   elif defined(__DBL_MANT_DIG__) && (__DBL_MANT_DIG__ == 53)
#     define _COM_AZURE_DEV__BASE__DOUBLE _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16
#   else
#     error Unsupported double type
#   endif
#  else
#    error Unsupported double type
#  endif
#endif
#if !defined(_COM_AZURE_DEV__BASE__DOUBLE)
#  define _COM_AZURE_DEV__BASE__DOUBLE _COM_AZURE_DEV__BASE__IEEE_754_DOUBLE_PRECISION
#endif

#if defined(__SIZEOF_LONG_DOUBLE__)
#  if (__SIZEOF_LONG_DOUBLE__ == 4)
#    if defined(__LDBL_MANT_LDIG__) && (__LDBL_MANT_LDIG__ == 24)
#      define _COM_AZURE_DEV__BASE__LONG_DOUBLE _COM_AZURE_DEV__BASE__IEEE_754_SINGLE_PRECISION
#    endif
#  elif (__SIZEOF_LONG_DOUBLE__ == 8)
#    if defined(__LDBL_MANT_DIG__) && (__LDBL_MANT_DIG__ == 53)
#      define _COM_AZURE_DEV__BASE__LONG_DOUBLE _COM_AZURE_DEV__BASE__IEEE_754_DOUBLE_PRECISION
#    endif
#  elif (__SIZEOF_LONG_DOUBLE__ == 16) // HP-UX+Solaris sparc
#    if defined(__LDBL_MANT_DIG__) && (__LDBL_MANT_DIG__ == 113)
#      define _COM_AZURE_DEV__BASE__LONG_DOUBLE _COM_AZURE_DEV__BASE__IEEE_QUADRUPLE_PRECISION
#    elif defined(__LDBL_MANT_DIG__) && (__LDBL_MANT_DIG__ == 53)
#      define _COM_AZURE_DEV__BASE__LONG_DOUBLE _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16
#    else
#      error Unsupported long double type
#    endif
#  else
#    error Unsupported long double type
#  endif
#endif
#if !defined(_COM_AZURE_DEV__BASE__LONG_DOUBLE)
#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC) || \
    (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__ARM32) || \
    (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__ARM64)
#  define _COM_AZURE_DEV__BASE__LONG_DOUBLE _COM_AZURE_DEV__BASE__IEEE_754_DOUBLE_PRECISION
#elif (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__WASM32) || \
      (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__WASM64)
#  define _COM_AZURE_DEV__BASE__LONG_DOUBLE _COM_AZURE_DEV__BASE__IEEE_QUADRUPLE_PRECISION
#else
#  define _COM_AZURE_DEV__BASE__LONG_DOUBLE _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16
#endif
#endif

#define _COM_AZURE_DEV__BASE__HAVE_MEMCHR
#define _COM_AZURE_DEV__BASE__HAVE_MEMCMP
#define _COM_AZURE_DEV__BASE__HAVE_MEMCPY
#define _COM_AZURE_DEV__BASE__HAVE_MEMMOVE
#define _COM_AZURE_DEV__BASE__HAVE_MEMSET

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#if defined(_COM_AZURE_DEV__BASE__SHARED_LIBRARY_BUILD)
#  define _COM_AZURE_DEV__BASE__API __declspec(dllexport)
#elif defined(_COM_AZURE_DEV__BASE__SHARED_LIBRARY)
#  define _COM_AZURE_DEV__BASE__API __declspec(dllimport)
#endif
#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_GCC)
// __attribute__((dllexport))
// __attribute__((dllimport))
#endif
#else
#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_GCC)
#if defined(_COM_AZURE_DEV__BASE__SHARED_LIBRARY_BUILD)
// #  define _COM_AZURE_DEV__BASE__API __attribute__((visibility("default")))
#  define _COM_AZURE_DEV__BASE__PRIVATE_API __attribute__((visibility("hidden")))
#elif defined(_COM_AZURE_DEV__BASE__SHARED_LIBRARY)
#  define _COM_AZURE_DEV__BASE__API
#endif
#endif
#endif

#if !defined(_COM_AZURE_DEV__BASE__API)
#  define _COM_AZURE_DEV__BASE__API
#endif

#if !defined(_COM_AZURE_DEV__BASE__PRIVATE_API)
#  define _COM_AZURE_DEV__BASE__PRIVATE_API
#endif

#define _COM_AZURE_DEV__BASE__ALLOW_DEPRECATED

#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_LLVM)
#  define _COM_AZURE_DEV__BASE__PRETTY_FUNCTION __PRETTY_FUNCTION__
#elif (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC)
#  define _COM_AZURE_DEV__BASE__PRETTY_FUNCTION __FUNCSIG__
#else
#  define _COM_AZURE_DEV__BASE__PRETTY_FUNCTION __func__
#endif
