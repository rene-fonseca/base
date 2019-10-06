/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2006 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/collection/Collection.h>
#include <base/collection/Enumeration.h>
#include <base/OutOfRange.h>
#include <base/MemoryException.h>
#include <base/mem/Reference.h>
#include <base/string/FormatOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A stack is a last-in-first-out data structure. This class is implemented as a
  linked list.
  
  @short Stack collection.
  @ingroup collections
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.1
*/

template<class TYPE>
class Stack : public Collection {
protected:

  /*
    A node of a stack.
  */
  class StackNode {
  protected:
    
    StackNode* next = nullptr;
    StackNode* previous = nullptr;
    TYPE value;
  public:
    
    inline StackNode(
      StackNode* _next,
      StackNode* _previous,
      const TYPE& _value) throw()
      : next(_next), previous(_previous), value(_value) {
    }
    
    inline StackNode* getNext() const throw() {
      return next;
    }
    
    inline void setNext(StackNode* value) throw() {
      next = value;
    }
    
    inline StackNode* getPrevious() const throw() {
      return previous;
    }
    
    inline void setPrevious(StackNode* value) throw() {
      previous = value;
    }
    
    inline TYPE* getValue() throw() {
      return &value;
    }
    
    inline const TYPE* getValue() const throw() {
      return &value;
    }
  };

  /*
    Enumerator of elements of a stack.
    
    @short Non-modifying enumerator of elements of a stack.
    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  
  template<class TRAITS>
  class StackReadEnumerator : public Enumerator<TRAITS> {
  public:
    
    typedef typename Enumerator<TRAITS>::Value Value;
    typedef typename Enumerator<TRAITS>::Pointer Pointer;
  private:
    
    /** The current position in the enumeration. */
    const StackNode* current = nullptr;
  public:
    
    /**
      Initializes an enumeration of all the elements of a stack.
      
      @param begin Specifies the beginning of the enumeration.
    */
    explicit inline StackReadEnumerator(const StackNode* begin) throw()
      : current(begin) {
    }
    
    /**
      Initializes enumeration from other enumeration.
    */
    inline StackReadEnumerator(const StackReadEnumerator& copy) throw()
      : current(copy.current) {
    }
    
    /**
      Returns true if the enumeration still contains elements.
    */
    inline bool hasNext() const throw() {
      return current != nullptr;
    }

    /**
      Returns the next element and advances the position of this enumeration.
    */
    inline Pointer next() throw(EndOfEnumeration) {
      bassert(current != 0, EndOfEnumeration());
      Pointer temp = current->getValue();
      current = current->getNext();
      return temp;
    }
    
    /**
      Returns true if the enumerations are pointing to the same position.
    */
    inline bool operator==(const StackReadEnumerator& eq) const throw() {
      return current == eq.current;
    }
    
    /**
      Returns true if the enumerations aren't pointing to the same position.
    */
    inline bool operator!=(const StackReadEnumerator& eq) const throw() {
      return current != eq.current;
    }
  };
  
  
  
public:
  typedef StackReadEnumerator<ReadEnumeratorTraits<TYPE> > ReadEnumerator;

