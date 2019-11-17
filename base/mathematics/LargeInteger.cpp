/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#include <base/mathematics/LargeInteger.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

LargeInteger::LargeInteger()
{
}

LargeInteger::LargeInteger(const LargeInteger& copy)
  : value(copy.value)
{
}

LargeInteger::LargeInteger(LargeInteger&& move)
  : value(std::move(move.value))
{
}

LargeInteger& LargeInteger::operator=(const LargeInteger& assign)
{
  value = assign.value;
  return *this;
}

LargeInteger& LargeInteger::operator=(LargeInteger&& assign)
{
  value = std::move(assign.value);
  return *this;
}

void LargeInteger::setSize(MemorySize size)
{
  value.setSize(size, 0); // unsigned extend
}

LargeInteger LargeInteger::plus() const noexcept
{
  return *this;
}

LargeInteger LargeInteger::minus() const noexcept
{
  LargeInteger result = *this;
  // TAG: FIXME
  return result;
}

LargeInteger& LargeInteger::negate() noexcept
{
  auto src = value.getElements();
  auto end = src + value.getSize();
  while (src != end) {
    // TAG: FIXME
  }
  return *this;
}

void LargeInteger::extend(MemorySize size) // TAG: bool signed
{
  if (size > value.getSize()) {
   value.setSize(value.getSize(), 0);
  }
}

void LargeInteger::trim()
{
  auto src = value.getElements();
  auto size = value.getSize();
  MemoryDiff i = size - 1;
  for (; (i >= 0) && src[i]; --i) {
  }
  ++i;
  if (i < size) {
    value.setSize(i);
  }
}

LargeInteger& LargeInteger::add(const LargeInteger& _value)
{
  extend(_value.getSize());
  value.setSize(value.getSize(), 0);
  return *this;
}

// TAG: remove zeros

bool LargeInteger::isZero() const noexcept
{
  for (const auto v : value) {
    if (v != 0) {
      return false;
    }
  }
  return true;
}

LargeInteger& LargeInteger::operator~() noexcept
{
  for (auto& v : value) {
    v = ~v;
  }
  return *this;
}

bool LargeInteger::operator==(const LargeInteger& a) const noexcept
{
  // size check
  return false;
}

LargeInteger& LargeInteger::operator&(const LargeInteger& a) noexcept
{
  extend(a.getSize());
  unsigned int* dest = nullptr; // TAG: iterate over both at the same time
  return *this;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
