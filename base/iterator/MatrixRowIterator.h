/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by Ren� M�ller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_ITERATOR__MATRIX_ROW_ITERATOR_H
#define _DK_SDU_MIP__BASE_ITERATOR__MATRIX_ROW_ITERATOR_H

#include <base/iterator/InterleavedIterator.h>
#include <base/iterator/SequenceIterator.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @author Ren� M�ller Fonseca
  @version 1.0
*/
template<class TRAITS>
class MatrixRowIterator : public InterleavedIterator<TRAITS> {
public:

  typedef SequenceIterator<TRAITS> ElementIterator;

  inline MatrixRowIterator(Pointer value, unsigned int columns) throw() :
    InterleavedIterator<TRAITS>(value, columns) {}

  inline ElementIterator getFirst() const throw() {
    return ElementIterator(element);
  }

  inline ElementIterator getEnd() const throw() {
    return ElementIterator(element + step);
  }

  inline ElementIterator operator[](unsigned int index) const throw() {
    ASSERT(index < step);
    return ElementIterator(element + index);
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
