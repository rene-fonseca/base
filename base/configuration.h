// TAG: temporary solution until automatic conguration works

/* This is an automatically generated file. Do NOT edit. */

#define _DK_SDU_MIP__BASE__CONFIGURE "aaa"
#define _DK_SDU_MIP__BASE__BUILD_DATE "bbb"
// #define _DK_SDU_MIP__BASE__RELEASE "ggg"

#if defined(__clang__)
#  define _DK_SDU_MIP__BASE__COMPILER _DK_SDU_MIP__BASE__COMPILER_CLANG
#elif defined(_MSC_VER)
#  define _DK_SDU_MIP__BASE__COMPILER _DK_SDU_MIP__BASE__COMPILER_MSC
#elif defined(__GNUG__)
#  define _DK_SDU_MIP__BASE__COMPILER _DK_SDU_MIP__BASE__GCC
#else
#  error "Unsupported compiler."
#endif

#define _DK_SDU_MIP__BASE__DEFAULT_MEMORY

#if defined(_WIN32)
#  define _DK_SDU_MIP__BASE__FLAVOR _DK_SDU_MIP__BASE__WIN32
#elif defined(__APPLE__)
#  define _DK_SDU_MIP__BASE__FLAVOR _DK_SDU_MIP__BASE__UNIX
#else
#  error "Unsupported flavor."
#endif

#if defined(_WIN32)
#  define _DK_SDU_MIP__BASE__OS _DK_SDU_MIP__BASE__WXP
#elif defined(__APPLE__)
#  define _DK_SDU_MIP__BASE__OS _DK_SDU_MIP__BASE__MACOS
#elif defined(__linux__)
#  define _DK_SDU_MIP__BASE__OS _DK_SDU_MIP__BASE__GNULINUX
#else
#  error "Unsupported OS."
#endif
#  define _DK_SDU_MIP__BASE__OS_MINOR 0

#define _DK_SDU_MIP__BASE__ARCH _DK_SDU_MIP__BASE__X86_64
// #define _DK_SDU_MIP__BASE__ARCH_MINOR _DK_SDU_MIP__BASE__xxx
#define _DK_SDU_MIP__BASE__EXPERIMENTAL
#define _DK_SDU_MIP__BASE__EXTENSIONS
#define _DK_SDU_MIP__BASE__NAMESPACE
#define _DK_SDU_MIP__BASE__CPP_BOOL
#define _DK_SDU_MIP__BASE__CPP_CONST
#define _DK_SDU_MIP__BASE__CPP_INLINE
#define _DK_SDU_MIP__BASE__CPP_STATIC_CONST
#define _DK_SDU_MIP__BASE__CPP_ANONYMOUS
#define _DK_SDU_MIP__BASE__CPP_SPECIFICATION
#define _DK_SDU_MIP__BASE__CHAR_SIZE 1
#define _DK_SDU_MIP__BASE__SHORT_SIZE 2
#define _DK_SDU_MIP__BASE__INT_SIZE 4
#define _DK_SDU_MIP__BASE__LONG_SIZE 4 // TAG: check this
#define _DK_SDU_MIP__BASE__LONG_LONG_SIZE 8
#define _DK_SDU_MIP__BASE__POINTER_SIZE 8

#if defined(_WIN32)
#  define _DK_SDU_MIP__BASE__WCHAR_SIZE 2
#else
#  define _DK_SDU_MIP__BASE__WCHAR_SIZE 4
#endif

#define _DK_SDU_MIP__BASE__BYTE_ORDER _DK_SDU_MIP__BASE__LITTLE_ENDIAN
#define _DK_SDU_MIP__BASE__FLOAT_WORD_ORDER _DK_SDU_MIP__BASE__LITTLE_ENDIAN
#define _DK_SDU_MIP__BASE__FLOAT _DK_SDU_MIP__BASE__IEEE_754_SINGLE_PRECISION
#define _DK_SDU_MIP__BASE__DOUBLE _DK_SDU_MIP__BASE__IEEE_754_DOUBLE_PRECISION
#define _DK_SDU_MIP__BASE__LONG_DOUBLE _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96
#define _DK_SDU_MIP__BASE__HAVE_MEMCHR
#define _DK_SDU_MIP__BASE__HAVE_MEMCMP
#define _DK_SDU_MIP__BASE__HAVE_MEMCPY
#define _DK_SDU_MIP__BASE__HAVE_MEMMOVE
#define _DK_SDU_MIP__BASE__HAVE_MEMSET
#define _DK_SDU_MIP__BASE__HAVE_WCSFTIME

#define _DK_SDU_MIP__BASE__PACKED
