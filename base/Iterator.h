/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE__ITERATOR_H
#define _DK_SDU_MIP__BASE__ITERATOR_H

#include <base/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Defines the types of a modifying (has write access to the elements) iterator.
*/
template<class VALUE>
class IteratorTraits {
public:

  typedef IteratorTraits SelfIteratorTraits;
  typedef VALUE Value;
  typedef VALUE& Reference;
  typedef VALUE* Pointer;
  typedef unsigned int Distance;
};

/**
  Defines the types of a non-modifying iterator (may only read the values of the elements).
*/
template<class VALUE>
class ReadIteratorTraits {
public:

  typedef ReadIteratorTraits SelfIteratorTraits;
  typedef VALUE Value;
  typedef const VALUE& Reference;
  typedef const VALUE* Pointer;
  typedef unsigned int Distance;
};

/**
  Iterator interface.

  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/
template<class TRAITS>
class Iterator {
private:

  /** Used to ensure that the specified traits template argument is an iterator. */
  typedef typename TRAITS::SelfIteratorTraits ValidTraits;
public:

  /** The type of the element. */
  typedef typename TRAITS::Value Value;
  /** The type of the difference between elements. */
  typedef typename TRAITS::Distance Distance;
  /** The type of a reference to an element . */
  typedef typename TRAITS::Reference Reference;
  /** The type of a pointer to an element. */
  typedef typename TRAITS::Pointer Pointer;
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
