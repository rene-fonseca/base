/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_PLATFORMS_COMPILER_BORLAND__BORLAND_H
#define _DK_SDU_MIP__BASE_PLATFORMS_COMPILER_BORLAND__BORLAND_H

namespace base {

#define _DK_SDU_MIP__BASE__COMPILER_VERSION __BORLANDC__
#define _DK_SDU_MIP__BASE__COMPILER_NAMESPACE com::borland::compiler

  namespace com {
    
    namespace borland {
      
      namespace compiler {
      }
    }
  }
  
  // import data types
  using com::borland::compiler;
  
} // end of namespace base

#endif
