/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/platforms/symbols.h>

int getByteOrder() {
  union {
    char c[sizeof(int)];
    int i;
  } u;
  u.i = 1;
  return u.c[0] ? _DK_SDU_MIP__BASE__LITTLE_ENDIAN : _DK_SDU_MIP__BASE__BIG_ENDIAN;
}

struct IEEE_754_SinglePrecision {
  static const bool HAS_IMPLIED_ONE = true;
  static const int BIAS = 0x7f;
  static const unsigned int SIGNIFICANT = 24;

  unsigned int mantissa0 : 23;
  unsigned int exponent : 8;
  unsigned int negative : 1;
};

struct IEEE_754_SinglePrecision_BE {
  static const bool HAS_IMPLIED_ONE = true;
  static const int BIAS = 0x7f;
  static const unsigned int SIGNIFICANT = 24;

  unsigned int negative : 1;
  unsigned int exponent : 8;
  unsigned int mantissa0 : 23;
};

struct IEEE_754_DoublePrecision { // little endian
  static const bool HAS_IMPLIED_ONE = true;
  static const int BIAS = 0x3ff;
  static const unsigned int SIGNIFICANT = 53;

  unsigned int mantissa0 : 32;
  unsigned int mantissa1 : 20;
  unsigned int exponent : 11;
  unsigned int negative : 1;
};

struct IEEE_754_DoublePrecision_BE { // big endian
  static const bool HAS_IMPLIED_ONE = true;
  static const int BIAS = 0x3ff;
  static const unsigned int SIGNIFICANT = 53;

  unsigned int negative : 1;
  unsigned int exponent : 11;
  unsigned int mantissa1 : 20;
  unsigned int mantissa0 : 32;
};

struct IEEE_754_DoublePrecision_FWOBE { // float word order is big endian
  static const bool HAS_IMPLIED_ONE = true;
  static const int BIAS = 0x3ff;
  static const unsigned int SIGNIFICANT = 53;

  unsigned int mantissa1 : 20;
  unsigned int exponent : 11;
  unsigned int negative : 1;
  unsigned int mantissa0 : 32;
};

struct IEEE_ExtendedDoublePrecision96 {
  static const bool HAS_IMPLIED_ONE = false;
  static const int BIAS = 0x3fff;
  static const unsigned int SIGNIFICANT = 64;

  unsigned int mantissa0 : 32;
  unsigned int mantissa1 : 32;
  unsigned int exponent : 15;
  unsigned int negative : 1;
  unsigned int empty : 16;
};

struct IEEE_ExtendedDoublePrecision96_BE {
  static const bool HAS_IMPLIED_ONE = false;
  static const int BIAS = 0x3fff;
  static const unsigned int SIGNIFICANT = 64;

  unsigned int negative : 1;
  unsigned int exponent : 15;
  unsigned int empty : 16;
  unsigned int mantissa1 : 32;
  unsigned int mantissa0 : 32;
};

struct IEEE_ExtendedDoublePrecision96_FWOBE {
  static const bool HAS_IMPLIED_ONE = false;
  static const int BIAS = 0x3fff;
  static const unsigned int SIGNIFICANT = 64;

  unsigned int exponent : 15;
  unsigned int negative : 1;
  unsigned int empty : 16;
  unsigned int mantissa1 : 32;
  unsigned int mantissa0 : 32;
};

struct IEEE_ExtendedDoublePrecision128 {
  static const bool HAS_IMPLIED_ONE = false;
  static const int BIAS = 0x3fff;
  static const unsigned int SIGNIFICANT = 64;

  unsigned int mantissa0 : 32;
  unsigned int mantissa1 : 32;
  unsigned int exponent : 15;
  unsigned int negative : 1;
  unsigned int empty0 : 16;
  unsigned int empty1 : 32;
};

struct IEEE_ExtendedDoublePrecision128_BE {
  static const bool HAS_IMPLIED_ONE = false;
  static const int BIAS = 0x3fff;
  static const unsigned int SIGNIFICANT = 64;

  unsigned int empty1 : 32;
  unsigned int exponent : 15;
  unsigned int negative : 1;
  unsigned int empty0 : 16;
  unsigned int mantissa1 : 32;
  unsigned int mantissa0 : 32;
};

struct IEEE_ExtendedDoublePrecision128_FWOBE {
  static const bool HAS_IMPLIED_ONE = false;
  static const int BIAS = 0x3fff;
  static const unsigned int SIGNIFICANT = 64;

  unsigned int empty1 : 32;
  unsigned int exponent : 15;
  unsigned int negative : 1;
  unsigned int empty0 : 16;
  unsigned int mantissa1 : 32;
  unsigned int mantissa0 : 32;
};

