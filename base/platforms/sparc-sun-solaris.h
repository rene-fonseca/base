/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#define _DK_SDU_MIP__BASE__FLAVOUR _DK_SDU_MIP__BASE__UNIX
#define _DK_SDU_MIP__BASE__OS _DK_SDU_MIP__BASE__SOLARIS
#define _DK_SDU_MIP__BASE__ARCH _DK_SDU_MIP__BASE__SPARC
#define _DK_SDU_MIP__BASE__ARCH_MINOR _DK_SDU_MIP__BASE__UNSPECIFIED

#define _DK_SDU_MIP__BASE__DEFAULT_BYTE_ORDER _DK_SDU_MIP__BASE__BIG_ENDIAN
#define _DK_SDU_MIP__BASE__DEFAULT_FLOAT_WORD_ORDER _DK_SDU_MIP__BASE__BIG_ENDIAN

#define _DK_SDU_MIP__BASE__DEFAULT_FLOAT _DK_SDU_MIP__BASE__IEEE_754_SINGLE_PRECISION
#define _DK_SDU_MIP__BASE__DEFAULT_DOUBLE _DK_SDU_MIP__BASE__IEEE_754_DOUBLE_PRECISION
#define _DK_SDU_MIP__BASE__DEFAULT_LONG_DOUBLE _DK_SDU_MIP__BASE__IEEE_QUADRUPLE_PRECISION

// Select additionally reentrant object
#define _REENTRANT 1

// Specifies that the Solaris extensions are utilized
#define __EXTENSIONS__ 1

// Specifies that the UNIX 98 functionality is utilized
#define _XOPEN_SOURCE 500

#if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
  #define _LARGEFILE64_SOURCE 1
#endif
