/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/string/FormatOutputStream.h>
#include <base/io/RandomInputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  MT-safe random generator.

  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Random {
public:

  /** Returns thread local random input stream. Do not preserve source outside of execution scope. */
  static RandomInputStream& getRandomInputStream() noexcept;

  /** Fills given buffer. */
  static void fill(uint8* dest, const uint8* end) noexcept;

  /** Fills fixed size array. */
  template<typename TYPE, MemorySize SIZE>
  static inline void fill(TYPE (&buffer)[SIZE]) noexcept {
    fill(buffer, buffer + SIZE);
  }

  /** Returns a random number for the given primitive of type int8, int16, int32, int64, int128, uint8, uint16, uint32, uint64, uint128. */
  template<typename TYPE>
  static TYPE random() noexcept;
};

template<>
_COM_AZURE_DEV__BASE__API int8 Random::random<int8>() noexcept;

template<>
_COM_AZURE_DEV__BASE__API int16 Random::random<int16>() noexcept;

template<>
_COM_AZURE_DEV__BASE__API int32 Random::random<int32>() noexcept;

template<>
_COM_AZURE_DEV__BASE__API int64 Random::random<int64>() noexcept;

template<>
_COM_AZURE_DEV__BASE__API int128 Random::random<int128>() noexcept;

template<>
_COM_AZURE_DEV__BASE__API uint8 Random::random<uint8>() noexcept;

template<>
_COM_AZURE_DEV__BASE__API uint16 Random::random<uint16>() noexcept;

template<>
_COM_AZURE_DEV__BASE__API uint32 Random::random<uint32>() noexcept;

template<>
_COM_AZURE_DEV__BASE__API uint64 Random::random<uint64>() noexcept;

template<>
_COM_AZURE_DEV__BASE__API uint128 Random::random<uint128>() noexcept;

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
