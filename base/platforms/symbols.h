/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

/*
  This header file defines the global symbols that are used to configure the
  framework.

  The following symbols must be defined by <base/platforms/platform.h>:

    // Specifies the flavour
    _DK_SDU_MIP__BASE__FLAVOUR

    // Specifies the operating-system
    _DK_SDU_MIP__BASE__OS

    // Specifies the architecture (major)
    _DK_SDU_MIP__BASE__ARCH
    // Specifies the architecture (minor)
    _DK_SDU_MIP__BASE__ARCH_MINOR

    // Specifies the native byte order
    _DK_SDU_MIP__BASE__BYTE_ORDER
    // Specifies the word order of floating-point representations
    _DK_SDU_MIP__BASE__FLOAT_WORD_ORDER

    // Specifies that char is unsigned
    _DK_SDU_MIP__BASE__UNSIGNED_CHAR

    // Specifies the representation of float
    _DK_SDU_MIP__BASE__FLOAT
    // Specifies the representation of double
    _DK_SDU_MIP__BASE__DOUBLE
    // Specifies the representation of long double
    _DK_SDU_MIP__BASE__LONG_DOUBLE
*/

#define _DK_SDU_MIP__BASE__UNSPECIFIED 0

// Supported flavours
#define _DK_SDU_MIP__BASE__UNIX 1 // selects The Single UNIX Specification 2
#define _DK_SDU_MIP__BASE__WIN32 2 // selects WIN32 api

// Specific operating systems api's
#define _DK_SDU_MIP__BASE__GNULINUX 1 // only Unix
#define _DK_SDU_MIP__BASE__IRIX65 2 // only Unix
#define _DK_SDU_MIP__BASE__SOLARIS 3 // only Unix
#define _DK_SDU_MIP__BASE__CYGWIN 4 // only Unix
#define _DK_SDU_MIP__BASE__WINNT4 5 // only WIN32
#define _DK_SDU_MIP__BASE__W2K 6 // only WIN32

// Major architecture groups
#define _DK_SDU_MIP__BASE__X86 1
#define _DK_SDU_MIP__BASE__SPARC 2
#define _DK_SDU_MIP__BASE__MIPS 3
#define _DK_SDU_MIP__BASE__ITANIUM 4

// Architectures (minor)
#define _DK_SDU_MIP__BASE__486 1 // only with x86
#define _DK_SDU_MIP__BASE__586 2 // only with x86
#define _DK_SDU_MIP__BASE__686 3 // only with x86
#define _DK_SDU_MIP__BASE__MIPSI 1 // only with MIPS
#define _DK_SDU_MIP__BASE__MIPSII 2 // only with MIPS
#define _DK_SDU_MIP__BASE__MIPSIII 3 // only with MIPS

// Supported byte orders
#define _DK_SDU_MIP__BASE__BIG_ENDIAN 1
#define _DK_SDU_MIP__BASE__LITTLE_ENDIAN 2

// Supported floating-point representations
#define _DK_SDU_MIP__BASE__IEEE_754_SINGLE_PRECISION 1
#define _DK_SDU_MIP__BASE__IEEE_754_DOUBLE_PRECISION 2
#define _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96 3
#define _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128 4
#define _DK_SDU_MIP__BASE__IEEE_QUADRUPLE_PRECISION 5
