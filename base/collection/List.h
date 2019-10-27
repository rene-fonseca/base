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
#include <base/collection/Enumeration.h>
#include <base/Iterator.h>
#include <base/collection/IteratorException.h>
#include <base/collection/InvalidEnumeration.h>
#include <base/collection/InvalidNode.h>
#include <base/collection/EmptyContainer.h>
#include <base/collection/Array.h>
#include <base/mem/Reference.h>
#include <base/string/FormatOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  A node in a list.

  @short A node in a list.
  @ingroup collections
*/

template<class TYPE>
class ListNode {
protected:

  /** The next node in the list. */
  ListNode* next = nullptr;
  /** The previous node in the list. */
  ListNode* previous = nullptr;
  /** The value associated with the node. */
  TYPE value;
public:

  inline ListNode(ListNode* _next, ListNode* _previous, const TYPE& _value)
    : next(_next),
      previous(_previous),
      value(_value)
  {
  }
  
  inline ListNode* getNext() const noexcept
  {
    return next;
  }
  
  inline void setNext(ListNode* next) noexcept
  {
    this->next = next;
  }
  
  inline ListNode* getPrevious() const noexcept
  {
    return previous;
  }

  inline void setPrevious(ListNode* previous) noexcept
  {
    this->previous = previous;
  }

  inline TYPE* getValue() noexcept
  {
    return &value;
  }

  inline const TYPE* getValue() const noexcept
  {
    return &value;
  }

  inline void setValue(const TYPE& value) noexcept
  {
    this->value = value;
  }
};



class CollectionOwnerContext : public ReferenceCountedObject {
  friend class IteratorOwnerContext;
private:

  MemorySize iteratorModificationId = 0;
public:

  inline void onBreakIterators() noexcept
  {
    ++iteratorModificationId;
  }

  inline ~CollectionOwnerContext()
  {
    iteratorModificationId = -1; // mark destructed - doesnt happen in the normal case since iterator will have handle to owner - this can be bad through since execution order can change! weak reference would be desired
  }
};

class IteratorOwnerContext {
private:

  Reference<CollectionOwnerContext> owner; // weak reference is preferred
  MemorySize modificationId = 0; // modification id when iterator got constructed
public:

  void setOwner(const Reference<CollectionOwnerContext>& _owner, MemorySize _modificationId) noexcept
  {
    owner = _owner;
    modificationId = _modificationId;
  }

  void ensureUnmodified() const
  {
    if (owner && (modificationId != owner->iteratorModificationId)) {
      throw IteratorException("Iterator used after owner modification.");
    }
  }

  static void ensureCompatible(const IteratorOwnerContext& a, const IteratorOwnerContext& b)
  {
    a.ensureUnmodified();
    b.ensureUnmodified();
    // nullptr allowed
    if (a.owner != b.owner) {
      throw IteratorException("Comparison of iterators belonging to different owners.");
    }
  }

  template<class TYPE>
  static void ensureCompatibleIterators(const TYPE& a, const TYPE& b)
  {
    ensureCompatible(a.ownerContext, b.ownerContext);
  }
};

#if defined(_COM_AZURE_DEV__BASE__DEBUG)
#  define _COM_AZURE_DEV__BASE__PROTECT_ITERATORS
#endif

#if defined(_COM_AZURE_DEV__BASE__PROTECT_ITERATORS)
#  define _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED() ownerContext.ensureUnmodified()
#  define _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_COMPATIBLE(A, B) IteratorOwnerContext::ensureCompatibleIterators(A, B)
#  define _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_ADD_CONTEXT() \
  friend class IteratorOwnerContext; \
  IteratorOwnerContext ownerContext
#else
#  define _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED() if (true) {}
#  define _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_COMPATIBLE(A, B) if (true) {}
#  define _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_ADD_CONTEXT()
#endif



template<class TYPE>
class ListIterator {
public:

  typedef TYPE Value;
  typedef TYPE* Pointer;
  typedef TYPE& Reference;
  typedef MemoryDiff Distance;
  typedef BidirectionalIterator Category;
protected:

  ListNode<TYPE>* node = nullptr;
  _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_ADD_CONTEXT(); // TAG: ListNode can handle some modifications
public:

  /**
    Initializes iterator.

    @param value The initial value of the iterator.
  */
  explicit inline ListIterator(ListNode<TYPE>* _node) noexcept
    : node(_node)
  {
  }

  /**
    Initializes iterator from other iterator.
  */
  inline ListIterator(const ListIterator& copy) noexcept
    : node(copy.node)
  {
  }

  /**
    Initializes iterator from other iterator.
  */
  template<class POLY>
  inline ListIterator(const ListIterator<POLY>& copy) noexcept
    : node(copy.getValue())
  {
  }

