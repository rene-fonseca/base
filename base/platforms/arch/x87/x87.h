/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

/*
  Known functions:
  
  acos
  acosh
  asin
  asinh
  atan
  atan2
  cabs
  cbrt
  ceil
  copysign
  cos
  cosh
  erf
  erfc
  exp
  exp10
  exp2
  expm1
  fabs
  fdim
  finite
  floor
  fma
  fmax
  fmin
  fmod
  frexp
  hypot
  ilogb
  invsqrt
  isnan
  ldexp
  lgamma
  llrint
  llround
  log
  log10
  log1p
  log2
  logb
  lrint
  lround
  modf
  nearbyint
  nextafter
  nexttoward
  pow
  remainder
  remquo
  rint
  round
  scalb
  scalbln
  scalbn
  significand
  sin
  sinh
  sqrt
  tan
  tanh
  tgamma
  trunc
*/

namespace base {

  namespace arch {

    namespace x87 {

      enum RoundingFlag {
        ROUND_TOWARD_NEAREST = 0x0000,
        ROUND_DOWN = 0x0400,
        ROUND_UP = 0x0800,
        ROUND_TOWARD_ZERO = 0x0c00
      };

      enum PrecisionFlag {
        PRECISION_SINGLE = 0x0000,
        PRECISION_RESERVED = 0x0100,
        PRECISION_DOUBLE = 0x0200,
        PRECISION_EXTENDED = 0x0300
      };

      enum ExceptionFlag {
        EXCEPTION_INVALID_OPERAND = 0x0001,
        EXCEPTION_DENORMAL_OPERAND = 0x0002,
        EXCEPTION_ZERO_DIVIDE = 0x0004,
        EXCEPTION_OVERFLOW = 0x0008,
        EXCEPTION_UNDERFLOW = 0x0010,
        EXCEPTION_PRECISION = 0x0020
      };
      
#define _DK_SDU_MIP__BASE__BUILTIN_ATANL builtin::atanl
      inline long double atanl(long double value) throw() {
        long double result;
        asm (
          "fld1\n\t" // load 1
          "fpatan\n\t" // replaces st(1) with atan(st(1)/st(0)) and pops the stack (sign is ok)
          : "=t" (result) // output(s)
          : "0" (value) // input(s)
        );
        return result;
      }

#define _DK_SDU_MIP__BASE__BUILTIN_ATAN2L builtin::atan2l
      inline long double atan2l(long double y, long double x) throw() {
        long double result;
        asm (
          "fpatan\n\t" // replaces st(1) with atan(st(1)/st(0)) and pops the stack (sign is ok)
          : "=t" (result) // output(s)
          : "0" (x), "u" (y) // TAG: GCC 3.1 generates stupid code // input(s)
          : "st(1)" // clobbered
        );
        return result;
      }

#define _DK_SDU_MIP__BASE__BUILTIN_COTANL builtin::cotanl
      inline long double cotanl(long double value) throw() {
        long double result;
        asm (
          "fld1\n\t" // load 1
          "fxch\n\t" // swap 1 and input
          "fptan\n\t" // replaces st(0) with its tangent and push 1 onto stack
          "fdivrp\n\t" // 1/tan
          "ffree %%st(1)\n\t" // free garbage 1 if present (prevent stack overflow)
          : "=t" (result) // output(s)
          : "0" (value) // input(s)
        );
        return result; // we return original value on error
      }

#define _DK_SDU_MIP__BASE__BUILTIN_FREXPL builtin::frexpl
      inline long double frexp(long double value, int* exponent) throw() {
        long double mantissa;
        if (exponent) {
          asm (
            "fxtract\n\t" // store exponent in st(0) and push mantissa onto stack
            "fld1\n\t" // load 1
            "fchs\n\t" // negate
            "fxch\n\t"
            "fscale\n\t" // divide by 2
            "fstp %%st(1)\n\t"
            "fxch\n\t"
            "fld1\n\t"
            "faddp\n\t" // add 1 to exponent (multiply by 2)
            "fistpl %1\n\t"
            : "=t" (mantissa) // TAG: must be in range [1/2; 1[ or 0 // output(s)
            : "0" (value), "m" (exponent) // input(s)
          );
        } else {
          asm (
            "fxtract\n\t" // store exponent in st(0) and push mantissa onto stack
            "ffree %%st(1)\n\t" // throw away exponent
            "fld1\n\t" // load 1
            "fchs\n\t" // negate
            "fxch\n\t"
            "fscale\n\t" // divide by 2
            "fstp %%st(1)\n\t"
            : "=t" (mantissa) // output(s)
            : "0" (value) // input(s)
          );
        }
        return mantissa;
      }
  
#define _DK_SDU_MIP__BASE__BUILTIN_FABSF builtin::fabsf
      inline float fabsf(float value) throw() {
        float result;
        asm (
          "fabs\n\t"
          : "=t" (result) // output(s)
          : "0" (value) // input(s)
        );
        return result;
      }

#define _DK_SDU_MIP__BASE__BUILTIN_FABS builtin::fabs
      inline double fabs(double value) throw() {
        double result;
        asm (
          "fabs\n\t"
          : "=t" (result) // output(s)
          : "0" (value) // input(s)
        );
        return result;
      }

#define _DK_SDU_MIP__BASE__BUILTIN_FABSL builtin::fabsl
      inline long double fabsl(long double value) throw() {
        long double result;
        asm (
          "fabs\n\t"
          : "=t" (result) // output(s)
          : "0" (value) // input(s)
        );
        return result;
      }

#define _DK_SDU_MIP__BASE__BUILTIN_LOGL builtin::logl
      inline long double logl(long double value) throw() {
        long double result; // log2(x)/log2(e)
        asm (
          "fld1\n\t" // {1; x}
          "fldl2e\n\t" // {log2(e); 1; x}
          "fdivp\n\t" // {1/log2(e); x}
          "fxch\n\t" // {x; 1/log2(e)}
          "fyl2x\n\t" // {1/log2(e) * log2(x)}
          : "=t" (result) // output(s)
          : "0" (value) // input(s)
        );
        return result;
      }      
      
