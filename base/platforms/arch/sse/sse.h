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

// TAG: watch out for accuracy
// FAST/APPROX must be enabled explicitly

#if !defined(_DK_SDU_MIP__BASE__BUILTIN_SQRTF)
#  define _DK_SDU_MIP__BASE__BUILTIN_SQRTF builtin::sse::invsqrtf
#endif

namespace base {
  
  namespace arch {

    namespace sse {

      inline float invsqrtf(float value) throw() {
        float result;
        asm (
          "rsqrtss %0,%0\n\t"
          : "=x" (result)
          : "0" (value)
        );
        return result;
      }

      inline float sqrtf(float value) throw() {
        float result;
        asm (
          "sqrtss %0,%0\n\t"
          : "=x" (result)
          : "0" (value)
        );
        return result;
      }
      
    }; // end of namespace sse
  
  }; // end of namespace arch
  
}; // end of namespace base

#endif
