/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/FloatingPoint.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

template<>
void analyseFloatingPoint<IEEE_754_SinglePrecision>(const IEEE_754_SinglePrecision& value, unsigned int& precision, unsigned int* mantissa, int& exponent, unsigned int& flags) throw() {
  typedef IEEE_754_SinglePrecision Representation;

  unsigned int fieldExponent = value.exponent;
  mantissa[0] = value.mantissa0;
  flags = value.negative ? FP_NEGATIVE : 0;
  if (~fieldExponent == 0) {
    if (mantissa[0] == 0) { // check for infinity
      flags |= FP_INFINITY;
    } else {
      flags &= ~FP_NEGATIVE;
      flags |= FP_NAN;
    }
    precision = 0;
    exponent = 0;
  } else {
    flags |= FP_VALUE; // ordinary value
    if ((fieldExponent == 0) && (mantissa[0] == 0)) { // check for zero
      flags |= FP_ZERO;
      exponent = 0;
      precision = Representation::SIGNIFICANT;
    } else {
      if (fieldExponent == 0) { // denormalized value (does not have implied one)
        flags |= FP_DENORMALIZED;
      }

      if (Representation::HAS_IMPLIED_ONE) {
        if (flags & FP_DENORMALIZED) {
          precision = Representation::SIGNIFICANT - 1;
        } else {
          precision = Representation::SIGNIFICANT;
          mantissa[(Representation::SIGNIFICANT - 1)/32] |= 1 << ((Representation::SIGNIFICANT - 1)%32);
        }
      } else {
        precision = Representation::SIGNIFICANT;
      }
      exponent = fieldExponent - Representation::BIAS;
    }
  }
}

template<>
void analyseFloatingPoint<IEEE_754_DoublePrecision>(const IEEE_754_DoublePrecision& value, unsigned int& precision, unsigned int* mantissa, int& exponent, unsigned int& flags) throw() {
  typedef IEEE_754_DoublePrecision Representation;

  unsigned int fieldExponent = value.exponent;
  mantissa[0] = value.mantissa0;
  mantissa[1] = value.mantissa1;
  flags = value.negative ? FP_NEGATIVE : 0;
  if (~fieldExponent == 0) {
    if ((mantissa[1] == 0) && (mantissa[0] == 0)) { // check for infinity
      flags |= FP_INFINITY;
    } else {
      flags &= ~FP_NEGATIVE;
      flags |= FP_NAN;
    }
    precision = 0;
    exponent = 0;
  } else {
    flags |= FP_VALUE; // ordinary value
    if ((fieldExponent == 0) && (mantissa[1] == 0) && (mantissa[0] == 0)) { // check for zero
      flags |= FP_ZERO;
      exponent = 0;
      precision = Representation::SIGNIFICANT;
    } else {
      if (fieldExponent == 0) { // denormalized value (does not have implied one)
        flags |= FP_DENORMALIZED;
      }

      if (Representation::HAS_IMPLIED_ONE) {
        if (flags & FP_DENORMALIZED) {
          precision = Representation::SIGNIFICANT - 1;
        } else {
          precision = Representation::SIGNIFICANT;
          mantissa[(Representation::SIGNIFICANT - 1)/32] |= 1 << ((Representation::SIGNIFICANT - 1)%32);
        }
      } else {
        precision = Representation::SIGNIFICANT;
      }
      exponent = fieldExponent - Representation::BIAS;
    }
  }
}

template<>
void analyseFloatingPoint<IEEE_ExtendedDoublePrecision96>(const IEEE_ExtendedDoublePrecision96& value, unsigned int& precision, unsigned int* mantissa, int& exponent, unsigned int& flags) throw() {
  typedef IEEE_ExtendedDoublePrecision96 Representation;

  unsigned int fieldExponent = value.exponent;
  mantissa[0] = value.mantissa0;
  mantissa[1] = value.mantissa1;
  flags = value.negative ? FP_NEGATIVE : 0;
  if (~fieldExponent == 0) {
    if ((mantissa[1] == 0) && (mantissa[0] == 0)) { // check for infinity
      flags |= FP_INFINITY;
    } else {
      flags &= ~FP_NEGATIVE;
      flags |= FP_NAN;
    }
    precision = 0;
    exponent = 0;
  } else {
    flags |= FP_VALUE; // ordinary value
    if ((fieldExponent == 0) && (mantissa[1] == 0) && (mantissa[0] == 0)) { // check for zero
      flags |= FP_ZERO;
      exponent = 0;
      precision = Representation::SIGNIFICANT;
    } else {
      if (fieldExponent == 0) { // denormalized value (does not have implied one)
        flags |= FP_DENORMALIZED;
      }

      if (Representation::HAS_IMPLIED_ONE) {
        if (flags & FP_DENORMALIZED) {
          precision = Representation::SIGNIFICANT - 1;
        } else {
          precision = Representation::SIGNIFICANT;
          mantissa[(Representation::SIGNIFICANT - 1)/32] |= 1 << ((Representation::SIGNIFICANT - 1)%32);
        }
      } else {
        precision = Representation::SIGNIFICANT;
      }
      exponent = fieldExponent - Representation::BIAS;
    }
  }
}

