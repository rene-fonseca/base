/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP_BASE_FORWARD_ITERATOR_H
#define _DK_SDU_MIP_BASE_FORWARD_ITERATOR_H

#include <base/Iterator.h>
#include <base/OutOfBounds.h>

/**
  Forward iterator interface.

  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE, class IT = IteratorTraits<TYPE> >
class ForwardIterator : public Iterator<TYPE, IT> {
public:

  /**
    Returns true if next may be called without exception.

    @return True if the iteration has a valid
  */
  virtual bool hasNext() const throw() = 0;

  /**
    Returns the next element. Throws OutOfBounds if ...

    @return The next element.
  */
  virtual bool next() throw(OutOfBounds) = 0;

  /**
    Moves to the next element.
  */
//  virtual ForwardIterator& operator++() = 0;

  /**
    Skips a specified number of elements. Throws OutOfBounds if the end is exceeded.

    @param distance The desired number of elements skip.
  */
//  virtual void forward(Distance distance) throw(OutOfBounds) = 0;
};

template<class TYPE>
class ReadOnlyForwardIterator : public ForwardIterator<TYPE, ReadOnlyIteratorTraits<TYPE> > {
};

#endif