      // y = ln(1 + x) = log2(1 + x)/log2(e) - "log epsilon"
#define _DK_SDU_MIP__BASE__BUILTIN_LOG1PL builtin::log1pl
      inline long double log1pl(long double value) throw() {
        long double result;
        asm (
          "fld1\n\t" // load 1
          "fldl2e\n\t" // load log2(e)
          "fdivp\n\t" // 1/log2(e)
          "fxch\n\t"
          "fyl2xp1\n\t" // replaces st(1) with (st(1) * log2(1 + st(0))) and pop stack
          : "=t" (result) // output(s)
          : "0" (value) // input(s)
        );
        return result;
      }

      // y = log2(x)
#define _DK_SDU_MIP__BASE__BUILTIN_LOG2L builtin::log2l
      inline long double log2l(long double value) throw() {
        long double result;
        asm (
          "fld1\n\t" // {1; x}
          "fxch\n\t" // {x; 1}
          "fyl2x\n\t" // {1 * log2(x)}
          : "=t" (result) // output(s)
          : "0" (value) // input(s)
        );
        return result;
      }

#define _DK_SDU_MIP__BASE__BUILTIN_LOG10L builtin::log10l
      inline long double log10l(long double value) throw() {
        long double result; // log2(x)/log2(10)
        asm (
          "fld1\n\t" // {1; x}
          "fldl2t\n\t" // {log2(10); 1; x}
          "fdivp\n\t" // {1/log2(10); x}
          "fxch\n\t" // {x; 1/log2(10)}
          "fyl2x\n\t" // {1/log2(10) * log2(x)}          
          : "=t" (result) // output(s)
          : "0" (value) // input(s)
        );
        return result;
      }

#define _DK_SDU_MIP__BASE__BUILTIN_LOGBL builtin::lognl
      inline long double lognl(long double value, long double base) throw() {
        long double result; // log2(value)/(log2(e) * base)
        asm (
          "fyl2x\n\t" // {1 * log2(base); x}
          "fld1\n\t" // {1; 1 * log2(base); x}
          "fdivrp\n\t" // {1/(log2(base)); x}
          "fxch\n\t" // {x; 1/(log2(base))}
          "fyl2x\n\t" // {1/(log2(base)) * log2(x)}
          : "=t" (result) // output(s)
          : "f" (base), "f" (1), "f" (value) // input(s)
        );
        return result;
      }