struct IEEE_QuadruplePrecision {
  static const bool HAS_IMPLIED_ONE = true;
  static const int BIAS = 0x3fff;
  static const unsigned int SIGNIFICANT = 113;

  unsigned int mantissa0 : 32;
  unsigned int mantissa1 : 32;
  unsigned int mantissa2 : 32;
  unsigned int mantissa3 : 16;
  unsigned int exponent : 15;
  unsigned int negative : 1;
};

struct IEEE_QuadruplePrecision_BE {
  static const bool HAS_IMPLIED_ONE = true;
  static const int BIAS = 0x3fff;
  static const unsigned int SIGNIFICANT = 113;

  unsigned int negative : 1;
  unsigned int exponent : 15;
  unsigned int mantissa3 : 16;
  unsigned int mantissa2 : 32;
  unsigned int mantissa1 : 32;
  unsigned int mantissa0 : 32;
};

struct IEEE_QuadruplePrecision_FWOBE {
  static const bool HAS_IMPLIED_ONE = true;
  static const int BIAS = 0x3fff;
  static const unsigned int SIGNIFICANT = 113;
  // TAG: fix here
  unsigned int mantissa0 : 32;
  unsigned int mantissa1 : 32;
  unsigned int mantissa2 : 32;
  unsigned int mantissa3 : 16;
  unsigned int exponent : 15;
  unsigned int negative : 1;
};

template<typename PRIMITIVE, typename REPRESENTATION>
bool isIEEE_754_SinglePrecision() {
  if (sizeof(PRIMITIVE) != sizeof(REPRESENTATION)) {
    return false;
  }

  union {
    PRIMITIVE primitive;
    REPRESENTATION fields;
  } temp;

  temp.primitive = 1;
  if (!(
    (!temp.fields.negative) &&
    (temp.fields.exponent == REPRESENTATION::BIAS) &&
    (temp.fields.mantissa0 == 0)
  )) {
    return false;
  }

  temp.primitive = 0; // +INFINITY
  temp.primitive = 1/temp.primitive;
  if (!(
    (!temp.fields.negative) &&
    (temp.fields.exponent == 255) &&
    (temp.fields.mantissa0 == 0)
  )) {
    return false;
  }

  temp.primitive = 0; // -INFINITY
  temp.primitive = -1/temp.primitive;
  if (!(
    (temp.fields.negative) &&
    (temp.fields.exponent == 255) &&
    (temp.fields.mantissa0 == 0)
  )) {
    return false;
  }

  temp.primitive = 0;
  if (!(
    (!temp.fields.negative) &&
    (temp.fields.exponent == 0) &&
    (temp.fields.mantissa0 == 0)
  )) {
    return false;
  }

  return true;
}

template<typename PRIMITIVE, typename REPRESENTATION>
bool isIEEE_754_DoublePrecision() {
  if (sizeof(PRIMITIVE) != sizeof(REPRESENTATION)) {
    return false;
  }

  union {
    PRIMITIVE primitive;
    REPRESENTATION fields;
  } temp;

  temp.primitive = 1;
  if (!(
    (!temp.fields.negative) &&
    (temp.fields.exponent == REPRESENTATION::BIAS) &&
    (temp.fields.mantissa0 == 0) &&
    (temp.fields.mantissa1 == 0)
  )) {
    return false;
  }

  temp.primitive = 0; // +INFINITY
  temp.primitive = 1/temp.primitive;
  if (!(
    (!temp.fields.negative) &&
    (temp.fields.exponent == 2047) &&
    (temp.fields.mantissa0 == 0) &&
    (temp.fields.mantissa1 == 0)
  )) {
    return false;
  }

  temp.primitive = 0; // -INFINITY
  temp.primitive = -1/temp.primitive;
  if (!(
    (temp.fields.negative) &&
    (temp.fields.exponent == 2047) &&
    (temp.fields.mantissa0 == 0) &&
    (temp.fields.mantissa1 == 0)
  )) {
    return false;
  }

  temp.primitive = 0;
  if (!(
    (!temp.fields.negative) &&
    (temp.fields.exponent == 0) &&
    (temp.fields.mantissa0 == 0) &&
    (temp.fields.mantissa1 == 0)
  )) {
    return false;
  }

  return true;
}

