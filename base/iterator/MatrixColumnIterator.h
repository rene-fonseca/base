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
#include <base/Dimension.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Column iterator for matrix.

  @short Matrix column iterator.
  @ingroup iterators
  @version  1.0
*/

template<class TRAITS>
class MatrixColumnIterator : public SequenceIterator<TRAITS> {
private:

  /** The dimension of the matrix. */
  Dimension dimension;
public:

  typedef typename SequenceIterator<TRAITS>::Distance Distance;
  typedef typename SequenceIterator<TRAITS>::Pointer Pointer;
  typedef typename SequenceIterator<TRAITS>::Reference Reference;
  typedef InterleavedIterator<TRAITS> ElementIterator;

  inline MatrixColumnIterator(Pointer value, const Dimension& _dimension) throw()
    : SequenceIterator<TRAITS>(value), dimension(_dimension) {
  }

  /**
    Prefix increment.
  */
  inline MatrixColumnIterator& operator++() throw() {
    ++this->element;
    return *this;
  }

  /**
    Postfix decrement.
  */
  inline MatrixColumnIterator operator++(int) throw() {
    MatrixColumnIterator result(*this);
    ++this->element;
    return result;
  }

  /**
    Prefix decrement.
  */
  inline MatrixColumnIterator& operator--() throw() {
    --this->element;
    return *this;
  }

  /**
    Postfix decrement.
  */
  inline MatrixColumnIterator operator--(int) throw() {
    MatrixColumnIterator result(*this);
    --this->element;
    return result;
  }

  /**
    Move the specified distance forward.
  */
  inline MatrixColumnIterator& operator+=(Distance distance) throw() {
    this->element += distance;
    return *this;
  }

  /**
    Move the specified distance backwards.
  */
  inline MatrixColumnIterator& operator-=(Distance distance) throw() {
    this->element -= distance;
    return *this;
  }

  inline ElementIterator getFirst() const throw() {
    return ElementIterator(this->element, dimension.getWidth());
  }

  inline ElementIterator getEnd() const throw() {
    return ElementIterator(this->element + dimension.getSize(), dimension.getWidth());
  }

  inline ElementIterator operator[](unsigned int index) const throw() {
    return ElementIterator(this->element + this->columns * index, dimension.getWidth());
  }
};

template<class TRAITS>
inline MatrixColumnIterator<TRAITS> operator+(const MatrixColumnIterator<TRAITS>& left, int right) throw() {
  MatrixColumnIterator<TRAITS> result(left);
  return result += right;
}

template<class TRAITS>
inline MatrixColumnIterator<TRAITS> operator-(const MatrixColumnIterator<TRAITS>& left, int right) throw() {
  MatrixColumnIterator<TRAITS> result(left);
  return result -= right;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
