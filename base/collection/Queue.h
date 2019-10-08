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

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

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
    unsigned int size = 0;
  public:

    /**
      Initializes an empty queue.
    */
    QueueImpl() throw() {
    }

    /**
      Initializes queue from other queue.
    */
    QueueImpl(const QueueImpl& copy) throw() {
      const Node* node = copy.first;
      while (node) {
        push(*node->getValue());
        node = node->getNext();
      }
    }

    /**
      Returns the number of elements of the queue.
    */
    inline unsigned int getSize() const throw() {
      return size;
    }

    /**
      Returns true if the queue is empty.
    */
    inline bool isEmpty() const throw() {
      return !size;
    }

    /**
      Adds the value to the end of the queue.
    */
    void push(const TYPE& value) throw(MemoryException) {
      Node* node = new Node(0, value);
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
    Value pop() throw(InvalidNode) {
      if (!size) {
        throw InvalidNode("Queue is empty", this);
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
    ~QueueImpl() throw() {
      while (first) {
        Node* temp = first;
        first = first->getNext();
        delete temp;
      }
    }
  };

  /**
    The elements of the queue.
  */
  Reference<QueueImpl > elements;
public:

  /**
    Initializes an empty queue.
  */
  Queue() throw() : elements(new QueueImpl()) {
  }

  /**
    Initializes queue from other queue.
  */
  inline Queue(const Queue& copy) throw() : elements(copy.elements) {
  }

  /**
    Returns the number of elements in the queue.
  */
  inline unsigned int getSize() const throw() {
    SharedSynchronize<Guard>(*this);
    return elements->getSize();
  }

  /**
    Returns true if the queue is empty.
  */
  inline bool isEmpty() const throw() {
    SharedSynchronize<Guard>(*this);
    return elements->isEmpty();
  }

  /**
    Adds the specified value to the end of the queue.

    @param value The value to be added to the queue.
  */
  void push(const TYPE& value) throw(MemoryException) {
    ExclusiveSynchronize<Guard>(*this);
    elements.copyOnWrite();
    elements->push(value);
  }
  
  /**
    Removes the element at the front of the queue. Raises InvalidNode if the
    queue is empty.
  */
  Value pop() throw(InvalidNode) {
    ExclusiveSynchronize<Guard>(*this);
    elements.copyOnWrite();
    return elements->pop();
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
