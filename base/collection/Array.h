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

#include <base/collection/Collection.h>
#include <base/collection/Enumeration.h>
#include <base/collection/InvalidEnumeration.h>
#include <base/collection/InvalidNode.h>
#include <base/mem/ReferenceCountedAllocator.h>
#include <base/mem/Reference.h>
#include <base/OutOfRange.h>
#include <base/string/FormatOutputStream.h>
#include <base/Functor.h>
#include <base/collection/Algorithm.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  The Array collection is a container for an ordered sequence of elements which
  provides random access to the individual elements. Elements must have a
  default constructor.
  
  @short Array collection.
  @ingroup collections
  @version 1.0
*/

template<class TYPE>
class Array : public Collection {
public:

  /** The type of the values. */
  typedef TYPE Value;
  // typedef TYPE* Pointer;
  // typedef TYPE& Reference;

  typedef typename Allocator<TYPE>::Iterator Iterator;
  typedef typename Allocator<TYPE>::ReadIterator ReadIterator;
  typedef typename Allocator<TYPE>::Enumerator Enumerator;
  typedef typename Allocator<TYPE>::ReadEnumerator ReadEnumerator;
  
  /*
    Reference to an element within an array.
  */
  class Element {
    friend class Array;
  private:
    
    Array& array;
    MemorySize index = 0;

    Element(const Element& copy) noexcept;
    Element& operator=(const Element& copy);

    inline Element(Array& _array, MemorySize _index) noexcept
      : array(_array),
        index(_index)
    {
    }
  public:

    /**
      Sets the value.
    */
    inline Element& operator=(const Value& value)
    {
      array.setAt(index, value);
      return *this;
    }

    /**
      Returns the value.
    */
    inline operator const Value&() const
    {
      return array.getAt(index);
    }

    /**
      Returns the value.
    */
    inline operator Value&()
    {
      return array.getAt(index);
    }
  };
private:

  /** The elements of the array. */
  Reference<ReferenceCountedAllocator<Value> > elements;
  
  /** Moves all values up by 1 index. */
  static inline void moveUp(Value* src, MemorySize count)
  {
    if (!count) {
      return;
    }
    const Value* begin = src;
    Value* end = src + count;
    if (!std::is_fundamental<TYPE>() && std::is_move_assignable<TYPE>()) {
      for (; end != begin; --end) {
        end[0] = moveObject(end[-1]); // move forward
      }
    } else  {
      for (; end != begin; --end) {
        end[0] = end[-1]; // move forward
      }
    }
  }

  /** Moves all values up by n indices. */
  static inline void moveUp(Value* src, MemorySize count, MemoryDiff offset)
  {
    if (!count) {
      return;
    }
    const Value* begin = src;
    Value* end = src + count;
    if (!std::is_fundamental<TYPE>() && std::is_move_assignable<TYPE>()) {
      for (; end != begin; --end) {
        end[offset - 1] = moveObject(end[-1]); // move forward
      }
    } else  {
      for (; end != begin; --end) {
        end[offset - 1] = end[-1]; // move forward
      }
    }
  }

  /** Moves all values down by 1 index. */
  static inline void moveDown(Value* src, MemorySize count)
  {
    if (!count) {
      return;
    }
    const Value* end = src + count;
    if (!std::is_fundamental<TYPE>() && std::is_move_assignable<TYPE>()) {
      for (; src != end; ++src) {
        src[0] = moveObject(src[1]); // move down
      }
    } else  {
      for (; src != end; ++src) {
        src[0] = src[1]; // move down
      }
    }
  }

  /** Moves all values down by n indices. */
  static inline void moveDown(Value* src, const MemorySize count, const MemorySize offset)
  {
    BASSERT(offset > 0);
    if (!count) {
      return;
    }
    const Value* end = src + count;
    if (!std::is_fundamental<TYPE>() && std::is_move_assignable<TYPE>()) {
      for (; src != end; ++src) {
        src[0] = moveObject(src[offset]); // move down
      }
    } else  {
      for (; src != end; ++src) {
        src[0] = src[offset]; // move down
      }
    }
  }
public:

