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

#define _DK_SDU_MIP__BASE__BUILTIN_ATANL builtin::atanl
      inline long double atanl(long double value) throw() {
        long double result;
        asm (
          "fld1\n\t" // load 1
          "fpatan\n\t" // replaces st(1) with atan(st(1)/st(0)) and pops the stack (sign is ok)
          : "=t" (result)
          : "0" (value)
        );
        return result;
      }

#define _DK_SDU_MIP__BASE__BUILTIN_ATAN2L builtin::atan2l
      inline long double atan2l(long double y, long double x) {
        long double result;
        asm (
          "fpatan\n\t" // replaces st(1) with atan(st(1)/st(0)) and pops the stack (sign is ok)
          : "=t" (result)
          : "0" (x), "u" (y) // TAG: GCC 3.1 generates stupid code
          : "st(1)"
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
          : "=t" (result)
          : "0" (value)
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
            : "=t" (mantissa) // TAG: must be in range [1/2; 1[ or 0
            : "0" (value), "m" (exponent)
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
            : "=t" (mantissa)
            : "0" (value)
          );
        }
        return mantissa;
      }
  
#define _DK_SDU_MIP__BASE__BUILTIN_FABSF builtin::fabsf
      inline float fabsf(float value) throw() {
        float result;
        asm (
          "fabs\n\t"
          : "=t" (result)
          : "0" (value)
        );
        return result;
      }

#define _DK_SDU_MIP__BASE__BUILTIN_FABS builtin::fabs
      inline double fabs(double value) throw() {
        double result;
        asm (
          "fabs\n\t"
          : "=t" (result)
          : "0" (value)
        );
        return result;
      }

#define _DK_SDU_MIP__BASE__BUILTIN_FABSL builtin::fabsl
      inline long double fabsl(long double value) throw() {
        long double result;
        asm (
          "fabs\n\t"
          : "=t" (result)
          : "0" (value)
        );
        return result;
      }

      // y = ln(x) = log2(x)/log2(e)
#define _DK_SDU_MIP__BASE__BUILTIN_LOGL builtin::logl
      inline long double logl(long double value) throw() {
        long double result;
        asm (
          "fld1\n\t" // load 1
          "fldl2e\n\t" // load log2(e)
          "fdivp\n\t" // 1/log2(e)
          "fxch\n\t"
          "fyl2x\n\t" // replaces st(1) with (st(1) * log2(st(0))) and pop stack
          : "=t" (result)
          : "0" (value)
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
          : "=t" (result)
          : "0" (value)
        );
        return result;    
      }

      // y = log2(x)
#define _DK_SDU_MIP__BASE__BUILTIN_LOG2L builtin::log2l
      inline long double log2l(long double value) throw() {
        long double result;
        asm (
          "fld1\n\t" // load 1
          "fxch\n\t"
          "fyl2x\n\t" // replaces st(1) with (st(1) * log2(st(0))) and pop stack
          : "=t" (result)
          : "0" (value)
        );
        return result;
      }

      // y = log10(x) = log2(x)/log2(10)
#define _DK_SDU_MIP__BASE__BUILTIN_LOG10L builtin::log10l
      inline long double log10l(long double value) throw() {
        long double result;
        asm (
          "fld1\n\t" // load 1
          "fldl2t\n\t" // load log2(10)
          "fdivp\n\t" // 1/log2(10)
          "fxch\n\t"
          "fyl2x\n\t" // replaces st(1) with (st(1) * log2(st(0))) and pop stack
          : "=t" (result)
          : "0" (value)
        );
        return result;
      }

      // y = logb(x) = log2(x)/log2(b)
