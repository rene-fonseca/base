/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__QUEUE_H
#define _DK_SDU_MIP__BASE_COLLECTION__QUEUE_H

#include <base/collection/Collection.h>
#include <base/collection/SingleLinkedNode.h>
#include <base/collection/InvalidNode.h>
#include <base/MemoryException.h>
#include <base/mem/ReferenceCountedObjectPointer.h>
#include <base/mem/ReferenceCountedObject.h>
#include <base/concurrency/Synchronize.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Queue implemented using a unidirectional linked list. The first value pushed
  onto the queue is also the first to be pop'ed.

  @ingroup collections
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.0
*/

template<class TYPE, class LOCK = Unsafe>
class Queue : public Collection, public Synchronizeable<LOCK> {
public:

  /** The type of a value. */
  typedef TYPE Value;
  /** The type of a node. */
  typedef SingleLinkedNode<Value> Node;
protected:

  /**
    Internal queue implementation.

    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class QueueImpl : public ReferenceCountedObject {
  private:

    /** The first node in the queue. */
    Node* first;
    /** The last node in the queue. */
    Node* last;
    /** The number of elements in the queue. */
    unsigned int size;
  public:

    /**
      Initializes an empty queue.
    */
    QueueImpl() throw() : first(0), last(0), size(0) {}

    /**
      Initializes queue from other queue.
    */
    QueueImpl(const QueueImpl& copy) throw() : first(0), last(0), size(0) {
      const Node* node = copy.first;
      while (node) {
        push(*node->getValue());
        node = node->getNext();
      }
    }

    /**
      Returns the number of elements of the queue.
    */
    inline unsigned int getSize() const throw() {return size;}

    /**
      Returns true if the queue is empty.
    */
    inline bool isEmpty() const throw() {return !size;}

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
        throw InvalidNode("Queue is empty");
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
  ReferenceCountedObjectPointer<QueueImpl > elements;
public:

  /**
    Initializes an empty queue.
  */
  Queue() throw() : elements(new QueueImpl()) {}

  /**
    Initializes queue from other queue.
  */
  Queue(const Queue& copy) throw(MemoryException) : elements(copy.elements) {}

  /**
    Returns the number of elements in the queue.
  */
  inline unsigned int getSize() const throw() {
    SynchronizeShared();
    return elements->getSize();
  }

  /**
    Returns true if the queue is empty.
  */
  inline bool isEmpty() const throw() {
    SynchronizeShared();
    return elements->isEmpty();
  }

  /**
    Adds the specified value to the end of the queue.

    @param value The value to be added to the queue.
  */
  void push(const TYPE& value) throw(MemoryException) {
    SynchronizeExclusively();
    elements.copyOnWrite();
    elements->push(value);
  }

  /**
    Removes the element at the front of the queue. Throws 'InvalidNode' if the
    queue is empty.
  */
  Value pop() throw(InvalidNode) {
    SynchronizeExclusively();
    elements.copyOnWrite();
    return elements->pop();
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