      // z = x^y = 2^log2(x)^y = 2^(log2(x) * y)
#define _DK_SDU_MIP__BASE__BUILTIN_POWL builtin::powl
      inline long double powl(long double x, long double y) throw() {
        long double result;
        asm (
          "fyl2x\n\t" // replaces st(1) with (st(1) * log2(st(0))) and pop stack
          "fld %%st(0)\n\t" // duplicate st(0)
          "frndint\n\t" // round to integer (we do not care about direction)
          "fsub %%st(1),%%st(0)\n\t" // fraction representation in st(1)
          "fld1\n\t" // load 1
          "fscale\n\t" // 1 * 2^(~trunc(log2(x) * y))
          "fstp %%st(1)\n\t" // throw away rounded value
          "fxch\n\t" // bring fraction part to front
          "f2xm1\n\t" // 2^~frac(log2(x) * y) - 1
          "fld1\n\t" // load 1
          "faddp\n\t" // 2^~frac(log2(x) * y)
          "fmulp\n\t" // 2^(~trunc(log2(x) * y)) * 2^(~frac(log2(x) * y))
          : "=t" (result) // output(s)
          : "0" (x), "u" (y) // input(s)
          : "st(1)" // clobbered
        );
        return result;
        // TAG: need to handle all cases (e.g. 0^0 = 1)
      }

      // y = exp(x) = 2^x = 2^(trunc(x) + frac(x)) = 2^(trunc(x)) * 2^(frac(x))
      inline long double exp2l(long double value) throw() {
        long double result;
        asm (
          "fld %%st(0)\n\t" // {x; x}
          "frndint\n\t" // round to integer (we do not care about direction)
          "fsub %%st(1),%%st(0)\n\t" // fraction representation in st(1)
          "fld1\n\t" // load 1
          "fscale\n\t" // 1 * 2^(~trunc(log2(e) * x))
          "fstp %%st(1)\n\t" // throw away rounded value
          "fxch\n\t" // bring fraction part to front
          "f2xm1\n\t" // 2^~frac(log2(e) * x) - 1
          "fld1\n\t" // load 1
          "faddp\n\t" // 2^~frac(log2(e) * x)
          "fmulp\n\t" // 2^(~trunc(log2(e) * x)) * 2^(~frac(log2(e) * x))
          : "=t" (result) // output(s)
          : "0" (value) // input(s)
        );
        return result;
      }
      
