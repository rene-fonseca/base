/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_PLATFORMS_COMPILER_SUNWSPRO__SUNWSPRO_H
#define _DK_SDU_MIP__BASE_PLATFORMS_COMPILER_SUNWSPRO__SUNWSPRO_H

namespace base {

#define _DK_SDU_MIP__BASE__COMPILER_VERSION __SUNPRO_CC
#define _DK_SDU_MIP__BASE__COMPILER_NAMESPACE compiler::org::sunwspro
  
  namespace compiler {
    
    namespace com {
      
      namespace sun {
      };
    };
  };
  
  // import data types
  using compiler::com::sun;
  
}; // end of namespace base

#endif
