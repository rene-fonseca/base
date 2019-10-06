/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2006 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/iterator/InterleavedIterator.h>
#include <base/iterator/SequenceIterator.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Row iterator for matrix.

  @short Matrix row iterator
  @ingroup iterators
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
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
    Element(const Element& copy) throw();
    Element& operator=(const Element& eq) throw();
    
    inline Element(const ElementIterator& _iterator) throw()
      : iterator(_iterator) {
    }
  public:
    
    inline Element& operator=(Value value) throw() {
      *iterator = value;
      return *this;
    }
    
    inline operator ElementIterator() const throw() {
      return iterator;
    }
    
    inline operator Value() const throw() {
      return *iterator;
    }
  };
  
  
  
  inline MatrixRowIterator(Pointer element, unsigned int columns) throw()
    : InterleavedIterator<TRAITS>(element, columns) {
  }

  /**
    Prefix increment.
  */
  inline MatrixRowIterator& operator++() throw() {
    InterleavedIterator<TRAITS>::element += InterleavedIterator<TRAITS>::step;
    return *this;
  }

  /**
    Postfix decrement.
  */
  inline MatrixRowIterator operator++(int) throw() {
    MatrixRowIterator result(*this);
    InterleavedIterator<TRAITS>::element += InterleavedIterator<TRAITS>::step;
    return result;
  }

  /**
    Prefix decrement.
  */
  inline MatrixRowIterator& operator--() throw() {
    InterleavedIterator<TRAITS>::element -= InterleavedIterator<TRAITS>::step;
    return *this;
  }

  /**
    Postfix decrement.
  */
  inline MatrixRowIterator operator--(int) throw() {
    MatrixRowIterator result(*this);
    InterleavedIterator<TRAITS>::element -= InterleavedIterator<TRAITS>::step;
    return result;
  }

  /**
    Move the specified distance forward.
  */
  inline MatrixRowIterator& operator+=(Distance distance) throw() {
    InterleavedIterator<TRAITS>::element += InterleavedIterator<TRAITS>::step * distance;
    return *this;
  }

  /**
    Move the specified distance backwards.
  */
  inline MatrixRowIterator& operator-=(Distance distance) throw() {
    InterleavedIterator<TRAITS>::element -= InterleavedIterator<TRAITS>::step * distance;
    return *this;
  }
  
  inline ElementIterator getFirst() const throw() {
    return ElementIterator(InterleavedIterator<TRAITS>::element);
  }

  inline ElementIterator getEnd() const throw() {
    return ElementIterator(InterleavedIterator<TRAITS>::element + InterleavedIterator<TRAITS>::step);
  }

  inline Element operator[](unsigned int index) const throw() {
    return Element(ElementIterator(InterleavedIterator<TRAITS>::element + index));
  }
};

template<class TRAITS>
inline MatrixRowIterator<TRAITS> operator+(const MatrixRowIterator<TRAITS>& left, int right) throw() {
  MatrixRowIterator<TRAITS> result(left);
  return result += right;
}

template<class TRAITS>
inline MatrixRowIterator<TRAITS> operator-(const MatrixRowIterator<TRAITS>& left, int right) throw() {
  MatrixRowIterator<TRAITS> result(left);
  return result -= right;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
