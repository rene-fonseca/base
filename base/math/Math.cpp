/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/math/Math.h>
#include <base/UInt128.h>
#include <base/UnitTest.h>
#include <math.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace {

  constexpr int64 power10_64(unsigned int exponent) noexcept
  {
    return (exponent > 0) ? (static_cast<int64>(10) * power10_64(exponent - 1)) : 1;
  }

  constexpr int64 power5_64(unsigned int exponent) noexcept
  {
    return (exponent > 0) ? (static_cast<int64>(5) * power5_64(exponent - 1)) : 1;
  }

  constexpr int64 power2_64(unsigned int exponent) noexcept
  {
    return (exponent > 0) ? (static_cast<int64>(2) * power2_64(exponent - 1)) : 1;
  }

  constexpr int32 power10_32(unsigned int exponent) noexcept
  {
    return (exponent > 0) ? (static_cast<int32>(10) * power10_32(exponent - 1)) : 1;
  }

  constexpr int32 power5_32(unsigned int exponent) noexcept
  {
    return (exponent > 0) ? (static_cast<int32>(5) * power5_32(exponent - 1)) : 1;
  }

  constexpr int32 power2_32(unsigned int exponent) noexcept
  {
    return (exponent > 0) ? (static_cast<int32>(2) * power2_32(exponent - 1)) : 1;
  }
}

// values < 2^63
const int64 Math::EXPONENTS10_64[19] = { // 64 bit has room for 10 ^ 18 - excluding 1 bit for sign
  power10_64(0),power10_64(1),power10_64(2),power10_64(3),power10_64(4),power10_64(5),power10_64(6),power10_64(7),
  power10_64(8),power10_64(9),power10_64(10),power10_64(11),power10_64(12),power10_64(13),power10_64(14),
  power10_64(15),power10_64(16),power10_64(17),power10_64(18)
};

const int64 Math::EXPONENTS5_64[28] = { // 64 bit has room for 5 ^ 27 - excluding 1 bit for sign
  power5_64(0),power5_64(1),power5_64(2),power5_64(3),power5_64(4),power5_64(5),power5_64(6),power5_64(7),
  power5_64(8),power5_64(9),power5_64(10),power5_64(11),power5_64(12),power5_64(13),power5_64(14),
  power5_64(15),power5_64(16),power5_64(17),power5_64(18),power5_64(19),power5_64(20),power5_64(21),
  power5_64(22),power5_64(23),power5_64(24),power5_64(25),power5_64(26),power5_64(27)
};

const int64 Math::EXPONENTS2_64[63] = { // 64 bit has room for 2 ^ 62 - excluding 1 bit for sign
  power2_64(0),power2_64(1),power2_64(2),power2_64(3),power2_64(4),power2_64(5),power2_64(6),power2_64(7),
  power2_64(8),power2_64(9),power2_64(10),power2_64(11),power2_64(12),power2_64(13),power2_64(14),power2_64(15),
  power2_64(16),power2_64(17),power2_64(18),power2_64(19),power2_64(20),power2_64(21),power2_64(22),power2_64(23),
  power2_64(24),power2_64(25),power2_64(26),power2_64(27),power2_64(28),power2_64(29),power2_64(30),power2_64(31),
  power2_64(32),power2_64(33),power2_64(34),power2_64(35),power2_64(36),power2_64(37),power2_64(38),power2_64(39),
  power2_64(40),power2_64(41),power2_64(42),power2_64(43),power2_64(44),power2_64(45),power2_64(46),power2_64(47),
  power2_64(48),power2_64(49),power2_64(50),power2_64(51),power2_64(52),power2_64(53),power2_64(54),power2_64(55),
  power2_64(56),power2_64(57),power2_64(58),power2_64(59),power2_64(60),power2_64(61),power2_64(62)
};

// values < 2^31
const int32 Math::EXPONENTS10_32[10] = { // 32 bit has room for 10 ^ 9 - excluding 1 bit for sign
  power10_32(0),power10_32(1),power10_32(2),power10_32(3),power10_32(4),power10_32(5),power10_32(6),power10_32(7),
  power10_32(8),power10_32(9)
};

const int32 Math::EXPONENTS5_32[14] = { // 32 bit has room for 5 ^ 13 - excluding 1 bit for sign
  power5_32(0),power5_32(1),power5_32(2),power5_32(3),power5_32(4),power5_32(5),power5_32(6),power5_32(7),
  power5_32(8),power5_32(9),power5_32(10),power5_32(11),power5_32(12),power5_32(13)
};

const int32 Math::EXPONENTS2_32[31] = { // 32 bit has room for 2 ^ 30 - excluding 1 bit for sign
  power2_32(0),power2_32(1),power2_32(2),power2_32(3),power2_32(4),power2_32(5),power2_32(6),power2_32(7),
  power2_32(8),power2_32(9),power2_32(10),power2_32(11),power2_32(12),power2_32(13),power2_32(14),power2_32(15),
  power2_32(16),power2_32(17),power2_32(18),power2_32(19),power2_32(20),power2_32(21),power2_32(22),power2_32(23),
  power2_32(24),power2_32(25),power2_32(26),power2_32(27),power2_32(28),power2_32(29),power2_32(30)
};

