/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__ENUMERATION_H
#define _DK_SDU_MIP__BASE_COLLECTION__ENUMERATION_H

#include <base/collection/EndOfEnumeration.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Enumeration interface that is used to traverse individual elements of a
  collection one by one.

  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE, class REF, class PTR>
class Enumeration {
public:

  /** The type of the values being enumerated. */
  typedef TYPE Value;
  /** The type of a reference to a value. */
  typedef REF Reference;
  /** The type of a pointer to a value. */
  typedef PTR Pointer;

  /**
    Returns true if the enumeration has more elements.
  */
  virtual bool hasNext() const throw() = 0;

  /**
    Returns the next element of the enumeration.
  */
  virtual Pointer next() throw(EndOfEnumeration) = 0;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