  /**
    Sets the size of the array.
  */
  void setSize(MemorySize size)
  {
    if (!elements || (size != elements->getSize())) {
      if (elements) {
        elements.copyOnWrite();
        elements->setSize(size);
      } else {
        elements = new ReferenceCountedAllocator<Value>(size);
      }
    }
  }
  
  void setSize(MemorySize size, const TYPE& value)
  {
    if (!elements || (size != elements->getSize())) {
      if (elements) {
        elements.copyOnWrite();
      } else {
        elements = new ReferenceCountedAllocator<Value>();
      }
      elements->setSize(size, value);
    }
  }
  
  /**
    Ensure capacity.
  */
  void ensureCapacity(MemorySize capacity)
  {
    elements.copyOnWrite();
    if (elements) {
      elements->ensureCapacity(capacity);
    }
  }

  /**
    Returns the capacity.
  */
  MemorySize getCapacity() const noexcept
  {
    if (elements) {
      return elements->getCapacity();
    }
    return 0;
  }

  /**
    Returns the elements of the array for modifying access.
  */
  inline Value* getElements()
  {
    elements.copyOnWrite();
    return elements->getElements();
  }

  /**
    Returns the elements of the array for non-modifying access.
  */
  inline const Value* getElements() const noexcept
  {
    return elements->getElements();
  }

  /**
    Initializes an empty array.
  */
  Array() : elements(new ReferenceCountedAllocator<Value>())
  {
  }

  /**
    Initializes an array with the specified size.

    @param size The size of the array.
  */
  explicit Array(MemorySize size)
    : elements(new ReferenceCountedAllocator<Value>(size))
  {
  }

  /**
    Initializes array with the specified number of elements.

    @param size The initial number of elements in the array.
    @param value The value used to initialize the elements.
  */
  Array(
    MemorySize size,
    const Value& value)
    : elements(new ReferenceCountedAllocator<Value>())
  {
    elements->setSize(size, value);
  }

  /**
    Initializes array from initializer list.
  */
  Array(std::initializer_list<Value> l)
  {
    append(l);
  }

  /**
    Initializes array from other array.
  */
  Array(const Array& copy) noexcept
    : elements(copy.elements)
  {
  }

  Array(Array&& move) noexcept
  {
    elements = moveObject(move.elements);
  }

  /**
    Assignment of array to array.
  */
  Array& operator=(const Array& copy) noexcept
  {
    elements = copy.elements; // self assignment allowed
    return *this;
  }

  Array& operator=(Array&& move) noexcept
  {
    elements = moveObject(move.elements);
    return *this;
  }

  /**
    Returns the number fo elements in the array.
  */
  inline MemorySize getSize() const noexcept
  {
    return elements ? elements->getSize() : 0;
  }

  /**
    Returns true if the array is empty.
  */
  inline bool isEmpty() const noexcept
  {
    return getSize() == 0;
  }

  /** Returns the first element. */
  inline TYPE& getFirst()
  {
    return getAt(0);
  }

  /** Returns the first element. */
  inline const TYPE& getFirst() const
  {
    return getAt(0);
  }

  /** Returns the last element. */
  inline TYPE& getLast()
  {
    return getAt(getSize() - 1);
  }

  /** Returns the last element. */
  inline const TYPE& getLast() const
  {
    return getAt(getSize() - 1);
  }

  /**
    Returns the first element of the allocator as a modifying array.
  */
  inline Iterator getBeginIterator() noexcept
  {
    return elements->getBeginIterator();
  }

  /**
    Returns the end of the allocator as a modifying array.
  */
  inline Iterator getEndIterator() noexcept
  {
    return elements->getEndIterator();
  }

  /**
    Returns the first element of the allocator as a non-modifying array.
  */
  inline ReadIterator getBeginReadIterator() const noexcept
  {
    return elements->getBeginReadIterator();
  }

  /**
    Returns the end of the allocator as a non-modifying array.
  */
  inline ReadIterator getEndReadIterator() const noexcept
  {
    return elements->getEndReadIterator();
  }

