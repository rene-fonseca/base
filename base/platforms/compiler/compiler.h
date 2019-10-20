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

#if (!defined(_COM_AZURE_DEV__BASE__COMPILER) && defined(_COM_AZURE_DEV__BASE__DEFAULT_COMPILER))
#  define _COM_AZURE_DEV__BASE__COMPILER _COM_AZURE_DEV__BASE__DEFAULT_COMPILER
#endif

#if !defined(_COM_AZURE_DEV__BASE__COMPILER) || \
    (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_GCC)
#  include <base/platforms/compiler/gcc/gcc.h>
#elif (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC)
#  include <base/platforms/compiler/msc/msc.h>
#elif (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_LLVM)
#  include <base/platforms/compiler/llvm/llvm.h>
#elif (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_SUNWSPRO)
#  include <base/platforms/compiler/sunwspro/sunwspro.h>
#elif (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MIPSPRO)
#  include <base/platforms/compiler/mipspro/mipspro.h>
#elif (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_INTEL)
#  include <base/platforms/compiler/intel/intel.h>
#elif (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_BORLAND)
#  include <base/platforms/compiler/borland/borland.h>
#elif (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_DOXYGEN)
#  include <base/platforms/compiler/doxygen/doxygen.h>
#endif

#if !defined(_COM_AZURE_DEV__BASE__PACKED)
#  define _COM_AZURE_DEV__BASE__PACKED
#endif
#if !defined(_COM_AZURE_DEV__BASE__PACKED__BEGIN)
#  define _COM_AZURE_DEV__BASE__PACKED__BEGIN
#endif
#if !defined(_COM_AZURE_DEV__BASE__PACKED__END)
#  define _COM_AZURE_DEV__BASE__PACKED__END
#endif

#if !defined(_COM_AZURE_DEV__BASE__CPP_INLINE) && !defined(inline)
#  if defined(_COM_AZURE_DEV__BASE__COMPILER_INLINE)
#    define inline _COM_AZURE_DEV__BASE__COMPILER_INLINE
#  else
#    define inline // remove inline
#  endif
#endif // inline

#if !defined(_COM_AZURE_DEV__BASE__CPP_RESTRICT) && !defined(restrict)
#  if defined(_COM_AZURE_DEV__BASE__COMPILER_RESTRICT)
#    define restrict _COM_AZURE_DEV__BASE__COMPILER_RESTRICT
#  else
#    define restrict // remove restrict
#  endif
#endif // restrict
