/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_MEM__ALLOCATOR_ENUMERATOR_H
#define _DK_SDU_MIP__BASE_MEM__ALLOCATOR_ENUMERATOR_H

#include <base/Base.h>
#include <base/collection/Enumeration.h>
#include <base/collection/EndOfEnumeration.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Enumeration of successive elements.

  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE, class REF, class PTR>
class AllocatorEnumeration : public Enumeration<TYPE, REF, PTR> {
public:

  /** The type of the values being enumerated. */
  typedef TYPE Value;
  /** The type of a reference to a value. */
  typedef REF Reference;
  /** The type of a pointer to a value. */
  typedef PTR Pointer;
private:

  /** The current position in the enumeration. */
  Pointer current;
  /** The end of the enumeration. */
  Pointer end;
public:

  /**
    Initializes an enumeration of all the elements specified by 'begin' and 'end'.

    @param begin Specifies the beginning of the enumeration.
    @param end Specifies the end of the enumeration.
  */
  inline AllocatorEnumeration(Pointer begin, Pointer end) throw() : current(begin), end(end) {
  }

  /**
    Initializes enumeration from other enumeration.
  */
  inline AllocatorEnumeration(const AllocatorEnumeration& copy) throw() : current(copy.current), end(copy.end) {}

  /**
    Returns true if the enumeration still contains elements.
  */
  inline bool hasNext() const throw() {
    return current != end;
  }

  /**
    Returns the next element and advances the position of this enumeration.
  */
  inline Pointer next() throw(EndOfEnumeration) {
    if (current == end) {
      throw EndOfEnumeration();
    }
    return current++;
  }

  /**
    Returns true if the enumerations are pointing to the same position.
  */
  inline bool operator==(const AllocatorEnumeration& eq) const throw() {
    return current == eq.current;
  }

  /**
    Returns true if the enumerations aren't pointing to the same position.
  */
  inline bool operator!=(const AllocatorEnumeration& eq) const throw() {
    return current != eq.current;
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