  /**
    Initializes iterator from other iterator.
  */
  inline ListIterator& operator=(const ListIterator& eq) noexcept
  {
    node = eq.node;
    return *this;
  }

  /**
    Initializes iterator from other iterator.
  */
  template<class POLY>
  inline ListIterator& operator=(const ListIterator<POLY>& eq) noexcept
  {
    node = eq.getValue();
    return *this;
  }

  /**
    Prefix increment.
  */
  inline ListIterator& operator++() noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    ASSERT(node);
    node = node->getNext();
    return *this;
  }

  /**
    Postfix increment.
  */
  inline ListIterator operator++(int) noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    ListIterator result(*this);
    ASSERT(node);
    node = node->getNext();
    return result;
  }

  /**
    Prefix decrement.
  */
  inline ListIterator& operator--() noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    ASSERT(node);
    node = node->getPrevious();
    return *this;
  }

  /**
    Postfix decrement.
  */
  inline ListIterator operator--(int) noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    ListIterator result(*this);
    ASSERT(node);
    node = node->getPrevious();
    return result;
  }

  /**
    Moves the specified distance forward.
  */
  ListIterator& operator+=(Distance distance) noexcept
  {
    if (distance < 0) {
      while (distance++) {
        --(*this);
      }
    } else {
      while (distance--) {
        ++(*this);
      }
    }
    return *this;
  }

  /**
    Moves the specified distance backwards.
  */
  ListIterator& operator-=(Distance distance) noexcept
  {
    if (distance < 0) {
      while (distance++) {
        ++(*this);
      }
    } else {
      while (distance--) {
        --(*this);
      }
    }
    return *this;
  }

  /**
    Returns true if the iterators are equal.
  */
  inline bool operator==(const ListIterator& eq) const noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_COMPATIBLE(*this, eq);
    return node == eq.node;
  }

  /**
    Returns true if the iterators aren't equal.
  */
  inline bool operator!=(const ListIterator& eq) const noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_COMPATIBLE(*this, eq);
    return node != eq.node;
  }

  /**
    Access the element.
  */
  inline Reference operator*() const noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    return *(node->getValue());
  }

  /**
    Access the element.
  */
  inline Pointer operator->() const noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    return node->getValue();
  }

  /**
    Returns the pointer value of the iterator.
  */
  inline Pointer getValue() const noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    return node->getValue();
  }
};



template<class TYPE>
class ListReadIterator {
public:

  typedef TYPE Value;
  typedef const TYPE* Pointer;
  typedef const TYPE& Reference;
  typedef MemoryDiff Distance;
  typedef BidirectionalIterator Category;
protected:

  const ListNode<TYPE>* node = nullptr;
public:

  /**
    Initializes iterator.

    @param value The initial value of the iterator.
  */
  explicit inline ListReadIterator(const ListNode<TYPE>* _node) noexcept
    : node(_node)
  {
  }

  /**
    Initializes iterator from other iterator.
  */
  inline ListReadIterator(const ListReadIterator& copy) noexcept
    : node(copy.node)
  {
  }

  /**
    Initializes iterator from other iterator.
  */
  template<class POLY>
  inline ListReadIterator(const ListReadIterator<POLY>& copy) noexcept
    : node(copy.getValue())
  {
  }

  /**
    Initializes iterator from other iterator.
  */
  inline ListReadIterator& operator=(const ListReadIterator& eq) noexcept
  {
    node = eq.node;
    return *this;
  }

  /**
    Initializes iterator from other iterator.
  */
  template<class POLY>
  inline ListReadIterator& operator=(const ListReadIterator<POLY>& eq) noexcept
  {
    node = eq.getValue();
    return *this;
  }

  /**
    Prefix increment.
  */
  inline ListReadIterator& operator++() noexcept
  {
    ASSERT(node);
    node = node->getNext();
    return *this;
  }

  /**
    Postfix increment.
  */
  inline ListReadIterator operator++(int) noexcept
  {
    ListReadIterator result(*this);
    ASSERT(node);
    node = node->getNext();
    return result;
  }

  /**
    Prefix decrement.
  */
  inline ListReadIterator& operator--() noexcept
  {
    ASSERT(node);
    node = node->getPrevious();
    return *this;
  }

  /**
    Postfix decrement.
  */
  inline ListReadIterator operator--(int) noexcept
  {
    ListReadIterator result(*this);
    ASSERT(node);
    node = node->getPrevious();
    return result;
  }

  /**
    Moves the specified distance forward.
  */
  ListReadIterator& operator+=(Distance distance) noexcept
  {
    if (distance < 0) {
      while (distance++) {
        --(*this);
      }
    } else {
      while (distance--) {
        ++(*this);
      }
    }
    return *this;
  }

