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
#include <base/collection/InvalidEnumeration.h>
#include <base/collection/InvalidNode.h>
#include <base/collection/EmptyContainer.h>
#include <base/collection/Array.h>
#include <base/mem/Reference.h>
#include <base/string/FormatOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Linked list data structure.

  You can enumerate the elements of a list like this (myMember is a member
  function of MyClass):
  @code
    List<MyClass> myList;
    List<MyClass>::Enumeration enu(myList);
    while (enu.hasNext()) {
      enu.next()->myMember();
    }
  @endcode

  @short List collection.
  @ingroup collections
*/

template<class TYPE>
class List : public Collection {
public:

  /** The type of a value. */
  typedef TYPE Value;
  /** The type of a node. */
  typedef DoubleLinkedNode<Value> Node;

  typedef DoubleLinkedNodeIterator<TYPE> Iterator;
  typedef DoubleLinkedNodeReadIterator<TYPE> ReadIterator;
  typedef DoubleLinkedNodeEnumerator<EnumeratorTraits<TYPE> > Enumerator;
  typedef DoubleLinkedNodeReadEnumerator<ReadEnumeratorTraits<TYPE> > ReadEnumerator;
protected:

  /*
    Internal list implementation.
  */
  class ListImpl : public ReferenceCountedObject {
  private:

    /** The first node in the list. */
    Node* first = nullptr;
    /** The last node in the list. */
    Node* last = nullptr;
    /** The number of elements in the list. */
    MemorySize size = 0;
  protected:

    /**
      Inserts the value at the specified node of this list.

      @param node Node specifying the position. Must not be nullptr.
      @param value The value to be inserted.
    */
    Node* insert(Node* node, const TYPE& value)
    {
      Node* previous = node->getPrevious();
      Node* newNode = new Node(node, previous, value);
      node->setPrevious(newNode);
      if (previous) {
        previous->setNext(newNode);
      }
      ++size;
      return newNode;
    }

    /**
      Removes the specified node from this list.

      @param node The node to be removed. Must not be nullptr.
    */
    void remove(Node* node)
    {
      Node* next = node->getNext();
      Node* previous = node->getPrevious();
      if (next) {
        next->setPrevious(previous);
      }
      if (previous) {
        previous->setNext(next);
      }
      --size;
      delete node; // could throw
    }
  public:

    /**
      Initializes an empty list.
    */
    inline ListImpl() noexcept
    {
    }
    
    /**
      Initializes list from other list.
    */
    ListImpl(const ListImpl& copy)
    {
      const Node* node = copy.getFirst();
      while (node) {
        append(node->getValue());
        node = node->getNext();
      }
    }

    ListImpl(ListImpl&& move) noexcept
    {
      if (this != &move) {
        first = move.first;
        move.first = nullptr;
        last = move.last;
        move.last = nullptr;
        size = move.size;
        move.size = 0;
      }
    }

    /**
      Returns the number of elements of the list.
    */
    inline MemorySize getSize() const noexcept
    {
      return size;
    }

    /**
      Returns the first node of the list.
    */
    inline Node* getFirst() noexcept
    {
      return first;
    }

    /**
      Returns the first node of the list.
    */
    inline const Node* getFirst() const noexcept
    {
      return first;
    }

    /**
      Returns the last node of the list.
    */
    inline Node* getLast() noexcept
    {
      return last;
    }

    /**
      Returns the last node of the list.
    */
    inline const Node* getLast() const noexcept
    {
      return last;
    }

    /**
      Appends the value to the end of this list.
    */
    void append(const TYPE& value)
    {
      Node* node = new Node(nullptr, last, value);
      if (last) { // list is not empty
        last->setNext(node);
      } else { // list is empty
        first = node;
      }
      last = node;
      ++size;
    }

    /**
      Appends the value to the end of this list.
    */
    void append(TYPE&& value)
    {
      Node* node = CreateDoubleLinkedNode<TYPE, GetDoubleLinkedNodeConstruction<TYPE>::HOW>::createNode(nullptr, last, std::move(value));
      if (last) { // list is not empty
        last->setNext(node);
      } else { // list is empty
        first = node;
      }
      last = node;
      ++size;
    }

    /**
      Prepends the value to the beginning of this list.
    */
    void prepend(const TYPE& value)
    {
      Node* node = new Node(first, nullptr, value);
      if (first) { // list is not empty
        first->setPrevious(node);
      } else { // list is empty
        last = node;
      }
      first = node;
      ++size;
    }

    /**
      Prepends the value to the beginning of this list.
    */
    void prepend(TYPE&& value)
    {
      Node* node = CreateDoubleLinkedNode<TYPE, GetDoubleLinkedNodeConstruction<TYPE>::HOW>::createNode(first, nullptr, std::move(value));
      if (first) { // list is not empty
        first->setPrevious(node);
      } else { // list is empty
        last = node;
      }
      first = node;
      ++size;
    }