  /**
    Returns the first element of the allocator as a non-modifying array.
  */
  inline ReadIterator begin() const noexcept
  {
    return getBeginReadIterator();
  }

  /**
    Returns the end of the allocator as a non-modifying array.
  */
  inline ReadIterator end() const noexcept
  {
    return getEndReadIterator();
  }

  /**
    Returns the first element of the allocator as a non-modifying array.
  */
  inline ReadIterator cbegin() const noexcept
  {
    return getBeginReadIterator();
  }

  /**
    Returns the end of the allocator as a non-modifying array.
  */
  inline ReadIterator cend() const noexcept
  {
    return getEndReadIterator();
  }

  /**
    Returns the first element of the allocator as a modifying array.
  */
  inline Iterator begin() noexcept
  {
    elements.copyOnWrite();
    return getBeginIterator();
  }

  /**
    Returns the end of the allocator as a modifying array.
  */
  inline Iterator end() noexcept
  {
    elements.copyOnWrite();
    return getEndIterator();
  }
  
  inline operator RandomAccessIterable<Iterator>() noexcept
  {
    return RandomAccessIterable<Iterator>(begin(), end());
  }
  
  /**
    Returns a modifying enumerator of the array.
  */
  inline Enumerator getEnumerator() noexcept
  {
    return elements->getEnumerator();
  }

  /**
    Returns a non-modifying enumerator of the array.
  */
  inline ReadEnumerator getReadEnumerator() const noexcept
  {
    return elements->getReadEnumerator();
  }

  /**
    Appends the value to this array.

    @param value The value to be appended.
  */
  void append(const Value& value)
  {
    setSize(getSize() + 1, value);
  }

  /**
    Appends the value to this array.

    @param value The value to be appended.
  */
  void append(Value&& value)
  {
    const auto size = getSize();
    setSize(size + 1); // requires default constructor
    getElements()[size] = moveObject(value);
  }

  /**
    Append from initializer list.
  */
  void append(std::initializer_list<Value> l)
  {
    const auto size = getSize(); // original size
    setSize(size + l.size());
    auto dest = getElements() + size;
    for (auto i = l.begin(); i != l.end(); ++i) {
      *dest++ = *i;
    }
  }
  
  /**
    Prepends the value to this array.

    @param value The value to be prepended.
  */
  void prepend(const Value& value)
  {
    const auto size = getSize(); // original size
    setSize(size + 1); // better to prepend in allocator
    auto dest = getElements(); // size must be set before
    moveUp(dest, size);
    dest[0] = value;
  }

  /**
    Prepends the value to this array.

    @param value The value to be prepended.
  */
  void prepend(Value&& value)
  {
    const auto size = getSize(); // original size
    setSize(size + 1); // better to prepend in allocator
    auto dest = getElements(); // size must be set before
    moveUp(dest, size);
    dest[0] = moveObject(value);
  }

  /**
    Prepends from initializer list.
  */
  void prepend(std::initializer_list<Value> l)
  {
    const auto size = getSize(); // original size
    setSize(size + l.size());
    auto dest = getElements();
    moveUp(dest, size, l.size());
    for (auto i = l.begin(); i != l.end(); ++i) {
      *dest++ = *i;
    }
  }

  /**
    Inserts the value at the specified position. Raises OutOfRange if the
    specified index is invalid.

    @param index Specifies the insert position.
    @param value The value to be inserted.
  */
  void insert(MemorySize index, const Value& value)
  {
    if (!(index <= getSize())) {
      _throw OutOfRange(this);
    }
    const auto size = getSize(); // original size
    setSize(size + 1); // better to prepend in allocator
    auto dest = getElements(); // size must be set before
    moveUp(dest, size - index);
    dest[index] = value;
  }

  /**
    Inserts the value at the specified position. Raises OutOfRange if the
    specified index is invalid.

    @param index Specifies the insert position.
    @param value The value to be inserted.
  */
  void insert(MemorySize index, Value&& value)
  {
    if (!(index <= getSize())) {
      _throw OutOfRange(this);
    }
    const auto size = getSize(); // original size
    setSize(size + 1); // better to prepend in allocator
    auto dest = getElements(); // size must be set before
    moveUp(dest, size - index);
    dest[index] = moveObject(value);
  }

