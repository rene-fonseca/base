/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__STACK_H
#define _DK_SDU_MIP__BASE_COLLECTION__STACK_H

#include <base/collection/Collection.h>
#include <base/OutOfRange.h>
#include <base/MemoryException.h>
#include <base/mem/ReferenceCountedObject.h>
#include <base/mem/ReferenceCountedObjectPointer.h>

/**
  Stack using a linked list implementation.

  @author René Møller Fonseca
  @version 1.1
*/
template<class TYPE>
class Stack : public Collection {
protected:

  /**
    A node of a stack.
  */
  class StackNode {
  protected:
    StackNode* next;
    StackNode* previous;
    TYPE value;
  public:
    inline StackNode(StackNode* n, StackNode* p, const TYPE& v) throw() : next(n), previous(p), value(v) {}
    inline StackNode* getNext() const throw() {return next;}
    inline void setNext(StackNode* value) throw() {next = value;}
    inline StackNode* getPrevious() const throw() {return previous;}
    inline void setPrevious(StackNode* value) throw() {previous = value;}
    inline TYPE* getValue() throw() {return &value;}
    inline const TYPE* getValue() const throw() {return &value;}
  };



  /**
    Internal stack implementation.

    @author René Møller Fonseca
    @version 1.0
  */
  class StackImpl : public ReferenceCountedObject {
  protected:

    /** The node on the top of the stack. */
    StackNode* top;
    /** The node at the bottom of the stack. */
    StackNode* bottom;
    /** The number of elements on the stack. */
    unsigned int size;
  public:

    /**
      Initialize an empty stack.
    */
    inline StackImpl() throw() : top(0), bottom(0) {}

    /**
      Initialize stack from other stack.
    */
    inline StackImpl(const StackImpl& copy) throw(MemoryException) : top(0), bottom(0) {
      const StackNode* node = bottom;
      while (node) {
        push(*node->getValue());
        node = node->getPrevious();
      }
    }

    /**
      Returns the number of elements on the stack.
    */
    inline unsigned int getSize() const throw() {return size;}

    /**
      Returns true if the stack is empty.
    */
    inline bool isEmpty() const throw() {return size == 0;}

    /**
      Returns the element at the specified index from the top. Throws
      'OutOfRange' if index is invalid.

      @param index Index of the element. Default is 0 corresponding to the top.
    */
    inline TYPE peek(unsigned int index = 0) const throw(OutOfRange) {
      if (index >= getSize()) {
        throw OutOfRange();
      }
      const StackNode* node = top;
      while (index) {
        node = node->getNext();
        --index;
      }
      return *node->getValue();
    }

    /**
      Pushes an element onto the stack.

      @param value The value to be pushed.
    */
    inline void push(const TYPE& value) throw(MemoryException) {
      if (isEmpty()) {
        StackNode* node = new StackNode(top, 0, value);
        top->setPrevious(node); // top is not null
        top = node;
        ++size;
      } else {
        top = new StackNode(0, 0, value);
        bottom = top;
        ++size;
      }
    }

    /*
      Pops the top element from the stack. Throws 'OutOfRange' if the stack is
      empty.

      @return The value of the top element.
    */
    inline TYPE pop() throw(OutOfRange) {
      if (isEmpty()) {
        throw OutOfRange();
      }
      StackNode* temp = top;
      top = top->getNext();
      --size;
      if (isEmpty()) {
        bottom = 0;
      }
      TYPE result = *top->getValue();
      delete temp;
      return result;
    }

    /**
      Pops the specified number of elements from the stack. Throws 'OutOfRange'
      if the stack does not contain the specified number of elements.

      @param count The number of elements to pop of the stack.
    */
    inline void pop(unsigned int count) throw(OutOfRange) {
      if (count > getSize()) {
        throw OutOfRange();
      }
      while (count) {
        StackNode* temp = top;
        top = top->getNext();
        --size;
        delete temp;
        --count;
      }
      if (isEmpty()) {
        bottom = 0;
      }
    }

    /**
      Removes all the elements from the stack.
    */
    inline void removeAll() throw() {
      while (size) {
        StackNode* temp = top;
        top = top->getNext();
        --size;
        delete temp;
      }
      bottom = 0;
    }

    /**
      Destroys the stack.
    */
    inline ~StackImpl() throw() {
      removeAll();
    }
  };



  /** The elements of the stack. */
  ReferenceCountedObjectPointer<StackImpl<TYPE> > elements;

  /**
    Makes a new copy of the internal representation of the elements if shared
    by several stacks. This member function must be called explicitly before
    most modifications to the stack.
  */
/*  inline void copyOnWrite() throw(MemoryException) {
    if (elements.isMultiReferenced()) { // do we have the elements for our self
      elements = new StackImpl<TYPE>(*elements); // make a copy of the elements
    }
  }*/
public:

  /**
    Initializes an empty stack.
  */
  Stack() throw() : elements(0) {}

  /**
    Initializes stack from other stack.
  */
  Stack(const Stack& copy) throw(MemoryException) : elements(copy.elements) {}

  /**
    Returns the number of elements on the stack.
  */
  unsigned int getSize() const throw() {
    return elements->getSize();
  }

  /**
    Returns true if the stack is empty.
  */
  bool isEmpty() const throw() {
    return elements->isEmpty();
  }

  /**
    Returns the element at the specified index from the top. Throws
    'OutOfRange' if index is invalid.

    @param index Index of the element. Default is 0 corresponding to the top.
  */
  TYPE peek(unsigned int index = 0) const throw(OutOfRange) {
    return elements->peek(index);
  }

  /**
    Pushes an element onto the stack.
  */
  void push(const TYPE& value) throw(MemoryException) {
    elements.copyOnWrite();
    elements->push(value);
  }

  /**
    Pops the top element from the stack. Throws 'OutOfRange' if the stack is
    empty.

    @return The value of the top element.
  */
  TYPE pop() throw(OutOfRange) {
    elements.copyOnWrite();
    return elements->pop();
  }

  /**
    Pops the specified number of elements from the stack. Throws 'OutOfRange'
    if the stack does not contain the specified number of elements.

    @param count The number of elements to pop of the stack.
  */
  void pop(unsigned int count) throw(OutOfRange) {
    elements.copyOnWrite();
    elements->pop(count);
  }

  /**
    Removes all the elements from the stack.
  */
  void removeAll() throw() {
    elements = 0; // no need to copy
  }
};

#endif
