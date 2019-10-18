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
#include <base/io/RandomInputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace {

  // TAG: we need MT-safety for random
  RandomInputStream randomInputStream;
}

void Random::fill(uint8* dest, const uint8* end) noexcept
{
  auto size = randomInputStream.read(dest, end - dest, false);
}

#define RANDOM_IMPL(TYPE) \
  template<> \
  TYPE Random::random<TYPE>() noexcept { \
    TYPE result = 0; \
    auto size = randomInputStream.read(reinterpret_cast<uint8*>(&result), sizeof(result), false); \
    return result; \
  }

RANDOM_IMPL(int8);
RANDOM_IMPL(int16);
RANDOM_IMPL(int32);
RANDOM_IMPL(int64);
// RANDOM_IMPL(int128);

template<>
int128 Random::random<int128>() noexcept {
  int128 result;
  auto size = randomInputStream.read(reinterpret_cast<uint8*>(&result), sizeof(result), false);
  return result;
}

RANDOM_IMPL(uint8);
RANDOM_IMPL(uint16);
RANDOM_IMPL(uint32);
RANDOM_IMPL(uint64);
// RANDOM_IMPL(uint128);

template<>
uint128 Random::random<uint128>() noexcept {
  uint128 result;
  auto size = randomInputStream.read(reinterpret_cast<uint8*>(&result), sizeof(result), false);
  return result;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
