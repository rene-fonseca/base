/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _BASE_RANDOM_ITERATOR_H
#define _BASE_RANDOM_ITERATOR_H

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
    Returns the current position.
  */
  virtual Distance getPosition() const throw() = 0;

  /**
    Sets the current position. Throws OutOfBounds if the position is invalid.
  */
  virtual void setPosition(Distance position) throw(OutOfBounds) = 0;

  /**
    Return the element at the specified position. Throws OutOfBounds if the specified position is invalid.

    @param position Index of the element to be returned.
  */
  virtual Reference at(Distance position) const throw(OutOfBounds) = 0;

  virtual Pointer operator*() const throw() = 0;

  virtual Pointer operator->() const throw() = 0;

  /**
    Returns the specified element.

    @param index The index of the desired element.
  */
  virtual Reference operator[](Distance position) const throw(OutOfBounds) = 0;
};

template<class TYPE>
class ReadOnlyRandomIterator : public RandomIterator<TYPE, ReadOnlyIteratorTraits<TYPE> > {
};

#endif
