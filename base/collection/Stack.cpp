/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Stack.h"

template Stack<void*>;

template<class TYPE>
Stack<TYPE>::Stack() throw() : top(NULL), size(0) {
}

template<class TYPE>
Stack<TYPE>::Stack(const Stack& copy) throw(MemoryException) : top(NULL), size(0) {
  StackNode* node = copy.top;
  while (node) {
    push(*node->getValue());
    node = node->getNext();
  }
}

template<class TYPE>
TYPE Stack<TYPE>::peek(unsigned int index = 0) const throw(OutOfRange) {
  if (index >= size) {
    throw OutOfRange();
  }
  StackNode* node = top;
  while (index) {
    node = node->getNext();
    --index;
  }
  return node->getValue();
}

template<class TYPE>
void Stack<TYPE>::push(TYPE& value) throw(MemoryException) {
  top = new StackNode(top, value);
  ++size;
}

template<class TYPE>
TYPE Stack<TYPE>::pop() throw(OutOfRange) {
  if (isEmpty()) {
    throw OutOfRange();
  }
  StackNode* temp = top;
  TYPE result = top->getValue();
  top = top->getNext();
  --size;
  delete temp;
  return result;
}

template<class TYPE>
void Stack<TYPE>::pop(unsigned int count) throw(OutOfRange) {
  while (count) {
    pop();
    --count;
  }
}

template<class TYPE>
Stack<TYPE>::~Stack() throw() {
  pop(size); // does not throw OutOfRange
}
