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

#include <base/Iterator.h>
#include <base/collection/Enumeration.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  A double linked node. This class is used by collections that only require
  bidirectional traversal of elements.

  @short Double linked node
*/

template<class TYPE>
class DoubleLinkedNode {
protected:

  /** The next node in the container. */
  DoubleLinkedNode* next = nullptr;
  /** The previous node in the container. */
  DoubleLinkedNode* previous = nullptr;
  /** The value associated with the node. */
  TYPE value;

  DoubleLinkedNode(int, int, const TYPE& _value); // force nullptr
public:

  /**
    Initializes a linked node with no previous and next node.
  */
  inline DoubleLinkedNode(NullPtr, NullPtr, const TYPE& _value)
    : value(_value)
  {
  }

  /**
    Initializes a linked node with no previous and next node.
  */
  inline DoubleLinkedNode(NullPtr, NullPtr, TYPE&& _value)
    : value(moveObject(_value))
  {
  }

  /**
    Initializes a linked node with default construction of value.

    @param next The next node in the container.
    @param previous The previous node in the container.
  */
  inline DoubleLinkedNode(DoubleLinkedNode* _next, DoubleLinkedNode* _previous)
    : next(_next),
      previous(_previous)
  {
  }

  /**
    Initializes a linked node.

    @param next The next node in the container.
    @param previous The previous node in the container.
    @param value The value to be associated with the node.
  */
  inline DoubleLinkedNode(DoubleLinkedNode* _next, DoubleLinkedNode* _previous, const TYPE& _value)
    : next(_next),
      previous(_previous),
      value(_value)
  {
  }

  /**
    Initializes a linked node.

    @param next The next node in the container.
    @param previous The previous node in the container.
    @param value The value to be associated with the node.
  */
  inline DoubleLinkedNode(DoubleLinkedNode* _next, DoubleLinkedNode* _previous, TYPE&& _value)
    : next(_next),
      previous(_previous),
      value(moveObject(_value))
  {
  }

  /**
    Returns the next node.
  */
  inline DoubleLinkedNode* getNext() const noexcept
  {
    return next;
  }

  /**
    Sets the next node.
  */
  inline void setNext(DoubleLinkedNode* next) noexcept
  {
    this->next = next;
  }

  /**
    Returns the previous node.
  */
  inline DoubleLinkedNode* getPrevious() const noexcept
  {
    return previous;
  }

  /**
    Sets the previous node.
  */
  inline void setPrevious(DoubleLinkedNode* previous) noexcept
  {
    this->previous = previous;
  }

  /**
    Returns the value of the node.
  */
  inline TYPE& getValue() noexcept
  {
    return value;
  }

  /**
    Returns the value of the node.
  */
  inline const TYPE& getValue() const noexcept
  {
    return value;
  }

  /**
    Sets the value of the node.
  */
  inline void setValue(const TYPE& value) noexcept
  {
    this->value = value;
  }

  /**
    Sets the value of the node.
  */
  inline void setValue(TYPE&& value) noexcept
  {
    this->value = value;
  }

  inline TYPE& operator*() noexcept
  {
    return *value;
  }

  inline const TYPE& operator*() const noexcept
  {
    return *value;
  }
};

template<class TYPE>
class DoubleLinkedNodeIterator {
public:

  typedef TYPE Value;
  typedef TYPE* Pointer;
  typedef TYPE& Reference;
  typedef MemoryDiff Distance;
  typedef BidirectionalIterator Category;
protected:

  DoubleLinkedNode<TYPE>* node = nullptr;
  _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_ADD_CONTEXT();
public:

  /**
    Initializes iterator.

    @param value The initial value of the iterator.
  */
  explicit inline DoubleLinkedNodeIterator(DoubleLinkedNode<TYPE>* _node) noexcept
    : node(_node)
  {
  }

  /**
    Initializes iterator from other iterator.
  */
  inline DoubleLinkedNodeIterator(const DoubleLinkedNodeIterator& copy) noexcept
    : node(copy.node)
  {
  }

  /**
    Initializes iterator from other iterator.
  */
  inline DoubleLinkedNodeIterator& operator=(const DoubleLinkedNodeIterator& copy) noexcept
  {
    node = copy.node;
    return *this;
  }