  void insert(const Iterator& it, const Value& value)
  {
    insert(it - begin(), value);
  }

  void insert(const Iterator& it, Value&& value)
  {
    insert(it - begin(), moveObject(value));
  }

  /**
    Remove the element specified by the index. Raises OutOfRange if the index is
    invalid.

    @param index The index of the element to be removed.
  */
  void remove(MemorySize index)
  {
    if (!(index < getSize())) {
      _throw OutOfRange(this);
    }
    auto dest = getElements(); // size must be set after
    const auto size = getSize(); // original size
    moveDown(dest + index, size - index - 1);
    setSize(size - 1);
  }

  /** Removes the element at the given position. */
  void remove(const Iterator& it)
  {
    remove(it - begin());
  }

  /** Removes the elements within the given positions. */
  void remove(MemorySize _begin, MemorySize _end)
  {
    if (!(_begin < getSize())) {
      _throw OutOfRange(this);
    }
    if (!(_end < getSize())) {
      _throw OutOfRange(this);
    }
    if (!(_begin <= _end)) {
      _throw OutOfRange(this);
    }
    
    auto count = _end - _begin;
    if (!count) {
      return; // nothing to do
    }
    auto dest = getElements(); // size must be set after
    const auto size = getSize(); // original size
    moveDown(dest + _begin, size - _begin - count, count);
    setSize(size - count);
  }
  
  /** Removes the elements within the given positions. */
  void remove(const Iterator& _begin, const Iterator& _end)
  {
    auto b = begin();
    remove(_begin - b, _end - b);
  }

  /**
    Removes all the elements from this array.
  */
  void removeAll()
  {
    elements = nullptr; // no need to copy
  }

  /** Returns a new Array for the given elements. */
  Array slice(MemorySize _begin, MemorySize _end) const
  {
    if (!(_begin < getSize())) {
      _throw OutOfRange(this);
    }
    if (!(_end < getSize())) {
      _throw OutOfRange(this);
    }
    if (!(_begin <= _end)) {
      _throw OutOfRange(this);
    }

    auto count = _end - _begin;
    Array result;
    result.setSize(count);
    auto dest = result.getElements();
    base::copy(dest, getElements() + _begin, count);
    return result;
  }

  /** Returns a new Array for the given elements. */
  Array slice(const Iterator& _begin, const Iterator& _end) const
  {
    const auto b = begin();
    return slice(_begin - b, _end - b);
  }

  /**
    Returns the element at the specified index. Raises OutOfRange if the index
    is invalid.

    @param index The index of the element.
  */
  Value& getAt(MemorySize index)
  {
    if (!(index < getSize())) {
      _throw OutOfRange(this);
    }
    return getElements()[index];
  }

  /**
    Returns the element at the specified index. Raises OutOfRange if the index
    is invalid.

    @param index The index of the element.
  */
  const Value& getAt(MemorySize index) const
  {
    if (!(index < getSize())) {
      _throw OutOfRange(this);
    }
    return getElements()[index];
  }

  /**
    Sets the element at the specified index. Raises OutOfRange if the index is
    invalid.

    @param index The index of the element.
    @param value The desired value.
  */
  void setAt(MemorySize index, const Value& value)
  {
    if (!(index < getSize())) {
      _throw OutOfRange(this);
    }
    getElements()[index] = value;
  }

  /**
    Sets the element at the specified index. Raises OutOfRange if the index is
    invalid.

    @param index The index of the element.
    @param value The desired value.
  */
  void setAt(MemorySize index, Value&& value)
  {
    if (!(index < getSize())) {
      _throw OutOfRange(this);
    }
    getElements()[index] = moveObject(value);
  }

  /**
    Returns a reference to the element at the specified index. Raises OutOfRange
    if the index is invalid.

    @param index The index of the element.
  */
  inline TYPE& operator[](MemorySize index)
  {
    return getAt(index);
  }

