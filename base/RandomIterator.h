/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP_BASE_RANDOM_ITERATOR_H
#define _DK_SDU_MIP_BASE_RANDOM_ITERATOR_H

#include "base/BidirectionalIterator.h"

/**
  Random iterator interface.

  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE, class IT = IteratorTraits<TYPE> >
class RandomIterator : public Iterator<TYPE, IT> {
public:

  /**
    Returns the current index.
  */
  virtual Distance getIndex() const throw() = 0;

  /**
    Sets the current index. Throws 'OutOfBounds' if the index is invalid.

    @param index The desired index.
  */
  virtual void setIndex(Distance index) throw(OutOfBounds) = 0;

  /**
    Returns the specified element. Throws 'OutOfBounds' if the specified index is invalid.

    @param index The index of the desired element.
  */
  virtual Reference operator[](Distance index) const throw(OutOfBounds) = 0;
};

template<class TYPE>
class ReadOnlyRandomIterator : public RandomIterator<TYPE, ReadOnlyIteratorTraits<TYPE> > {
};

#endif
