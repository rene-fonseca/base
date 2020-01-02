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

#include <base/collection/Collection.h>
#include <base/collection/DoubleLinkedNode.h>
#include <base/collection/Enumeration.h>
#include <base/OutOfRange.h>
#include <base/MemoryException.h>
#include <base/mem/Reference.h>
#include <base/string/FormatOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  A stack is a last-in-first-out data structure. This class is implemented as a
  linked list.
  
  @short Stack collection.
  @ingroup collections
  @version 1.1
*/

template<class TYPE>
class Stack : public Collection {
protected:

  typedef DoubleLinkedNode<TYPE> StackNode;
  typedef DoubleLinkedNodeIterator<TYPE> Iterator;
  typedef DoubleLinkedNodeReadIterator<TYPE> ReadIterator;

  /*
    Enumerator of elements of a stack.
    
    @short Non-modifying enumerator of elements of a stack.
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
    explicit inline StackReadEnumerator(const StackNode* begin) noexcept
      : current(begin)
    {
    }
    
    /**
      Initializes enumeration from other enumeration.
    */
    inline StackReadEnumerator(const StackReadEnumerator& copy) noexcept
      : current(copy.current)
    {
    }
    
    /**
      Returns true if the enumeration still contains elements.
    */
    inline bool hasNext() const noexcept
    {
      return current != nullptr;
    }

    /**
      Returns the next element and advances the position of this enumeration.
    */
    Pointer next()
    {
      if (!current) {
        _throw EndOfEnumeration();
      }
      auto temp = current;
      current = current->getNext();
      return &(temp->getValue());
    }
    
    /**
      Returns true if the enumerations are pointing to the same position.
    */
    inline bool operator==(const StackReadEnumerator& compare) const noexcept
    {
      return current == compare.current;
    }
    
    /**
      Returns true if the enumerations aren't pointing to the same position.
    */
    inline bool operator!=(const StackReadEnumerator& compare) const noexcept
    {
      return current != compare.current;
    }
  };
  
  
  
public:
  typedef StackReadEnumerator<ReadEnumeratorTraits<TYPE> > ReadEnumerator;