  /**
    Prefix increment.
  */
  inline DoubleLinkedNodeIterator& operator++() noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    BASSERT(node);
    node = node->getNext();
    return *this;
  }

  /**
    Postfix increment.
  */
  inline DoubleLinkedNodeIterator operator++(int) noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    DoubleLinkedNodeIterator result(*this);
    BASSERT(node);
    node = node->getNext();
    return result;
  }

  /**
    Prefix decrement.
  */
  inline DoubleLinkedNodeIterator& operator--() noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    BASSERT(node);
    node = node->getPrevious();
    return *this;
  }

  /**
    Postfix decrement.
  */
  inline DoubleLinkedNodeIterator operator--(int) noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    DoubleLinkedNodeIterator result(*this);
    BASSERT(node);
    node = node->getPrevious();
    return result;
  }

  /**
    Moves the specified distance forward.
  */
  DoubleLinkedNodeIterator& operator+=(Distance distance) noexcept
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
  DoubleLinkedNodeIterator& operator-=(Distance distance) noexcept
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
  inline bool operator==(const DoubleLinkedNodeIterator& compare) const noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_COMPATIBLE(*this, compare);
    return node == compare.node;
  }

  /**
    Returns true if the iterators aren't equal.
  */
  inline bool operator!=(const DoubleLinkedNodeIterator& compare) const noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_COMPATIBLE(*this, compare);
    return node != compare.node;
  }

  /**
    Access the element.
  */
  inline Reference operator*() const noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    return node->getValue();
  }

  /**
    Access the element.
  */
  inline Pointer operator->() const noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    return &(node->getValue());
  }

  /**
    Returns the pointer value of the iterator.
  */
  inline Pointer getValue() const noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    return &(node->getValue());
  }

  /**
    Returns the pointer value of the iterator.
  */
  inline DoubleLinkedNode<TYPE>* getNode() const noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    return node;
  }
};

template<class TYPE>
class DoubleLinkedNodeReadIterator {
public:

  typedef TYPE Value;
  typedef const TYPE* Pointer;
  typedef const TYPE& Reference;
  typedef MemoryDiff Distance;
  typedef BidirectionalIterator Category;
protected:

  const DoubleLinkedNode<TYPE>* node = nullptr;
  _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_ADD_CONTEXT();
public:

  /**
    Initializes iterator.

    @param value The initial value of the iterator.
  */
  explicit inline DoubleLinkedNodeReadIterator(const DoubleLinkedNode<TYPE>* _node) noexcept
    : node(_node)
  {
  }

  /**
    Initializes iterator from other iterator.
  */
  inline DoubleLinkedNodeReadIterator(const DoubleLinkedNodeReadIterator& copy) noexcept
    : node(copy.node)
  {
  }

  /**
    Initializes iterator from other iterator.
  */
  inline DoubleLinkedNodeReadIterator& operator=(const DoubleLinkedNodeReadIterator& copy) noexcept
  {
    node = copy.node;
    return *this;
  }

  /**
    Prefix increment.
  */
  inline DoubleLinkedNodeReadIterator& operator++() noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    BASSERT(node);
    node = node->getNext();
    return *this;
  }

  /**
    Postfix increment.
  */
  inline DoubleLinkedNodeReadIterator operator++(int) noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    DoubleLinkedNodeReadIterator result(*this);
    BASSERT(node);
    node = node->getNext();
    return result;
  }

  /**
    Prefix decrement.
  */
  inline DoubleLinkedNodeReadIterator& operator--() noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    BASSERT(node);
    node = node->getPrevious();
    return *this;
  }

  /**
    Postfix decrement.
  */
  inline DoubleLinkedNodeReadIterator operator--(int) noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    DoubleLinkedNodeReadIterator result(*this);
    BASSERT(node);
    node = node->getPrevious();
    return result;
  }

  /**
    Moves the specified distance forward. O(n) complexity.
  */
  DoubleLinkedNodeReadIterator& operator+=(Distance distance) noexcept
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
    Moves the specified distance backwards. O(n) complexity.
  */
  DoubleLinkedNodeReadIterator& operator-=(Distance distance) noexcept
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
  inline bool operator==(const DoubleLinkedNodeReadIterator& compare) const noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_COMPATIBLE(*this, compare);
    return node == compare.node;
  }

  /**
    Returns true if the iterators aren't equal.
  */
  inline bool operator!=(const DoubleLinkedNodeReadIterator& compare) const noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_COMPATIBLE(*this, compare);
    return node != compare.node;
  }

  /**
    Access the element.
  */
  inline Reference operator*() const noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    return node->getValue();
  }

  /**
    Access the element.
  */
  inline Pointer operator->() const noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    return &(node->getValue());
  }

  /**
    Returns the pointer value of the iterator.
  */
  inline Pointer getValue() const noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    return &(node->getValue());
  }

  /**
    Returns the pointer value of the iterator.
  */
  inline const DoubleLinkedNode<TYPE>* getNode() const noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    return node;
  }
};

/**
  Enumeration of elements in a DoubleLinkedNode.

  @short Modifying enumerator of DoubleLinkedNode.
*/
template<class TRAITS>
class DoubleLinkedNodeEnumerator : public Enumerator<TRAITS> {
public:

  typedef typename Enumerator<TRAITS>::Value Value;
  typedef typename Enumerator<TRAITS>::Pointer Pointer;
protected:

  /** The current position in the enumeration. */
  DoubleLinkedNode<Value>* current = nullptr;
public:

  /**
    Initializes an enumeration of all the elements of a DoubleLinkedNode.

    @param begin Specifies the beginning of the enumeration.
  */
  explicit inline DoubleLinkedNodeEnumerator(DoubleLinkedNode<Value>* begin) noexcept
    : current(begin)
  {
  }