    /**
      Inserts the value at the specified position.

      @param enu Enumeration of this list specifying the position.
      @param value The value to be inserted.
    */
/*    void insert(Enumeration& enu, const TYPE& value) throw(InvalidEnumeration) {
      if (enu.getOwner() != this) {
        throw InvalidEnumeration();
      }
  //    insert(enu.node, value);
    }*/

    /**
      Removes this first node of this list.
    */
    void removeFirst()
    {
      if (!first) {
        throw EmptyContainer();
      }
      Node* node = first;
      first = first->getNext();
      if (!first) {
        last = nullptr;
      }
      remove(node);
    }

    /**
      Removes the last node of this list.
    */
    void removeLast()
    {
      if (!last) {
        throw EmptyContainer();
      }
      Node* node = last;
      last = last->getPrevious();
      if (!last) {
        first = nullptr;
      }
      remove(node);
    }

    void shuffle()
    {
      MemorySize n = size;
      if (n <= 1) {
        return; // nothing to do
      }

      PrimitiveArray<Node*> nodes(n);
      {
        auto src = first;
        for (MemorySize i = 0; i < n; ++i) { // fill buffer
          nodes[i] = src;
          src = src->getNext();
        }
        ASSERT(!src);
      }
      base::shuffle(&nodes[0], &nodes[0] + nodes.size());

      // rebuild node list
      {
        auto src = nodes.begin();
        auto node = *src++;
        node->setPrevious(nullptr);
        node->setNext(nullptr);
        first = node;
        last = node;

        const auto end = nodes.end();
        while (src != end) {
          node = *src++;
          node->setPrevious(last);
          node->setNext(nullptr);
          last->setNext(node);
          last = node;
        }
      }
    }

    /**
      Destroys the list.
    */
    ~ListImpl()
    {
      while (first) {
        Node* node = first;
        first = first->getNext();
        delete node;
      }
    }
  };

  /**
    The elements of the list.
  */
  Reference<ListImpl> elements;

  /**
    Returns the first node of the list.
  */
  inline Node* getFirstNode()
  {
    elements.copyOnWrite();
    return elements->getFirst();
  }

  /**
    Returns the first node of the list.
  */
  inline const Node* getFirstNode() const noexcept
  {
    return elements->getFirst();
  }

  /**
    Returns the last node of the list.
  */
  inline Node* getLastNode()
  {
    elements.copyOnWrite();
    return elements->getLast();
  }

  /**
    Returns the last node of the list.
  */
  inline const Node* getLastNode() const noexcept
  {
    return elements->getLast();
  }
public:

  /**
    Initializes an empty list.
  */
  List()
    : elements(new ListImpl())
  {
  }

  List(std::initializer_list<TYPE> values)
    : elements(new ListImpl())
  {
    for (const auto& value : values) {
      append(value);
    }
  }
  
  /**
    Initializes list from other list.
  */
  inline List(const List& copy) noexcept
    : elements(copy.elements)
  {
  }
  
  /**
    Assignment of list by list.
  */
  inline List& operator=(const List& copy) noexcept
  {
    elements = copy.elements;
    return *this;
  }
  
  // Node read-only

  /**
    Returns the number of elements in the list.
  */
  inline MemorySize getSize() const noexcept
  {
    return elements->getSize();
  }

  /**
    Returns true if the list is empty.
  */
  inline bool isEmpty() const noexcept
  {
    return elements->getSize() == 0;
  }

  /** Returns the first element. */
  TYPE& getFirst()
  {
    auto node = getFirstNode();
    if (!node) {
      throw EmptyContainer();
    }
    return node->getValue();
  }

  /** Returns the first element. */
  const TYPE& getFirst() const
  {
    auto node = getFirstNode();
    if (!node) {
      throw EmptyContainer();
    }
    return node->getValue();
  }

  /** Returns the last element. */
  TYPE& getLast()
  {
    auto node = getLastNode();
    if (!node) {
      throw EmptyContainer();
    }
    return node->getValue();
  }

  /** Returns the last element. */
  const TYPE& getLast() const
  {
    auto node = getLastNode();
    if (!node) {
      throw EmptyContainer();
    }
    return node->getValue();
  }

  /**
    Returns a modifying enumerator of the list.
  */
  Enumerator getEnumerator()
  {
    elements.copyOnWrite();
    return Enumerator(getFirstNode());
  }

  /**
    Returns a non-modifying enumerator of the list.
  */
  inline ReadEnumerator getReadEnumerator() const noexcept
  {
    return ReadEnumerator(getFirstNode());
  }

  Iterator begin()
  {
    elements.copyOnWrite();
    return Iterator(getFirstNode());
  }

  Iterator end()
  {
    elements.copyOnWrite();
    return Iterator(nullptr);
  }

  ReadIterator begin() const noexcept
  {
    return ReadIterator(getFirstNode());
  }

  ReadIterator end() const noexcept
  {
    return ReadIterator(nullptr);
  }

  ReadIterator cbegin() const noexcept
  {
    return ReadIterator(getFirstNode());
  }

