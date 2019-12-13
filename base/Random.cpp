/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/Random.h>
#include <base/concurrency/ThreadLocalContext.h>
#include <base/concurrency/ApplicationSynchronize.h>
#include <base/UnitTest.h>
#include <random>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace _impl {

  inline RandomInputStream& getRandomInputStream() noexcept
  {
    auto tls = Thread::getLocalContext();
    if (tls) {
      return tls->randomInputStream; // nullptr not expected
    }
    static RandomInputStream dummy;
    return dummy;
  }
}

RandomInputStream& Random::getRandomInputStream() noexcept
{
  return _impl::getRandomInputStream();
}

void Random::fill(uint8* dest, const uint8* end) noexcept
{
  auto size = _impl::getRandomInputStream().read(dest, static_cast<unsigned int>(end - dest), false);
}

#define RANDOM_IMPL(TYPE) \
  template<> \
  TYPE Random::random<TYPE>() noexcept { \
    TYPE result = 0; \
    auto size = _impl::getRandomInputStream().read(reinterpret_cast<uint8*>(&result), sizeof(result), false); \
    return result; \
  }

RANDOM_IMPL(int8);
RANDOM_IMPL(int16);
RANDOM_IMPL(int32);
RANDOM_IMPL(long);
RANDOM_IMPL(long long);
// RANDOM_IMPL(int128);

template<>
int128 Random::random<int128>() noexcept {
  int128 result;
  auto size = _impl::getRandomInputStream().read(reinterpret_cast<uint8*>(&result), sizeof(result), false);
  return result;
}

RANDOM_IMPL(uint8);
RANDOM_IMPL(uint16);
RANDOM_IMPL(uint32);
RANDOM_IMPL(unsigned long);
RANDOM_IMPL(unsigned long long);
// RANDOM_IMPL(uint128);

template<>
uint128 Random::random<uint128>() noexcept {
  uint128 result;
  auto size = _impl::getRandomInputStream().read(reinterpret_cast<uint8*>(&result), sizeof(result), false);
  return result;
}

namespace _impl {

  std::random_device randomDevice;

  inline unsigned int randomDirect() noexcept {
    ApplicationSynchronize _guard;
    return randomDevice();
  }
}

unsigned int Random::randomDirect() noexcept
{
  return _impl::randomDirect();
}

// TAG: add buffer support also

#if defined(_COM_AZURE_DEV__BASE__TESTS)

class TEST_CLASS(Random) : public UnitTest {
public:

  TEST_PRIORITY(100);
  TEST_PROJECT("base");
  TEST_IMPACT(PRIVACY);

  void run() override
  {
    unsigned int random1 = Random::randomDirect();
    unsigned int random2 = Random::randomDirect();
    unsigned int random3 = Random::randomDirect();
    unsigned int random4 = Random::randomDirect();
    TEST_ASSERT((random1 != random2) && (random3 != random4));

    uint8 buffer1[4096];
    fill<uint8>(buffer1, sizeof(buffer1), 0);
    uint8 buffer2[sizeof(buffer1)];
    fill<uint8>(buffer2, sizeof(buffer2), 0);
    RandomInputStream& ris = Random::getRandomInputStream();
    TEST_ASSERT(ris.read(buffer1, sizeof(buffer1), false) == sizeof(buffer1));
    TEST_ASSERT(ris.read(buffer2, sizeof(buffer2), false) == sizeof(buffer2));

    TEST_ASSERT(compare<uint8>(buffer1, buffer2, sizeof(buffer1)) != 0);
  }
};

TEST_REGISTER(Random);

#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
