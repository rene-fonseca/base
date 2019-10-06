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

namespace base {

#define _DK_SDU_MIP__BASE__COMPILER_VERSION __COMPILER_VERSION
#define _DK_SDU_MIP__BASE__COMPILER_NAMESPACE com::sgi2::mipspro

  namespace com {
    
    // TAG: "sgi" is defined as symbol
    namespace sgi2 {
      
      namespace mipspro {
      }
    }
  }

  // import data types
  using namespace com::sgi2::mipspro;
  
} // end of namespace base
