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
#include <base/mem/ReferenceCountedCapacityAllocator.h>
#include <base/mem/Reference.h>
#include <base/OutOfRange.h>
#include <base/string/FormatOutputStream.h>
#include <base/Functor.h>

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

  typedef typename CapacityAllocator<TYPE>::Iterator Iterator;
  typedef typename CapacityAllocator<TYPE>::ReadIterator ReadIterator;
  typedef typename CapacityAllocator<TYPE>::Enumerator Enumerator;
  typedef typename CapacityAllocator<TYPE>::ReadEnumerator ReadEnumerator;
  
  /*
    Reference to an element within an array.
  */
  class Element {
    friend class Array;
  private:
    
    Array& array;
    MemorySize index = 0;

    Element(const Element& copy) noexcept;
    Element& operator=(const Element& eq) noexcept;
    
    inline Element(Array& _array, MemorySize _index) noexcept
      : array(_array),
        index(_index)
    {
    }
  public:

    /**
      Sets the value.
    */
    inline Element& operator=(const Value& value) throw(OutOfRange)
    {
      array.setAt(index, value);
      return *this;
    }

    /**
      Returns the value.
    */
    inline operator const Value&() const throw(OutOfRange)
    {
      return array.getAt(index);
    }
  };
private:

  /** The elements of the array. */
  Reference<ReferenceCountedCapacityAllocator<Value> > elements;
  /** The number of elements in the array. */
  MemorySize size = 0;
public:

  /**
    Sets the size of the array.
  */
  inline void setSize(MemorySize size)
  {
    if (size != this->size) {
      this->size = size;
      if (elements.isValid()) {
        elements->setSize(size);
      } else {
        elements = new ReferenceCountedCapacityAllocator<Value>(size, ReferenceCountedCapacityAllocator<Value>::DEFAULT_GRANULARITY);
      }
    }
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
  Array() : elements(new ReferenceCountedCapacityAllocator<Value>())
  {
  }

  /**
    Initializes an empty array with the specified granularity.

    @param granularity The desired granularity.
  */
  explicit Array(MemorySize granularity)
    : elements(new ReferenceCountedCapacityAllocator<Value>(granularity))
  {
  }

  /**
    Initializes array with the specified number of elements.

    @param size The initial number of elements in the array.
    @param value The value used to initialize the elements.
    @param granularity The desired granularity. Default is
    ReferenceCountedCapacityAllocator<Value>::DEFAULT_GRANULARITY.
  */
  Array(
    MemorySize _size,
    Value value,
    MemorySize granularity = ReferenceCountedCapacityAllocator<Value>::DEFAULT_GRANULARITY)
    : elements(new ReferenceCountedCapacityAllocator<Value>(_size, granularity)),
      size(_size)
  {
    fill(getElements(), size, value);
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
  inline Array(const Array& copy) noexcept
    : elements(copy.elements),
      size(copy.size) {
  }

  Array(Array&& move) noexcept
  {
    elements = move.elements;
    move.elements = nullptr;
    size = move.size;
    move.size = 0;
  }

  /**
    Assignment of array to array.
  */
  inline Array& operator=(const Array& eq) noexcept
  {
    if (&eq != this) {
      elements = eq.elements;
      size = eq.size;
    }
    return *this;
  }

  Array& operator=(Array&& move) noexcept
  {
    if (&move != this) {
      elements = move.elements;
      move.elements = nullptr;
      size = move.size;
      move.size = 0;
    }
    return *this;
  }

  /**
    Returns the number fo elements in the array.
  */
  inline MemorySize getSize() const noexcept
  {
    return size;
  }

  /**
    Returns true if the array is empty.
  */
  inline bool isEmpty() const noexcept
  {
    return size == 0;
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
    Returns the first element of the allocator as a modifying array.
  */
  inline Iterator begin() noexcept
  {
    return getBeginIterator();
  }

  /**
    Returns the end of the allocator as a modifying array.
  */
  inline Iterator end() noexcept
  {
    return getEndIterator();
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
    MemorySize size = getSize();
    setSize(size + 1);
    Value* elements = getElements(); // size must be set before
    elements[size] = value;
  }

  /**
    Append from initializer list.
  */
  inline void append(std::initializer_list<Value> l)
  {
    // optimize this
    for (auto i = l.begin(); i != l.end(); ++i) {
      append(*i);
    }
  }

  /**
    Prepends the value to this array.

    @param value The value to be prepended.
  */
  void prepend(const Value& value)
  {
    setSize(getSize() + 1);
    Value* elements = getElements(); // size must be set before
    move(elements + 1, elements, getSize());
    elements[0] = value;
  }

  /**
    Prepends from initializer list.
  */
  inline void prepend(std::initializer_list<Value> l)
  {
    // optimize this
    auto i = l.end();
    while (i != l.begin()) {
      prepend(*--i);
    }
  }

  /**
    Inserts the value at the specified position. Raises OutOfRange if the
    specified index is invalid.

    @param index Specifies the insert position.
    @param value The value to be inserted.
  */
  void insert(MemorySize index, const Value& value) throw(OutOfRange)
  {
    bassert(index <= getSize(), OutOfRange(this));
    setSize(getSize() + 1);
    Value* elements = getElements(); // size must be set before
    move(elements + index + 1, elements + index, getSize() - index);
    elements[index] = value;
  }

  /**
    Remove the element specified by the index. Raises OutOfRange if the index is
    invalid.

    @param index The index of the element to be removed.
  */
  void remove(MemorySize index) throw(OutOfRange)
  {
    bassert(index < getSize(), OutOfRange(this));
    Value* elements = getElements(); // size must be set after
    move(elements + index, elements + index + 1, getSize() - index - 1);
    setSize(getSize() - 1);
  }

  /**
    Removes all the elements from this array.
  */
  void removeAll()
  {
    elements = new ReferenceCountedCapacityAllocator<Value>(); // no need to copy
    size = 0;
  }

  /**
    Returns the element at the specified index. Raises OutOfRange if the index
    is invalid.

    @param index The index of the element.
  */
  const Value& getAt(MemorySize index) const throw(OutOfRange)
  {
    bassert(index < getSize(), OutOfRange(this));
    return getElements()[index];
  }

  /**
    Sets the element at the specified index. Raises OutOfRange if the index is
    invalid.

    @param index The index of the element.
    @param value The desired value.
  */
  void setAt(MemorySize index, const Value& value) throw(OutOfRange)
  {
    bassert(index < getSize(), OutOfRange(this));
    getElements()[index] = value;
  }

  /**
    Returns a reference to the element at the specified index. Raises OutOfRange
    if the index is invalid.

    @param index The index of the element.
  */
  inline Element operator[](MemorySize index) throw(OutOfRange)
  {
    return Element(*this, index);
  }

  /**
    Returns the element at the specified index. Raises OutOfRange if the index
    is invalid.

    @param index The index of the element.
  */
  inline const Value& operator[](MemorySize index) const throw(OutOfRange)
  {
    return getAt(index);
  }
};

/**
  Writes a string representation of an array to a format stream.
  
  @relates Array
*/
template<class TYPE>
FormatOutputStream& operator<<(
  FormatOutputStream& stream, const Array<TYPE>& value) throw(IOException)
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
