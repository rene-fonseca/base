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

#if defined(__clang__)
#  define _COM_AZURE_DEV__BASE__COMPILER _COM_AZURE_DEV__BASE__COMPILER_LLVM
#  define _COM_AZURE_DEV__BASE__DEMANGLE _COM_AZURE_DEV__BASE__DEMANGLE_LLVM
#elif defined(_MSC_VER)
#  define _COM_AZURE_DEV__BASE__COMPILER _COM_AZURE_DEV__BASE__COMPILER_MSC
#  define _COM_AZURE_DEV__BASE__DEMANGLE _COM_AZURE_DEV__BASE__DEMANGLE_MSC
#elif defined(__GNUG__)
#  define _COM_AZURE_DEV__BASE__COMPILER _COM_AZURE_DEV__BASE__COMPILER_GCC
#  define _COM_AZURE_DEV__BASE__DEMANGLE _COM_AZURE_DEV__BASE__DEMANGLE_GCCV3
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
#  define _COM_AZURE_DEV__BASE__OS _COM_AZURE_DEV__BASE__MACOS
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
#  define _COM_AZURE_DEV__BASE__OS_MINOR 0

#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC)
#if defined(_M_AMD64)
#  define _COM_AZURE_DEV__BASE__ARCH _COM_AZURE_DEV__BASE__X86_64
#elif defined(_M_ARM64)
#  define _COM_AZURE_DEV__BASE__ARCH _COM_AZURE_DEV__BASE__ARM64
#elif defined(_M_ARM)
#  define _COM_AZURE_DEV__BASE__ARCH _COM_AZURE_DEV__BASE__ARM
#elif defined(_M_IX86)
#  define _COM_AZURE_DEV__BASE__ARCH _COM_AZURE_DEV__BASE__X86
#else
#  error Unsupported arch.
#endif
#elif defined(__aarch64__)
#  define _COM_AZURE_DEV__BASE__ARCH _COM_AZURE_DEV__BASE__ARM64
#elif defined(__arm__)
#  define _COM_AZURE_DEV__BASE__ARCH _COM_AZURE_DEV__BASE__ARM
#elif defined(__powerpc__) || defined(__powerpc64__)
#  define _COM_AZURE_DEV__BASE__ARCH _COM_AZURE_DEV__BASE__PPC
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
#define _COM_AZURE_DEV__BASE__LONG_SIZE 4 // TAG: check this
#define _COM_AZURE_DEV__BASE__LONG_LONG_SIZE 8
#if (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__ARM) || \
    (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__X86)
#  define _COM_AZURE_DEV__BASE__POINTER_SIZE 4
#else
#  define _COM_AZURE_DEV__BASE__POINTER_SIZE 8
#endif

#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#  define _COM_AZURE_DEV__BASE__WCHAR_SIZE 2
#else
#  define _COM_AZURE_DEV__BASE__WCHAR_SIZE 4
#endif

#define _COM_AZURE_DEV__BASE__BYTE_ORDER _COM_AZURE_DEV__BASE__LITTLE_ENDIAN
#define _COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER _COM_AZURE_DEV__BASE__LITTLE_ENDIAN
#define _COM_AZURE_DEV__BASE__FLOAT _COM_AZURE_DEV__BASE__IEEE_754_SINGLE_PRECISION
#define _COM_AZURE_DEV__BASE__DOUBLE _COM_AZURE_DEV__BASE__IEEE_754_DOUBLE_PRECISION
#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC) || \
    (_COM_AZURE_DEV__BASE__ARCH == _COM_AZURE_DEV__BASE__ARM)
#  define _COM_AZURE_DEV__BASE__LONG_DOUBLE _COM_AZURE_DEV__BASE__IEEE_754_DOUBLE_PRECISION
#else
#  define _COM_AZURE_DEV__BASE__LONG_DOUBLE _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16
#endif
#define _COM_AZURE_DEV__BASE__HAVE_MEMCHR
#define _COM_AZURE_DEV__BASE__HAVE_MEMCMP
#define _COM_AZURE_DEV__BASE__HAVE_MEMCPY
#define _COM_AZURE_DEV__BASE__HAVE_MEMMOVE
#define _COM_AZURE_DEV__BASE__HAVE_MEMSET
#define _COM_AZURE_DEV__BASE__HAVE_WCSFTIME
#if (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__MACOS) && \
    (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__FREEBSD) && \
    (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__OPENBSD) && \
    (_COM_AZURE_DEV__BASE__OS != _COM_AZURE_DEV__BASE__WASI)
#  define _COM_AZURE_DEV__BASE__LARGE_FILE_SYSTEM
#endif

#if (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__MACOS) || \
    (_COM_AZURE_DEV__BASE__OS == _COM_AZURE_DEV__BASE__GNULINUX)
#  define _COM_AZURE_DEV__BASE__INET_IPV6
#endif


#if (_COM_AZURE_DEV__BASE__FLAVOR == _COM_AZURE_DEV__BASE__WIN32)
#if defined(_COM_AZURE_DEV__BASE__SHARED_LIBRARY_BUILD)
#  define _COM_AZURE_DEV__BASE__API __declspec(dllexport)
#elif defined(_COM_AZURE_DEV__BASE__SHARED_LIBRARY)
#  define _COM_AZURE_DEV__BASE__API __declspec(dllimport)
#endif
#else
// TAG: add other platform
// TAG: add __attribute__((visibility("default")))
#endif

#if !defined(_COM_AZURE_DEV__BASE__API)
#  define _COM_AZURE_DEV__BASE__API
#endif
