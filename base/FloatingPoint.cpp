/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/FloatingPoint.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

void FloatingPoint::IEEE754SinglePrecision::setValue(const FloatingPoint::Representation::IEEEExtendedDoublePrecision96& _value) throw() {
  value.negative = _value.negative;
  if (~_value.exponent == 0) {
    value.exponent = ~0;
    if ((_value.mantissa1 == 0) && (_value.mantissa0 == 0)) {
      value.mantissa0 = 0; // infinity
    } else {
      value.mantissa0 = ~0; // nan
    }
  } else if ((_value.exponent == 0) && (_value.mantissa1 == 0) && (_value.mantissa0 == 0)) { // check for zero
    value.exponent = 0;
    value.mantissa0 = 0;
  } else {
    int exponent = _value.exponent - _value.BIAS + value.BIAS;
    if (_value.mantissa1 == 0) {
      if (_value.mantissa0 == 0) {
        value.mantissa0 = 0;
        value.exponent = 0;
      } else {
        exponent -= 32;
        unsigned int mantissa = _value.mantissa0;
        unsigned int shift = 0;
        while ((mantissa >> 31) == 0) { // check if first is 1
          shift++;
          mantissa <<= 1;
        }
        shift++; // skip the 1
        mantissa <<= 1;
        exponent -= shift;
        value.mantissa0 = mantissa >> (32 - 23);
        value.exponent = exponent; // possible overflow and underflow
      }
    } else {
      unsigned int mantissa = _value.mantissa1;
      unsigned int shift = 0;
      while ((mantissa >> 31) == 0) { // check if first is 1
        shift++;
        mantissa <<= 1;
      }
      shift++; // skip the 1
      mantissa <<= 1;
      exponent -= shift;
      mantissa |= _value.mantissa0 >> (32 - shift);
      value.mantissa0 = mantissa >> (32 - 23);
      value.exponent = exponent; // possible overflow and underflow
    }
  }
}
    
void FloatingPoint::IEEE754SinglePrecision::setValue(const FloatingPoint::Representation::IEEEExtendedDoublePrecision128& _value) throw() {
  value.negative = _value.negative;
  if (~_value.exponent == 0) {
    value.exponent = ~0;
    if ((_value.mantissa1 == 0) && (_value.mantissa0 == 0)) {
      value.mantissa0 = 0; // infinity
    } else {
      value.mantissa0 = ~0; // nan
    }
  } else if ((_value.exponent == 0) && (_value.mantissa1 == 0) && (_value.mantissa0 == 0)) { // check for zero
    value.exponent = 0;
    value.mantissa0 = 0;
  } else {
    int exponent = _value.exponent - _value.BIAS + value.BIAS;
    if (_value.mantissa1 == 0) {
      if (_value.mantissa0 == 0) {
        value.mantissa0 = 0;
        value.exponent = 0;
      } else {
        exponent -= 32;
        unsigned int mantissa = _value.mantissa0;
        unsigned int shift = 0;
        while ((mantissa >> 31) == 0) { // check if first is 1
          shift++;
          mantissa <<= 1;
        }
        shift++; // skip the 1
        mantissa <<= 1;
        exponent -= shift;
        value.mantissa0 = mantissa >> (32 - 23);
        value.exponent = exponent; // possible overflow and underflow
      }
    } else {
      unsigned int mantissa = _value.mantissa1;
      unsigned int shift = 0;
      while ((mantissa >> 31) == 0) { // check if first is 1
        shift++;
        mantissa <<= 1;
      }
      shift++; // skip the 1
      mantissa <<= 1;
      exponent -= shift;
      mantissa |= _value.mantissa0 >> (32 - shift);
      value.mantissa0 = mantissa >> (32 - 23);
      value.exponent = exponent; // possible overflow and underflow
    }
  }
}

