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

#include <base/mem/Heap.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/** Helper class for detecting leaks when these cannot be avoided. Only use when leaking is unavoidable. */
template<class TYPE>
class Leaky {
private:

  TYPE* buffer = nullptr;
  MemorySize size = 0;
public:

  inline Leaky() noexcept
  {
  }

  inline Leaky(TYPE* object) noexcept
    : buffer(object), size(object ? 1 : 0)
  {
  }

  inline Leaky(TYPE* _buffer, MemorySize _size) noexcept
    : buffer(_buffer), size(_size)
  {
  }

  inline Leaky(const Span<TYPE>& span) noexcept
    : buffer(span.buffer), size(span.size)
  {
  }

  inline void clear()
  {
    buffer = nullptr;
    size = 0;
  }

  inline ~Leaky()
  {
    if (buffer && (size > 0)) {
      if (Exception::isUnwinding()) {
        // auto type = Type::getType<TYPE*>();
        Heap::onLeak(nullptr /*&type*/, buffer, size * sizeof(TYPE));
      }
    }
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
