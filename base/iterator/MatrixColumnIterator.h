/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_ITERATOR__MATRIX_COLUMN_ITERATOR_H
#define _DK_SDU_MIP__BASE_ITERATOR__MATRIX_COLUMN_ITERATOR_H

#include <base/iterator/InterleavedIterator.h>
#include <base/iterator/SequenceIterator.h>
#include <base/Dimension.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Column iterator for matrix.

  @short Matrix column iterator.
  @ingroup iterators
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version  1.0
*/

template<class TRAITS>
class MatrixColumnIterator : public SequenceIterator<TRAITS> {
private:

  Dimension dimension;
public:

  typedef InterleavedIterator<TRAITS> ElementIterator;

  inline MatrixColumnIterator(Pointer value, const Dimension& _dimension) throw() :
    SequenceIterator<TRAITS>(value), dimension(_dimension) {}

  /**
    Prefix increment.
  */
  inline MatrixColumnIterator& operator++() throw() {
    ++element;
    return *this;
  }

  /**
    Postfix decrement.
  */
  inline MatrixColumnIterator operator++(int) throw() {
    MatrixColumnIterator result(*this);
    ++element;
    return result;
  }

  /**
    Prefix decrement.
  */
  inline MatrixColumnIterator& operator--() throw() {
    --element;
    return *this;
  }

  /**
    Postfix decrement.
  */
  inline MatrixColumnIterator operator--(int) throw() {
    MatrixColumnIterator result(*this);
    --element;
    return result;
  }

  /**
    Move the specified distance forward.
  */
  inline MatrixColumnIterator& operator+=(Distance distance) throw() {
    element += distance;
    return *this;
  }

  /**
    Move the specified distance backwards.
  */
  inline MatrixColumnIterator& operator-=(Distance distance) throw() {
    element -= distance;
    return *this;
  }

  inline ElementIterator getFirst() const throw() {
    return ElementIterator(element, dimension.getWidth());
  }

  inline ElementIterator getEnd() const throw() {
    return ElementIterator(element + dimension.getSize(), dimension.getWidth());
  }

  inline ElementIterator operator[](unsigned int index) const throw() {
    return ElementIterator(element + columns * index, dimension.getWidth());
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

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
