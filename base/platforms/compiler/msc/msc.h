/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_PLATFORMS_COMPILER_MSC__MSC_H
#define _DK_SDU_MIP__BASE_PLATFORMS_COMPILER_MSC__MSC_H

#define _DK_SDU_MIP__BASE__COMPILER_VERSION _MSC_VER
#define _DK_SDU_MIP__BASE__COMPILER_NAMESPACE com::microsoft::compiler

#define _DK_SDU_MIP__BASE__FUNCTION __FUNCDNAME__

#define _DK_SDU_MIP__BASE__COMPILER_ABI _DK_SDU_MIP__BASE__ABI_MSC

namespace com {
  
  namespace microsoft {
    
    namespace compiler {
    }
  }
}

using namespace com::microsoft::compiler;

#endif
