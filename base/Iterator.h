/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__ITERATOR_H
#define _DK_SDU_MIP__BASE__ITERATOR_H

#include <base/OutOfBounds.h>

/**
  Defines the types of a writeable iterator.
*/
template<class TYPE>
class IteratorTraits {
public:
  typedef TYPE Value;
  typedef unsigned int Distance; // should be signed for direction?
  typedef TYPE* Pointer;
  typedef TYPE& Reference;
};

/**
  Defines the types of a read-only iterator.
*/
template<class TYPE>
class ReadOnlyIteratorTraits {
public:
  typedef TYPE Value;
  typedef unsigned int Distance;
  typedef const TYPE* Pointer;
  typedef const TYPE& Reference;
};

/**
  Iterator interface.

  @author René Møller Fonseca
  @version 1.0
*/
template<class TYPE, class IT = IteratorTraits<TYPE> >
class Iterator {
public:

  /** The type of the element. */
  typedef typename IT::Value Value;
  /** The type of the difference between elements. */
  typedef typename IT::Distance Distance;
  /** Type of '->'. */
  typedef typename IT::Pointer Pointer;
  /** Type of '*'. */
  typedef typename IT::Reference Reference;

  /**
    Returns the element at the current position.
  */
//  virtual Pointer operator->() const throw() = 0;

  /**
    Returns the element at the current position.
  */
  virtual Reference getElement() const throw(OutOfBounds) = 0;

//  inline Reference operator*() const throw(OutOfBounds) {return getElement();};
};

#endif
