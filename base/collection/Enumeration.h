/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__ENUMERATION_H
#define _DK_SDU_MIP__BASE_COLLECTION__ENUMERATION_H

#include <base/collection/EndOfEnumeration.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Defines the types of a modifying (has write access to the elements) enumerator.

  @short Descriptor for modifying enumerator.
  @ingroup collections
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/
template<class VALUE>
class EnumeratorTraits {
protected:

  typedef EnumeratorTraits SelfEnumeratorTraits;
public:

  typedef VALUE Value;
  typedef VALUE* Pointer;
  typedef unsigned int Distance;
};

/**
  Defines the types of a non-modifying enumerator (may only read the values of
  the elements).
  
  @short Descriptor for non-modifying enumerator.
  @ingroup collections
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/
template<class VALUE>
class ReadEnumeratorTraits {
protected:

  typedef ReadEnumeratorTraits SelfEnumeratorTraits;
public:

  typedef VALUE Value;
  typedef const VALUE* Pointer;
  typedef unsigned int Distance;
};

/**
  Enumeration interface used to traverse individual elements of a collection
  one by one. If possible, the enumeration interface should be used in
  preference to the iteration interface.

  @short Enumeration interface for modifying access
  @ingroup collections
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TRAITS>
class Enumerator {
private:

  /**
    Used to ensure that the specified traits template argument is an enumerator.
  */
  typedef typename TRAITS::SelfEnumeratorTraits ValidTraits;
public:

  /** The type of the element. */
  typedef typename TRAITS::Value Value;
  /** The type of the difference between elements. */
  typedef typename TRAITS::Distance Distance;
  /** The type of a pointer to an element. */
  typedef typename TRAITS::Pointer Pointer;

  /**
    Returns true if the enumerator has more elements.
  */
  virtual bool hasNext() const throw() = 0;

  /**
    Returns the next element of the enumerator.
  */
  virtual Pointer next() throw(EndOfEnumeration) = 0;

  /**
    Destroys the enumerator.
  */
  virtual inline ~Enumerator() throw() {
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
