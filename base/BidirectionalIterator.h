/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP_BASE_BIDIRECTIONAL_ITERATOR_H
#define _DK_SDU_MIP_BASE_BIDIRECTIONAL_ITERATOR_H

#include "base/ForwardIterator.h"

/**
  Bidirectional iterator interface.

  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE, class IT = IteratorTraits<TYPE> >
class BidirectionalIterator : public Iterator<TYPE, IT> {
public:

  /**
    Returns the previous element in the iteration.

    @return The previous element. Return NULL if no more elements.
  */
  virtual Pointer previous() throw() = 0;

  /**
    Skips a specified number of elements. Throws OutOfBounds if the end is exceeded.

    @param distance The desired number of elements skip.
  */
  virtual void rewind(Distance distance) throw(OutOfBounds) = 0;
};

template<class TYPE>
class ReadOnlyBidirectionalIterator : public BidirectionalIterator<TYPE, ReadOnlyIteratorTraits<TYPE> > {
};

#endif