  ReadIterator cend() const noexcept
  {
    return ReadIterator(nullptr);
  }

  // Node manip

  /**
    Appends the specified value to the end of this list.

    @param value The value to be appended to the list.
  */
  void append(const TYPE& value)
  {
    elements.copyOnWrite();
    elements->append(value);
  }

  /**
    Appends the specified value to the end of this list.

    @param value The value to be appended to the list.
  */
  void append(TYPE&& value)
  {
    elements.copyOnWrite();
    elements->append(std::move(value));
  }

  /**
    Appends the specified value to the end of this list.

    @param value The value to be appended to the list.
  */
  void add(const TYPE& value) // alias to allow switch between container types
  {
    elements.copyOnWrite();
    elements->append(value);
  }

  /**
    Appends the specified value to the end of this list.

    @param value The value to be appended to the list.
  */
  void add(TYPE&& value) // alias to allow switch between container types
  {
    elements.copyOnWrite();
    elements->append(std::move(value));
  }

  /**
    Prepends the value to the beginning of this list.

    @param value The value to be prepended to the list.
  */
  void prepend(const TYPE& value)
  {
    elements.copyOnWrite();
    elements->prepend(value);
  }

  /**
    Prepends the value to the beginning of this list.

    @param value The value to be prepended to the list.
  */
  void prepend(TYPE&& value)
  {
    elements.copyOnWrite();
    elements->prepend(std::move(value));
  }

  /**
    Inserts the value at the specified position.

    @param enu Enumeration of this list specifying the position.
    @param value The value to be inserted.
  */
  void insert(const Iterator& it, const TYPE& value)
  {
    elements.copyOnWrite();
#if 0
    if (it.getOwner() != this) {
      throw InvalidIterator();
    }
#endif
    // TAG: allow empty also - nullptr
    Node* node = it.getNode();
    // TAG: elements->insert(node, value);
  }

  /**
    Removes this first node of this list.
  */
  void removeFirst()
  {
    elements.copyOnWrite();
    elements->removeFirst();
  }

  /**
    Removes the last node of this list.
  */
  void removeLast()
  {
    elements.copyOnWrite();
    elements->removeLast();
  }

  /**
    Removes all the elements from this list.
  */
  void removeAll()
  {
    elements = new ListImpl(); // copyOnWrite is not required
  }

  /**
    Sorts the elements of this list. Requires operator<= for TYPE.
  */
  void sort();
  
  /**
    Shuffles elements. See https ://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle#Modern_method.

    Uses temporary buffer but avoid copy construction of values.
  */
  void shuffle();

  /**
    Removes the node specified by the iterator from this list.

    @param it Iterator specifying the element to be removed.
  */
  void remove(const Iterator& it)
  {
#if 0
    if (it.getOwner() != this) {
      throw InvalidIterator();
    }
#endif
    elements.copyOnWrite();
#if 0
    Node* node = nullptr; //enu.node;
    enu.next(); // raises exception if end has been reached
    remove(node);
#endif
  }

  /** Returns true if not empty. */
  inline operator bool() const noexcept
  {
    return !isEmpty();
  }
};

/**
  Writes a string representation of a list to a format stream.

  @relates List
*/
template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const List<TYPE>& value) throw(IOException)
{
  typename List<TYPE>::ReadEnumerator enu = value.getReadEnumerator();
  stream << '[';
  while (enu.hasNext()) {
    stream << *enu.next();
    if (enu.hasNext()) {
      stream << ';';
    }
  }
  stream << ']';
  return stream;
}

/** Bubble sort. O(n^2). Only forward iterator required. operator<= used for comparison of values. */
template<class TYPE>
void bubbleSort(const TYPE& _begin, const TYPE& _end)
{
  // implementation with forward iterator only - bidirectional iterator not required
  // TAG: we could do a merge sort also - by splitting items in the middle and sorting independently

  // static_assert(std::is_same<ForwardIterator, typename TYPE::Category>::value, "Iterator must be ForwardIterator.");
  const ForwardIterator* ensureForwardIterator = static_cast<const typename TYPE::Category*>(nullptr);

  TYPE end = _end; // we lower end per loop
  while (true) { // loop until all items sorted
    TYPE current = _begin; // restart
    if (current == end) { // done - no more items to sort
      break;
    }

    // move max to end
    bool swapped = false;
    TYPE next = current;
    ++next;
    while (next != end) {
      if (!(*current <= *next)) { // dont swap on = but < is the primary comparison operator for algorithms
        swapper(*current, *next);
        swapped = true;
      }
      ++current;
      ++next;
    }
    if (!swapped) {
      return; // nothing to do
    }
    end = current; // max now at end so no need to look at this again
  }
}

template<class TYPE>
void List<TYPE>::sort()
{
  elements.copyOnWrite();
  bubbleSort(begin(), end());
}

template<class TYPE>
void List<TYPE>::shuffle()
{
  MemorySize n = getSize();
  if (n <= 1) {
    return; // nothing to do
  }
  elements.copyOnWrite();
  elements->shuffle();
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
