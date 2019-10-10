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

#include <base/collection/OrderedBinaryTree.h>
#include <base/collection/Association.h>
#include <base/collection/Queue.h>
#include <base/concurrency/Synchronize.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Priority queue. As for the Queue collection the first value pushed onto the
  queue is also the first to be pop'ed. However, values associated with higher
  priority get to the top before values of less priority.

  @short Priority queue collection.
  @ingroup collections
  @see Queue
  @version 1.0
*/

template<class VALUE, class PRIORITY = int, class LOCK = Unsafe>
class PriorityQueue : public Collection, public Synchronizeable<LOCK> {
public:

  /** The type of a value of the queue. */
  typedef VALUE Value;
  /** The type of the priority. */
  typedef PRIORITY Priority;
  /** The type of an association used internally by the priority queue. */
  typedef Association<Priority, Queue<Value> > Node;
protected:

  /** The elements of the priority queue. */
  OrderedBinaryTree<Node> elements;
  /** The number of elements in the priority queue. */
  unsigned int size = 0;
public:

  /**
    Initializes an empty priority queue.
  */
  PriorityQueue() throw() {
  }

  /**
    Initializes a priority queue from other priority queue.
  */
  PriorityQueue(const PriorityQueue& copy) throw(MemoryException)
    : elements(copy.elements) {
  }

  /**
    Returns the number of elements in the priority queue.
  */
  inline unsigned int getSize() const throw() {
    return size;
  }

  /**
    Returns true if the priority queue is empty.
  */
  inline bool isEmpty() const throw() {
    return !size;
  }

  /**
    Pushes the specified value onto the priority queue.

    @param priority The priority of the value.
    @param value The value to be added to the queue.
  */
  void push(const Priority& priority, const Value& value) throw(MemoryException) {
    auto node = elements.find(Node(priority));
    if (node) { // does the priority already exist in the tree
      Queue<Value>& queue = node->getValue()->getValue();
      queue.push(value);
    } else {
      Queue<Value> queue;
      queue.push(value);
      elements.add(Node(priority, queue));
    }
    ++size; // always increment 'cause we always push a value onto some queue
  }

  /**
    Removes the element at the front of the priority queue. Raises
    InvalidNode if the priority queue is empty.
  */
  Value pop() throw(InvalidNode) {
    Value result;
    bassert(size, InvalidNode("Priority queue is empty", this));

    auto node = elements.getLast();
    bool removeQueue = false;
    {
      Queue<Value>& queue = node->getValue()->getValue();
      result = queue.pop(); // queue is never empty
      removeQueue = queue.isEmpty();
    }
    if (removeQueue) {
      elements.remove(node); // remove node from binary tree
    }
    --size; // always decrement 'cause we always pop a value from some queue
    return result;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
