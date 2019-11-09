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
  A single linked node. This class is used by collections that only require
  unidirectional traversal of elements.

  @short Single linked node.
  @ingroup collections
*/

template<class TYPE>
class SingleLinkedNode {
protected:

  /** The next node in the container. */
  SingleLinkedNode* next = nullptr;
  /** The value associated with the node. */
  TYPE value;

  SingleLinkedNode(int, const TYPE& _value) noexcept; // force nullptr
public:

  /**
    Initializes a linked node.

    @param next The next node in the container.
    @param value The value to be associated with the node.
  */
  inline SingleLinkedNode(nullptr_t, const TYPE& _value) noexcept
    : value(_value)
  {
  }

  /**
    Initializes a linked node.

    @param next The next node in the container.
    @param value The value to be associated with the node.
  */
  inline SingleLinkedNode(SingleLinkedNode* _next, const TYPE& _value) noexcept
    : next(_next),
      value(_value)
  {
  }

  /**
    Returns the next node.
  */
  inline SingleLinkedNode* getNext() const noexcept
  {
    return next;
  }

  /**
    Sets the next node.
  */
  inline void setNext(SingleLinkedNode* next) noexcept
  {
    this->next = next;
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
};

template<class TYPE>
class SingleLinkedNodeIterator {
public:

  typedef TYPE Value;
  typedef TYPE* Pointer;
  typedef TYPE& Reference;
  typedef MemoryDiff Distance;
  typedef ForwardIterator Category;
protected:

  SingleLinkedNode<TYPE>* node = nullptr;
  _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_ADD_CONTEXT();
public:

  /**
    Initializes iterator.

    @param value The initial value of the iterator.
  */
  explicit inline SingleLinkedNodeIterator(SingleLinkedNode<TYPE>* _node) noexcept
    : node(_node)
  {
  }

  /**
    Initializes iterator from other iterator.
  */
  inline SingleLinkedNodeIterator(const SingleLinkedNodeIterator& copy) noexcept
    : node(copy.node)
  {
  }

  /**
    Initializes iterator from other iterator.
  */
  inline SingleLinkedNodeIterator& operator=(const SingleLinkedNodeIterator& copy) noexcept
  {
    node = copy.node;
    return *this;
  }

  /**
    Prefix increment.
  */
  inline SingleLinkedNodeIterator& operator++() noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    BASSERT(node);
    node = node->getNext();
    return *this;
  }

  /**
    Postfix increment.
  */
  inline SingleLinkedNodeIterator operator++(int) noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    SingleLinkedNodeIterator result(*this);
    BASSERT(node);
    node = node->getNext();
    return result;
  }

  /**
    Prefix decrement.
  */
  inline SingleLinkedNodeIterator& operator--() noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    BASSERT(node);
    node = node->getPrevious();
    return *this;
  }

  /**
    Postfix decrement.
  */
  inline SingleLinkedNodeIterator operator--(int) noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    SingleLinkedNodeIterator result(*this);
    BASSERT(node);
    node = node->getPrevious();
    return result;
  }

  /**
    Moves the specified distance forward.
  */
  SingleLinkedNodeIterator& operator+=(Distance distance) noexcept
  {
    if (distance < 0) {
      while (distance++) {
        --(*this);
      }
    }
    else {
      while (distance--) {
        ++(*this);
      }
    }
    return *this;
  }

  /**
    Moves the specified distance backwards.
  */
  SingleLinkedNodeIterator& operator-=(Distance distance) noexcept
  {
    if (distance < 0) {
      while (distance++) {
        ++(*this);
      }
    }
    else {
      while (distance--) {
        --(*this);
      }
    }
    return *this;
  }

  /**
    Returns true if the iterators are equal.
  */
  inline bool operator==(const SingleLinkedNodeIterator& eq) const noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_COMPATIBLE(*this, eq);
    return node == eq.node;
  }

  /**
    Returns true if the iterators aren't equal.
  */
  inline bool operator!=(const SingleLinkedNodeIterator& eq) const noexcept
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
class SingleLinkedNodeReadIterator {
public:

  typedef TYPE Value;
  typedef TYPE* Pointer;
  typedef TYPE& Reference;
  typedef MemoryDiff Distance;
  typedef ForwardIterator Category;
protected:

  const SingleLinkedNode<TYPE>* node = nullptr;
  _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_ADD_CONTEXT();
public:

  /**
    Initializes iterator.

    @param value The initial value of the iterator.
  */
  explicit inline SingleLinkedNodeReadIterator(const SingleLinkedNode<TYPE>* _node) noexcept
    : node(_node)
  {
  }

  /**
    Initializes iterator from other iterator.
  */
  inline SingleLinkedNodeReadIterator(const SingleLinkedNodeReadIterator& copy) noexcept
    : node(copy.node)
  {
  }

  /**
    Initializes iterator from other iterator.
  */
  inline SingleLinkedNodeReadIterator& operator=(const SingleLinkedNodeReadIterator& copy) noexcept
  {
    node = copy.node;
    return *this;
  }

  /**
    Prefix increment.
  */
  inline SingleLinkedNodeReadIterator& operator++() noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    BASSERT(node);
    node = node->getNext();
    return *this;
  }

  /**
    Postfix increment.
  */
  inline SingleLinkedNodeReadIterator operator++(int) noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    SingleLinkedNodeReadIterator result(*this);
    BASSERT(node);
    node = node->getNext();
    return result;
  }

  /**
    Prefix decrement.
  */
  inline SingleLinkedNodeReadIterator& operator--() noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    BASSERT(node);
    node = node->getPrevious();
    return *this;
  }

  /**
    Postfix decrement.
  */
  inline SingleLinkedNodeReadIterator operator--(int) noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_UNMODIFIED();
    SingleLinkedNodeReadIterator result(*this);
    BASSERT(node);
    node = node->getPrevious();
    return result;
  }

  /**
    Moves the specified distance forward.
  */
  SingleLinkedNodeReadIterator& operator+=(Distance distance) noexcept
  {
    if (distance < 0) {
      while (distance++) {
        --(*this);
      }
    }
    else {
      while (distance--) {
        ++(*this);
      }
    }
    return *this;
  }

  /**
    Moves the specified distance backwards.
  */
  SingleLinkedNodeReadIterator& operator-=(Distance distance) noexcept
  {
    if (distance < 0) {
      while (distance++) {
        ++(*this);
      }
    }
    else {
      while (distance--) {
        --(*this);
      }
    }
    return *this;
  }

  /**
    Returns true if the iterators are equal.
  */
  inline bool operator==(const SingleLinkedNodeReadIterator& eq) const noexcept
  {
    _COM_AZURE_DEV__BASE__PROTECT_ITERATORS_COMPATIBLE(*this, eq);
    return node == eq.node;
  }

  /**
    Returns true if the iterators aren't equal.
  */
  inline bool operator!=(const SingleLinkedNodeReadIterator& eq) const noexcept
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

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE

_COM_AZURE_DEV__BASE__STD_ITERATOR_TRAITS(base::SingleLinkedNodeIterator);
_COM_AZURE_DEV__BASE__STD_ITERATOR_TRAITS(base::SingleLinkedNodeReadIterator);
