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

#include <base/ForwardIterator.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  @short Bidirectional iterator interface.
  @version 1.0
*/

template<class TYPE, class IT = IteratorTraits<TYPE> >
class BidirectionalIterator : public Iterator<TYPE, IT> {
public:

  /**
    Returns the previous element in the iteration.

    @return The previous element. Return 0 if no more elements.
  */
  virtual Pointer previous() throw() = 0;

  /**
    Skips a specified number of elements. Raises OutOfBounds if the end is
    exceeded.

    @param distance The desired number of elements skip.
  */
  virtual void rewind(Distance distance) throw(OutOfBounds) = 0;
};

template<class TYPE>
class ReadOnlyBidirectionalIterator : public BidirectionalIterator<TYPE, ReadOnlyIteratorTraits<TYPE> > {
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
