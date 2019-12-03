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

/*
  This header file defines the global symbols that are used to configure the
  framework.

  The following symbols must be defined by the private and public configuration
  header.

    // Specifies the flavor
    _COM_AZURE_DEV__BASE__FLAVOR

    // Specifies the compiler
    _COM_AZURE_DEV__BASE__COMPILER
    
    // Specifies the ABI
    _COM_AZURE_DEV__BASE__ABI
    // Specifies the ABI flavor
    _COM_AZURE_DEV__BASE__ABI_FLAVOR
    
    // Specifies the demangling API
    _COM_AZURE_DEV__BASE__DEMANGLE
    
    // Specifies the operating-system
    _COM_AZURE_DEV__BASE__OS
    
    // Specifies the architecture (major)
    _COM_AZURE_DEV__BASE__ARCH
    // Specifies the architecture (minor)
    _COM_AZURE_DEV__BASE__ARCH_MINOR
    
    // Specifies the native byte order
    _COM_AZURE_DEV__BASE__BYTE_ORDER
    // Specifies the word order of floating-point representations
    _COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER
    
    // Specifies that char is unsigned
    _COM_AZURE_DEV__BASE__UNSIGNED_CHAR
    
    // Specifies the representation of float
    _COM_AZURE_DEV__BASE__FLOAT
    // Specifies the representation of double
    _COM_AZURE_DEV__BASE__DOUBLE
    // Specifies the representation of long double
    _COM_AZURE_DEV__BASE__LONG_DOUBLE
*/

#define _COM_AZURE_DEV__BASE__UNSPECIFIED 0

// Named built-in primitives
#define _COM_AZURE_DEV__BASE__CHAR 1
#define _COM_AZURE_DEV__BASE__UNSIGNED_CHAR 2
#define _COM_AZURE_DEV__BASE__SHORT 3
#define _COM_AZURE_DEV__BASE__UNSIGNED_SHORT 4
#define _COM_AZURE_DEV__BASE__INT 5
#define _COM_AZURE_DEV__BASE__UNSIGNED_INT 6
#define _COM_AZURE_DEV__BASE__LONG 7
#define _COM_AZURE_DEV__BASE__UNSIGNED_LONG 8
#define _COM_AZURE_DEV__BASE__LONG_LONG 9
#define _COM_AZURE_DEV__BASE__UNSIGNED_LONG_LONG 10

// Supported flavors
#define _COM_AZURE_DEV__BASE__UNIX 1 // selects The Single UNIX Specification 2
#define _COM_AZURE_DEV__BASE__WIN32 2 // selects WIN32 api

// Specific compilers
#define _COM_AZURE_DEV__BASE__COMPILER_GCC 1
#define _COM_AZURE_DEV__BASE__COMPILER_SUNWSPRO 2
#define _COM_AZURE_DEV__BASE__COMPILER_MIPSPRO 3
#define _COM_AZURE_DEV__BASE__COMPILER_INTEL 4
#define _COM_AZURE_DEV__BASE__COMPILER_BORLAND 5
#define _COM_AZURE_DEV__BASE__COMPILER_MSC 6
#define _COM_AZURE_DEV__BASE__COMPILER_LLVM 7
#define _COM_AZURE_DEV__BASE__COMPILER_DOXYGEN 8

// Specific ABIs
#define _COM_AZURE_DEV__BASE__ABI_V3MV 1 // V3 multi vendor ABI
#define _COM_AZURE_DEV__BASE__ABI_GCCV2 2 // GCC version 2
#define _COM_AZURE_DEV__BASE__ABI_SUNWSPRO 3 // SUN Workshop Compiler
#define _COM_AZURE_DEV__BASE__ABI_MIPSPRO 4 // SGI MIPSpro compiler
#define _COM_AZURE_DEV__BASE__ABI_INTEL 5 // Intel Compiler
#define _COM_AZURE_DEV__BASE__ABI_BORLAND 6 // Borland Compiler
#define _COM_AZURE_DEV__BASE__ABI_MSC 7 // Microsoft Compiler
#define _COM_AZURE_DEV__BASE__ABI_LLVM 8 // Clang/LLVM Compiler

// Supported ABI flavors
#define _COM_AZURE_DEV__BASE__ABI_32 1 // 32 bit pointer...
#define _COM_AZURE_DEV__BASE__ABI_64 2 // 64 bit pointer...