  /**
    Returns the element at the specified index. Raises OutOfRange if the index
    is invalid.

    @param index The index of the element.
  */
  inline const Value& operator[](MemorySize index) const
  {
    return getAt(index);
  }

  /** Returns true if equal. */
  bool operator==(const Array& compare) const
  {
    if (getSize() != compare.getSize()) {
      return false;
    }
    auto a = cbegin();
    const auto aEnd = cend();
    auto b = compare.cbegin();
    for (; a != aEnd; ++a, ++b) {
      if (!(*a == *b)) {
        return false;
      }
    }
    return true;
  }

  /** Returns true if not equal. */
  inline bool operator!=(const Array& compare) const
  {
    return !operator==(compare);
  }

  /** Returns true if less than. */
  bool operator<(const Array& compare) const
  {
    auto a = cbegin();
    auto b = compare.cbegin();
    const auto aEnd = cend();
    const auto bEnd = compare.cend();
    if (getSize() <= compare.getSize()) {
      for (; a != aEnd; ++a, ++b) {
        if (*a < *b) {
          return true;
        } else if (*a == *b) {
          continue;
        }
        return false;
      }
      return b != bEnd;
    } else {
      for (; b != bEnd; ++a, ++b) {
        if (*a < *b) {
          return true;
        } else if (*a == *b) {
          continue;
        }
        return false;
      }
      return false;
    }
  }
  
  inline bool operator>=(const Array& compare) const
  {
    return !operator<(compare);
  }
  
  /** Returns true if not empty. */
  inline operator bool() const noexcept
  {
    return getSize() != 0;
  }

  /** Shuffles the elements. */
  void shuffle()
  {
    base::shuffle(begin(), end());
  }
  
  /** Returns the index of the first found value. Returns -1 if not found. */
  MemoryDiff indexOf(const TYPE& value) const
  {
    ReadIterator src = cbegin();
    const ReadIterator end = cend();
    while (src != end) {
      if (*src == value) {
        return src - cbegin();
      }
      ++src;
    }
    return -1;
  }

  /** Returns the index of the last found value. Returns -1 if not found. */
  MemoryDiff lastIndexOf(const TYPE& value) const
  {
    ReadIterator src = cend();
    const ReadIterator begin = cbegin();
    while (src != begin) {
      --src;
      if (*src == value) {
        return src - begin;
      }
    }
    return -1;
  }
  
  /** Returns reference to the item if found by the given value. */
  const TYPE* find(const TYPE& value) const
  {
    ReadIterator src = cbegin();
    const ReadIterator end = cend();
    while (src != end) {
      if (*src == value) {
        return &*src;
      }
      ++src;
    }
    return nullptr;
  }

  /** Sorts the array. */
  void sort()
  {
    elements.copyOnWrite();
    if (getSize() >= 128) {
      mergeSort(begin(), end());
    } else {
      bubbleSort(begin(), end());
    }
  }

  /** Sorts the array. */
  template<class PREDICATE>
  void sort(PREDICATE predicate)
  {
    elements.copyOnWrite();
    if (getSize() >= 128) {
      mergeSort(begin(), end(), predicate);
    } else {
      bubbleSort(begin(), end(), predicate);
    }
  }

  /** Appends value. */
  inline Array& operator<<(const TYPE& value)
  {
    append(value);
    return *this;
  }

  /** Appends value. */
  inline Array& operator<<(TYPE&& value)
  {
    append(moveObject(value));
    return *this;
  }
};

template<class TYPE>
void swapper(typename Array<TYPE>::Element& a, typename Array<TYPE>::Element& b)
{
  typename Array<TYPE>::Value& aa = a;
  typename Array<TYPE>::Value& bb = b;
  swapper(aa, bb);
}

/**
  Writes a string representation of an array to a format stream.
  
  @relates Array
*/
template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Array<TYPE>& value)
{
  typename Array<TYPE>::ReadEnumerator enu = value.getReadEnumerator();
  stream << '[';
  while (enu.hasNext()) {
    stream << *enu.next();
    if (enu.hasNext()) {
      stream << ';';
    }
  }
  stream << ']';
  return stream;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
