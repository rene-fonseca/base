/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__SINGLE_LINKED_NODE_H
#define _DK_SDU_MIP__BASE_COLLECTION__SINGLE_LINKED_NODE_H

#include <base/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A single linked node. This class is used by collections that only require
  unidirectional traversal of elements.

  @short Single linked node.
  @author René Møller Fonseca
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

    @param n The next node in the container.
    @param v The value to be associated with the node.
  */
  inline SingleLinkedNode(SingleLinkedNode* n, const TYPE& v) :
    next(n), value(v) {}

  /**
    Returns the next node.
  */
  inline SingleLinkedNode* getNext() const throw() {return next;}

  /**
    Sets the next node.
  */
  inline void setNext(SingleLinkedNode* next) throw() {this->next = next;}

  /**
    Returns the value of the node.
  */
  inline TYPE* getValue() throw() {return &value;}

  /**
    Returns the value of the node.
  */
  inline const TYPE* getValue() const throw() {return &value;}

  /**
    Sets the value of the node.
  */
  inline void setValue(TYPE value) throw() {this->value = value;}
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
