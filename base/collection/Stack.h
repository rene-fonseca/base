/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__STACK_H
#define _DK_SDU_MIP__BASE_COLLECTION__STACK_H

#include "base/Object.h"
#include "Collection.h"
#include "base/OutOfRange.h"
#include "base/MemoryException.h"

/**
  Stack using a linked list implementation.

  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE>
class Stack : public Object /*, public Collection*/ {
protected:

  class StackNode {
  protected:

    StackNode* next;
    TYPE value;
  public:

    inline StackNode(StackNode* n, const TYPE& v) : next(n), value(v) {}
    inline StackNode* getNext() const throw() {return next;}
    inline TYPE* getValue() throw() {return &value;}
  };

  /** The node on the top of the stack. */
  StackNode* top;
  /** The number of nodes in the stack. */
  unsigned int size;
public:

  /**
    Initializes an empty stack.
  */
  Stack() throw();

  /**
    Initializes stack from other stack.
  */
  Stack(const Stack& copy) throw(MemoryException);

  /**
    Returns the number of nodes on the stack.
  */
  inline unsigned int getSize() const throw() {return size;}

  /**
    Returns true if the stack is empty.
  */
  inline bool isEmpty() const throw() {return size == 0;}

  /**
    Returns the element at the specified index from the top. Throws
    'OutOfRange' if index if

    @param index Index of the element. Default is 0.
  */
  TYPE peek(unsigned int index = 0) const throw(OutOfRange);

  /**
    Pushes an element onto the stack.
  */
  void push(TYPE& value) throw(MemoryException);

  /**
    Returns the top element of the stack and removes it. Throws 'OutOfRange'
    if the stack is empty.
  */
  TYPE pop() throw(OutOfRange);

  /**
    Pops the specified number of elements from the stack. Throws 'OutOfRange'
    if the stack does not contain the specified number of elements.
  */
  void pop(unsigned int count) throw(OutOfRange);

  /**
    Destroys the stack.
  */
  ~Stack() throw();
};

#endif
