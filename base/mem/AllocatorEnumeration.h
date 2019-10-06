/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2006 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/Base.h>
#include <base/collection/Enumeration.h>
#include <base/collection/EndOfEnumeration.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Enumerator used to traverse elements of a sequence (not to be confused with
  the Array collection).

  @short Enumerator of elements of a sequence.
  @ingroup memory
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TRAITS>
class AllocatorEnumerator : public Enumerator<TRAITS> {
public:

  typedef typename Enumerator<TRAITS>::Pointer Pointer;
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
  inline AllocatorEnumerator(Pointer begin, Pointer _end) throw()
    : current(begin), end(_end) {
  }

  /**
    Initializes enumeration from other enumeration.
  */
  inline AllocatorEnumerator(const AllocatorEnumerator& copy) throw()
    : current(copy.current), end(copy.end) {
  }

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
    bassert(current != end, EndOfEnumeration(this));
    return current++;
  }

  /**
    Returns true if the enumerations are referencing the same element.
  */
  inline bool operator==(const AllocatorEnumerator& eq) const throw() {
    return current == eq.current;
  }

  /**
    Returns true if the enumerations are not referencing the same element.
  */
  inline bool operator!=(const AllocatorEnumerator& eq) const throw() {
    return current != eq.current;
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