template<>
void analyseFloatingPoint<IEEE_ExtendedDoublePrecision128>(const IEEE_ExtendedDoublePrecision128& value, unsigned int& precision, unsigned int* mantissa, int& exponent, unsigned int& flags) throw() {
  typedef IEEE_ExtendedDoublePrecision128 Representation;

  unsigned int fieldExponent = value.exponent;
  mantissa[0] = value.mantissa0;
  mantissa[1] = value.mantissa1;
  flags = value.negative ? FP_NEGATIVE : 0;
  if (~fieldExponent == 0) {
    if ((mantissa[1] == 0) && (mantissa[0] == 0)) { // check for infinity
      flags |= FP_INFINITY;
    } else {
      flags &= ~FP_NEGATIVE;
      flags |= FP_NAN;
    }
    precision = 0;
    exponent = 0;
  } else {
    flags |= FP_VALUE; // ordinary value
    if ((fieldExponent == 0) && (mantissa[1] == 0) && (mantissa[0] == 0)) { // check for zero
      flags |= FP_ZERO;
      exponent = 0;
      precision = Representation::SIGNIFICANT;
    } else {
      if (fieldExponent == 0) { // denormalized value (does not have implied one)
        flags |= FP_DENORMALIZED;
      }

      if (Representation::HAS_IMPLIED_ONE) {
        if (flags & FP_DENORMALIZED) {
          precision = Representation::SIGNIFICANT - 1;
        } else {
          precision = Representation::SIGNIFICANT;
          mantissa[(Representation::SIGNIFICANT - 1)/32] |= 1 << ((Representation::SIGNIFICANT - 1)%32);
        }
      } else {
        precision = Representation::SIGNIFICANT;
      }
      exponent = fieldExponent - Representation::BIAS;
    }
  }
}

template<>
void analyseFloatingPoint<IEEE_QuadruplePrecision>(const IEEE_QuadruplePrecision& value, unsigned int& precision, unsigned int* mantissa, int& exponent, unsigned int& flags) throw() {
  typedef IEEE_QuadruplePrecision Representation;

  unsigned int fieldExponent = value.exponent;
  mantissa[0] = value.mantissa0;
  mantissa[1] = value.mantissa1;
  mantissa[2] = value.mantissa2;
  mantissa[3] = value.mantissa3;
  flags = value.negative ? FP_NEGATIVE : 0;
  if (~fieldExponent == 0) {
    if ((mantissa[3] == 0) && (mantissa[2] == 0) && (mantissa[1] == 0) && (mantissa[0] == 0)) { // check for infinity
      flags |= FP_INFINITY;
    } else {
      flags &= ~FP_NEGATIVE;
      flags |= FP_NAN;
    }
    precision = 0;
    exponent = 0;
  } else {
    flags |= FP_VALUE; // ordinary value
    if ((fieldExponent == 0) && (mantissa[3] == 0) && (mantissa[2] == 0) && (mantissa[1] == 0) && (mantissa[0] == 0)) { // check for zero
      flags |= FP_ZERO;
      exponent = 0;
      precision = Representation::SIGNIFICANT;
    } else {
      if (fieldExponent == 0) { // denormalized value (does not have implied one)
        flags |= FP_DENORMALIZED;
      }

      if (Representation::HAS_IMPLIED_ONE) {
        if (flags & FP_DENORMALIZED) {
          precision = Representation::SIGNIFICANT - 1;
        } else {
          precision = Representation::SIGNIFICANT;
          mantissa[(Representation::SIGNIFICANT - 1)/32] |= 1 << ((Representation::SIGNIFICANT - 1)%32);
        }
      } else {
        precision = Representation::SIGNIFICANT;
      }
      exponent = fieldExponent - Representation::BIAS;
    }
  }
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