protected:
  
  /*
    Internal stack implementation.
  */
  class StackImpl : public ReferenceCountedObject {
  protected:

    /** The node on the top of the stack. */
    StackNode* top = nullptr;
    /** The node at the bottom of the stack. */
    StackNode* bottom = nullptr;
    /** The number of elements on the stack. */
    MemorySize size = 0;
  public:

    /**
      Initialize an empty stack.
    */
    inline StackImpl() noexcept
    {
    }
    
    /**
      Initialize stack from other stack.
    */
    StackImpl(const StackImpl& copy)
    {
      const StackNode* node = copy.bottom;
      while (node) {
        push(node->getValue());
        node = node->getPrevious();
      }
    }
    
    /**
      Returns the number of elements on the stack.
    */
    inline MemorySize getSize() const noexcept
    {
      return size;
    }
    
    /**
      Returns true if the stack is empty.
    */
    inline bool isEmpty() const noexcept
    {
      return size == 0;
    }

    /**
      Returns the top node of the stack.
    */
    inline StackNode* getTop() noexcept
    {
      return top;
    }

    /**
      Returns the top node of the stack.
    */
    inline const StackNode* getTop() const noexcept
    {
      return top;
    }
    
    /**
      Returns the element at the specified index from the top. Raises
      OutOfRange if index is invalid.

      @param index Index of the element. Default is 0 corresponding to the top.
    */
    const TYPE& peek(MemorySize index = 0) const
    {
      if (index >= getSize()) {
        _throw OutOfRange(this);
      }
      const StackNode* node = top;
      while (index) {
        node = node->getNext();
        --index;
      }
      return node->getValue();
    }
    
    /**
      Pushes an element onto the stack.

      @param value The value to be pushed.
    */
    void push(const TYPE& value)
    {
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
    TYPE pop()
    {
      if (isEmpty()) {
        _throw OutOfRange(this);
      }
      StackNode* temp = top;
      top = top->getNext();
      --size;
      if (isEmpty()) {
        bottom = nullptr;
      } else {
        top->setPrevious(nullptr);
      }
      TYPE result = temp->getValue();
      delete temp;
      return result;
    }
    
    /**
      Pops the specified number of elements from the stack. Raises OutOfRange
      if the stack does not contain the specified number of elements.

      @param count The number of elements to pop of the stack.
    */
    void pop(MemorySize count)
    {
      if (count > getSize()) {
        _throw OutOfRange(this);
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

#if 0
    void popBottom()
    {
    }
#endif
    
    /**
      Removes all the elements from the stack.
    */
    void removeAll() noexcept
    {
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
    inline ~StackImpl() noexcept
    {
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
  inline void copyOnWrite()
  {
    if (elements.isMultiReferenced()) { // do we have the elements for our self
      elements = new StackImpl(*elements); // make a copy of the elements
    }
  }
public:

  /**
    Initializes an empty stack.
  */
  inline Stack() noexcept
    : elements(new StackImpl())
  {
  }
  
  /**
    Pushes all the values in left-to-right order.
  */
  Stack(std::initializer_list<TYPE> values)
    : elements(new StackImpl())
  {
    for (const auto& value : values) {
      push(value);
    }
  }
  
  /**
    Initializes stack from other stack.
  */
  inline Stack(const Stack& copy) noexcept
    : elements(copy.elements)
  {
  }

  /**
    Assignment of stack by stack.
  */
  inline Stack& operator=(const Stack& assign) noexcept
  {
    elements = assign.elements;
    return *this;
  }
  
  /**
    Returns the number of elements on the stack.
  */
  inline MemorySize getSize() const noexcept
  {
    return elements->getSize();
  }
  
  /**
    Returns true if the stack is empty.
  */
  inline bool isEmpty() const noexcept
  {
    return elements->isEmpty();
  }
  
  /**
    Returns the element at the specified index from the top. Raises
    OutOfRange if index is invalid.

    @param index Index of the element. Default is 0 corresponding to the top.
  */
  inline TYPE peek(MemorySize index = 0) const
  {
    return elements->peek(index);
  }
  
  /**
    Pushes an element onto the stack.
  */
  inline void push(const TYPE& value)
  {
    elements.copyOnWrite();
    elements->push(value);
  }
  
  /**
    Pops the top element from the stack. Raises OutOfRange if the stack is
    empty.

    @return The value of the top element.
  */
  inline TYPE pop()
  {
    elements.copyOnWrite();
    return elements->pop();
  }
  
  /**
    Pops the specified number of elements from the stack. Raises OutOfRange
    if the stack does not contain the specified number of elements.

    @param count The number of elements to pop of the stack.
  */
  inline void pop(MemorySize count)
  {
    if (count > 0) {
      elements.copyOnWrite();
      elements->pop(count);
    }
  }

  // TAG: add support for moving element to top/bottom by iterator
  
  /**
    Returns a non-modifying enumerator of the stack. The elements are enumerated
    from top to bottom.
  */
  inline ReadEnumerator getReadEnumerator() const noexcept
  {
    return ReadEnumerator(elements->getTop());
  }

  Iterator begin() noexcept
  {
    elements.copyOnWrite();
    return Iterator(elements->getTop());
  }

  Iterator end() noexcept
  {
    elements.copyOnWrite();
    return Iterator(nullptr);
  }

  ReadIterator cbegin() const noexcept
  {
    return ReadIterator(elements->getTop());
  }

  ReadIterator cend() const noexcept
  {
    return ReadIterator(nullptr);
  }

  /**
    Removes all the elements from the stack.
  */
  inline void removeAll() noexcept
  {
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
  const Stack<TYPE>& value)
{
  
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

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
