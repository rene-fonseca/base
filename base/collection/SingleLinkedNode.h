/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A single linked node. This class is used by collections that only require
  unidirectional traversal of elements.

  @short Single linked node.
  @ingroup collections
  @version 1.0
*/

template<class TYPE>
class SingleLinkedNode {
protected:

  /** The next node in the container. */
  SingleLinkedNode* next;
  /** The value associated with the node. */
  TYPE value;
public:

  /**
    Initializes a linked node.

    @param next The next node in the container.
    @param value The value to be associated with the node.
  */
  inline SingleLinkedNode(SingleLinkedNode* _next, const TYPE& _value) throw()
    : next(_next),
      value(_value) {
  }

  /**
    Returns the next node.
  */
  inline SingleLinkedNode* getNext() const throw() {
    return next;
  }

  /**
    Sets the next node.
  */
  inline void setNext(SingleLinkedNode* next) throw() {
    this->next = next;
  }

  /**
    Returns the value of the node.
  */
  inline TYPE* getValue() throw() {
    return &value;
  }

  /**
    Returns the value of the node.
  */
  inline const TYPE* getValue() const throw() {
    return &value;
  }

  /**
    Sets the value of the node.
  */
  inline void setValue(TYPE value) throw() {
    this->value = value;
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
