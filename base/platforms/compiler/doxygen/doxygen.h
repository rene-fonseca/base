/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#define _DK_SDU_MIP__BASE__COMPILER_NAMESPACE nl::stack::doxygen

#define _DK_SDU_MIP__BASE__ALIGNED(alignment)
#define _DK_SDU_MIP__BASE__DEPRECATED
#define _DK_SDU_MIP__BASE__NORETURN
#define _DK_SDU_MIP__BASE__PACKED
#define _DK_SDU_MIP__BASE__SECTION(name)
#define _DK_SDU_MIP__BASE__UNUSED
#define _DK_SDU_MIP__BASE__WEAK
#define _DK_SDU_MIP__BASE__FORCE_INLINE

#define _DK_SDU_MIP__BASE__COMPILER_INLINE inline
#define _DK_SDU_MIP__BASE__COMPILER_RESTRICT restrict

// explicit calling conventions
#define _DK_SDU_MIP__BASE__CALL_PASCAL
#define _DK_SDU_MIP__BASE__CALL_CPP

#define _DK_SDU_MIP__BASE__NAMESPACE
#define _DK_SDU_MIP__BASE__CHAR_SIZE 1
#define _DK_SDU_MIP__BASE__SHORT_SIZE 2
#define _DK_SDU_MIP__BASE__INT_SIZE 4
#define _DK_SDU_MIP__BASE__LONG_SIZE 4
#define _DK_SDU_MIP__BASE__LONG_LONG_SIZE 8
#define _DK_SDU_MIP__BASE__POINTER_SIZE 4
#define _DK_SDU_MIP__BASE__BYTE_ORDER _DK_SDU_MIP__BASE__LITTLE_ENDIAN
#define _DK_SDU_MIP__BASE__FLOAT_WORD_ORDER _DK_SDU_MIP__BASE__LITTLE_ENDIAN
#define _DK_SDU_MIP__BASE__FLOAT _DK_SDU_MIP__BASE__IEEE_754_SINGLE_PRECISION
#define _DK_SDU_MIP__BASE__DOUBLE _DK_SDU_MIP__BASE__IEEE_754_DOUBLE_PRECISION
#define _DK_SDU_MIP__BASE__LONG_DOUBLE _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96
#define _DK_SDU_MIP__BASE__CPP_BOOL
#define _DK_SDU_MIP__BASE__CPP_CONST
#define _DK_SDU_MIP__BASE__CPP_INLINE
#define _DK_SDU_MIP__BASE__CPP_STATIC_CONST
#define _DK_SDU_MIP__BASE__CPP_ANONYMOUS
#define _DK_SDU_MIP__BASE__CPP_SPECIFICATION

namespace nl {

  namespace stack {

    namespace doxygen {
    }
  }
}

using namespace nl::stack::doxygen;