      // y = exp(x) = 2^log2(e)^x = 2^(log2(e) * x)
      // y = 2^(trunc(log2(e) * x) + frac(log2(e) * x))
      // y = 2^(trunc(log2(e) * x)) * 2^(frac(log2(e) * x))
#define _DK_SDU_MIP__BASE__BUILTIN_EXPL builtin::expl
      inline long double expl(long double value) throw() {
        long double result;
        asm (
          "fldl2e\n\t" // {log2(e), x}
          "fmulp\n\t" // {log2(e) * x}
          "fld %%st(0)\n\t" // {log2(e) * x; log2(e) * x}
          "frndint\n\t" // round to integer (we do not care about direction)
          "fsub %%st(1),%%st(0)\n\t" // fraction representation in st(1)
          "fld1\n\t" // load 1
          "fscale\n\t" // 1 * 2^(~trunc(log2(e) * x))
          "fstp %%st(1)\n\t" // throw away rounded value
          "fxch\n\t" // bring fraction part to front
          "f2xm1\n\t" // 2^~frac(log2(e) * x) - 1
          "fld1\n\t" // load 1
          "faddp\n\t" // 2^~frac(log2(e) * x)
          "fmulp\n\t" // 2^(~trunc(log2(e) * x)) * 2^(~frac(log2(e) * x))
          : "=t" (result) // output(s)
          : "0" (value) // input(s)
        );
        return result;
      }

#define _DK_SDU_MIP__BASE__BUILTIN_LDEXPL builtin::ldexpl
      inline long double ldexpl(long double value, int exponent) throw() {
        long double result;
        asm (
          "fildl %2\n\t"
          "fxch\n\t"
          "fscale\n\t" // replaces st(0) with st(0) * 2^trunc(st(1))
          "fstp %%st(1)\n\t"
          : "=t" (result) // output(s)
          : "0" (value), "m" (exponent) // input(s)
          : "st(1)" // clobbered
        );
        return result;
      }

#define _DK_SDU_MIP__BASE__BUILTIN_SQRTL builtin::sqrtl
      inline long double sqrtl(long double value) throw() {
        long double result;
        asm (
          "fsqrt\n\t" // {sqrt(x)}
          : "=t" (result) // output(s)
          : "0" (value) // input(s)
        );
        return result;
      }

#define _DK_SDU_MIP__BASE__BUILTIN_COSL builtin::cosl
      inline long double cosl(long double value) throw() {
        long double result;
        asm (
          "fcos\n\t" // {cos(x)}
          : "=t" (result) // output(s)
          : "0" (value) // input(s)
        );
        return result;
      }
  
#define _DK_SDU_MIP__BASE__BUILTIN_SINL builtin::sinl
      inline long double sinl(long double value) throw() {
        long double result;
        asm (
          "fsin\n\t" // {sin(x)}
          : "=t" (result) // output(s)
          : "0" (value) // input(s)
        );
        return result;
      }

#define _DK_SDU_MIP__BASE__BUILTIN_SINCOSL builtin::sincosl
      inline long double sincosl(long double value) throw() {
        long double cosine;
        long double sine;
        asm (
          "fsincos\n\t" // replaces st(0) with sine and pushes cosine onto stack
          : "=t" (cosine), "=u" (sine) // output(s)
          : "0" (value) // input(s)
        );
        return cosine; // TAG: FIXME
      }

#define _DK_SDU_MIP__BASE__BUILTIN_TANL builtin::tanl
      inline long double tanl(long double value) throw() {
        long double result;
        asm (
          "fptan\n\t" // {1; tan(x); 1 or x; 1}
          "fmulp\n\t" // {tan(x); 1 or x}
          "ffree %%st(1)\n\t" // {tan(x) or x} (prevent stack overflow)
          : "=t" (result) // output(s)
          : "t" (value), "u" (1)
        );
        return result; // we return original value on error
      }
      
      inline long double asinl(long double value) throw() {
        long double result; // atan2(x, sqrt(1 - x^2))
        asm (
          "fld %%st(0)\n\t" // {x; x}
          "fmul %%st(0),%%st(1)\n\t" // {x^2; x}
          "fld1\n\t" // {1; x^2; x}
          "fsubrp\n\t" // {1 - x^2; x}
          "fsqrt\n\t" // {sqrt(1 - x^2); x}
          "fpatan\n\t" // {atan(x/sqrt(1 - x^2))}
          : "=t" (result) // output(s)
          : "0" (value) // input(s)
        );
        return result;
      }
      