void FloatingPoint::IEEE754DoublePrecision::setValue(const FloatingPoint::Representation::IEEEExtendedDoublePrecision96& _value) throw() {
  value.negative = _value.negative;
  if (~_value.exponent == 0) {
    value.exponent = ~0;
    if ((_value.mantissa1 == 0) && (_value.mantissa0 == 0)) {
      value.mantissa1 = 0; // infinity
      value.mantissa0 = 0; // infinity
    } else {
      value.mantissa1 = ~0; // nan
      value.mantissa0 = ~0; // nan
    }
  } else if ((_value.exponent == 0) && (_value.mantissa1 == 0) && (_value.mantissa0 == 0)) { // check for zero
    value.exponent = 0;
    value.mantissa1 = 0;
    value.mantissa0 = 0;
  } else {
    int exponent = _value.exponent - _value.BIAS + value.BIAS;
    if (_value.mantissa1 == 0) {
      if (_value.mantissa0 == 0) {
        value.mantissa1 = 0;
        value.mantissa0 = 0;
        value.exponent = 0;
      } else {
        exponent -= 32;
        unsigned int mantissa = _value.mantissa0;
        unsigned int shift = 0;
        while ((mantissa >> 31) == 0) { // check if first is 1
          shift++;
          mantissa <<= 1;
        }
        shift++; // skip the 1
        mantissa <<= 1;
        exponent -= shift;
        value.mantissa1 = mantissa >> (32 - 20);
        value.mantissa0 = static_cast<uint32>(mantissa) << (32 - 20);
        value.exponent = exponent; // possible overflow and underflow
      }
    } else {
      unsigned int mantissa = _value.mantissa1;
      unsigned int shift = 0;
      while ((mantissa >> 31) == 0) { // check if first is 1
        shift++;
        mantissa <<= 1;
      }
      shift++; // skip the 1
      mantissa <<= 1;
      exponent -= shift;
      mantissa |= _value.mantissa0 >> (32 - shift);
      unsigned int mantissa0 = _value.mantissa0 << (32 - shift);
      value.mantissa1 = mantissa >> (32 - 20);
      value.mantissa0 = (static_cast<uint32>(mantissa) << (32 - 20)) | (mantissa0 >> (32 - 20));
      value.exponent = exponent; // possible overflow and underflow
    }
  }
}
    
void FloatingPoint::IEEE754DoublePrecision::setValue(const FloatingPoint::Representation::IEEEExtendedDoublePrecision128& _value) throw() {
  value.negative = _value.negative;
  if (~_value.exponent == 0) {
    value.exponent = ~0;
    if ((_value.mantissa1 == 0) && (_value.mantissa0 == 0)) {
      value.mantissa1 = 0; // infinity
      value.mantissa0 = 0; // infinity
    } else {
      value.mantissa1 = ~0; // nan
      value.mantissa0 = ~0; // nan
    }
  } else if ((_value.exponent == 0) && (_value.mantissa1 == 0) && (_value.mantissa0 == 0)) { // check for zero
    value.exponent = 0;
    value.mantissa1 = 0;
    value.mantissa0 = 0;
  } else {
    int exponent = _value.exponent - _value.BIAS + value.BIAS;
    if (_value.mantissa1 == 0) {
      if (_value.mantissa0 == 0) {
        value.mantissa1 = 0;
        value.mantissa0 = 0;
        value.exponent = 0;
      } else {
        exponent -= 32;
        unsigned int mantissa = _value.mantissa0;
        unsigned int shift = 0;
        while ((mantissa >> 31) == 0) { // check if first is 1
          shift++;
          mantissa <<= 1;
        }
        shift++; // skip the 1
        mantissa <<= 1;
        exponent -= shift;
        value.mantissa1 = mantissa >> (32 - 20);
        value.mantissa0 = static_cast<uint32>(mantissa) << (32 - 20);
        value.exponent = exponent; // possible overflow and underflow
      }
    } else {
      unsigned int mantissa = _value.mantissa1;
      unsigned int shift = 0;
      while ((mantissa >> 31) == 0) { // check if first is 1
        shift++;
        mantissa <<= 1;
      }
      shift++; // skip the 1
      mantissa <<= 1;
      exponent -= shift;
      mantissa |= _value.mantissa0 >> (32 - shift);
      unsigned int mantissa0 = _value.mantissa0 << (32 - shift);
      value.mantissa1 = mantissa >> (32 - 20);
      value.mantissa0 = (static_cast<uint32>(mantissa) << (32 - 20)) | (mantissa0 >> (32 - 20));
      value.exponent = exponent; // possible overflow and underflow
    }
  }
}