protected:
  
  
  
  /*
    Internal stack implementation.

    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class StackImpl : public ReferenceCountedObject {
  protected:

    /** The node on the top of the stack. */
    StackNode* top = nullptr;
    /** The node at the bottom of the stack. */
    StackNode* bottom = nullptr;
    /** The number of elements on the stack. */
    unsigned int size = 0;
  public:

    /**
      Initialize an empty stack.
    */
    inline StackImpl() throw() {
    }
    
    /**
      Initialize stack from other stack.
    */
    inline StackImpl(const StackImpl& copy) throw(MemoryException) {
      const StackNode* node = copy.bottom;
      while (node) {
        push(*node->getValue());
        node = node->getPrevious();
      }
    }
    
    /**
      Returns the number of elements on the stack.
    */
    inline unsigned int getSize() const throw() {
      return size;
    }
    
    /**
      Returns true if the stack is empty.
    */
    inline bool isEmpty() const throw() {
      return size == 0;
    }

    /**
      Returns the top node of the stack.
    */
    inline const StackNode* getTop() const throw() {
      return top;
    }
    
    /**
      Returns the element at the specified index from the top. Raises
      OutOfRange if index is invalid.

      @param index Index of the element. Default is 0 corresponding to the top.
    */
    TYPE peek(unsigned int index = 0) const throw(OutOfRange) {
      if (index >= getSize()) {
        throw OutOfRange(this);
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
    void push(const TYPE& value) throw(MemoryException) {
      if (!isEmpty()) {
        StackNode* node = new StackNode(top, nullptr, value);
        top->setPrevious(node); // top is not null
        top = node;
        ++size;
      } else {
        top = new StackNode(nullptr, nullptr, value);
        bottom = top;
        ++size;
      }
    }
    
    /*
      Pops the top element from the stack. Raises OutOfRange if the stack is
      empty.

      @return The value of the top element.
    */
    TYPE pop() throw(OutOfRange) {
      if (isEmpty()) {
        throw OutOfRange(this);
      }
      StackNode* temp = top;
      top = top->getNext();
      --size;
      if (isEmpty()) {
        bottom = nullptr;
      } else {
        top->setPrevious(nullptr);
      }
      TYPE result = *temp->getValue();
      delete temp;
      return result;
    }
    
    /**
      Pops the specified number of elements from the stack. Raises OutOfRange
      if the stack does not contain the specified number of elements.

      @param count The number of elements to pop of the stack.
    */
    void pop(unsigned int count) throw(OutOfRange) {
      if (count > getSize()) {
        throw OutOfRange(this);
      }
      while (count) {
        StackNode* temp = top;
        top = top->getNext();
        --size;
        delete temp;
        --count;
      }
      if (isEmpty()) {
        bottom = nullptr;
      } else {
        top->setPrevious(nullptr);
      }
    }
    
    /**
      Removes all the elements from the stack.
    */
    void removeAll() throw() {
      while (size) {
        StackNode* temp = top;
        top = top->getNext();
        --size;
        delete temp;
      }
      bottom = nullptr;
    }
    
    /**
      Destroys the stack.
    */
    inline ~StackImpl() throw() {
      removeAll();
    }
  };
  
  /** The elements of the stack. */
  Reference<StackImpl> elements;
  
  /**
    Makes a new copy of the internal representation of the elements if shared
    by several stacks. This member function must be called explicitly before
    most modifications to the stack.
  */
  inline void copyOnWrite() throw(MemoryException) {
    if (elements.isMultiReferenced()) { // do we have the elements for our self
      elements = new StackImpl(*elements); // make a copy of the elements
    }
  }
public:

  /**
    Initializes an empty stack.
  */
  inline Stack() throw() : elements(new StackImpl()) {
  }
  
  /**
    Initializes stack from other stack.
  */
  inline Stack(const Stack& copy) throw()
    : elements(copy.elements) {
  }

  /**
    Assignment of stack by stack.
  */
  inline Stack& operator=(const Stack& eq) throw() {
    elements = eq.elements;
    return *this;
  }
  
  /**
    Returns the number of elements on the stack.
  */
  inline unsigned int getSize() const throw() {
    return elements->getSize();
  }
  
  /**
    Returns true if the stack is empty.
  */
  inline bool isEmpty() const throw() {
    return elements->isEmpty();
  }
  
  /**
    Returns the element at the specified index from the top. Raises
    OutOfRange if index is invalid.

    @param index Index of the element. Default is 0 corresponding to the top.
  */
  inline TYPE peek(unsigned int index = 0) const throw(OutOfRange) {
    return elements->peek(index);
  }
  
  /**
    Pushes an element onto the stack.
  */
  inline void push(const TYPE& value) throw(MemoryException) {
    elements.copyOnWrite();
    elements->push(value);
  }
  
  /**
    Pops the top element from the stack. Raises OutOfRange if the stack is
    empty.

    @return The value of the top element.
  */
  inline TYPE pop() throw(OutOfRange) {
    elements.copyOnWrite();
    return elements->pop();
  }
  
  /**
    Pops the specified number of elements from the stack. Raises OutOfRange
    if the stack does not contain the specified number of elements.

    @param count The number of elements to pop of the stack.
  */
  inline void pop(unsigned int count) throw(OutOfRange) {
    elements.copyOnWrite();
    elements->pop(count);
  }

  /**
    Returns a non-modifying enumerator of the stack. The elements are enumerated
    from top to bottom.
  */
  inline ReadEnumerator getReadEnumerator() const throw() {
    return ReadEnumerator(elements->getTop());
  }
  
  /**
    Removes all the elements from the stack.
  */
  inline void removeAll() throw() {
    elements = new StackImpl(); // no need to copy
  }
};

/**
  Writes a string representation of a stack to a format output stream.

  @relates Stack
*/
template<class TYPE>
FormatOutputStream& operator<<(
  FormatOutputStream& stream,
  const Stack<TYPE>& value) throw(IOException) {
  
  typename Stack<TYPE>::ReadEnumerator enu = value.getReadEnumerator();
  stream << '{';
  while (enu.hasNext()) {
    stream << *enu.next();
    if (enu.hasNext()) {
      stream << ';';
    }
  }
  stream << '}';
  return stream;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
