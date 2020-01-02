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
#include <base/collection/SingleLinkedNode.h>
#include <base/collection/InvalidNode.h>
#include <base/MemoryException.h>
#include <base/mem/Reference.h>
#include <base/concurrency/ExclusiveSynchronize.h>
#include <base/concurrency/SharedSynchronize.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Queue implemented using a unidirectional linked list. The first value pushed
  onto the queue is also the first to be pop'ed.

  @short Queue.
  @ingroup collections
  @version 1.0
*/

template<class TYPE, class LOCK = Unsafe>
class Queue : public Collection, public Synchronizeable<LOCK> {
public:

  /** The type of a value. */
  typedef TYPE Value;
  /** The type of the guard. */
  typedef LOCK Guard;
  /** The type of a node. */
  typedef SingleLinkedNode<Value> Node;
protected:

  /*
    Internal queue implementation.
    
    @short Queue implementation.
    @version 1.0
  */
  class QueueImpl : public ReferenceCountedObject {
  private:

    /** The first node in the queue. */
    Node* first = nullptr;
    /** The last node in the queue. */
    Node* last = nullptr;
    /** The number of elements in the queue. */
    MemorySize size = 0;
  public:

    /**
      Initializes an empty queue.
    */
    QueueImpl() noexcept
    {
    }

    /**
      Initializes queue from other queue.
    */
    QueueImpl(const QueueImpl& copy)
    {
      const Node* node = copy.first;
      while (node) {
        push(*node->getValue());
        node = node->getNext();
      }
    }

    /**
      Initializes queue from other queue.
    */
    QueueImpl(QueueImpl&& move) noexcept
      : first(moveObject(move.first)),
        last(moveObject(move.last)),
        size(moveObject(move.size))
    {
    }

    /**
      Returns the number of elements of the queue.
    */
    inline MemorySize getSize() const noexcept
    {
      return size;
    }

    /**
      Returns true if the queue is empty.
    */
    inline bool isEmpty() const noexcept
    {
      return !size;
    }

    /**
      Adds the value to the end of the queue.
    */
    void push(const TYPE& value)
    {
      Node* node = new Node(nullptr, value);
      if (!size) { // is queue empty
        first = node;
      } else { // queue is not empty
        last->setNext(node);
      }
      last = node;
      ++size;
    }
    
    /**
      Removes the value at the front of the queue.
    */
    Value pop()
    {
      if (!size) {
        _throw InvalidNode("Queue is empty.", this);
      }
      Node* temp = first;
      first = first->getNext();
      --size;
      if (!size) { // is queue empty
        last = 0;
      }
      Value result = *temp->getValue();
      delete temp;
      return result;
    }

    /**
      Destroys the queue.
    */
    ~QueueImpl()
    {
      while (first) {
        Node* temp = first;
        first = first->getNext();
        delete temp; // could throw
      }
    }
  };

  /**
    The elements of the queue.
  */
  Reference<QueueImpl> elements;
public:

  /**
    Initializes an empty queue.
  */
  Queue()
    : elements(new QueueImpl())
  {
  }

  /**
    Pushes all the values in left-to-right order.
  */
  Queue(std::initializer_list<TYPE> values)
    : elements(new QueueImpl())
  {
    for (const auto& value : values) {
      push(value);
    }
  }
  
  /**
    Initializes queue from other queue.
  */
  inline Queue(const Queue& copy) noexcept
    : elements(copy.elements)
  {
  }

  /**
    Initializes queue from other queue.
  */
  inline Queue(Queue&& move) noexcept
    : elements(moveObject(move.elements))
  {
  }

  /**
    Assigns queue from other queue.
  */
  inline Queue& operator=(const Queue& copy) noexcept
  {
    elements = copy.elements;
    return *this;
  }

  /**
    Returns the number of elements in the queue.
  */
  inline MemorySize getSize() const noexcept
  {
    SharedSynchronize<Guard>(*this);
    return elements->getSize();
  }

  /**
    Returns true if the queue is empty.
  */
  inline bool isEmpty() const noexcept
  {
    SharedSynchronize<Guard>(*this);
    return elements->isEmpty();
  }

  /**
    Adds the specified value to the end of the queue.

    @param value The value to be added to the queue.
  */
  void push(const TYPE& value)
  {
    ExclusiveSynchronize<Guard>(*this);
    elements.copyOnWrite();
    elements->push(value);
  }
  
  /**
    Removes the element at the front of the queue. Raises InvalidNode if the
    queue is empty.
  */
  Value pop()
  {
    ExclusiveSynchronize<Guard>(*this);
    elements.copyOnWrite();
    return elements->pop();
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