void FloatingPoint::IEEEQuadruplePrecision::setValue(const FloatingPoint::Representation::IEEEExtendedDoublePrecision96& _value) throw() {
  value.negative = _value.negative;
  if (~_value.exponent == 0) {
    if ((_value.mantissa1 == 0) && (_value.mantissa0 == 0)) {
      value.mantissa3 = 0; // infinity
      value.mantissa2 = 0; // infinity
      value.mantissa1 = 0; // infinity
      value.mantissa0 = 0; // infinity
    } else {
      value.mantissa3 = ~0; // nan
      value.mantissa2 = ~0; // nan
      value.mantissa1 = ~0; // nan
      value.mantissa0 = ~0; // nan
    }
    value.exponent = ~0;
  } else if ((_value.exponent == 0) && (_value.mantissa1 == 0) && (_value.mantissa0 == 0)) { // check for zero
    value.mantissa3 = 0;
    value.mantissa2 = 0;
    value.mantissa1 = 0;
    value.mantissa0 = 0;
    value.exponent = 0;
  } else {
    int exponent = _value.exponent - _value.BIAS + value.BIAS;
    if (_value.mantissa1 == 0) {
      if (_value.mantissa0 == 0) {
        value.mantissa3 = 0;
        value.mantissa2 = 0;
        value.mantissa1 = 0;
        value.mantissa0 = 0;
        value.exponent = 0;
      } else {
        exponent -= 32;
        unsigned int mantissa = _value.mantissa0;
        unsigned int shift = 0;
        while ((mantissa >> 31) == 0) { // check if first is 1
          shift++;
          mantissa <<= 1;
        }
        shift++; // skip the 1
        mantissa <<= 1;
        exponent -= shift;
        value.mantissa3 = mantissa >> (32 - 16);
        value.mantissa2 = static_cast<uint32>(mantissa) << (32 - 16);
        value.mantissa1 = 0;
        value.mantissa0 = 0;
        value.exponent = exponent;
      }
    } else {
      unsigned int mantissa = _value.mantissa1;
      unsigned int shift = 0;
      while ((mantissa >> 31) == 0) { // check if first is 1
        shift++;
        mantissa <<= 1;
      }
      shift++; // skip the 1
      mantissa <<= 1;
      exponent -= shift;
      mantissa |= _value.mantissa0 >> (32 - shift);
      unsigned int mantissa0 = _value.mantissa0 << (32 - shift);
      value.mantissa3 = mantissa >> (32 - 16);
      value.mantissa2 = (static_cast<uint32>(mantissa) << (32 - 16)) | (mantissa0 >> (32 - 16));
      value.mantissa1 = mantissa0 << (32 - 16);
      value.mantissa0 = 0;
      value.exponent = exponent;
    }
  }
}

void FloatingPoint::IEEEQuadruplePrecision::setValue(const FloatingPoint::Representation::IEEEExtendedDoublePrecision128& _value) throw() {
  value.negative = _value.negative;
  if (~_value.exponent == 0) {
    if ((_value.mantissa1 == 0) && (_value.mantissa0 == 0)) {
      value.mantissa3 = 0; // infinity
      value.mantissa2 = 0; // infinity
      value.mantissa1 = 0; // infinity
      value.mantissa0 = 0; // infinity
    } else {
      value.mantissa3 = ~0; // nan
      value.mantissa2 = ~0; // nan
      value.mantissa1 = ~0; // nan
      value.mantissa0 = ~0; // nan
    }
    value.exponent = ~0;
  } else if ((_value.exponent == 0) && (_value.mantissa1 == 0) && (_value.mantissa0 == 0)) { // check for zero
    value.mantissa3 = 0;
    value.mantissa2 = 0;
    value.mantissa1 = 0;
    value.mantissa0 = 0;
    value.exponent = 0;
  } else {
    int exponent = _value.exponent - _value.BIAS + value.BIAS;
    if (_value.mantissa1 == 0) {
      if (_value.mantissa0 == 0) {
        value.mantissa3 = 0;
        value.mantissa2 = 0;
        value.mantissa1 = 0;
        value.mantissa0 = 0;
        value.exponent = 0;
      } else {
        exponent -= 32;
        unsigned int mantissa = _value.mantissa0;
        unsigned int shift = 0;
        while ((mantissa >> 31) == 0) { // check if first is 1
          shift++;
          mantissa <<= 1;
        }
        shift++; // skip the 1
        mantissa <<= 1;
        exponent -= shift;
        value.mantissa3 = mantissa >> (32 - 16);
        value.mantissa2 = static_cast<uint32>(mantissa) << (32 - 16);
        value.mantissa1 = 0;
        value.mantissa0 = 0;
        value.exponent = exponent;
      }
    } else {
      unsigned int mantissa = _value.mantissa1;
      unsigned int shift = 0;
      while ((mantissa >> 31) == 0) { // check if first is 1
        shift++;
        mantissa <<= 1;
      }
      shift++; // skip the 1
      mantissa <<= 1;
      exponent -= shift;
      mantissa |= _value.mantissa0 >> (32 - shift);
      unsigned int mantissa0 = _value.mantissa0 << (32 - shift);
      value.mantissa3 = mantissa >> (32 - 16);
      value.mantissa2 = (static_cast<uint32>(mantissa) << (32 - 16)) | (mantissa0 >> (32 - 16));
      value.mantissa1 = mantissa0 << (32 - 16);
      value.mantissa0 = 0;
      value.exponent = exponent;
    }
  }
}

