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
#include <float.h>

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

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
