/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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

  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version  1.0
*/

template<class TRAITS>
class MatrixColumnIterator : public SequenceIterator<TRAITS> {
private:

  Dimension d;
public:

  typedef InterleavedIterator<TRAITS> ElementIterator;

  inline MatrixColumnIterator(Pointer value, const Dimension& dimension) throw() :
    SequenceIterator<TRAITS>(value), d(dimension) {}

  inline ElementIterator getFirst() const throw() {
    return ElementIterator(element, d.getColumns());
  }

  inline ElementIterator getEnd() const throw() {
    return ElementIterator(element + d.getSize(), d.getColumns());
  }

  inline ElementIterator operator[](unsigned int index) const throw() {
    ASSERT(index < d.getRows());
    return ElementIterator(element + columns * index, d.getColumns());
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
