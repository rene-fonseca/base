/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__DOUBLE_LINKED_NODE_H
#define _DK_SDU_MIP__BASE_COLLECTION__DOUBLE_LINKED_NODE_H

#include <base/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A double linked node. This class is used by collections that only require
  bidirectional traversal of elements.

  @short Double linked node
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TYPE>
class DoubleLinkedNode {
protected:

  /** The next node in the container. */
  DoubleLinkedNode* next;
  /** The previous node in the container. */
  DoubleLinkedNode* previous;
  /** The value associated with the node. */
  TYPE value;
public:

  /**
    Initializes a linked node.

    @param n The next node in the container.
    @param p The previous node in the container.
    @param v The value to be associated with the node.
  */
  inline DoubleLinkedNode(DoubleLinkedNode* n, DoubleLinkedNode* p, const TYPE& v) :
    next(n), previous(p), value(v) {}

  /**
    Returns the next node.
  */
  inline DoubleLinkedNode* getNext() const throw() {return next;}

  /**
    Sets the next node.
  */
  inline void setNext(DoubleLinkedNode* next) throw() {this->next = next;}

  /**
    Returns the previous node.
  */
  inline DoubleLinkedNode* getPrevious() const throw() {return previous;}

  /**
    Sets the previous node.
  */
  inline void setPrevious(DoubleLinkedNode* previous) throw() {this->previous = previous;}

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