  /**
    Moves the specified distance backwards.
  */
  ListReadIterator& operator-=(Distance distance) noexcept
  {
    if (distance < 0) {
      while (distance++) {
        ++(*this);
      }
    } else {
      while (distance--) {
        --(*this);
      }
    }
    return *this;
  }

  /**
    Returns true if the iterators are equal.
  */
  inline bool operator==(const ListReadIterator& eq) const noexcept
  {
    return node == eq.node;
  }

  /**
    Returns true if the iterators aren't equal.
  */
  inline bool operator!=(const ListReadIterator& eq) const noexcept
  {
    return node != eq.node;
  }

  /**
    Access the element.
  */
  inline Reference operator*() const noexcept
  {
    return *(node->getValue());
  }

  /**
    Access the element.
  */
  inline Pointer operator->() const noexcept
  {
    return node->getValue();
  }

  /**
    Returns the pointer value of the iterator.
  */
  inline Pointer getValue() const noexcept
  {
    return node->getValue();
  }
};



/**
  Enumeration of elements in a list.
  
  @short Enumerator of list with modification access.
  @ingroup collections
*/

template<class TRAITS>
class ListEnumerator : public Enumerator<TRAITS> {
public:

  typedef typename Enumerator<TRAITS>::Value Value;
  typedef typename Enumerator<TRAITS>::Pointer Pointer;
private:
  
  /** The current position in the enumeration. */
  ListNode<Value>* current = nullptr;
public:

  /**
    Initializes an enumeration of all the elements of a list.

    @param begin Specifies the beginning of the enumeration.
  */
  explicit inline ListEnumerator(ListNode<Value>* begin) noexcept
    : current(begin)
  {
  }

  /**
    Initializes enumeration from other enumeration.
  */
  inline ListEnumerator(const ListEnumerator& copy) noexcept
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
  inline Pointer next() throw(EndOfEnumeration)
  {
    bassert(current != nullptr, EndOfEnumeration());
    Pointer temp = current->getValue();
    current = current->getNext();
    return temp;
  }

  /**
    Returns true if the enumerations are pointing to the same position.
  */
  inline bool operator==(const ListEnumerator& eq) const noexcept
  {
    return current == eq.current;
  }

  /**
    Returns true if the enumerations aren't pointing to the same position.
  */
  inline bool operator!=(const ListEnumerator& eq) const noexcept
  {
    return current != eq.current;
  }
};

/**
  Enumeration of elements in a list.

  @short Non-modifying enumerator of list.
*/

template<class TRAITS>
class ListReadEnumerator : public Enumerator<TRAITS> {
protected:

  typedef typename Enumerator<TRAITS>::Value Value;
  typedef typename Enumerator<TRAITS>::Pointer Pointer;

  /** The current position in the enumeration. */
  const ListNode<Value>* current = nullptr;
public:

  /**
    Initializes an enumeration of all the elements of a list.

    @param begin Specifies the beginning of the enumeration.
  */
  explicit inline ListReadEnumerator(const ListNode<Value>* begin) noexcept
    : current(begin)
  {
  }

  /**
    Initializes enumeration from other enumeration.
  */
  inline ListReadEnumerator(const ListReadEnumerator& copy) noexcept
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
  inline Pointer next() throw(EndOfEnumeration)
  {
    bassert(current != nullptr, EndOfEnumeration());
    Pointer temp = current->getValue();
    current = current->getNext();
    return temp;
  }

  /**
    Returns true if the enumerations are pointing to the same position.
  */
  inline bool operator==(const ListReadEnumerator& eq) const noexcept {
    return current == eq.current;
  }