template<typename PRIMITIVE, typename REPRESENTATION>
bool isIEEE_ExtendedDoublePrecision96() {
  if (sizeof(PRIMITIVE) != sizeof(REPRESENTATION)) {
    return false;
  }

  union {
    PRIMITIVE primitive;
    REPRESENTATION fields;
  } temp;

  temp.primitive = 1;
  if (!(
    (!temp.fields.negative) &&
    (temp.fields.exponent == REPRESENTATION::BIAS) &&
    (temp.fields.mantissa0 == 0) &&
    (temp.fields.mantissa1 == 0x80000000)
  )) {
    return false;
  }

  temp.primitive = 0; // +INFINITY
  temp.primitive = 1/temp.primitive;
  if (!(
    (!temp.fields.negative) &&
    (temp.fields.exponent == 32767) &&
    (temp.fields.mantissa0 == 0) &&
    (temp.fields.mantissa1 == 0x80000000)
  )) {
    return false;
  }

  temp.primitive = 0; // -INFINITY
  temp.primitive = -1/temp.primitive;
  if (!(
    (temp.fields.negative) &&
    (temp.fields.exponent == 32767) &&
    (temp.fields.mantissa0 == 0) &&
    (temp.fields.mantissa1 == 0x80000000)
  )) {
    return false;
  }

  temp.primitive = 0;
  if (!(
    (!temp.fields.negative) &&
    (temp.fields.exponent == 0) &&
    (temp.fields.mantissa0 == 0) &&
    (temp.fields.mantissa1 == 0)
  )) {
    return false;
  }

  return true;
}

template<typename PRIMITIVE, typename REPRESENTATION>
bool isIEEE_ExtendedDoublePrecision128() {
  if (sizeof(PRIMITIVE) != sizeof(REPRESENTATION)) {
    return false;
  }

  union {
    PRIMITIVE primitive;
    REPRESENTATION fields;
  } temp;

  temp.primitive = 1;
  if (!(
    (!temp.fields.negative) &&
    (temp.fields.exponent == REPRESENTATION::BIAS) &&
    (temp.fields.mantissa0 == 0) &&
    (temp.fields.mantissa1 == 0x80000000)
  )) {
    return false;
  }

  temp.primitive = 0; // +INFINITY
  temp.primitive = 1/temp.primitive;
  if (!(
    (!temp.fields.negative) &&
    (temp.fields.exponent == 32767) &&
    (temp.fields.mantissa0 == 0) &&
    (temp.fields.mantissa1 == 0x80000000)
  )) {
    return false;
  }

  temp.primitive = 0; // -INFINITY
  temp.primitive = -1/temp.primitive;
  if (!(
    (temp.fields.negative) &&
    (temp.fields.exponent == 32767) &&
    (temp.fields.mantissa0 == 0) &&
    (temp.fields.mantissa1 == 0x80000000)
  )) {
    return false;
  }

  temp.primitive = 0;
  if (!(
    (!temp.fields.negative) &&
    (temp.fields.exponent == 0) &&
    (temp.fields.mantissa0 == 0) &&
    (temp.fields.mantissa1 == 0)
  )) {
    return false;
  }

  return true;
}

template<typename PRIMITIVE, typename REPRESENTATION>
bool isIEEE_QuadruplePrecision() {
  if (sizeof(PRIMITIVE) != sizeof(REPRESENTATION)) {
    return false;
  }

  union {
    PRIMITIVE primitive;
    REPRESENTATION fields;
  } temp;

  temp.primitive = 1;
  if (!(
    (!temp.fields.negative) &&
    (temp.fields.exponent == REPRESENTATION::BIAS) &&
    (temp.fields.mantissa0 == 0) &&
    (temp.fields.mantissa1 == 0) &&
    (temp.fields.mantissa2 == 0) &&
    (temp.fields.mantissa3 == 0)
  )) {
    return false;
  }

  temp.primitive = 0; // +INFINITY
  temp.primitive = 1/temp.primitive;
  if (!(
    (!temp.fields.negative) &&
    (temp.fields.exponent == 32767) &&
    (temp.fields.mantissa0 == 0) &&
    (temp.fields.mantissa1 == 0) &&
    (temp.fields.mantissa2 == 0) &&
    (temp.fields.mantissa3 == 0)
  )) {
    return false;
  }

  temp.primitive = 0; // -INFINITY
  temp.primitive = -1/temp.primitive;
  if (!(
    (temp.fields.negative) &&
    (temp.fields.exponent == 32767) &&
    (temp.fields.mantissa0 == 0) &&
    (temp.fields.mantissa1 == 0) &&
    (temp.fields.mantissa2 == 0) &&
    (temp.fields.mantissa3 == 0)
  )) {
    return false;
  }

  temp.primitive = 0;
  if (!(
    (!temp.fields.negative) &&
    (temp.fields.exponent == 0) &&
    (temp.fields.mantissa0 == 0) &&
    (temp.fields.mantissa1 == 0) &&
    (temp.fields.mantissa2 == 0) &&
    (temp.fields.mantissa3 == 0)
  )) {
    return false;
  }

  return true;
}

