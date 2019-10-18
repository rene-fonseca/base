/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/io/RandomInputStream.h>
#include <base/string/FormatOutputStream.h>
#include <base/concurrency/ApplicationSynchronize.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

namespace {

  std::random_device randomDevice;

  inline unsigned int getSeed() noexcept {
    ApplicationSynchronize _guard;
    return randomDevice();
  }
}

RandomInputStream::RandomInputStream(uint32 seed) noexcept
  : engine(seed ? seed : getSeed())
{
}

unsigned int RandomInputStream::available() const noexcept
{
  return PrimitiveTraits<unsigned int>::MAXIMUM;
}

unsigned int RandomInputStream::read(uint8* buffer, const unsigned int _size, bool nonblocking) noexcept
{
  const uint8* end = buffer + _size;

  unsigned int bits = engine();

  if (_size <= sizeof(unsigned int)) {
    if (_size == sizeof(unsigned int)) {
      *reinterpret_cast<unsigned int*>(buffer) = bits;
      return _size;
    }
    while (buffer != end) {
      *buffer++ = static_cast<uint8>(bits & 0xff);
      bits >>= 8;
    }
    return _size;
  }

  unsigned int size = _size;

  // fix alignment
  switch (getAddressOf(buffer) & (sizeof(uint32) - 1)) {
  case 1: // fall through
    if (buffer != end) {
      *buffer++ = ((bits >> 0) & 0xff);
      --size;
    }
  case 2: // fall through
    if (buffer != end) {
      *buffer++ = ((bits >> 8) & 0xff);
      --size;
    }
  case 3: // fall through
    if (buffer != end) {
      *buffer++ = ((bits >> 16) & 0xff);
      --size;
    }
  }

  // fill aligned
  uint32* dest = reinterpret_cast<uint32*>(buffer);
  const uint32* endDest = dest + (size & (sizeof(uint32) - 1)); // round down!
  while (dest != endDest) {
    *dest++ = engine();
  }
  buffer = reinterpret_cast<uint8*>(dest);

  bits = engine();
  while (buffer != end) { // only a few loops
    *buffer++ = ((bits >> 0) & 0xff);
    bits >>= 8;
    --size;
  }

  return _size; // always fills up buffer
}

unsigned int RandomInputStream::skip(unsigned int count) noexcept
{
  return count; // do nothing
}

void RandomInputStream::close() noexcept
{
  // do nothing
}

void RandomInputStream::wait() const noexcept
{
}

bool RandomInputStream::wait(unsigned int timeout) const noexcept
{
  return true;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
