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

#include <base/Primitives.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Sequence of values in memory. The memory is not owned by class. You must ensure that the memory is not released.
*/
template<class TYPE>
class Span {
public: // avoid refactor for now

  TYPE* buffer = nullptr;
  MemorySize size = 0;
public:

  /** Initializes empty span. */
  inline Span() noexcept
  {
  }

  /** Initializes span. */
  inline Span(TYPE* _buffer, MemorySize _size) noexcept
    : buffer(_buffer), size(_size)
  {
    BASSERT(!size || (buffer && size));
  }

  /** Initializes span. */
  inline Span(TYPE* _buffer, const TYPE* end) noexcept
    : buffer(_buffer), size(end - _buffer)
  {
    BASSERT(!size || (buffer && size));
  }

  /** Begin iterator. */
  inline TYPE* begin() noexcept
  {
    return buffer;
  }

  /** End iterator. */
  inline TYPE* end() noexcept
  {
    return buffer + size;
  }

  /** Begin iterator. */
  inline const TYPE* cbegin() const noexcept
  {
    return buffer;
  }

  /** End iterator. */
  inline const TYPE* cend() const noexcept
  {
    return buffer + size;
  }

  /** Returns the size. */
  inline MemorySize getSize() const noexcept
  {
    return size;
  }

  /** Returns the item at the given index. */
  inline TYPE& operator[](MemorySize index) noexcept
  {
    return buffer[index];
  }

  /** Returns the item at the given index. */
  inline const TYPE& operator[](MemorySize index) const noexcept
  {
    return buffer[index];
  }

  /** Returns true if non-empty. */
  inline operator bool() const noexcept
  {
    return size != 0;
  }
};

/**
  Sequence of values in memory. The memory is not owned by class. You must ensure that the memory is not released.
*/
template<class TYPE>
class ConstSpan {
private:

  const TYPE* buffer = nullptr;
  MemorySize size = 0;
public:

  /** Initializes empty span. */
  inline ConstSpan() noexcept
  {
  }

  /** Initializes span. */
  inline ConstSpan(const TYPE* _buffer, MemorySize _size) noexcept
    : buffer(_buffer), size(_size)
  {
    BASSERT(!size || (buffer && size));
  }

  /** Initializes span. */
  inline ConstSpan(const TYPE* _buffer, const TYPE* end) noexcept
    : buffer(_buffer), size(end - _buffer)
  {
    BASSERT(!size || (buffer && size));
  }

  /** Initializes span. */
  inline ConstSpan(const Span<TYPE>& span) noexcept
    : buffer(span.buffer), size(span.size)
  {
    BASSERT(!size || (buffer && size));
  }

  /** Begin iterator. */
  inline const TYPE* begin() const noexcept
  {
    return buffer;
  }

  /** End iterator. */
  inline const TYPE* end() const noexcept
  {
    return buffer + size;
  }

  /** Begin iterator. */
  inline const TYPE* cbegin() const noexcept
  {
    return buffer;
  }

  /** End iterator. */
  inline const TYPE* cend() const noexcept
  {
    return buffer + size;
  }

  /** Returns the size. */
  inline MemorySize getSize() const noexcept
  {
    return size;
  }

  /** Returns the item at the given index. */
  inline const TYPE& operator[](MemorySize index) const noexcept
  {
    return buffer[index];
  }

  /** Returns true if non-empty. */
  inline operator bool() const noexcept
  {
    return size != 0;
  }
};

/** Copies items from span. Destination must have room for size of span. */
template<class TYPE>
TYPE* copyTo(TYPE* dest, const ConstSpan<TYPE>& span) noexcept
{
  auto src = span.begin();
  auto end = span.end();
  for (; src != end; ++dest, ++src) { // src buffer is smaller
    *dest = *src;
  }
  return dest;
}

/** Copies items from span. */
template<class TYPE>
TYPE* copyTo(TYPE* dest, const TYPE* end, const ConstSpan<TYPE>& span) noexcept
{
  const TYPE* src = span.begin();
  if ((end - dest) >= span.getSize()) {
    const TYPE* e = span.end();
    for (; src != e; ++dest, ++src) { // src buffer is smaller
      *dest = *src;
    }
  } else {
    for (; dest != end; ++dest, ++src) { // dest buffer is smaller
      *dest = *src;
    }
  }
  return dest;
}

/** Copies items from span. */
template<class TYPE>
TYPE* copyTo(const Span<TYPE>& _dest, const ConstSpan<TYPE>& _src) noexcept
{
  TYPE* dest = _dest.begin();
  const TYPE* src = _src.begin();
  if (_dest.getSize() >= _src.getSize()) {
    const TYPE* end = _src.end();
    for (; src != end; ++dest, ++src) { // src buffer is smaller
      *dest = *src;
    }
  } else {
    const TYPE* end = _dest.end();
    for (; dest != end; ++dest, ++src) { // dest buffer is smaller
      *dest = *src;
    }
  }
  return dest;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
