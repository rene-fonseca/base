/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__STACK_H
#define _DK_SDU_MIP__BASE_COLLECTION__STACK_H

#include <base/collection/Collection.h>
#include <base/OutOfRange.h>
#include <base/MemoryException.h>

/**
  Stack using a linked list implementation.

  @author René Møller Fonseca
  @version 1.0
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
    TYPE value;
  public:
    inline StackNode(StackNode* n, const TYPE& v) : next(n), value(v) {}
    inline StackNode* getNext() const throw() {return next;}
    inline TYPE* getValue() throw() {return &value;}
  };

  /** The node on the top of the stack. */
  StackNode* top;
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
    Returns the element at the specified index from the top. Throws
    'OutOfRange' if index is invalid.

    @param index Index of the element. Default is 0 corresponding to the top.
  */
  TYPE peek(unsigned int index = 0) const throw(OutOfRange);

  /**
    Pushes an element onto the stack.
  */
  void push(TYPE value) throw(MemoryException);

  /**
    Pops the top element from the stack. Throws 'OutOfRange' if the stack is
    empty.

    @return The top element.
  */
  TYPE pop() throw(OutOfRange);

  /**
    Pops the specified number of elements from the stack. Throws 'OutOfRange'
    if the stack does not contain the specified number of elements.
  */
  void pop(unsigned int count) throw(OutOfRange);

  /**
    Removes all the elements from the stack.
  */
  void removeAll() throw();

  /**
    Destroys the stack.
  */
  ~Stack() throw();
};



template<class TYPE> class Stack<TYPE*>;
template<> class Stack<void*>;



template<>
class Stack<void*> : public Collection {
protected:

  typedef void* TYPE;

  class StackNode {
  protected:
    StackNode* next;
    TYPE value;
  public:
    inline StackNode(StackNode* n, const TYPE& v) : next(n), value(v) {}
    inline StackNode* getNext() const throw() {return next;}
    inline TYPE* getValue() throw() {return &value;}
  };

  StackNode* top;
  unsigned int size;
public:

  Stack() throw();
  Stack(const Stack& copy) throw(MemoryException);
  void* peek(unsigned int index = 0) const throw(OutOfRange);
  void push(void* value) throw(MemoryException);
  void* pop() throw(OutOfRange);
  void pop(unsigned int count) throw(OutOfRange);
  void removeAll() throw();
  ~Stack() throw();
};



template<class TYPE>
class Stack<TYPE*> : private Stack<void*> {
public:

  typedef Stack<void*> Base;

  inline Stack() throw() : Base() {}
  inline Stack(const Stack& copy) throw(MemoryException) : Base(copy) {}
  inline unsigned int getSize() const throw() {return Base::getSize();}
  inline bool isEmpty() const throw() {return Base::isEmpty();}
  inline TYPE peek(unsigned int index = 0) const throw(OutOfRange) {return static_cast<TYPE>(Base::peek(index));}
  inline void push(TYPE value) throw(MemoryException) {Base::push(value);}
  inline TYPE pop() throw(OutOfRange) {return static_cast<TYPE>(Base::pop());}
  inline void pop(unsigned int count) throw(OutOfRange) {Base::pop(count);}
};

#endif
