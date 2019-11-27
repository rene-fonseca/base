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

#include <base/iterator/InterleavedIterator.h>
#include <base/iterator/SequenceIterator.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Row iterator for matrix.

  @short Matrix row iterator
  @ingroup iterators
  @version 1.0
*/
template<class TRAITS>
class MatrixRowIterator : public InterleavedIterator<TRAITS> {
public:

  typedef typename InterleavedIterator<TRAITS>::Distance Distance;
  typedef typename InterleavedIterator<TRAITS>::Pointer Pointer;
  typedef SequenceIterator<TRAITS> ElementIterator;
  
  /*
    Reference to an element within the iterator.
  */
  class Element {
    friend class MatrixRowIterator;
  public:
    
    typedef typename ElementIterator::Value Value;
  private:
    
    ElementIterator iterator;
    Element(const Element& copy) noexcept;
    Element& operator=(const Element& assign) noexcept;
    
    inline Element(const ElementIterator& _iterator) noexcept
      : iterator(_iterator)
    {
    }
  public:
    
    inline Element& operator=(const Value& value) noexcept
    {
      *iterator = value;
      return *this;
    }
    
    inline operator ElementIterator() const noexcept
    {
      return iterator;
    }
    
    inline operator const Value&() const noexcept
    {
      return *iterator;
    }
  };
  
  
  
  inline MatrixRowIterator(Pointer element, unsigned int columns) noexcept
    : InterleavedIterator<TRAITS>(element, columns)
  {
  }

  /**
    Prefix increment.
  */
  inline MatrixRowIterator& operator++() noexcept
  {
    InterleavedIterator<TRAITS>::element += InterleavedIterator<TRAITS>::step;
    return *this;
  }

  /**
    Postfix decrement.
  */
  inline MatrixRowIterator operator++(int) noexcept
  {
    MatrixRowIterator result(*this);
    InterleavedIterator<TRAITS>::element += InterleavedIterator<TRAITS>::step;
    return result;
  }

  /**
    Prefix decrement.
  */
  inline MatrixRowIterator& operator--() noexcept
  {
    InterleavedIterator<TRAITS>::element -= InterleavedIterator<TRAITS>::step;
    return *this;
  }

  /**
    Postfix decrement.
  */
  inline MatrixRowIterator operator--(int) noexcept
  {
    MatrixRowIterator result(*this);
    InterleavedIterator<TRAITS>::element -= InterleavedIterator<TRAITS>::step;
    return result;
  }

  /**
    Move the specified distance forward.
  */
  inline MatrixRowIterator& operator+=(Distance distance) noexcept
  {
    InterleavedIterator<TRAITS>::element += InterleavedIterator<TRAITS>::step * distance;
    return *this;
  }

  /**
    Move the specified distance backwards.
  */
  inline MatrixRowIterator& operator-=(Distance distance) noexcept
  {
    InterleavedIterator<TRAITS>::element -= InterleavedIterator<TRAITS>::step * distance;
    return *this;
  }
  
  inline ElementIterator getFirst() const noexcept
  {
    return ElementIterator(InterleavedIterator<TRAITS>::element);
  }

  inline ElementIterator getEnd() const noexcept
  {
    return ElementIterator(InterleavedIterator<TRAITS>::element + InterleavedIterator<TRAITS>::step);
  }

  inline Element operator[](unsigned int index) const noexcept
  {
    return Element(ElementIterator(InterleavedIterator<TRAITS>::element + index));
  }
};

template<class TRAITS>
inline MatrixRowIterator<TRAITS> operator+(const MatrixRowIterator<TRAITS>& left, int right) noexcept
{
  MatrixRowIterator<TRAITS> result(left);
  return result += right;
}

template<class TRAITS>
inline MatrixRowIterator<TRAITS> operator-(const MatrixRowIterator<TRAITS>& left, int right) noexcept
{
  MatrixRowIterator<TRAITS> result(left);
  return result -= right;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