  /**
    Returns true if the enumerations aren't pointing to the same position.
  */
  inline bool operator!=(const ListReadEnumerator& eq) const noexcept {
    return current != eq.current;
  }
};



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
  typedef ListNode<Value> Node;

  typedef ListEnumerator<EnumeratorTraits<TYPE> > Enumerator;
  typedef ListReadEnumerator<ReadEnumeratorTraits<TYPE> > ReadEnumerator;
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
    inline void insert(Node* node, const TYPE& value) noexcept
    {
      Node* previous = node->getPrevious();
      Node* temp = new Node(node, previous, value);
      node->setPrevious(temp);
      if (previous) {
        previous->setNext(temp);
      }
      ++size;
    }

    /**
      Removes the specified node from this list.

      @param node The node to be removed. Must not be NULL.
    */
    inline void remove(Node* node) noexcept
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
      delete node;
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
    ListImpl(const ListImpl& copy) noexcept
    {
      const Node* node = copy.getFirst();
      while (node) {
        append(*node->getValue());
        node = node->getNext();
      }
    }

    ListImpl(ListImpl&& copy) noexcept
    {
      first = copy.first;
      copy.first = nullptr;
      last = copy.last;
      copy.last = nullptr;
      size = copy.size;
      copy.size = 0;
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
      if (size) {
        Node* node = new Node(nullptr, last, value);
        last->setNext(node);
        last = node;
        ++size;
      } else { // list is empty
        Node* node = new Node(nullptr, nullptr, value);
        first = node;
        last = node;
        ++size;
      }
    }

    /**
      Prepends the value to the beginning of this list.
    */
    void prepend(const TYPE& value)
    {
      if (size) {
        Node* node = new Node(first, nullptr, value);
        first->setPrevious(node);
        first = node;
        ++size;
      } else { // list is empty
        Node* node = new Node(nullptr, nullptr, value);
        first = node;
        last = node;
        ++size;
      }
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
    void removeFirst() throw(EmptyContainer)
    {
      if (!first) {
        throw EmptyContainer();
      }
      remove(first);
    }

    /**
      Removes the last node of this list.
    */
    void removeLast() throw(EmptyContainer)
    {
      if (!last) {
        throw EmptyContainer();
      }
      remove(last);
    }

    void shuffle()
    {
      MemorySize n = size;
      if (n <= 1) {
        return; // nothing to do
      }

      PrimitiveArray<ListNode<TYPE>*> nodes(n);
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
  inline Node* getFirst()
  {
    elements.copyOnWrite();
    return elements->getFirst();
  }

  /**
    Returns the first node of the list.
  */
  inline const Node* getFirst() const noexcept
  {
    return elements->getFirst();
  }

  /**
    Returns the last node of the list.
  */
  inline Node* getLast()
  {
    elements.copyOnWrite();
    return elements->getLast();
  }

  /**
    Returns the last node of the list.
  */
  inline const Node* getLast() const noexcept
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

  /**
    Initializes list from other list.
  */
  inline List(const List& copy) noexcept
    : elements(copy.elements) {
  }
  
  /**
    Assignment of list by list.
  */
  inline List& operator=(const List& eq) noexcept
  {
    elements = eq.elements;
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

  /**
    Returns a modifying enumerator of the list.
  */
  inline Enumerator getEnumerator() noexcept
  {
    return Enumerator(getFirst());
  }

  /**
    Returns a non-modifying enumerator of the list.
  */
  inline ReadEnumerator getReadEnumerator() const noexcept
  {
    return ReadEnumerator(getFirst());
  }

  ListIterator<TYPE> begin() noexcept
  {
    return ListIterator<TYPE>(getFirst());
  }

  ListIterator<TYPE> end() noexcept
  {
    return ListIterator<TYPE>(nullptr);
  }

  ListReadIterator<TYPE> begin() const noexcept
  {
    return ListReadIterator<TYPE>(getFirst());
  }

  ListReadIterator<TYPE> end() const noexcept
  {
    return ListReadIterator<TYPE>(nullptr);
  }

  /**
    Returns the index of the node with the specified value in this list.

    @return -1 if the node isn't in the list.
  */
/*  MemoryDiff indexOf(const TYPE& value) const noexcept
  {
    const ListNode* current = first;
    MemoryDiff index = 0;
    while (current) {
      if (current->getValue() == value) {
        return index;
      }
      current = current->getNext();
    }
    return -1; // value not found
  }*/

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
    Prepends the value to the beginning of this list.

    @param value The value to be prepended to the list.
  */
  void prepend(const TYPE& value)
  {
    elements.copyOnWrite();
    elements->prepend(value);
  }

  /**
    Appends the specified value to the end of this list.

    @param value The value to be appended to the list.
  */
  void add(const TYPE& value)
  {
    elements.copyOnWrite();
    elements->append(value);
  }

  /**
    Inserts the value at the specified position.

    @param enu Enumeration of this list specifying the position.
    @param value The value to be inserted.
  */
/*  void insert(Enumeration& enu, const TYPE& value) throw(InvalidEnumeration)
  {
    elements.copyOnWrite();
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
    Removes the node specified by the enumeration from this list.

    @param enu Enumeration specifying the element to be removed.
  */
/*  void remove(ListEnumeration& enu) throw(InvalidEnumeration, EndOfEnumeration)
  {
    if (enu.getOwner() != this) {
      throw InvalidEnumeration();
    }
    ListNode* node = nullptr; //enu.node;
    enu.next(); // raises exception if end has been reached
    remove(node);
  }*/
};

/**
  Writes a string representation of a list to a format stream.

  @relates List
*/
template<class TYPE>
FormatOutputStream& operator<<(
  FormatOutputStream& stream, const List<TYPE>& value) throw(IOException)
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

_COM_AZURE_DEV__BASE__STD_ITERATOR_TRAITS(base::ListIterator);
_COM_AZURE_DEV__BASE__STD_ITERATOR_TRAITS(base::ListReadIterator);
