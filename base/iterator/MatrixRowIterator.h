/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_ITERATOR__MATRIXROWITERATOR_H
#define _DK_SDU_MIP__BASE_ITERATOR__MATRIXROWITERATOR_H

#include <base/iterator/InterleavedIterator.h>
#include <base/iterator/SequenceIterator.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @author René Møller Fonseca
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
