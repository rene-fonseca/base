/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Ren� M�ller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#define _DK_SDU_MIP__BASE__FLAVOUR _DK_SDU_MIP__BASE__UNIX
#define _DK_SDU_MIP__BASE__OS _DK_SDU_MIP__BASE__IRIX65
#define _DK_SDU_MIP__BASE__ARCH _DK_SDU_MIP__BASE__MIPS
#define _DK_SDU_MIP__BASE__ARCH_MINOR _DK_SDU_MIP__BASE__UNSPECIFIED

#define _DK_SDU_MIP__BASE__DEFAULT_BYTE_ORDER _DK_SDU_MIP__BASE__BIG_ENDIAN
#define _DK_SDU_MIP__BASE__DEFAULT_FLOAT_WORD_ORDER _DK_SDU_MIP__BASE__LITTLE_ENDIAN

#define _DK_SDU_MIP__BASE__DEFAULT_FLOAT _DK_SDU_MIP__BASE__IEEE_754_SINGLE_PRECISION
#define _DK_SDU_MIP__BASE__DEFAULT_DOUBLE _DK_SDU_MIP__BASE__IEEE_754_DOUBLE_PRECISION
#define _DK_SDU_MIP__BASE__DEFAULT_LONG_DOUBLE _DK_SDU_MIP__BASE__IEEE_754_DOUBLE_PRECISION

// Select additionally reentrant object
#define _REENTRANT 1

// Specifies that the UNIX 98 functionality and SGI extensions are utilized
#define _SGI_SOURCE 1

// Enable C++ specific prototypes.
#define _LANGUAGE_C_PLUS_PLUS 1

#if defined(_DK_SDU_MIP__BASE__LARGE_FILE_SYSTEM)
  #define _LARGEFILE64_SOURCE 1
#endif