const uint8 Math::BIT_REVERSAL[] = {
  0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
  0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
  0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,
  0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
  0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4,
  0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
  0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,
  0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
  0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,
  0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
  0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea,
  0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
  0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6,
  0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
  0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,
  0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
  0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1,
  0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
  0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,
  0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
  0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,
  0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
  0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed,
  0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
  0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3,
  0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
  0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,
  0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
  0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7,
  0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
  0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef,
  0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff
};

float Math::frac(float value) noexcept
{
  float i = 0;
  return modff(value, &i);
}

double Math::frac(double value) noexcept
{
  double i = 0;
  return modf(value, &i);
#if 0
  if (value >= 0) {
    return value - floor(value);
  } else {
    return ceil(value) - value;
  }
#endif
}

long double Math::frac(long double value) noexcept
{
  long double i = 0;
  return modfl(value, &i);
}

bool Math::isFinite(float value) noexcept
{
  return isfinite(value);
}

bool Math::isFinite(double value) noexcept
{
  return isfinite(value);
}

bool Math::isFinite(long double value) noexcept
{
  return isfinite(value);
}

bool Math::isInfinity(float value) noexcept
{
  return isinf(value);
}

bool Math::isInfinity(double value) noexcept
{
  return isinf(value);
}

bool Math::isInfinity(long double value) noexcept
{
  return isinf(value);
}

bool Math::isNaN(float value) noexcept
{
  return isnan(value);
}

bool Math::isNaN(double value) noexcept
{
  return isnan(value);
}

bool Math::isNaN(long double value) noexcept
{
  return isnan(value);
}

const uint8* Math::getBitReversalData() noexcept
{
  return BIT_REVERSAL;
}

double Math::lngamma(double value) noexcept
{
  // TAG: need better approximation
  // B_2n/(2n * (2n-1) x^(2n-1))
  const double coefficients[7] = {
    1./6 * 1./2,
    -1./30 * 1./12,
    1./42 * 1./30,
    -1./30 * 1./56,
    5./66 * 1./90,
    -691./2730 * 1./132,
    7./6 * 1./182
  };
  
  // 0.5 * Math::ln(2 * PI)
  const double LN_2_PI_OVER_2 = 0.9189385332046727417803297364056176398613974736377834128171515404827656959272603976947432986359541976;
  double result = (value - 0.5) * Math::ln(value) - value + LN_2_PI_OVER_2;

  double factor = 1/value;
  double square = factor * factor;
  for (unsigned int i = 0; i < getArraySize(coefficients); ++i) {
    result += coefficients[i] * factor;
    factor *= square;
  }
  return result;
}

unsigned int Math::gcd(unsigned int m, unsigned int n) noexcept
{
  unsigned int M = (m > n) ? m : n;
  unsigned int N = (m > n) ? n : m;
  if (N == 0) {
    return 1;
  }
  while (true) {
    unsigned int rest = M%N;
    if (rest == 0) {
      break;
    }
    M = N;
    N = rest;
  }
  return N;
}

int Math::gcd(int m, int n) noexcept
{
  bool negative = (m < 0) && (n < 0);
  if (m < 0) {
    m = -m;
  }
  if (n < 0) {
    n = -n;
  }
  int M = (m > n) ? m : n;
  int N = (m > n) ? n : m;
  if (N == 0) {
    return 1;
  }
  while (true) {
    unsigned int rest = M%N;
    if (rest == 0) {
      break;
    }
    M = N;
    N = rest;
  }
  return negative ? -N : N;
}

