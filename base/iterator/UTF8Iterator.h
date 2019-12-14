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

#include <base/Iterator.h>
#include <base/Primitives.h>
#include <base/string/Unicode.h>
#include <base/string/MultibyteException.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Iterator used to traverse UTF-8.
*/
class UTF8Iterator {
protected:

  typedef const uint8* Pointer;
  typedef MemoryDiff Distance;
  typedef ForwardIterator Category;

  /** The position of the iterator. */
  Pointer element = nullptr;

  /** Reads UCS4 (doesnt check if UCS4 code is disallowed). */
  static Pointer next(Pointer src, ucs4& ch)
  {
    const int status = Unicode::readUCS4(src, ch);
    if (status > 0) {
      src += status;
    } else {
      switch (status) {
      case Unicode::ERROR_INCOMPLETE:
        throw MultibyteException(MultibyteException::INCOMPLETE_CHARACTER_CODE);
      case Unicode::ERROR_BAD_ENCODING:
        throw MultibyteException(MultibyteException::INVALID_CHARACTER_CODE);
      default:
        throw MultibyteException();
      }
    }
    return src;
  }
public:

  /**
    Initializes iterator.

    @param value The initial value of the iterator.
  */
  inline UTF8Iterator(Pointer value) noexcept
    : element(value)
  {
  }

  /**
    Initializes iterator from other iterator.
  */
  inline UTF8Iterator(const UTF8Iterator& copy) noexcept
    : element(copy.element)
  {
  }
  
  /**
    Initializes iterator from other iterator.
  */
  inline UTF8Iterator& operator=(const UTF8Iterator& assign) noexcept
  {
    element = assign.element;
    return *this;
  }

  /**
    Prefix increment.
  */
  UTF8Iterator& operator++()
  {
    ucs4 ch = 0;
    element = next(element, ch);
    return *this;
  }

  /**
    Postfix increment.
  */
  UTF8Iterator operator++(int) noexcept
  {
    UTF8Iterator result(*this);
    ucs4 ch = 0;
    element = next(element, ch);
    return result;
  }

  /**
    Moves the specified distance forward.
  */
  UTF8Iterator& operator+=(Distance distance) noexcept
  {
    BASSERT(distance >= 0);
    while (distance--) {
      ++element;
    }
    return *this;
  }
  
  /**
    Returns true if the iterators are equal.
  */
  inline bool operator==(const UTF8Iterator& compare) const noexcept
  {
    return element == compare.element;
  }

  /**
    Returns true if the iterators aren't equal.
  */
  inline bool operator!=(const UTF8Iterator& compare) const noexcept
  {
    return element != compare.element;
  }

  /**
    Returns true if this iterator is less than the specified iterator.
  */
  inline bool operator<(const UTF8Iterator& compare) const noexcept
  {
    return element < compare.element;
  }

  inline bool operator<=(const UTF8Iterator& compare) const noexcept
  {
    return element <= compare.element;
  }
  
  /**
    Returns true if this iterator is greater than or equal to the specified
    iterator.
  */
  inline bool operator>=(const UTF8Iterator& compare) const noexcept
  {
    return element >= compare.element;
  }

  inline bool operator>(const UTF8Iterator& compare) const noexcept
  {
    return element > compare.element;
  }
  
  /**
    Returns true if the UTF-8 encoding is valid at the current position. Does NOT check if code is an allowed UCS4
    code as such should be skipped fully.
  */
  bool hasValidCode() const
  {
    ucs4 ch = 0;
    const int status = Unicode::readUCS4(element, ch);
    return status > 0;
  }

  /** Skip a single byte - not encoded character. Used to skip bad codes only. */
  void skip() noexcept
  {
    BASSERT(*element); // we do not want to skip past null terminator
    ++element;
  }

  /**
    Access the element.
  */
  ucs4 operator*() const
  {
    ucs4 ch = 0;
    next(element, ch);
    return ch;
  }

  /**
    Returns the element at the specified index from this element.
  */
  ucs4 operator[](Distance index) const
  {
    auto copy = *this;
    copy += index;
    return *copy;
  }
};

/** UTF-8 enumerator. */
class UTF8Enumerator {
private:

  UTF8Iterator src = nullptr;
  UTF8Iterator end = nullptr;
public:

  inline UTF8Enumerator() noexcept
  {
  }

  inline UTF8Enumerator(const UTF8Iterator& _begin, const UTF8Iterator& _end) noexcept
    : src(_begin), end(_end)
  {
  }

  inline bool hasNext() const noexcept
  {
    return src != end;
  }

  inline ucs4 next()
  {
    return *src++;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
