/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/FloatingPoint.h>
#include <base/string/ASCIITraits.h>
#include <base/mathematics/Math.h>
#include <base/UnitTest.h>
#include <limits>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

// TAG: locale specific
// TAG: support hex format
// TAG: support grouping of digits

float FloatingPoint::getFloatAsHex(const String& value) throw(InvalidFormat) {
  String::ReadIterator i = value.getBeginReadIterator();
  const String::ReadIterator end = value.getEndReadIterator();
  
  // precision
  const int MAXIMUM_NUMBER_OF_DIGITS = // max. significant digits
    (FloatRepresentation::SIGNIFICANT +
     (FloatRepresentation::HAS_IMPLIED_ONE ? 1 : 0) + (16/2 - 1))/(16/2);
  uint8 mantissa[MAXIMUM_NUMBER_OF_DIGITS];
  int numberOfDigits = 0; // total number of mantissa digits
  int scale = 0; // 2^scale
  
  // skip leading spaces // TAG: check flags if spaces should be skipped
  while ((i != end) && ASCIITraits::isSpace(*i)) {
    ++i;
  }
  
  // TAG: check flags if named special values are allowed
  if ((i != end) && !ASCIITraits::isHexDigit(*i)) {
    // compare case-less from i to end (possible excluding ending spaces)
    if (value == "nan") {
      return 0; // TAG: fixme
    } else if ((value == "+infinity") || (value == "infinity")) {
      return 0; // TAG: fixme
    } else if (value == "-infinity") {
      return 0; // TAG: fixme
    } else {
      throw InvalidFormat("Not a number", Type::getType<FloatingPoint>());
    }
  }
  
  // skip prefix 0x or 0X // TAG: check flags if required
  if ((i != end) && (*i == '0')) {
    ++i;
    if ((i != end) && ((*i == 'x') || (*i == 'X'))) {
      ++i;
    } else {
      throw InvalidFormat("Not a number", Type::getType<FloatingPoint>());
    }
  } else {
    throw InvalidFormat("Not a number", Type::getType<FloatingPoint>());
  }
  
  // read sign if present // TAG: what order of prefix and sign
  // TAG: check flags if sign is allowed
  bool negative = false;
  if (i != end) {
    if (*i == '+') {
      ++i;
    } else if (*i == '-') {
      negative = true;
      ++i;
    }
  }
  
  // read digits if present before possible radix
  if ((i != end) && ASCIITraits::isHexDigit(*i)) {
    while ((i != end) && ASCIITraits::isHexDigit(*i)) {
      if (numberOfDigits < MAXIMUM_NUMBER_OF_DIGITS) {
        mantissa[numberOfDigits++] = ASCIITraits::digitToValue(*i);
      }
      ++i;
    }
  }
  
  // read radix if present
  if ((i != end) && (*i == '.')) {
    ++i;
    scale = 16/4 * numberOfDigits;
    
    // read digits if present after radix
    if ((i != end) && ASCIITraits::isHexDigit(*i)) {
      // not normalized
      if (numberOfDigits == 0) {
        while (*i == '0') {
          scale -= 16/4;
          ++i;
        }
      }
      while ((i != end) && ASCIITraits::isHexDigit(*i)) {
        if (numberOfDigits < MAXIMUM_NUMBER_OF_DIGITS) {
          mantissa[numberOfDigits++] = ASCIITraits::digitToValue(*i);
        }
        ++i;
      }
    }
  }
  
  if ((numberOfDigits == 0) && (scale == 0)) {
    throw InvalidFormat("Not a number", Type::getType<FloatingPoint>());
  }
  
  int exponent = 0;
  // read exponent if present
  if ((i != end) && ((*i == 'p') || (*i == 'P'))) {
    ++i;
    
    bool negativeExponent = false;
    // read sign of exponent if present
    if (i != end) {
      if (*i == '+') {
        ++i;
      } else if (*i == '-') {
        negative = true;
        ++i;
      }
    }
    
    bassert(
      (i != end) && ASCIITraits::isDigit(*i),
      InvalidFormat("Not a number", Type::getType<FloatingPoint>())
    );
    
    // read exponent value
    if (negativeExponent) {
      while (i != end) {
        char ch = *i++;
        if (!ASCIITraits::isDigit(ch)) {
          break;
        }
        exponent = exponent * 10 - ASCIITraits::digitToValue(ch);
        bassert(
          exponent <= MAXIMUM_BINARY_EXPONENT,
          InvalidFormat("Invalid exponent", Type::getType<FloatingPoint>())
        );
      }
    } else {
      while (i != end) {
        char ch = *i++;
        if (!ASCIITraits::isDigit(ch)) {
          break;
        }
        exponent = exponent * 10 + ASCIITraits::digitToValue(ch);
        bassert(
          exponent <= MAXIMUM_BINARY_EXPONENT,
          InvalidFormat("Invalid exponent", Type::getType<FloatingPoint>())
        );
      }
    }
  }
  
  // suffix spaces // TAG: check flags if spaces should be skipped
  while ((i != end) && ASCIITraits::isSpace(*i)) {
    ++i;
  }
  
  if (i != end) {
    throw InvalidFormat("Not a number");
  }

  if (numberOfDigits == 0) {
    return 0.0f;
  }
  
  FloatingPoint::IEEE754SinglePrecision result(
    negative,
    mantissa,
    numberOfDigits,
    exponent + scale
  );
  
  return static_cast<float>(result);
}

