/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Stack.h"

template<class TYPE>
Stack<TYPE>::Stack() throw() : top(NULL) {
}

template<class TYPE>
Stack<TYPE>::Stack(const Stack& copy) throw(MemoryException) : top(NULL) {
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
  return *node->getValue();
}

template<class TYPE>
void Stack<TYPE>::push(TYPE value) throw(MemoryException) {
  top = new StackNode(top, value);
  ++size;
}

template<class TYPE>
TYPE Stack<TYPE>::pop() throw(OutOfRange) {
  if (isEmpty()) {
    throw OutOfRange();
  }
  StackNode* temp = top;
  TYPE result = *top->getValue();
  top = top->getNext();
  --size;
  delete temp;
  return result;
}

template<class TYPE>
void Stack<TYPE>::pop(unsigned int count) throw(OutOfRange) {
  if (count > size) {
    throw OutOfRange();
  }
  while (count) {
    StackNode* temp = top;
    top = top->getNext();
    --size;
    delete temp;
    --count;
  }
}

template<class TYPE>
void Stack<TYPE>::removeAll() throw() {
  pop(size);
}

template<class TYPE>
Stack<TYPE>::~Stack() throw() {
  removeAll();
}



Stack<void*>::Stack() throw() : top(NULL) {
}

Stack<void*>::Stack(const Stack& copy) throw(MemoryException) : top(NULL) {
  StackNode* node = copy.top;
  while (node) {
    push(*node->getValue());
    node = node->getNext();
  }
}

void* Stack<void*>::peek(unsigned int index = 0) const throw(OutOfRange) {
  if (index >= size) {
    throw OutOfRange();
  }
  StackNode* node = top;
  while (index) {
    node = node->getNext();
    --index;
  }
  return *node->getValue();
}

void Stack<void*>::push(void* value) throw(MemoryException) {
  top = new StackNode(top, value);
  ++size;
}

void* Stack<void*>::pop() throw(OutOfRange) {
  if (isEmpty()) {
    throw OutOfRange();
  }
  StackNode* temp = top;
  void*& result = *top->getValue();
  top = top->getNext();
  --size;
  delete temp;
  return result;
}

void Stack<void*>::pop(unsigned int count) throw(OutOfRange) {
  if (count > size) {
    throw OutOfRange();
  }
  while (count) {
    StackNode* temp = top;
    top = top->getNext();
    --size;
    delete temp;
    --count;
  }
}

void Stack<void*>::removeAll() throw() {
  pop(size);
}

Stack<void*>::~Stack() throw() {
  removeAll();
}