#define _DK_SDU_MIP__BASE__BUILTIN_LOGBL builtin::logbl // TAG: do NOT confuse with ISOC logb
      inline long double logbl(long double base, long double value) throw() {
        long double result;
        asm (
          "fld1\n\t" // load 1
          "fxch\n\t"
          "fyl2x\n\t" // replaces st(1) with (st(1) * log2(st(0))) and pop stack
          "fdivp\n\t" // 1/log2(base)
          "fxch\n\t"
          "fyl2x\n\t" // replaces st(1) with (st(1) * log2(st(0))) and pop stack
          : "=t" (result)
          : "0" (base), "u" (value)
        );
        return result;
      }
  
      // fracl()
      // floorl(): round towards -oo
      // roundl(): round to nearest integer (round up if frac(X)>=0.5)
      // truncl(): round towards 0


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
          : "=t" (result)
          : "0" (x), "u" (y)
        );
        return result;
        // TAG: need to handle all cases (e.g. 0^0 = 1)
      }
  
      // y = exp(x) = 2^log2(e)^x = 2^(log2(e) * x)
      // y = 2^(trunc(log2(e) * x) + frac(log2(e) * x))
      // y = 2^(trunc(log2(e) * x)) * 2^(frac(log2(e) * x))
#define _DK_SDU_MIP__BASE__BUILTIN_EXPL builtin::expl
      inline long double expl(long double value) throw() {
        long double result;
        asm (
          "fldl2e\n\t" // load log2(e)
          "fmulp\n\t" // log2(e) * x
          "fld %%st(0)\n\t" // duplicate st(0)
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
          : "=t" (result)
          : "0" (value)
        );
        return result;
      }

#define _DK_SDU_MIP__BASE__BUILTIN_LDEXPL builtin::ldexpl
      inline long double ldexpl(long double value, int exponent) {
        long double result;
        asm (
          "fildl %2\n\t"
          "fxch\n\t"
          "fscale\n\t" // replaces st(0) with st(0) * 2^trunc(st(1))
          "fstp %%st(1)\n\t"
          : "=t" (result)
          : "0" (value), "m" (exponent)
          : "st(1)"
        );
        return result;
      }

#define _DK_SDU_MIP__BASE__BUILTIN_SQRTL builtin::sqrtl
      inline long double sqrtl(long double value) throw() {
        long double result;
        asm (
          "fsqrt\n\t" // replaces st(0) with square root
          : "=t" (result)
          : "0" (value)
        );
        return result;
      }

#define _DK_SDU_MIP__BASE__BUILTIN_COSL builtin::cosl
      inline long double cosl(long double value) throw() {
        long double result;
        asm (
          "fcos\n\t" // replaces st(0) with cosine
          : "=t" (result)
          : "0" (value)
        );
        return result;
      }
  
#define _DK_SDU_MIP__BASE__BUILTIN_SINL builtin::sinl
      inline long double sinl(long double value) throw() {
        long double result;
        asm (
          "fsin\n\t" // replaces st(0) with sine
          : "=t" (result)
          : "0" (value)
        );
        return result;
      }

#define _DK_SDU_MIP__BASE__BUILTIN_SINCOSL builtin::sincosl
      inline long double sincosl(long double value) throw() {
        long double cosine;
        long double sine;
        asm (
          "fsincos\n\t" // replaces st(0) with sine and pushes cosine onto stack
          : "=t" (cosine), "=u" (sine)
          : "0" (value)
        );
        return cosine; // TAG: FIXME
      }

#define _DK_SDU_MIP__BASE__BUILTIN_TANL builtin::tanl
      inline long double tanl(long double value) throw() {
        long double result;
        asm (
          "fld1\n\t" // load 1
          "fxch\n\t" // swap 1 and input
          "fptan\n\t" // replaces st(0) with its tangent and push 1 onto stack
          "fmulp\n\t" // multiply result with 1
          "ffree %%st(1)\n\t" // free garbage 1 if present (prevent stack overflow)
          : "=t" (result)
          : "0" (value)
        );
        return result; // we return original value on error
      }
  
    }; // end of namespace x87

  }; // end of namespace arch
  
}; // end of namespace base