FloatingPoint::IEEE754SinglePrecision::IEEE754SinglePrecision(
  bool negative,
  const uint8* mantissa,
  unsigned int size,
  int exponent) throw(InvalidFormat) {
  static const int EXPLICIT_SIGNIFICANT =
    value.HAS_IMPLIED_ONE ? (value.SIGNIFICANT - 1) : value.SIGNIFICANT;
  value.negative = negative;
  
  bassert(
    (exponent >= -static_cast<int>(MAXIMUM_BINARY_EXPONENT)) && (exponent <= MAXIMUM_BINARY_EXPONENT),
    InvalidFormat("Invalid exponent", Type::getType<FloatingPoint>())
  );
  
  while (size && (*mantissa == '0')) { // skip leading zeros
    exponent -= 4;
    --size;
    ++mantissa;
  }
  
  static const uint8 SIGNIFICANT_BITS[16] = {
    0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4 // 0 is not possible
  };
  
  const uint8 bitsInFirst = SIGNIFICANT_BITS[*mantissa & 0xf];
  size = minimum((value.SIGNIFICANT + 3)/4 + 1, size); // number of significant digits to use
  uint32 mantissa0 = 0;
  for (unsigned int i = size; i > 0; i--) {
    mantissa0 = (mantissa0 << 4) | static_cast<unsigned int>(*mantissa++ & 0xf);
  }
  mantissa0 >>= (bitsInFirst + (value.SIGNIFICANT + 3)/4) - value.SIGNIFICANT - 1; // remove garbage
  ++mantissa0; // round up
  mantissa0 >>= 1;
  if (mantissa0 >> value.SIGNIFICANT) {
    mantissa0 >>= 1;
    ++exponent;
  }
  
  if (exponent < value.MINIMUM_EXPONENT) {
    const int shift = value.MINIMUM_EXPONENT - exponent;
    // TAG: check flags if denormalization is allowed
    // TAG: check flags if we should use zero if shift exceeds EXPLICIT_SIGNIFICANT
    bassert(
      shift <= EXPLICIT_SIGNIFICANT, // cannot shift out implied one
      InvalidFormat("Underflow", Type::getType<FloatingPoint>())
    );
    mantissa0 >>= shift; // denormalize
    exponent += shift;
  } else if (exponent > value.MAXIMUM_EXPONENT) {
    throw InvalidFormat("Overflow", Type::getType<FloatingPoint>());
  }
  value.mantissa0 = mantissa0;
  value.exponent = exponent + value.BIAS;
}