// Demangling APIs
#define _COM_AZURE_DEV__BASE__DEMANGLE_V3MV 1 // V3 multi vendor ABI
#define _COM_AZURE_DEV__BASE__DEMANGLE_GCCV3 2 // GCC version 3
#define _COM_AZURE_DEV__BASE__DEMANGLE_GCCV23 3 // GCC in-between version 2 and 3
#define _COM_AZURE_DEV__BASE__DEMANGLE_GCCV2 4 // GCC version 2
#define _COM_AZURE_DEV__BASE__DEMANGLE_SUNWSPRO 5 // SUN Workshop Compiler
#define _COM_AZURE_DEV__BASE__DEMANGLE_MIPSPRO 6 // SGI MIPSpro compiler
#define _COM_AZURE_DEV__BASE__DEMANGLE_INTEL 7 // Intel Compiler
#define _COM_AZURE_DEV__BASE__DEMANGLE_BORLAND 8 // Borland Compiler
#define _COM_AZURE_DEV__BASE__DEMANGLE_MSC 9 // Microsoft Compiler
#define _COM_AZURE_DEV__BASE__DEMANGLE_LLVM 10 // Clang/LLVM Compiler

// Specific operating systems APIs
#define _COM_AZURE_DEV__BASE__GNULINUX 1 // only Unix
#define _COM_AZURE_DEV__BASE__IRIX65 2 // only Unix
#define _COM_AZURE_DEV__BASE__SOLARIS 3 // only Unix
#define _COM_AZURE_DEV__BASE__CYGWIN 4 // only Unix
#define _COM_AZURE_DEV__BASE__WINNT4 5 // only WIN32
#define _COM_AZURE_DEV__BASE__W2K 6 // only WIN32
#define _COM_AZURE_DEV__BASE__WXP 7 // only WIN32
#define _COM_AZURE_DEV__BASE__BEOS 8
#define _COM_AZURE_DEV__BASE__AIX 9
#define _COM_AZURE_DEV__BASE__MACOS 10 // only Unix
#define _COM_AZURE_DEV__BASE__WEBASSEMBLY 11 // only Unix

// Major architecture groups
#define _COM_AZURE_DEV__BASE__X86 1
#define _COM_AZURE_DEV__BASE__SPARC 2
#define _COM_AZURE_DEV__BASE__MIPS 3
#define _COM_AZURE_DEV__BASE__ITANIUM 4
#define _COM_AZURE_DEV__BASE__ALPHA 5
#define _COM_AZURE_DEV__BASE__PPC 6
#define _COM_AZURE_DEV__BASE__RS 7
#define _COM_AZURE_DEV__BASE__X86_64 8
#define _COM_AZURE_DEV__BASE__S390 9
#define _COM_AZURE_DEV__BASE__ARM 10
#define _COM_AZURE_DEV__BASE__WASM32 11
#define _COM_AZURE_DEV__BASE__WASM64 12

// Architectures (minor)
#define _COM_AZURE_DEV__BASE__486 1 // only with X86
#define _COM_AZURE_DEV__BASE__586 2 // only with X86
#define _COM_AZURE_DEV__BASE__686 3 // only with X86
#define _COM_AZURE_DEV__BASE__MIPSI 1
#define _COM_AZURE_DEV__BASE__MIPSII 2
#define _COM_AZURE_DEV__BASE__MIPSIII 3
#define _COM_AZURE_DEV__BASE__SPARC_V7 1
#define _COM_AZURE_DEV__BASE__SPARC_V8 2
#define _COM_AZURE_DEV__BASE__SPARC_SPARCLITE 3
#define _COM_AZURE_DEV__BASE__SPARC_SPARCLET 4
#define _COM_AZURE_DEV__BASE__SPARC_V9 5

// Supported byte orders
#define _COM_AZURE_DEV__BASE__BIG_ENDIAN 1
#define _COM_AZURE_DEV__BASE__LITTLE_ENDIAN 2

// Supported floating-point representations
#define _COM_AZURE_DEV__BASE__IEEE_754_SINGLE_PRECISION 1
#define _COM_AZURE_DEV__BASE__IEEE_754_DOUBLE_PRECISION 2
#define _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96 3
#define _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16 4
#define _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128 5
#define _COM_AZURE_DEV__BASE__IEEE_QUADRUPLE_PRECISION 6