  /**
    Initializes enumeration from other enumeration.
  */
  inline DoubleLinkedNodeEnumerator(const DoubleLinkedNodeEnumerator& copy) noexcept
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
      throw EndOfEnumeration();
    }
    auto temp = current;
    current = current->getNext();
    return &(temp->getValue());
  }

  /**
    Returns true if the enumerations are pointing to the same position.
  */
  inline bool operator==(const DoubleLinkedNodeEnumerator& compare) const noexcept
  {
    return current == compare.current;
  }

  /**
    Returns true if the enumerations aren't pointing to the same position.
  */
  inline bool operator!=(const DoubleLinkedNodeEnumerator& compare) const noexcept
  {
    return current != compare.current;
  }
};

/**
  Enumeration of elements in a DoubleLinkedNode.

  @short Non-modifying enumerator of DoubleLinkedNode.
*/
template<class TRAITS>
class DoubleLinkedNodeReadEnumerator : public Enumerator<TRAITS> {
public:

  typedef typename Enumerator<TRAITS>::Value Value;
  typedef typename Enumerator<TRAITS>::Pointer Pointer;
protected:

  /** The current position in the enumeration. */
  const DoubleLinkedNode<Value>* current = nullptr;
public:

  /**
    Initializes an enumeration of all the elements of a DoubleLinkedNode.

    @param begin Specifies the beginning of the enumeration.
  */
  explicit inline DoubleLinkedNodeReadEnumerator(const DoubleLinkedNode<Value>* begin) noexcept
    : current(begin)
  {
  }

  /**
    Initializes enumeration from other enumeration.
  */
  inline DoubleLinkedNodeReadEnumerator(const DoubleLinkedNodeReadEnumerator& copy) noexcept
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
      throw EndOfEnumeration();
    }
    auto temp = current;
    current = current->getNext();
    return &(temp->getValue());
  }

  /**
    Returns true if the enumerations are pointing to the same position.
  */
  inline bool operator==(const DoubleLinkedNodeReadEnumerator& compare) const noexcept
  {
    return current == compare.current;
  }

  /**
    Returns true if the enumerations aren't pointing to the same position.
  */
  inline bool operator!=(const DoubleLinkedNodeReadEnumerator& compare) const noexcept
  {
    return current != compare.current;
  }
};

enum {
  CREATE_DOUBLE_LINKED_NODE,
  CREATE_DOUBLE_LINKED_NODE_BY_MOVE_CONSTRUCT,
  CREATE_DOUBLE_LINKED_NODE_BY_MOVE_ASSIGN,
};

/** Helper class for choosing between copy and move. Uses copy construction by default. */
template<class TYPE, int>
class CreateDoubleLinkedNode {
public:

  static inline DoubleLinkedNode<TYPE>* createNode(DoubleLinkedNode<TYPE>* next,
                                                   DoubleLinkedNode<TYPE>* previous,
                                                   TYPE&& value)
  {
    return new DoubleLinkedNode<TYPE>(next, previous, value); // copy by default
  }
};

template<class TYPE>
class CreateDoubleLinkedNode<TYPE, CREATE_DOUBLE_LINKED_NODE_BY_MOVE_CONSTRUCT> {
public:

  // requires move constructible
  static inline DoubleLinkedNode<TYPE>* createNode(DoubleLinkedNode<TYPE>* next,
                                                   DoubleLinkedNode<TYPE>* previous,
                                                   TYPE&& value)
  {
    return new DoubleLinkedNode<TYPE>(next, previous, moveObject(value)); // move construction
  }
};

template<class TYPE>
class CreateDoubleLinkedNode<TYPE, CREATE_DOUBLE_LINKED_NODE_BY_MOVE_ASSIGN> {
public:

  // requires default construction and move assignable
  static inline DoubleLinkedNode<TYPE>* createNode(DoubleLinkedNode<TYPE>* next,
                                                   DoubleLinkedNode<TYPE>* previous,
                                                   TYPE&& value)
  {
    auto temp = new DoubleLinkedNode<TYPE>(next, previous); // default initialization
    temp->getValue() = moveObject(value);
    return temp;
  }
};

template<class TYPE>
class GetDoubleLinkedNodeConstruction {
public:

  static constexpr int HOW = std::is_move_constructible<TYPE>() ? CREATE_DOUBLE_LINKED_NODE_BY_MOVE_CONSTRUCT :
    ((std::is_move_assignable<TYPE>() && std::is_default_constructible<TYPE>()) ? CREATE_DOUBLE_LINKED_NODE_BY_MOVE_ASSIGN : CREATE_DOUBLE_LINKED_NODE);
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE

_COM_AZURE_DEV__BASE__STD_ITERATOR_TRAITS(base::DoubleLinkedNodeIterator);
_COM_AZURE_DEV__BASE__STD_ITERATOR_TRAITS(base::DoubleLinkedNodeReadIterator);
