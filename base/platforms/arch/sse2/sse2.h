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

#if !defined(_COM_AZURE_DEV__BASE__BUILTIN_SQRTF)
#  define _COM_AZURE_DEV__BASE__BUILTIN_SQRTF builtin::sse2::invsqrtf
#endif

namespace base {

  using namespace compiler::org::gnu;
  
  namespace arch {
  
    namespace sse2 {

      static inline sse128 myfunc(sse128 a, sse128 b) noexcept {
        sse128 result;
        asm (
          "addss %0,%1\n\t"
          : "=&x" (result)
          : "0" (a), "x" (b)
        );
        return result;
      }
      
      inline float64 invsqrt(float64 value) noexcept {
        float64 result;
        asm (
          "rsqrtss %0,%0\n\t"
          : "=x" (result)
          : "0" (value)
        );
        return result;
      }

      inline float64 sqrt(float64 value) noexcept {
        float64 result;
        asm (
          "sqrtsd %0,%0\n\t"
          : "=x" (result)
          : "0" (value)
        );
        return result;
      }
      
    }; // end of namespace sse2
    
  }; // end of namespace arch
  
}; // end of namespace base

#endif
