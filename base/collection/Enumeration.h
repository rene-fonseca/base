/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__ENUMERATION_H
#define _DK_SDU_MIP__BASE_COLLECTION__ENUMERATION_H

#include <base/collection/EndOfEnumeration.h>

/**
  Enumeration interface that is used to traverse individual elements of a
  collection one by one.

  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE>
class Enumeration {
public:

  /**
    Returns true if the enumeration has more elements.
  */
  virtual bool hasNext() const throw() = 0;

  /**
    Returns the next element of the enumeration.

    @return NULL if no more elements are available in the enumeration.
  */
  virtual const TYPE* const next() throw(EndOfEnumeration) = 0;
};

#endif