      inline long double acosl(long double value) throw() {
        long double result; // atan2(sqrt(1 - x^2), x)
        asm (
          "fld %%st(0)\n\t" // {x; x}
          "fmul %%st(0),%%st(1)\n\t" // {x^2; x}
          "fld1\n\t" // {1; x^2; x}
          "fsubrp\n\t" // {1 - x^2; x}
          "fsqrt\n\t" // {sqrt(1 - x^2); x}
          "fxch\n\t" // {x; sqrt(1 - x^2)}
          "fpatan\n\t" // {atan(sqrt(1 - x^2)/x)}
          : "=t" (result) // output(s)
          : "0" (value) // input(s)
        );
        return result;
      }

      inline long double ceill(long double value) throw() {
        long double result;
        unsigned int original;
        unsigned int temporary;
        asm (
          "fstcw %1\n\t" // save control word
          "movl %1,%2\n\t" // load control word
          "andl $0xf3ff,%2\n\t" // mask rounding flags
          "orl $0x0800,%2\n\t" // round toward +infinity
          "movl %2,%1\n\t" // store temporary control word
          "fldcw %3\n\t" // activate rounding mode
          "frndint\n\t" // round toward +infinity
          "fldcw %1\n\t" // restore control word
          : "=t" (result) // output(s)
          : "0" (value), "m" (original), "r", "m" (temporary) // input(s)
        );
        return result;
      }
      
      inline long double floorl(long double value) throw() {
        long double result;
        unsigned int original;
        unsigned int temporary;
        asm (
          "fstcw %1\n\t" // save control word
          "movl %1,%2\n\t" // load control word
          "andl $0xf3ff,%2\n\t" // mask rounding flags
          "orl $0x0400,%2\n\t" // round toward -infinity
          "movl %2,%1\n\t" // store temporary control word
          "fldcw %3\n\t" // activate rounding mode
          "frndint\n\t" // round toward -infinity
          "fldcw %1\n\t" // restore control word
          : "=t" (result) // output(s)
          : "0" (value), "m" (original), "r", "m" (temporary) // input(s)
        );
        return result;
      }
      
      inline long double roundl(long double value) throw() {
        long double result;
        unsigned int original;
        unsigned int temporary;
        asm (
          "fstcw %1\n\t" // save control word
          "movl %1,%2\n\t" // load control word
          "andl $0xf3ff,%2\n\t" // mask rounding flags - round toward nearest
          "movl %2,%1\n\t" // store temporary control word
          "fldcw %3\n\t" // activate rounding mode
          "frndint\n\t" // round toward nearest
          "fldcw %1\n\t" // restore control word
          : "=t" (result) // output(s)
          : "0" (value), "m" (original), "r", "m" (temporary) // input(s)
        );
        return result;
      }
      
      inline long double truncl(long double value) throw() {
        long double result;
        unsigned int original;
        unsigned int temporary;
        asm (
          "fstcw %1\n\t" // save control word
          "movl %1,%2\n\t" // load control word
          "andl $0xf3ff,%2\n\t" // mask rounding flags
          "orl $0x0c00,%2\n\t" // round toward zero
          "movl %2,%1\n\t" // store temporary control word
          "fldcw %3\n\t" // activate rounding mode
          "frndint\n\t" // round toward zero
          "fldcw %1\n\t" // restore control word
          : "=t" (result) // output(s)
          : "0" (value), "m" (original), "r", "m" (temporary) // input(s)
        );
        return result;
      }
      
      inline long double fracl(long double value) throw() {
        long double result;
        asm (
          "fld %%st(0)\n\t" // {x; x}
          "fstcw %1\n\t" // save control word
          "movl %1,%2\n\t" // load control word
          "andl $0xf3ff,%2\n\t" // mask rounding flags
          "orl $0x0c00,%2\n\t" // round toward zero
          "movl %2,%1\n\t" // store temporary control word
          "fldcw %3\n\t" // activate rounding mode
          "frndint\n\t" // round toward zero
          "fldcw %1\n\t" // restore control word
          "fsubp\n\t" // fraction
          : "=t" (result) // output(s)
          : "0" (value) // input(s)
        );
        return result;
      }
      
    }; // end of namespace x87

  }; // end of namespace arch
  
}; // end of namespace base
