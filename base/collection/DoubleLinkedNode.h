/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/features.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  A double linked node. This class is used by collections that only require
  bidirectional traversal of elements.

  @short Double linked node
  @version 1.0
*/

template<class TYPE>
class DoubleLinkedNode {
protected:

  /** The next node in the container. */
  DoubleLinkedNode* next = nullptr;
  /** The previous node in the container. */
  DoubleLinkedNode* previous = nullptr;
  /** The value associated with the node. */
  TYPE value;
public:

  /**
    Initializes a linked node.

    @param next The next node in the container.
    @param previous The previous node in the container.
    @param value The value to be associated with the node.
  */
  inline DoubleLinkedNode(DoubleLinkedNode* _next, DoubleLinkedNode* _previous, const TYPE& _value)
    : next(_next), previous(_previous), value(_value) {
  }

  /**
    Returns the next node.
  */
  inline DoubleLinkedNode* getNext() const throw() {
    return next;
  }

  /**
    Sets the next node.
  */
  inline void setNext(DoubleLinkedNode* next) throw() {
    this->next = next;
  }

  /**
    Returns the previous node.
  */
  inline DoubleLinkedNode* getPrevious() const throw() {
    return previous;
  }

  /**
    Sets the previous node.
  */
  inline void setPrevious(DoubleLinkedNode* previous) throw() {
    this->previous = previous;
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

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
