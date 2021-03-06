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
  MemorySize size = 0;
public:

  /**
    Initializes an empty priority queue.
  */
  PriorityQueue() noexcept
  {
  }

  /** Initializes list from initializer list. */
  PriorityQueue(std::initializer_list<Pair<Priority, Value> > values)
  {
    for (const auto& value : values) {
      push(value.getFirst(), value.getSecond());
    }
  }

  /**
    Initializes a priority queue from other priority queue.
  */
  PriorityQueue(const PriorityQueue& copy) noexcept
    : elements(copy.elements)
  {
  }

  /**
    Initializes a priority queue from other priority queue.
  */
  PriorityQueue(PriorityQueue&& move) noexcept
    : elements(moveObject(move.elements)),
      size(moveObject(move.size))
  {
    move.size = 0;
  }

  /** Assign container. */
  PriorityQueue& operator=(const PriorityQueue& assign)
  {
    elements = assign.elements;
    size = assign.size;
    return *this;
  }

  /** Assign container. */
  PriorityQueue& operator=(PriorityQueue&& assign)
  {
    elements = moveObject(assign.elements);
    size = assign.size;
    assign.size = 0;
    return *this;
  }

  /**
    Returns the number of elements in the priority queue.
  */
  inline MemorySize getSize() const noexcept
  {
    return size;
  }

  /**
    Returns true if the priority queue is empty.
  */
  inline bool isEmpty() const noexcept
  {
    return !size;
  }

  /**
    Pushes the specified value onto the priority queue.

    @param priority The priority of the value.
    @param value The value to be added to the queue.
  */
  void push(const Priority& priority, const Value& value)
  {
    auto node = elements.find(Node(priority));
    if (node) { // does the priority already exist in the tree
      auto& nodeValue = node->getValue();
      Queue<Value>& queue = nodeValue.getValue();
      queue.push(value);
    } else {
      Queue<Value> queue;
      queue.push(value);
      elements.add(Node(priority, queue));
    }
    ++size; // always increment 'cause we always push a value onto some queue
  }
  
  /**
    Pushes the specified value onto the priority queue.

    @param priority The priority of the value.
    @param value The value to be added to the queue.
  */
  void push(const Priority& priority, Value&& value)
  {
    auto node = elements.find(Node(priority));
    if (node) { // does the priority already exist in the tree
      auto& nodeValue = node->getValue();
      Queue<Value>& queue = nodeValue.getValue();
      queue.push(moveObject(value));
    } else {
      Queue<Value> queue;
      queue.push(moveObject(value));
      elements.add(Node(priority, queue));
    }
    ++size; // always increment 'cause we always push a value onto some queue
  }

  /**
    Removes the element at the front of the priority queue. Raises
    InvalidNode if the priority queue is empty.
  */
  Value pop()
  {
    Value result;
    bassert(size, InvalidNode("Priority queue is empty.", this));

    auto node = elements.getLast();
    bool removeQueue = false;
    {
      auto& nodeValue = node->getValue();
      Queue<Value>& queue = nodeValue.getValue();
      result = queue.pop(); // queue is never empty
      removeQueue = queue.isEmpty();
    }
    if (removeQueue) {
      elements.remove(node); // remove node from binary tree
    }
    --size; // always decrement 'cause we always pop a value from some queue
    return result;
  }
  
  /**
    Removes all the keys from this set.
  */
  void removeAll() noexcept
  {
    elements.removeAll();
    size = 0;
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
