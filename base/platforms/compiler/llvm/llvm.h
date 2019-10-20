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

namespace base {

// #define _COM_AZURE_DEV__BASE__COMPILER_PRIMITIVES

#define _COM_AZURE_DEV__BASE__COMPILER_VERSION __clang_major__
#define _COM_AZURE_DEV__BASE__COMPILER_NAMESPACE org::llvm::compiler

#define _COM_AZURE_DEV__BASE__FUNCTION __PRETTY_FUNCTION__

#define _COM_AZURE_DEV__BASE__ALIGNED(alignment) __attribute__ ((aligned (alignment)))
#define _COM_AZURE_DEV__BASE__DEPRECATED __attribute__ ((deprecated))
#define _COM_AZURE_DEV__BASE__NORETURN __attribute__ ((noreturn))
#define _COM_AZURE_DEV__BASE__PACKED __attribute__ ((packed))
#define _COM_AZURE_DEV__BASE__SECTION(name) __attribute__ ((section (name)))
#define _COM_AZURE_DEV__BASE__UNUSED __attribute__ ((unused))
#define _COM_AZURE_DEV__BASE__WEAK __attribute__ ((weak))
#define _COM_AZURE_DEV__BASE__FORCE_INLINE __attribute__ ((always_inline))

#define _COM_AZURE_DEV__BASE__COMPILER_INLINE __inline
#define _COM_AZURE_DEV__BASE__COMPILER_RESTRICT __restrict

// explicit calling conventions
#define _COM_AZURE_DEV__BASE__CALL_PASCAL __attribute__((stdcall))
#define _COM_AZURE_DEV__BASE__CALL_CPP __attribute__((cdecl))

#define _COM_AZURE_DEV__BASE__COMPILER_ABI _COM_AZURE_DEV__BASE__ABI_V3MV

  namespace org {
      
    namespace llvm {

      namespace compiler {
        
#if 0
#if defined(_COM_AZURE_DEV__BASE__COMPILER_MMX_EXTENSION)
        /** MMX extension data type. */
        typedef int v8qi __attribute__ ((mode (V8QI)));
        /** MMX extension data type. */
        typedef int v4hi __attribute__ ((mode (V4HI)));
        /** MMX extension data type. */
        typedef int v2si __attribute__ ((mode (V2SI)));
#endif

#if defined(_COM_AZURE_DEV__BASE__COMPILER_SSE_EXTENSION)
        /** Streaming SIMD Extensions (SSE) data type. */
        typedef int sse64 __attribute__ ((mode (V4HI)));
#endif
    
#if defined(_COM_AZURE_DEV__BASE__COMPILER_SSE2_EXTENSION)
        /** Vector of 4 32 bit floating point values (SSE2). */
        typedef int v4sf __attribute__ ((mode (V4SF)));
        /** Vector of 4 32 bit integers (SSE2). */
        typedef int v4si __attribute__ ((mode (V4SI)));
        /** Streaming SIMD Extensions (SSE2) data type. */
        typedef int sse128 __attribute__ ((mode (V4SF)));
#endif
    
#if defined(_COM_AZURE_DEV__BASE__COMPILER__SUPPORTS_128)
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
#endif

      } // end of namespace compiler
  
    } // end of namespace llvm
 
  } // end of namespace org

  // import LLVM data types
  using namespace org::llvm::compiler;

} // end of namespace base
