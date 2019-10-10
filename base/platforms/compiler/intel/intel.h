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

#define _COM_AZURE_DEV__BASE__ALIGNED(alignment) __attribute__ ((aligned (alignment)))
#define _COM_AZURE_DEV__BASE__DEPRECATED __attribute__ ((deprecated))
#define _COM_AZURE_DEV__BASE__NORETURN __attribute__ ((noreturn))
#define _COM_AZURE_DEV__BASE__PACKED __attribute__ ((packed))
#define _COM_AZURE_DEV__BASE__SECTION(name) __attribute__ ((section (name)))
#define _COM_AZURE_DEV__BASE__UNUSED __attribute__ ((unused))
#define _COM_AZURE_DEV__BASE__WEAK __attribute__ ((weak))
#define _COM_AZURE_DEV__BASE__FORCE_INLINE __attribute__ ((always_inline))

#define _COM_AZURE_DEV__BASE__COMPILER_VERSION __INTEL_COMPILER
#define _COM_AZURE_DEV__BASE__COMPILER_NAMESPACE com::intel::compiler

  namespace com {
    
    namespace intel {
      
      namespace compiler {
      }
    }
  }
  
  // import data types
  using namespace com::intel::compiler;
  
} // end of namespace base
