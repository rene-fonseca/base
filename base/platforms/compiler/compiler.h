/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_PLATFORMS_COMPILER__COMPILER_H
#define _DK_SDU_MIP__BASE_PLATFORMS_COMPILER__COMPILER_H

#if !defined(_DK_SDU_MIP__BASE__COMPILER) || \
    (_DK_SDU_MIP__BASE__COMPILER == _DK_SDU_MIP__BASE__COMPILER_GCC)
  #include <base/platforms/compiler/gcc/gcc.h>
#elif (_DK_SDU_MIP__BASE__COMPILER == _DK_SDU_MIP__BASE__COMPILER_SUNWSPRO)
  #include <base/platforms/compiler/sunwspro/sunwspro.h>
#elif (_DK_SDU_MIP__BASE__COMPILER == _DK_SDU_MIP__BASE__COMPILER_MIPSPRO)
  #include <base/platforms/compiler/mipspro/mipspro.h>
#elif (_DK_SDU_MIP__BASE__COMPILER == _DK_SDU_MIP__BASE__COMPILER_INTEL)
  #include <base/platforms/compiler/intel/intel.h>
#elif (_DK_SDU_MIP__BASE__COMPILER == _DK_SDU_MIP__BASE__COMPILER_BORLAND)
  #include <base/platforms/compiler/borland/borland.h>
#elif (_DK_SDU_MIP__BASE__COMPILER == _DK_SDU_MIP__BASE__COMPILER_MSC)
  #include <base/platforms/compiler/msvc/msc.h>
#endif

#if !defined(_DK_SDU_MIP__BASE__CPP_INLINE) && !defined(inline)
  #if defined(_DK_SDU_MIP__BASE__COMPILER_INLINE)
    #define inline _DK_SDU_MIP__BASE__COMPILER_INLINE
  #else
    #define inline
  #endif
#endif // inline

#if !defined(_DK_SDU_MIP__BASE__CPP_RESTRICT) && !defined(restrict)
  #if defined(_DK_SDU_MIP__BASE__COMPILER_RESTRICT)
    #define restrict _DK_SDU_MIP__BASE__COMPILER_RESTRICT
  #else
    #define restrict
  #endif
#endif // restrict

#endif