float FloatingPoint::getFloat(const String& value) throw(InvalidFormat) {
  String::ReadIterator i = value.getBeginReadIterator();
  const String::ReadIterator end = value.getEndReadIterator();
  
  bool digits = false;

  // skip leadign spaces
  while ((i != end) && ASCIITraits::isSpace(*i)) {
    ++i;
  }

  // read sign if present
  bool negative = false;
  if (i != end) {
    if (*i == '+') {
      ++i;
    } else if (*i == '-') {
      negative = true;
      ++i;
    }
  }
  
  // TAG: check for nan, nan(xxxxxxxxxxxxx), +-inf, +-infinity
  
  // TAG: check for 0x or 0X and read as hexadecimal format if present
  
  // read digits if present before possible radix
  if ((i != end) && ASCIITraits::isDigit(*i)) {
    digits = true;
    while ((i != end) && (*i == '0')) { // skip leading zeros
      ++i;
    }
    
    while ((i != end) && ASCIITraits::isDigit(*i)) {
      ++i;
    }
  }
  
  // read radix if present
  if ((i != end) && (*i == '.')) {
    ++i;
    
    // read digits if present after radix
    if ((i != end) && ASCIITraits::isDigit(*i)) {
      digits = true;
      while ((i != end) && ASCIITraits::isDigit(*i)) {
        ++i;
      }
    }
  }

  if (!digits) {
    throw InvalidFormat("Not a number");
  }

  // read exponent if present
  if ((i != end) && ((*i == 'e') || (*i == 'E'))) {
    ++i;
    
    // read sign of exponent if present
    if (i != end) {
      if (*i == '+') {
        ++i;
      } else if (*i == '-') {
        ++i;
      }
    }
    
    // read exponent value
    if ((i != end) && ASCIITraits::isDigit(*i)) {
      while ((i != end) && ASCIITraits::isDigit(*i)) {
        ++i;
      }
    } else {
      throw InvalidFormat("Not a number");
    }
  }
  
  // TAG: skip spaces?
  
  if (i != end) {
    throw InvalidFormat("Not a number");
  }
  
  // TAG: fixme
  throw InvalidFormat("Not a number");
  return 0;
}
  
double FloatingPoint::getDouble(const String& value) throw(InvalidFormat) {
  // TAG: fixme
  return 0;
}

long double FloatingPoint::getLongDouble(const String& value) throw(InvalidFormat) {
  // TAG: fixme
  return 0;
}