template<typename PRIMITIVE>
int checkFloat(int& floatWordOrder) {
  if (getByteOrder() == _DK_SDU_MIP__BASE__LITTLE_ENDIAN) {
    if (isIEEE_754_SinglePrecision<PRIMITIVE, IEEE_754_SinglePrecision>()) {
      return _DK_SDU_MIP__BASE__IEEE_754_SINGLE_PRECISION;
    } else if (isIEEE_754_DoublePrecision<PRIMITIVE, IEEE_754_DoublePrecision>()) {
      floatWordOrder = _DK_SDU_MIP__BASE__LITTLE_ENDIAN;
      return _DK_SDU_MIP__BASE__IEEE_754_DOUBLE_PRECISION;
    } else if (isIEEE_754_DoublePrecision<PRIMITIVE, IEEE_754_DoublePrecision_FWOBE>()) {
      floatWordOrder = _DK_SDU_MIP__BASE__BIG_ENDIAN;
      return _DK_SDU_MIP__BASE__IEEE_754_DOUBLE_PRECISION;
    } else if (isIEEE_ExtendedDoublePrecision96<PRIMITIVE, IEEE_ExtendedDoublePrecision96>()) {
      floatWordOrder = _DK_SDU_MIP__BASE__LITTLE_ENDIAN;
      return _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96;
    } else if (isIEEE_ExtendedDoublePrecision96<PRIMITIVE, IEEE_ExtendedDoublePrecision96_FWOBE>()) {
      floatWordOrder = _DK_SDU_MIP__BASE__BIG_ENDIAN;
      return _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96;
    } else if (isIEEE_ExtendedDoublePrecision128<PRIMITIVE, IEEE_ExtendedDoublePrecision128>()) {
      floatWordOrder = _DK_SDU_MIP__BASE__LITTLE_ENDIAN;
      return _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128;
    } else if (isIEEE_ExtendedDoublePrecision128<PRIMITIVE, IEEE_ExtendedDoublePrecision128_FWOBE>()) {
      floatWordOrder = _DK_SDU_MIP__BASE__BIG_ENDIAN;
      return _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128;
    } else if (isIEEE_QuadruplePrecision<PRIMITIVE, IEEE_QuadruplePrecision>()) {
      floatWordOrder = _DK_SDU_MIP__BASE__LITTLE_ENDIAN;
      return _DK_SDU_MIP__BASE__IEEE_QUADRUPLE_PRECISION;
    } else if (isIEEE_QuadruplePrecision<PRIMITIVE, IEEE_QuadruplePrecision_FWOBE>()) {
      floatWordOrder = _DK_SDU_MIP__BASE__BIG_ENDIAN;
      return _DK_SDU_MIP__BASE__IEEE_QUADRUPLE_PRECISION;
    } else {
      floatWordOrder = _DK_SDU_MIP__BASE__UNSPECIFIED;
      return _DK_SDU_MIP__BASE__UNSPECIFIED;
    }
  } else { // big endian
    floatWordOrder = _DK_SDU_MIP__BASE__BIG_ENDIAN;
    if (isIEEE_754_SinglePrecision<PRIMITIVE, IEEE_754_SinglePrecision_BE>()) {
      return _DK_SDU_MIP__BASE__IEEE_754_SINGLE_PRECISION;
    } else if (isIEEE_754_DoublePrecision<PRIMITIVE, IEEE_754_DoublePrecision_BE>()) {
      return _DK_SDU_MIP__BASE__IEEE_754_DOUBLE_PRECISION;
    } else if (isIEEE_ExtendedDoublePrecision96<PRIMITIVE, IEEE_ExtendedDoublePrecision96_BE>()) {
      return _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_96;
    } else if (isIEEE_ExtendedDoublePrecision128<PRIMITIVE, IEEE_ExtendedDoublePrecision128_BE>()) {
      return _DK_SDU_MIP__BASE__IEEE_EXTENDED_DOUBLE_PRECISION_128;
    } else if (isIEEE_QuadruplePrecision<PRIMITIVE, IEEE_QuadruplePrecision_BE>()) {
      return _DK_SDU_MIP__BASE__IEEE_QUADRUPLE_PRECISION;
    } else {
      floatWordOrder = _DK_SDU_MIP__BASE__UNSPECIFIED;
      return _DK_SDU_MIP__BASE__UNSPECIFIED;
    }
  }
}