uint64 Math::muldiv(uint64 value, uint64 mul, uint64 div)
{
  const int b1 = getHighestBit(value);
  const int b2 = getHighestBit(mul);
  if ((b1 + b2) <= 64) {
    return value * mul / div;
  }
  const unsigned int b3 = getHighestBit(div);
  // b1 > 0 and b2 > 0
  if (((static_cast<uint64>(1) << (b2 - 1) == mul) &&
       (static_cast<uint64>(1) << (b3 - 1)) == div)) { // both are power of 2 // dont care if b3 == 0
    const int shift = b2 - b3;
    if (shift >= 0) {
      return value << shift;
    } else {
      return value >> -shift;
    }
  }
  
  if ((mul % div) == 0) {
    return value * (mul / div);
  }
  auto temp = UInt128(value) * UInt128(mul);
  if (div) {
    temp /= UInt128(div);
  }
  // return value * mul / div; // ignore overflow
  return temp;
}

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Math) : public UnitTest {
public:

  TEST_PRIORITY(40);
  TEST_PROJECT("base/math");
  TEST_IMPACT(IMPORTANT)
  TEST_TIMEOUT_MS(30 * 1000);

  void run() override
  {
    // TAG: verify out of domain results
    // TAG: verify nan/inf results

    Math::sin(0.0);
    Math::cos(0.0);
    Math::tan(0.0);
    Math::sin(0.0f);
    Math::cos(0.0f);
    Math::tan(0.0f);
    Math::sin(0.0L);
    Math::cos(0.0L);
    Math::tan(0.0L);

    Math::asin(0.0f);
    Math::acos(0.0f);
    Math::atan(0.0f);
    Math::asin(0.0);
    Math::acos(0.0);
    Math::atan(0.0);
    Math::asin(0.0L);
    Math::acos(0.0L);
    Math::atan(0.0L);

    Math::atan2(1.0f, 1.0f);
    Math::atan2(1.0, 1.0);
    Math::atan2(1.0L, 1.0L);

    // Emscripten issue: ::ceill works fine

    TEST_ASSERT(Math::ceil(constant::E_F) > constant::E_F);
    TEST_ASSERT(Math::ceil(constant::E) > constant::E);
    TEST_ASSERT(Math::ceil(constant::E_L) > constant::E_L);

    TEST_ASSERT(Math::floor(constant::E_F) < constant::E_F);
    TEST_ASSERT(Math::floor(constant::E) < constant::E);
    TEST_ASSERT(Math::floor(constant::E_L) < constant::E_L);

    TEST_ASSERT(Math::round(constant::E_F) > constant::E_F);
    TEST_ASSERT(Math::round(constant::E) > constant::E);
    TEST_ASSERT(Math::round(constant::E_L) > constant::E_L);
    TEST_ASSERT((Math::trunc(constant::E_F) < constant::E_F) && ((Math::trunc(constant::E_F) - 2) < 1e-6f));
    TEST_ASSERT((Math::trunc(constant::E) < constant::E) && ((Math::trunc(constant::E) - 2) < 1e-6));
    TEST_ASSERT((Math::trunc(constant::E_L) < constant::E_L) && ((Math::trunc(constant::E_L) - 2) < 1e-6L));
    TEST_ASSERT(Math::abs(Math::frac(constant::E_F)) < 1);
    TEST_ASSERT(Math::abs(Math::frac(constant::E)) < 1);
    TEST_ASSERT(Math::abs(Math::frac(constant::E_L)) < 1);

    TEST_ASSERT(Math::abs(constant::E_F) == constant::E_F);
    TEST_ASSERT(Math::abs(constant::E) == constant::E);
    TEST_ASSERT(Math::abs(constant::E_L) == constant::E_L);
    TEST_ASSERT(Math::abs(-constant::E_F) == constant::E_F);
    TEST_ASSERT(Math::abs(-constant::E) == constant::E);
    TEST_ASSERT(Math::abs(-constant::E_L) == constant::E_L);

    TEST_ASSERT(Math::abs(Math::sqrt(constant::E_F) * Math::sqrt(constant::E_F) - constant::E_F) < 1e-6f);
    TEST_ASSERT(Math::abs(Math::sqrt(constant::E) * Math::sqrt(constant::E) - constant::E) < 1e-8);
    TEST_ASSERT(Math::abs(Math::sqrt(constant::E_L) * Math::sqrt(constant::E_L) - constant::E_L) < 1e-8L);

    TEST_ASSERT(Math::abs(Math::ln(constant::E_F) - 1) < 1e-6f);
    TEST_ASSERT(Math::abs(Math::ln(constant::E) - 1) < 1e-8);
    TEST_ASSERT(Math::abs(Math::ln(constant::E_L) - 1) < 1e-8);
    TEST_ASSERT(Math::abs(Math::log10(10.0f) - 1) < 1e-6f);
    TEST_ASSERT(Math::abs(Math::log10(10.0) - 1) < 1e-8);
    TEST_ASSERT(Math::abs(Math::log10(10.0L) - 1) < 1e-8);
    TEST_ASSERT(Math::abs(Math::log2(2.0f) - 1) < 1e-6f);
    TEST_ASSERT(Math::abs(Math::log2(2.0) - 1) < 1e-8);
    TEST_ASSERT(Math::abs(Math::log2(2.0L) - 1) < 1e-8);
    TEST_ASSERT(Math::abs(Math::exp(0.0f) - 1) < 1e-6f);
    TEST_ASSERT(Math::abs(Math::exp(0.0) - 1) < 1e-8);
    TEST_ASSERT(Math::abs(Math::exp(0.0L) - 1) < 1e-8);
    TEST_ASSERT(Math::abs(Math::exp(1.0f) - constant::E_F) < 1e-6f);
    TEST_ASSERT(Math::abs(Math::exp(1.0) - constant::E) < 1e-8);
    TEST_ASSERT(Math::abs(Math::exp(1.0L) - constant::E_L) < 1e-8);

    Math::cbrt(constant::E_F);
    Math::cbrt(constant::E);
    Math::cbrt(constant::E_L);

    Math::hypot(constant::E_F, constant::PI_F);
    Math::hypot(constant::E, constant::PI);
    Math::hypot(constant::E_L, constant::PI_L);

    Math::power(constant::E_F, constant::E_F);
    Math::power(constant::E, constant::E);
    Math::power(constant::E_L, constant::E_L);
  }
};

TEST_REGISTER(Math);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
