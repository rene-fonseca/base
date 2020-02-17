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
#include <base/mem/Reference.h>
#include <base/string/FormatOutputStream.h>
#include <base/collection/Algorithm.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Linked list data structure. Use of List reduces copying of elements.

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
      Detaches the specified node from this list.

      @param node The node to be detached. Must not be nullptr.
    */
    Node* detachNode(Node* node) noexcept
    {
      BASSERT(node);
      Node* next = node->getNext();
      Node* previous = node->getPrevious();
      if (next) {
        next->setPrevious(previous);
      }
      if (previous) {
        previous->setNext(next);
      }
      --size;
      if (node == first) {
        first = node->getNext();
      }
      if (node == last) {
        last = node->getPrevious();
      }
      node->setNext(nullptr);
      node->setPrevious(nullptr);
      return node;
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

    inline void attachBefore(Node* node, Node* next)
    {
      BASSERT(node && !(node->getNext()) && !(node->getPrevious()));
      if (!next) {
        first = node;
        last = node;
        ++size;
        return;
      }
      auto p = next->getPrevious();
      if (p) {
        p->setNext(node);
      }
      node->setNext(next);
      node->setPrevious(p);
      next->setPrevious(node);
      if (!node->getPrevious()) {
        first = node;
      }
      if (!last) {
        last = node;
      }
      ++size;
    }

    inline void attachAfter(Node* node, Node* previous)
    {
      BASSERT(node && !(node->getNext()) && !(node->getPrevious()));
      if (!previous) {
        first = node;
        last = node;
        ++size;
        return;
      }
      auto n = previous->getNext();
      if (n) {
        n->setPrevious(node);
      }
      node->setPrevious(previous);
      node->setNext(n);
      previous->setNext(node);
      if (!node->getNext()) {
        last = node;
      }
      if (!first) {
        first = node;
      }
      ++size;
    }

    /**
      Inserts the value at the specified node of this list.

      @param node Node specifying the position. Must not be nullptr.
      @param value The value to be inserted.
    */
    Node* insert(Node* node, const TYPE& value)
    {
      // if node is nullptr then we insert at end
      Node* previous = !node ? last : node->getPrevious();
      Node* newNode = new Node(node, previous, value);
      if (node) {
        node->setPrevious(newNode);
      }
      if (previous) {
        previous->setNext(newNode);
      }
      if (!previous) {
        first = newNode;
      }
      if (previous == last) {
        last = newNode;
      }
      ++size;
      return newNode;
    }

    /**
      Inserts the value at the specified node of this list.

      @param node Node specifying the position. Must not be nullptr.
      @param value The value to be inserted.
    */
    Node* insert(Node* node, TYPE&& value)
    {
      // if node is nullptr then we insert at end
      Node* previous = !node ? last : node->getPrevious();
      Node* newNode = CreateDoubleLinkedNode<TYPE, GetDoubleLinkedNodeConstruction<TYPE>::HOW>::createNode(node, previous, moveObject(value));
      if (node) {
        node->setPrevious(newNode);
      }
      if (previous) {
        previous->setNext(newNode);
      }
      if (!previous) {
        first = newNode;
      }
      if (previous == last) {
        last = newNode;
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
      detachNode(node);
      delete node; // could throw
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
      Node* node = CreateDoubleLinkedNode<TYPE, GetDoubleLinkedNodeConstruction<TYPE>::HOW>::createNode(nullptr, last, moveObject(value));
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
      Node* node = CreateDoubleLinkedNode<TYPE, GetDoubleLinkedNodeConstruction<TYPE>::HOW>::createNode(first, nullptr, moveObject(value));
      if (first) { // list is not empty
        first->setPrevious(node);
      } else { // list is empty
        last = node;
      }
      first = node;
      ++size;
    }

    /**
      Removes this first node of this list.
    */
    void removeFirst()
    {
      if (!first) {
        _throw EmptyContainer();
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
        _throw EmptyContainer();
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

      PrimitiveStackArray<Node*> nodes(n);
      {
        auto src = first;
        for (MemorySize i = 0; i < n; ++i) { // fill buffer
          nodes[i] = src;
          src = src->getNext();
        }
        BASSERT(!src);
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
      Moves the node to the front.
    */
    void moveToFront(Node* node)
    {
      if (node != first) {
        attachBefore(detachNode(node), first);
      }
    }

    /**
      Moves the node to the back.
    */
    void moveToBack(Node* node)
    {
      if (node != last) {
        attachAfter(detachNode(node), last);
      }
    }

    void removeAll()
    {
      while (first) {
        removeFirst();
      }
    }

    /**
      Destroys the list.
    */
    inline ~ListImpl()
    {
      removeAll(); // may throw
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

  /** Initializes list from initializer list. */
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

  /**
    Assignment of list by list.
  */
  inline List& operator=(List&& move) noexcept
  {
    elements = moveObject(move.elements);
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
      _throw EmptyContainer();
    }
    return node->getValue();
  }

  /** Returns the first element. */
  const TYPE& getFirst() const
  {
    auto node = getFirstNode();
    if (!node) {
      _throw EmptyContainer();
    }
    return node->getValue();
  }

  /** Returns the last element. */
  TYPE& getLast()
  {
    auto node = getLastNode();
    if (!node) {
      _throw EmptyContainer();
    }
    return node->getValue();
  }

  /** Returns the last element. */
  const TYPE& getLast() const
  {
    auto node = getLastNode();
    if (!node) {
      _throw EmptyContainer();
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

  /** Returns begin iterator. */
  Iterator begin()
  {
    elements.copyOnWrite();
    return Iterator(getFirstNode());
  }

  /** Returns end iterator. */
  Iterator end()
  {
    elements.copyOnWrite();
    return Iterator(nullptr);
  }

  /** Returns begin iterator. */
  ReadIterator begin() const noexcept
  {
    return ReadIterator(getFirstNode());
  }

  /** Returns end iterator. */
  ReadIterator end() const noexcept
  {
    return ReadIterator(nullptr);
  }

  /** Returns begin iterator. */
  ReadIterator cbegin() const noexcept
  {
    return ReadIterator(getFirstNode());
  }

  /** Returns end iterator. */
  ReadIterator cend() const noexcept
  {
    return ReadIterator(nullptr);
  }
  
  inline operator ForwardIterable<Iterator>() noexcept
  {
    return ForwardIterable<Iterator>(begin(), end());
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
    elements->append(moveObject(value));
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
    elements->append(moveObject(value));
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
    elements->prepend(moveObject(value));
  }

  /**
    Inserts the value before the specified position. If iterator is end iterator the value is inserted at the end.

    @param it Insert position.
    @param value The value to be inserted.
  */
  void insert(const Iterator& it, const TYPE& value)
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_COMPATIBLE(it, begin());
    if (elements.isMultiReferenced()) {
      _throw IteratorException("Iterator used for multi-referenced container.");
    }
    // elements.copyOnWrite(); // would invalidate iterator
    auto node = it.getNode();
    elements->insert(node, value);
  }

  /**
    Inserts the value before the specified position. If iterator is end iterator the value is inserted at the end.

    @param it Insert position.
    @param value The value to be inserted.
  */
  void insert(const Iterator& it, TYPE&& value)
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_COMPATIBLE(it, begin());
    if (elements.isMultiReferenced()) {
      _throw IteratorException("Iterator used for multi-referenced container.");
    }
    // elements.copyOnWrite(); // would invalidate iterator
    auto node = it.getNode();
    elements->insert(node, moveObject(value));
  }

  /**
    Removes the node specified by the iterator from this list.

    @param it Iterator specifying the element to be removed.
  */
  void remove(const Iterator& it)
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_COMPATIBLE(it, begin());
    if (elements.isMultiReferenced()) {
      _throw IteratorException("Iterator used for multi-referenced container.");
    }
    // elements.copyOnWrite(); // would invalidate iterator
    auto node = it.getNode();
    if (!node) {
      _throw IteratorException();
    }
    elements->remove(node);
  }

  /**
    Moves the value at the specified position to the front.
  */
  void moveToFront(const Iterator& it)
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_COMPATIBLE(it, begin());
    if (elements.isMultiReferenced()) {
      _throw IteratorException("Iterator used for multi-referenced container.");
    }
    // elements.copyOnWrite(); // would invalidate iterator
    auto node = it.getNode();
    if (!node) {
      _throw IteratorException();
    }
    elements->moveToFront(node);
  }

  /**
    Moves the value at the specified position to the back.
  */
  void moveToBack(const Iterator& it)
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_COMPATIBLE(it, begin());
    if (elements.isMultiReferenced()) {
      _throw IteratorException("Iterator used for multi-referenced container.");
    }
    // elements.copyOnWrite(); // would invalidate iterator
    auto node = it.getNode();
    if (!node) {
      _throw IteratorException();
    }
    elements->moveToBack(node);
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
  void sort()
  {
    ComputeTask profiler("List::sort()");
    elements.copyOnWrite();
    // TAG: swap nodes instead - at least when TYPE doesnt allow move
    bubbleSort(begin(), end());
  }

  /** Sorts the array. */
  template<class PREDICATE>
  void sort(PREDICATE predicate)
  {
    ComputeTask profiler("List::sort()");
    elements.copyOnWrite();
    // TAG: swap nodes instead - at least when TYPE doesnt allow move
    bubbleSort(begin(), end(), predicate);
  }

  /**
    Shuffles elements. See https ://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle#Modern_method.

    Uses temporary buffer but avoid copy construction of values.
  */
  void shuffle();

  /** Returns true if equal. */
  bool operator==(const List& compare) const
  {
    if (getSize() != compare.getSize()) {
      return false;
    }
    auto a = cbegin();
    const auto aEnd = cend();
    auto b = compare.cbegin();
    for (; a != aEnd; ++a, ++b) {
      if (!(*a == *b)) {
        return false;
      }
    }
    return true;
  }

  /** Returns true if not equal. */
  inline bool operator!=(const List& compare) const
  {
    return !operator==(compare);
  }

  /** Returns true if less than. */
  bool operator<(const List& compare) const
  {
    auto a = cbegin();
    auto b = compare.cbegin();
    const auto aEnd = cend();
    const auto bEnd = compare.cend();
    if (getSize() <= compare.getSize()) {
      for (; a != aEnd; ++a, ++b) {
        if (*a < *b) {
          return true;
        } else if (*a == *b) {
          continue;
        }
        return false;
      }
      return b != bEnd;
    } else {
      for (; b != bEnd; ++a, ++b) {
        if (*a < *b) {
          return true;
        } else if (*a == *b) {
          continue;
        }
        return false;
      }
      return false;
    }
  }
  
  /** Returns true if greater than or equal. */
  inline bool operator>=(const List& compare) const
  {
    return !operator<(compare);
  }

  /** Returns true if not empty. */
  inline operator bool() const noexcept
  {
    return !isEmpty();
  }
  
  /** Appends value. */
  inline List& operator<<(const TYPE& value)
  {
    append(value);
    return *this;
  }

  /** Appends value. */
  inline List& operator<<(TYPE&& value)
  {
    append(moveObject(value));
    return *this;
  }
};

/**
  Writes a string representation of a list to a format stream.

  @relates List
*/
template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const List<TYPE>& value)
{
  typename List<TYPE>::ReadEnumerator enu = value.getReadEnumerator();
  stream << '[';
  while (enu.hasNext()) {
    stream << enu.next();
    if (enu.hasNext()) {
      stream << ';';
    }
  }
  stream << ']';
  return stream;
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

_COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_CONTAINER(List)

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
