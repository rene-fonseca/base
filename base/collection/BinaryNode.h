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

#include <base/features.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  A binary linked node. This class is used by the binary tree collection.
  
  @short Binary node
  @see BinaryTree
  @version 1.0
*/

template<class TYPE>
class BinaryNode {
private:

  /** The parent node of this node. */
  BinaryNode* parent = nullptr;
  /** The left child node. */
  BinaryNode* left = nullptr;
  /** The right child node. */
  BinaryNode* right = nullptr;
  /** The value associated with the node. */
  TYPE value;

  BinaryNode(int, int, int, const TYPE& _value); // force nullptr
  BinaryNode(BinaryNode* _parent, int, int, const TYPE& _value); // force nullptr
public:

  /**
    Initializes a binary node.

    @param parent The parent node of this node.
    @param value The value to be associated with the node.
  */
  inline BinaryNode(BinaryNode* _parent, NullPtr left, NullPtr right, const TYPE& _value)
    : parent(_parent),
      value(_value)
  {
  }

  /**
    Initializes a binary node.

    @param parent The parent node of this node.
    @param value The value to be associated with the node.
  */
  inline BinaryNode(BinaryNode* _parent, NullPtr left, NullPtr right, TYPE&& _value)
    : parent(_parent),
      value(std::move(_value))
  {
  }

  /**
    Initializes a binary node.

    @param parent The parent node of this node.
    @param left The left child node.
    @param right The right child node.
    @param value The value to be associated with the node.
  */
  BinaryNode(BinaryNode* _parent, BinaryNode* _left, BinaryNode* _right, const TYPE& _value)
    : parent(_parent),
      left(_left),
      right(_right),
      value(_value)
  {
  }

  /**
    Initializes a binary node.

    @param parent The parent node of this node.
    @param left The left child node.
    @param right The right child node.
    @param value The value to be associated with the node.
  */
  BinaryNode(BinaryNode* _parent, BinaryNode* _left, BinaryNode* _right, TYPE&& _value)
    : parent(_parent),
      left(_left),
      right(_right),
      value(std::move(_value))
  {
  }

  /**
    Returns the parent node.
  */
  inline BinaryNode* getParent() const noexcept
  {
    return parent;
  }

  /**
    Sets the parent node.
  */
  inline void setParent(BinaryNode* parent) noexcept
  {
    this->parent = parent;
  }

  /**
    Returns the left child node.
  */
  inline BinaryNode* getLeft() const noexcept
  {
    return left;
  }

  /**
    Sets the left child node.
  */
  inline void setLeft(BinaryNode* left) noexcept
  {
    this->left = left;
  }

  /**
    Returns the right child node.
  */
  inline BinaryNode* getRight() const noexcept
  {
    return right;
  }

  /**
    Sets the right child node.
  */
  inline void setRight(BinaryNode* right) noexcept
  {
    this->right = right;
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
  inline void setValue(const TYPE& value)
  {
    this->value = value;
  }

  /**
    Sets the value of the node.
  */
  inline void setValue(TYPE&& value)
  {
    this->value = std::move(value);
  }
};



///**
//  Automation pointer that allows transfer of ownership and automatic deletion of the object. Do not construct more than one automation pointer from the 'normal' pointer.
//
//  @version 1.0
//*/
//
//template<class TYPE>
//class BinaryNodePointer {
//public:
//
//  /** Type of value. */
//  typedef TYPE Value;
//  /** Pointer to value. */
//  typedef TYPE* Pointer;
//  /** Reference to value. */
//  typedef TYPE& Reference;
//  /** Type of node. */
//  typedef BinaryNode<Value> Node;
//private:
//
//  /** Pointer to binary node. */
//  Node* ptr = nullptr;
//public:
//
//  /**
//    Initializes the pointer.
//
//    @param value The pointer to be automated.
//  */
//  explicit inline BinaryNodePointer(Node value = 0) noexcept
//    : ptr(value)
//  {
//  }
//
//  /**
//    Copy constructor. Transfers ownership from copy to this object (copy loses ownership).
//  */
//  inline BinaryNodePointer(BinaryNodePointer& copy) noexcept
//    : ptr(copy.ptr)
//  {
//  }
//
//  /**
//    Assignment operator.
//  */
//  inline BinaryNodePointer& operator=(BinaryNodePointer& obj) noexcept
//  {
//    if (&obj != this) { // protect against self assignment
//      ptr = obj.ptr;
//    }
//    return *this;
//  }
//
//  /**
//    Returns mutable value of binary node.
//  */
//  inline Reference operator*()
//  {
//    if (!ptr) {
//      throw NullPointer();
//    }
//    return *ptr->getValue();
//  }
//
//  /**
//    Returns constant value of binary node.
//  */
//  inline const Reference operator*() const
//  {
//    if (!ptr) {
//      throw NullPointer();
//    }
//    return *ptr->getValue();
//  }
//
//  /**
//    Returns mutable value of binary node.
//  */
//  inline Pointer operator->() noexcept
//  {
//    return ptr->getValue();
//  }
//
//  /**
//    Returns constant value of binary node.
//  */
//  inline const Pointer operator->() const noexcept
//  {
//    return ptr->getValue();
//  }
//};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