void FloatingPoint::IEEE754SinglePrecision::setValue(const FloatingPoint::Representation::IEEEExtendedDoublePrecision96& _value) noexcept
{
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
    
void FloatingPoint::IEEE754SinglePrecision::setValue(const FloatingPoint::Representation::IEEEExtendedDoublePrecision128& _value) noexcept
{
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

void FloatingPoint::IEEE754DoublePrecision::setValue(const FloatingPoint::Representation::IEEEExtendedDoublePrecision96& _value) noexcept
{
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
    
void FloatingPoint::IEEE754DoublePrecision::setValue(const FloatingPoint::Representation::IEEEExtendedDoublePrecision128& _value) noexcept
{
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

void FloatingPoint::IEEEQuadruplePrecision::setValue(const FloatingPoint::Representation::IEEEExtendedDoublePrecision96& _value) noexcept
{
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

void FloatingPoint::IEEEQuadruplePrecision::setValue(const FloatingPoint::Representation::IEEEExtendedDoublePrecision128& _value) noexcept
{
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
  unsigned int& flags) noexcept
{
  typedef FloatingPoint::Representation::IEEE754SinglePrecision Representation;
  
  unsigned int fieldExponent = value.exponent;
  mantissa[0] = value.mantissa0;
  flags = value.negative ? FloatingPoint::FP_NEGATIVE : 0;
  if (~fieldExponent == 0) {
    if (mantissa[0] == 0) { // check for infinity
      flags |= FloatingPoint::FP_INFINITY;
    } else {
      flags &= ~FloatingPoint::FP_NEGATIVE;
      flags |= FloatingPoint::FP_ANY_NAN;
    }
    precision = 0;
    exponent = 0;
  } else {
    flags |= FloatingPoint::FP_VALUE; // ordinary value
    if ((fieldExponent == 0) && (mantissa[0] == 0)) { // check for zero
      flags |= FloatingPoint::FP_ANY_ZERO;
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
  unsigned int& flags) noexcept
{
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
      flags |= FloatingPoint::FP_ANY_NAN;
    }
    precision = 0;
    exponent = 0;
  } else {
    flags |= FloatingPoint::FP_VALUE; // ordinary value
    if ((fieldExponent == 0) && (mantissa[1] == 0) && (mantissa[0] == 0)) { // check for zero
      flags |= FloatingPoint::FP_ANY_ZERO;
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
  unsigned int& flags) noexcept
{
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
      flags |= FloatingPoint::FP_ANY_NAN;
    }
    precision = 0;
    exponent = 0;
  } else {
    flags |= FloatingPoint::FP_VALUE; // ordinary value
    if ((fieldExponent == 0) && (mantissa[1] == 0) && (mantissa[0] == 0)) { // check for zero
      flags |= FloatingPoint::FP_ANY_ZERO;
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
  unsigned int& flags) noexcept
{
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
      flags |= FloatingPoint::FP_ANY_NAN;
    }
    precision = 0;
    exponent = 0;
  } else {
    flags |= FloatingPoint::FP_VALUE; // ordinary value
    if ((fieldExponent == 0) && (mantissa[1] == 0) && (mantissa[0] == 0)) { // check for zero
      flags |= FloatingPoint::FP_ANY_ZERO;
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
  unsigned int& flags) noexcept
{
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
      flags |= FloatingPoint::FP_ANY_NAN;
    }
    precision = 0;
    exponent = 0;
  } else {
    flags |= FloatingPoint::FP_VALUE; // ordinary value
    if ((fieldExponent == 0) &&
        (mantissa[3] == 0) && (mantissa[2] == 0) && (mantissa[1] == 0) && (mantissa[0] == 0)) { // check for zero
      flags |= FloatingPoint::FP_ANY_ZERO;
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

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(FloatingPoint) : public UnitTest {
public:

  TEST_PRIORITY(10);
  TEST_PROJECT("base");

  template<typename TYPE>
  static TYPE muldiv2(volatile TYPE value, volatile unsigned int loops) noexcept
  {
    for (unsigned int i = 0; i < loops; ++i) {
      value *= 2; // expected to exact since it only requires exponent addition and subtraction
    }
    for (unsigned int i = 0; i < loops; ++i) {
      value /= 2; // expected to exact since it only requires exponent addition and subtraction
    }
    return value;
  }

  template<typename TYPE>
  static TYPE getEpsilon() noexcept
  {
    TYPE result = 1;
    while ((static_cast<TYPE>(1) + result) != static_cast<TYPE>(1)) {
      result /= 2;
    }
    return result * 2;
  }

  template<typename TYPE>
  void test_IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16(TYPE _value) {
    TEST_ASSERT(sizeof(TYPE) == 16);
    if (sizeof(TYPE) == 16) {
_COM_AZURE_DEV__BASE__PACKED__BEGIN
      union {
        uint32 words[4];
        TYPE value;
      } _COM_AZURE_DEV__BASE__PACKED data;
_COM_AZURE_DEV__BASE__PACKED__END
      data.words[0] = 0xabcdfedc;
      data.words[1] = 0xabcdfedc;
      data.words[2] = 0xabcdfedc;
      data.words[3] = 0xabcdfedc;
      data.value = _value;
      TEST_ASSERT(data.words[0] != 0xabcdfedc);
      TEST_ASSERT(data.words[1] != 0xabcdfedc);
      TEST_ASSERT(data.words[2] != 0xabcdfedc);
      TEST_ASSERT((data.words[2] >> 16) == 0xabcd); // if only 10 bytes are used
      TEST_ASSERT(data.words[3] == 0xabcdfedc);
    }
  }
  
  void run() override
  {
    volatile int zero = 0;

    TEST_ASSERT(sizeof(float) <= sizeof(double));
    TEST_ASSERT(sizeof(double) <= sizeof(long double));

    TEST_ASSERT(sizeof(float) == sizeof(FloatingPoint::ToFloat));
    TEST_ASSERT(sizeof(double) == sizeof(FloatingPoint::ToDouble));
    TEST_ASSERT(sizeof(long double) == sizeof(FloatingPoint::ToLongDouble));

#if (_COM_AZURE_DEV__BASE__FLOAT == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16)
    test_IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16<float>(-123.456f);
    test_IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16<float>(-123.456f/zero);
#endif

#if (_COM_AZURE_DEV__BASE__DOUBLE == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16)
    test_IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16<double>(-123.456);
    test_IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16<double>(-123.456/zero);
#endif

#if (_COM_AZURE_DEV__BASE__LONG_DOUBLE == _COM_AZURE_DEV__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16)
    test_IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16<long double>(-123.456L);
    test_IEEE_EXTENDED_DOUBLE_PRECISION_96_ALIGN16<long double>(-123.456L/zero);
#endif

    TEST_ASSERT(FloatingPoint::ToFloat(-0.0f).isNegative());
    TEST_ASSERT(FloatingPoint::ToDouble(-0.0).isNegative());
    TEST_ASSERT(FloatingPoint::ToLongDouble(-0.0l).isNegative());

    TEST_ASSERT(muldiv2(constant::EULER_F, 113) == constant::EULER_F);
    TEST_ASSERT(muldiv2(constant::EULER, 113) == constant::EULER);
    TEST_ASSERT(muldiv2(constant::EULER_L, 113) == constant::EULER_L);
    TEST_ASSERT(muldiv2(-constant::EULER_F, 117) == -constant::EULER_F);
    TEST_ASSERT(muldiv2(-constant::EULER, 117) == -constant::EULER);
    TEST_ASSERT(muldiv2(-constant::EULER_L, 117) == -constant::EULER_L);

    TEST_ASSERT(getEpsilon<float>() < 2e-7f);
    TEST_ASSERT(getEpsilon<double>() <= getEpsilon<float>());
    TEST_ASSERT(getEpsilon<long double>() <= getEpsilon<double>());
    /*
    fout << getEpsilon<float>() << ENDL;
    fout << getEpsilon<double>() << ENDL;
    fout << getEpsilon<long double>() << ENDL;
    */

    FloatingPoint::ToFloat f1(1.0f);
    TEST_ASSERT(f1.isOrdinary());
    TEST_ASSERT(!f1.isNaN());
    TEST_ASSERT(!f1.isInfinity());
    TEST_ASSERT(!f1.isQuiteNaN());
    TEST_ASSERT(!f1.isSignalingNaN());

    FloatingPoint::ToDouble f2(1.0);
    TEST_ASSERT(f2.isOrdinary());
    TEST_ASSERT(!f2.isNaN());
    TEST_ASSERT(!f2.isInfinity());
    TEST_ASSERT(!f2.isQuiteNaN());
    TEST_ASSERT(!f2.isSignalingNaN());

    FloatingPoint::ToLongDouble f3(1.0L);
    TEST_ASSERT(f3.isOrdinary());
    TEST_ASSERT(!f3.isNaN());
    TEST_ASSERT(!f3.isInfinity());
    TEST_ASSERT(!f3.isQuiteNaN());
    TEST_ASSERT(!f3.isSignalingNaN());

    FloatingPoint::ToFloat f_n1(-1.0f);
    TEST_ASSERT(f_n1.isNegative());

    FloatingPoint::ToDouble d_n1(-1.0);
    TEST_ASSERT(d_n1.isNegative());

    FloatingPoint::ToLongDouble ld_n1(-1.0L);
    TEST_ASSERT(ld_n1.isNegative());

    FloatingPoint::ToFloat f_1div0(1.0f/zero);
    TEST_ASSERT(!f_1div0.isNaN());
    TEST_ASSERT(f_1div0.isInfinity());

    FloatingPoint::ToFloat f_n1div0(-1.0f/zero);
    TEST_ASSERT(!f_n1div0.isNaN());
    TEST_ASSERT(f_n1div0.isInfinity());
    TEST_ASSERT((-1.0f / zero) < 0);

    FloatingPoint::ToDouble d_1div0(1.0/zero);
    TEST_ASSERT(!d_1div0.isNaN());
    TEST_ASSERT(d_1div0.isInfinity());

    FloatingPoint::ToDouble d_n1div0(-1.0/zero);
    TEST_ASSERT(!d_n1div0.isNaN());
    TEST_ASSERT(d_n1div0.isInfinity());
    TEST_ASSERT((-1.0 / zero) < 0);

    FloatingPoint::ToLongDouble ld_1div0(1.0L/zero);
    TEST_ASSERT(!ld_1div0.isNaN());
    TEST_ASSERT(ld_1div0.isInfinity());

    FloatingPoint::ToLongDouble ld_n1div0(-1.0L/zero);
    TEST_ASSERT(!ld_n1div0.isNaN());
    TEST_ASSERT(ld_n1div0.isInfinity());
    TEST_ASSERT((-1.0l / zero) < 0);

    FloatingPoint::ToFloat f_nan(nanf(""));
    TEST_ASSERT(f_nan.isNaN());
    TEST_ASSERT(f_nan.isQuiteNaN());
    TEST_ASSERT(!f_nan.isSignalingNaN());

    FloatingPoint::ToDouble d_nan(nan(""));
    TEST_ASSERT(d_nan.isNaN());
    TEST_ASSERT(d_nan.isQuiteNaN());
    TEST_ASSERT(!d_nan.isSignalingNaN());
    
    FloatingPoint::ToLongDouble ld_nan(nanl(""));
    TEST_ASSERT(ld_nan.isNaN());
    TEST_ASSERT(ld_nan.isQuiteNaN());
    TEST_ASSERT(!ld_nan.isSignalingNaN());

    FloatingPoint::ToFloat f_0div0(0.0f/zero);
    TEST_ASSERT(f_0div0.isNaN());
    TEST_ASSERT(f_0div0.isQuiteNaN());
    TEST_ASSERT(!f_0div0.isSignalingNaN());

    FloatingPoint::ToDouble d_0div0(0.0/zero);
    TEST_ASSERT(d_0div0.isNaN());
    TEST_ASSERT(d_0div0.isQuiteNaN());
    TEST_ASSERT(!d_0div0.isSignalingNaN());

    FloatingPoint::ToLongDouble ld_0div0(0.0L/zero);
    TEST_ASSERT(ld_0div0.isNaN());
    TEST_ASSERT(ld_0div0.isQuiteNaN());
    TEST_ASSERT(!ld_0div0.isSignalingNaN());

    FloatingPoint::ToFloat f_qNaN(std::numeric_limits<float>::quiet_NaN());
    TEST_ASSERT(f_qNaN.isNaN());
    TEST_ASSERT(f_qNaN.isQuiteNaN());
    TEST_ASSERT(!f_qNaN.isSignalingNaN());
    
#if !(_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_MSC)
    FloatingPoint::ToFloat f_sNaN(std::numeric_limits<float>::signaling_NaN()); // MSC bug - returns qNaN / VS 16.3.8
    TEST_ASSERT(f_sNaN.isNaN());
    TEST_ASSERT(!f_sNaN.isQuiteNaN());
    TEST_ASSERT(f_sNaN.isSignalingNaN());
#endif

    FloatingPoint::ToDouble d_qNaN(std::numeric_limits<double>::quiet_NaN());
    TEST_ASSERT(d_qNaN.isNaN());
    TEST_ASSERT(d_qNaN.isQuiteNaN());
    TEST_ASSERT(!d_qNaN.isSignalingNaN());

    FloatingPoint::ToDouble d_sNaN(std::numeric_limits<double>::signaling_NaN());
    TEST_ASSERT(d_sNaN.isNaN());
    TEST_ASSERT(!d_sNaN.isQuiteNaN());
    TEST_ASSERT(d_sNaN.isSignalingNaN());

    FloatingPoint::ToLongDouble ld_qNaN(std::numeric_limits<long double>::quiet_NaN());
    TEST_ASSERT(ld_qNaN.isNaN());
    TEST_ASSERT(ld_qNaN.isQuiteNaN());
    TEST_ASSERT(!ld_qNaN.isSignalingNaN());

    FloatingPoint::ToLongDouble ld_sNaN(std::numeric_limits<long double>::signaling_NaN());
    TEST_ASSERT(ld_sNaN.isNaN());
    TEST_ASSERT(!ld_sNaN.isQuiteNaN());
    TEST_ASSERT(ld_sNaN.isSignalingNaN());

    FloatingPoint::ToFloat f_sqrt(Math::sqrt(-1.0f));
    TEST_ASSERT(f_sqrt.isQuiteNaN());
    FloatingPoint::ToDouble d_sqrt(Math::sqrt(-1.0));
    TEST_ASSERT(d_sqrt.isQuiteNaN());
    FloatingPoint::ToLongDouble ld_sqrt(Math::sqrt(-1.0L));
    TEST_ASSERT(ld_sqrt.isQuiteNaN());
  }
};

TEST_REGISTER(FloatingPoint);

#endif

// TAG: quick FormatOutputStream to get full info for float state

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
