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

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  A double linked node. This class is used by collections that only require
  bidirectional traversal of elements.

  @short Double linked node
  @version 1.0
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
  inline DoubleLinkedNode(nullptr_t, nullptr_t, const TYPE& _value)
    : value(_value)
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
  inline TYPE* getValue() noexcept
  {
    return &value;
  }

  /**
    Returns the value of the node.
  */
  inline const TYPE* getValue() const noexcept
  {
    return &value;
  }

  /**
    Sets the value of the node.
  */
  inline void setValue(const TYPE& value) noexcept
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
    ASSERT(node);
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
    ASSERT(node);
    node = node->getNext();
    return result;
  }

  /**
    Prefix decrement.
  */
  inline DoubleLinkedNodeIterator& operator--() noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    ASSERT(node);
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
    ASSERT(node);
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
  inline bool operator==(const DoubleLinkedNodeIterator& eq) const noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_COMPATIBLE(*this, eq);
    return node == eq.node;
  }

  /**
    Returns true if the iterators aren't equal.
  */
  inline bool operator!=(const DoubleLinkedNodeIterator& eq) const noexcept
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
class DoubleLinkedNodeReadIterator {
public:

  typedef TYPE Value;
  typedef TYPE* Pointer;
  typedef TYPE& Reference;
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
    ASSERT(node);
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
    ASSERT(node);
    node = node->getNext();
    return result;
  }

  /**
    Prefix decrement.
  */
  inline DoubleLinkedNodeReadIterator& operator--() noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    ASSERT(node);
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
    ASSERT(node);
    node = node->getPrevious();
    return result;
  }

  /**
    Moves the specified distance forward.
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
    Moves the specified distance backwards.
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
  inline bool operator==(const DoubleLinkedNodeReadIterator& eq) const noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_COMPATIBLE(*this, eq);
    return node == eq.node;
  }

  /**
    Returns true if the iterators aren't equal.
  */
  inline bool operator!=(const DoubleLinkedNodeReadIterator& eq) const noexcept
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
    return *node;
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

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE

_COM_AZURE_DEV__BASE__STD_ITERATOR_TRAITS(base::DoubleLinkedNodeIterator);
_COM_AZURE_DEV__BASE__STD_ITERATOR_TRAITS(base::DoubleLinkedNodeReadIterator);
