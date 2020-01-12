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

#define _COM_AZURE_DEV__BASE__COMPILER_NAMESPACE nl::stack::doxygen

#define _COM_AZURE_DEV__BASE__ALIGNED(alignment)
#define _COM_AZURE_DEV__BASE__DEPRECATED
#define _COM_AZURE_DEV__BASE__NORETURN
#define _COM_AZURE_DEV__BASE__PACKED
#define _COM_AZURE_DEV__BASE__SECTION(name)
#define _COM_AZURE_DEV__BASE__UNUSED
#define _COM_AZURE_DEV__BASE__WEAK
#define _COM_AZURE_DEV__BASE__FORCE_INLINE

#define _COM_AZURE_DEV__BASE__COMPILER_INLINE inline
#define _COM_AZURE_DEV__BASE__COMPILER_RESTRICT restrict

// explicit calling conventions
#define _COM_AZURE_DEV__BASE__CALL_PASCAL
#define _COM_AZURE_DEV__BASE__CALL_CPP

#define _COM_AZURE_DEV__BASE__NAMESPACE
#define _COM_AZURE_DEV__BASE__CHAR_SIZE 1
#define _COM_AZURE_DEV__BASE__SHORT_SIZE 2
#define _COM_AZURE_DEV__BASE__INT_SIZE 4
#define _COM_AZURE_DEV__BASE__LONG_SIZE 4
#define _COM_AZURE_DEV__BASE__LONG_LONG_SIZE 8
#define _COM_AZURE_DEV__BASE__POINTER_SIZE 4
#define _COM_AZURE_DEV__BASE__BYTE_ORDER _COM_AZURE_DEV__BASE__LITTLE_ENDIAN

#define _COM_AZURE_DEV__BASE__FLOAT_WORD_ORDER _COM_AZURE_DEV__BASE__LITTLE_ENDIAN
#define _COM_AZURE_DEV__BASE__FLOAT _COM_AZURE_DEV__BASE__IEEE_754_SINGLE_PRECISION
#define _COM_AZURE_DEV__BASE__DOUBLE _COM_AZURE_DEV__BASE__IEEE_754_DOUBLE_PRECISION
#define _COM_AZURE_DEV__BASE__LONG_DOUBLE _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96
#define _COM_AZURE_DEV__BASE__FLOAT128 _COM_AZURE_DEV__BASE__IEEE_QUADRUPLE_PRECISION

#define _COM_AZURE_DEV__BASE__CPP_BOOL
#define _COM_AZURE_DEV__BASE__CPP_CONST
#define _COM_AZURE_DEV__BASE__CPP_INLINE
#define _COM_AZURE_DEV__BASE__CPP_STATIC_CONST
#define _COM_AZURE_DEV__BASE__CPP_ANONYMOUS
#define _COM_AZURE_DEV__BASE__CPP_SPECIFICATION

namespace nl {

  namespace stack {

    namespace doxygen {
    }
  }
}

using namespace nl::stack::doxygen;