template<>
void analyseFloatingPoint<FloatingPoint::Representation::IEEE754SinglePrecision>(
  const FloatingPoint::Representation::IEEE754SinglePrecision& value,
  unsigned int& precision,
  unsigned int* mantissa,
  int& exponent,
  unsigned int& flags) throw() {
  typedef FloatingPoint::Representation::IEEE754SinglePrecision Representation;
  
  unsigned int fieldExponent = value.exponent;
  mantissa[0] = value.mantissa0;
  flags = value.negative ? FloatingPoint::FP_NEGATIVE : 0;
  if (~fieldExponent == 0) {
    if (mantissa[0] == 0) { // check for infinity
      flags |= FloatingPoint::FP_INFINITY;
    } else {
      flags &= ~FloatingPoint::FP_NEGATIVE;
      flags |= FloatingPoint::FP_NAN;
    }
    precision = 0;
    exponent = 0;
  } else {
    flags |= FloatingPoint::FP_VALUE; // ordinary value
    if ((fieldExponent == 0) && (mantissa[0] == 0)) { // check for zero
      flags |= FloatingPoint::FP_ZERO;
      exponent = 0;
      precision = Representation::SIGNIFICANT;
    } else {
      if (fieldExponent == 0) { // denormalized value (does not have implied one)
        flags |= FloatingPoint::FP_DENORMALIZED;
      }

      if (Representation::HAS_IMPLIED_ONE) {
        if (flags & FloatingPoint::FP_DENORMALIZED) {
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
void analyseFloatingPoint<FloatingPoint::Representation::IEEE754DoublePrecision>(
  const FloatingPoint::Representation::IEEE754DoublePrecision& value,
  unsigned int& precision,
  unsigned int* mantissa,
  int& exponent,
  unsigned int& flags) throw() {
  typedef FloatingPoint::Representation::IEEE754DoublePrecision Representation;
  
  unsigned int fieldExponent = value.exponent;
  mantissa[0] = value.mantissa0;
  mantissa[1] = value.mantissa1;
  flags = value.negative ? FloatingPoint::FP_NEGATIVE : 0;
  if (~fieldExponent == 0) {
    if ((mantissa[1] == 0) && (mantissa[0] == 0)) { // check for infinity
      flags |= FloatingPoint::FP_INFINITY;
    } else {
      flags &= ~FloatingPoint::FP_NEGATIVE;
      flags |= FloatingPoint::FP_NAN;
    }
    precision = 0;
    exponent = 0;
  } else {
    flags |= FloatingPoint::FP_VALUE; // ordinary value
    if ((fieldExponent == 0) && (mantissa[1] == 0) && (mantissa[0] == 0)) { // check for zero
      flags |= FloatingPoint::FP_ZERO;
      exponent = 0;
      precision = Representation::SIGNIFICANT;
    } else {
      if (fieldExponent == 0) { // denormalized value (does not have implied one)
        flags |= FloatingPoint::FP_DENORMALIZED;
      }

      if (Representation::HAS_IMPLIED_ONE) {
        if (flags & FloatingPoint::FP_DENORMALIZED) {
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
void analyseFloatingPoint<FloatingPoint::Representation::IEEEExtendedDoublePrecision96>(
  const FloatingPoint::Representation::IEEEExtendedDoublePrecision96& value,
  unsigned int& precision,
  unsigned int* mantissa,
  int& exponent,
  unsigned int& flags) throw() {
  typedef FloatingPoint::Representation::IEEEExtendedDoublePrecision96 Representation;
  
  unsigned int fieldExponent = value.exponent;
  mantissa[0] = value.mantissa0;
  mantissa[1] = value.mantissa1;
  flags = value.negative ? FloatingPoint::FP_NEGATIVE : 0;
  if (~fieldExponent == 0) {
    if ((mantissa[1] == 0) && (mantissa[0] == 0)) { // check for infinity
      flags |= FloatingPoint::FP_INFINITY;
    } else {
      flags &= ~FloatingPoint::FP_NEGATIVE;
      flags |= FloatingPoint::FP_NAN;
    }
    precision = 0;
    exponent = 0;
  } else {
    flags |= FloatingPoint::FP_VALUE; // ordinary value
    if ((fieldExponent == 0) && (mantissa[1] == 0) && (mantissa[0] == 0)) { // check for zero
      flags |= FloatingPoint::FP_ZERO;
      exponent = 0;
      precision = Representation::SIGNIFICANT;
    } else {
      if (fieldExponent == 0) { // denormalized value (does not have implied one)
        flags |= FloatingPoint::FP_DENORMALIZED;
      }

      if (Representation::HAS_IMPLIED_ONE) {
        if (flags & FloatingPoint::FP_DENORMALIZED) {
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
void analyseFloatingPoint<FloatingPoint::Representation::IEEEExtendedDoublePrecision128>(
  const FloatingPoint::Representation::IEEEExtendedDoublePrecision128& value,
  unsigned int& precision,
  unsigned int* mantissa,
  int& exponent,
  unsigned int& flags) throw() {
  typedef FloatingPoint::Representation::IEEEExtendedDoublePrecision128 Representation;
  
  unsigned int fieldExponent = value.exponent;
  mantissa[0] = value.mantissa0;
  mantissa[1] = value.mantissa1;
  flags = value.negative ? FloatingPoint::FP_NEGATIVE : 0;
  if (~fieldExponent == 0) {
    if ((mantissa[1] == 0) && (mantissa[0] == 0)) { // check for infinity
      flags |= FloatingPoint::FP_INFINITY;
    } else {
      flags &= ~FloatingPoint::FP_NEGATIVE;
      flags |= FloatingPoint::FP_NAN;
    }
    precision = 0;
    exponent = 0;
  } else {
    flags |= FloatingPoint::FP_VALUE; // ordinary value
    if ((fieldExponent == 0) && (mantissa[1] == 0) && (mantissa[0] == 0)) { // check for zero
      flags |= FloatingPoint::FP_ZERO;
      exponent = 0;
      precision = Representation::SIGNIFICANT;
    } else {
      if (fieldExponent == 0) { // denormalized value (does not have implied one)
        flags |= FloatingPoint::FP_DENORMALIZED;
      }

      if (Representation::HAS_IMPLIED_ONE) {
        if (flags & FloatingPoint::FP_DENORMALIZED) {
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
void analyseFloatingPoint<FloatingPoint::Representation::IEEEQuadruplePrecision>(
  const FloatingPoint::Representation::IEEEQuadruplePrecision& value,
  unsigned int& precision,
  unsigned int* mantissa,
  int& exponent,
  unsigned int& flags) throw() {
  typedef FloatingPoint::Representation::IEEEQuadruplePrecision Representation;
  
  unsigned int fieldExponent = value.exponent;
  mantissa[0] = value.mantissa0;
  mantissa[1] = value.mantissa1;
  mantissa[2] = value.mantissa2;
  mantissa[3] = value.mantissa3;
  flags = value.negative ? FloatingPoint::FP_NEGATIVE : 0;
  if (~fieldExponent == 0) {
    if ((mantissa[3] == 0) && (mantissa[2] == 0) && (mantissa[1] == 0) && (mantissa[0] == 0)) { // check for infinity
      flags |= FloatingPoint::FP_INFINITY;
    } else {
      flags &= ~FloatingPoint::FP_NEGATIVE;
      flags |= FloatingPoint::FP_NAN;
    }
    precision = 0;
    exponent = 0;
  } else {
    flags |= FloatingPoint::FP_VALUE; // ordinary value
    if ((fieldExponent == 0) &&
        (mantissa[3] == 0) && (mantissa[2] == 0) && (mantissa[1] == 0) && (mantissa[0] == 0)) { // check for zero
      flags |= FloatingPoint::FP_ZERO;
      exponent = 0;
      precision = Representation::SIGNIFICANT;
    } else {
      if (fieldExponent == 0) { // denormalized value (does not have implied one)
        flags |= FloatingPoint::FP_DENORMALIZED;
      }

      if (Representation::HAS_IMPLIED_ONE) {
        if (flags & FloatingPoint::FP_DENORMALIZED) {
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
