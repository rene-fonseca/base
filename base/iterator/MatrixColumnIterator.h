/***************************************************************************
    copyright   : (C) 2001 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_ITERATOR__MATRIXCOLUMNITERATOR_H
#define _DK_SDU_MIP__BASE_ITERATOR__MATRIXCOLUMNITERATOR_H

#include <base/iterator/InterleavedIterator.h>
#include <base/iterator/SequenceIterator.h>
#include <base/Dimension.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @author René Møller Fonseca
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
