/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Primitives.h>
#include <base/UnitTest.h>
#include <cfloat>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

const float PrimitiveTraits<float>::MINIMUM = FLT_MIN;
const float PrimitiveTraits<float>::MAXIMUM = FLT_MAX;

const double PrimitiveTraits<double>::MINIMUM = DBL_MIN;
const double PrimitiveTraits<double>::MAXIMUM = DBL_MAX;

const long double PrimitiveTraits<long double>::MINIMUM = DBL_MIN;
const long double PrimitiveTraits<long double>::MAXIMUM = DBL_MAX;

MemorySize NativeString::getLength() const noexcept
{
  return getNullTerminatedLength(value);
}

MemorySize NativeWideString::getLength() const noexcept
{
  return getNullTerminatedLength(value);
}

MemorySpan::MemorySpan(const uint8* src) noexcept
  : begin(src), end(begin + getNullTerminatedLength(src))
{
}

MemorySpan::MemorySpan(const char* src) noexcept
  : begin(reinterpret_cast<const uint8*>(src)), end(reinterpret_cast<const uint8*>(src + getNullTerminatedLength(src)))
{
}

uint8* MemorySpan::copyTo(uint8* dest, const uint8* destEnd) const noexcept
{
  auto src = begin;
  const MemorySize ds = destEnd - dest;
  const MemorySize ss = end - begin;
  if (ds < ss) {
    while (dest != destEnd) { // dest buffer is smaller
      *dest++ = *src++;
    }
  } else {
    while (src != end) { // src buffer is smaller
      *dest++ = *src++;
    }
  }
  return dest;
}

uint8* MemorySpan::copyTo(uint8* dest) const noexcept
{
  auto src = begin;
  while (src != end) { // src buffer is smaller
    *dest++ = *src++;
  }
  return dest;
}


#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(CPP) : public UnitTest {
public:

  TEST_PRIORITY(0);
  TEST_PROJECT("base");
  TEST_IMPACT(CRITICAL);
  TEST_TIMEOUT_MS(30 * 1000);

  void run() override
  {
    TEST_ASSERT(sizeof(char) == 1);
    TEST_ASSERT(sizeof(char) == sizeof(unsigned char));
    TEST_ASSERT(sizeof(short) == 2);
    TEST_ASSERT(sizeof(short) == sizeof(unsigned short));
    TEST_ASSERT((2 * sizeof(short)) == sizeof(int));
    TEST_ASSERT(sizeof(int) == sizeof(unsigned int));
    TEST_ASSERT(sizeof(int) <= sizeof(long));
    TEST_ASSERT(sizeof(long) == sizeof(unsigned long));
    TEST_ASSERT(sizeof(long) <= sizeof(long long));
    TEST_ASSERT(sizeof(long long) == sizeof(unsigned long long));

    TEST_ASSERT(sizeof(float) >= sizeof(uint32));
    TEST_ASSERT(sizeof(double) >= sizeof(uint64));
    TEST_ASSERT(sizeof(float) <= sizeof(double));
    TEST_ASSERT(sizeof(double) <= sizeof(long double));

    TEST_ASSERT((sizeof(size_t) == 4) || (sizeof(size_t) == 8));
    TEST_ASSERT(sizeof(size_t) == sizeof(void*));
  }
};

TEST_REGISTER(CPP);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
