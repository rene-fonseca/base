/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__BIDIRECTIONAL_ITERATOR_H
#define _DK_SDU_MIP__BASE__BIDIRECTIONAL_ITERATOR_H

#include <base/ForwardIterator.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Bidirectional iterator interface.

  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
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

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
