/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_PLATFORMS_COMPILER_GCC__GCC_H
#define _DK_SDU_MIP__BASE_PLATFORMS_COMPILER_GCC__GCC_H

namespace base {

#define _DK_SDU_MIP__BASE__COMPILER _DK_SDU_MIP__BASE__GCC
#define _DK_SDU_MIP__BASE__COMPILER_VERSION __GNUC__
#define _DK_SDU_MIP__BASE__COMPILER_NAMESPACE compiler::org::gnu

#define _DK_SDU_MIP__BASE__FUNCTION __PRETTY_FUNCTION__

#define _DK_SDU_MIP__BASE__ALIGNED(alignment) __attribute__ ((aligned (alignment)))
#define _DK_SDU_MIP__BASE__DEPRECATED __attribute__ ((deprecated))
#define _DK_SDU_MIP__BASE__NORETURN __attribute__ ((noreturn))
#define _DK_SDU_MIP__BASE__PACKED __attribute__ ((packed))
#define _DK_SDU_MIP__BASE__SECTION(name) __attribute__ ((section (name)))
#define _DK_SDU_MIP__BASE__UNUSED __attribute__ ((unused))
#define _DK_SDU_MIP__BASE__WEAK __attribute__ ((weak))
#define _DK_SDU_MIP__BASE__FORCE_INLINE __attribute__ ((always_inline))

#define _DK_SDU_MIP__BASE__COMPILER_INLINE __inline
#define _DK_SDU_MIP__BASE__COMPILER_RESTRICT __restrict

#define _DK_SDU_MIP__BASE__COMPILER_ISOC_FABSF __builtin_fabsf
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_FABS __builtin_fabs
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_FABSL __builtin_fabsl
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_COSF __builtin_cosf
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_COS __builtin_cos
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_COSL __builtin_cosl
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_SINF __builtin_sinf
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_SIN __builtin_sin
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_SINL __builtin_sinl
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_SQRTF __builtin_sqrtf
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_SQRT __builtin_sqrt
#define _DK_SDU_MIP__BASE__COMPILER_ISOC_SQRTL __builtin_sqrtl

#if (_DK_SDU_MIP__BASE__COMPILER_VERSION >= 3)
#  define _DK_SDU_MIP__BASE__COMPILER_ABI _DK_SDU_MIP__BASE__ABI_V3MV
#elif (_DK_SDU_MIP__BASE__COMPILER_VERSION == 2)
#  define _DK_SDU_MIP__BASE__COMPILER_ABI _DK_SDU_MIP__BASE__ABI_GCC2
#endif

#if (_DK_SDU_MIP__BASE__COMPILER_VERSION >= 3)
#  if (_DK_SDU_MIP__BASE__ARCH == _DK_SDU_MIP__BASE__X86)
#    if defined(_DK_SDU_MIP__BASE__ARCH_MMX_EXTENSION)
#      define _DK_SDU_MIP__BASE__MMX_EXTENSION _DK_SDU_MIP__BASE__ARCH_MMX_EXTENSION
#    endif
#    if defined(_DK_SDU_MIP__BASE__ARCH_SSE_EXTENSION)
#      define _DK_SDU_MIP__BASE__SSE_EXTENSION _DK_SDU_MIP__BASE__ARCH_SSE_EXTENSION
#    endif
#  endif
#endif

  namespace compiler {
    
    namespace org {
      
      namespace gnu {

#if defined(_DK_SDU_MIP__BASE__COMPILER_MMX_EXTENSION)
        /** MMX extension data type. */
        typedef int v8qi __attribute__ ((mode (V8QI)));
        /** MMX extension data type. */
        typedef int v4hi __attribute__ ((mode (V4HI)));
        /** MMX extension data type. */
        typedef int v2si __attribute__ ((mode (V2SI)));
#endif

#if defined(_DK_SDU_MIP__BASE__COMPILER_SSE_EXTENSION)
        /** Streaming SIMD Extensions (SSE) data type. */
        typedef int sse64 __attribute__ ((mode (V4HI)));
#endif
    
#if defined(_DK_SDU_MIP__BASE__COMPILER_SSE2_EXTENSION)
        /** Vector of 4 32 bit floating point values (SSE2). */
        typedef int v4sf __attribute__ ((mode (V4SF)));
        /** Vector of 4 32 bit integers (SSE2). */
        typedef int v4si __attribute__ ((mode (V4SI)));    
        /** Streaming SIMD Extensions (SSE2) data type. */
        typedef int sse128 __attribute__ ((mode (V4SF)));
#endif
    
#if defined(_DK_SDU_MIP__BASE__COMPILER__SUPPORTS_128)
        /** 128 bit signed integer. */
        typedef int int128 __attribute__ ((mode (TI)));
        /** 128 bit unsigned integer. */
        typedef unsigned int uint128 __attribute__ ((mode (TI)));
#endif
    
        /** 8 bit signed integer. */
        typedef int int8 __attribute__ ((mode (QI)));
        /** 8 bit unsigned integer. */
        typedef unsigned int uint8  __attribute__ ((mode (QI)));
        /** 16 bit signed integer. */
        typedef int int16 __attribute__ ((mode (HI)));
        /** 16 bit unsigned integer. */
        typedef unsigned int uint16 __attribute__ ((mode (HI)));
        /** 32 bit signed integer. */
        typedef int int32 __attribute__ ((mode (SI)));
        /** 32 bit unsigned integer. */
        typedef unsigned int uint32 __attribute__ ((mode (SI)));
        /** 64 bit signed integer. */
        typedef int int64 __attribute__ ((mode (DI)));
        /** 64 bit unsigned integer. */
        typedef unsigned int uint64 __attribute__ ((mode (DI)));
        /** 32 bit floating point data type. */
        typedef float float32 __attribute__ ((mode (SF)));
        /** 64 bit floating point data type. */
        typedef float float64 __attribute__ ((mode (DF)));
        /** Pointer difference. */
        typedef int diff __attribute__ ((mode (__pointer__)));
        /** Offset data type. */
        typedef unsigned int size __attribute__ ((mode (__pointer__)));
        
      }; // end of namespace gnu
  
    }; // end of namespace org
 
  }; // end of namespace compiler

  // import GCC data types
  using compiler::org::gnu;
  
}; // end of namespace base

#endif
