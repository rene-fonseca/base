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

#include <base/collection/BinaryTree.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Binary tree with the nodes ordered. All the values of the left and right
  subtrees are respectively less than and greater than the value for any node.

  @short Ordered binary tree
  @ingroup collections
  @version 1.0
*/

template<class TYPE>
class OrderedBinaryTree : protected BinaryTree<TYPE> {
public:

  /** The type of the value. */
  typedef TYPE Value;
  /** The type of the node. */
  typedef typename BinaryTree<TYPE>::Node Node;

  /** Modifying enumerator. */
  typedef InfixOrderEnumerator<EnumeratorTraits<Node> > Enumerator;
  /** Non-modifying enumerator. */
  typedef InfixOrderEnumerator<ReadEnumeratorTraits<Node> > ReadEnumerator;

//  class Enumeration;
//  friend class Enumeration;
//  class ReadOnlyEnumeration;
//  friend class ReadOnlyEnumeration;
//
//  /**
//    Enumeration of all the elements of an ordered binary tree.
//
//    @version 1.0
//  */
//  class Enumeration : public InfixOrderEnumeration<Value, Value&, Value*, Node*> {
//  public:
//
//    /**
//      Initializes an enumeration of all the elements of the specified ordered
//      binary tree.
//
//      @param tree The ordered binary tree being enumerated.
//    */
//    inline Enumeration(OrderedBinaryTree& tree) noexcept
//      : InfixOrderEnumeration<Value, Value&, Value*, Node*>(tree.getRoot())
//    {
//    }
//  };
//
//  /**
//    Non-modifying enumeration of all the elements of an ordered binary tree.
//
//    @version 1.0
//  */
//  class ReadOnlyEnumeration : public InfixOrderEnumeration<Value, const Value&, const Value*, const Node*> {
//  public:
//
//    /**
//      Initializes a non-modifying enumeration of all the elements of the
//      specified ordered binary tree.
//
//      @param tree The ordered binary tree being enumerated.
//    */
//    inline ReadOnlyEnumeration(const OrderedBinaryTree& tree) noexcept
//      : InfixOrderEnumeration<Value, const Value&, const Value*, const Node*>(tree.getRoot())
//    {
//    }
//  };

  using BinaryTree<TYPE>::getRoot;

  /**
    Initializes an empty ordered binary tree.
  */
  OrderedBinaryTree() noexcept {
  }

  /**
    Initializes binary tree from other binary tree.
  */
  OrderedBinaryTree(const OrderedBinaryTree& copy) noexcept
    : BinaryTree<TYPE>(copy) {
  }

  /**
    Assignment of ordered binary tree to ordered binary tree.
  */
  OrderedBinaryTree& operator=(const OrderedBinaryTree& eq) noexcept {
    BinaryTree<TYPE>::operator=(eq);
    return *this;
  }

  /**
    Returns a modifying enumerator of the ordered binary tree.
  */
  inline Enumerator getEnumerator() noexcept {
    return Enumerator(getRoot());
  }

  /**
    Returns a non-modifying enumerator of the ordered binary tree.
  */
  inline ReadEnumerator getReadEnumerator() const noexcept {
    return ReadEnumerator(getRoot());
  }

  /**
    Searches for the specified value in this tree.

    @param value The value to look for.

    @return The node with the matching value. 0 if not found.
  */
  Node* find(const Value& value) noexcept {
    Node* node = getRoot();

    while (node) {
      int result = compare(value, *node->getValue());
      if (result < 0) {
        node = node->getLeft();
      } else if (result > 0) {
        node = node->getRight();
      } else {
        return node;
      }
    }
    return nullptr; // not found
  }

  /**
    Searches for the specified value in this tree.

    @param value The value to look for.

    @return The node with the matching value. 0 if not found.
  */
  const Node* find(const Value& value) const noexcept {
    const Node* node = BinaryTree<Value>::getRoot();

    while (node) {
      int result = compare(value, *node->getValue());
      if (result < 0) {
        node = node->getLeft();
      } else if (result > 0) {
        node = node->getRight();
      } else {
        return node;
      }
    }
    return nullptr; // not found
  }

  /**
    Returns the first/smallest value of this tree.

    @return 0 if the tree is empty.
  */
  Node* getFirst() noexcept {
    Node* node = BinaryTree<Value>::getRoot();
    Node* previous = nullptr; // not found

    while (node) {
      previous = node;
      node = node->getLeft();
    }
    return previous;
  }

  /**
    Returns the last/highest value of this tree.

    @return 0 if the tree is empty.
  */
  Node* getLast() noexcept {
    Node* node = BinaryTree<Value>::getRoot();
    Node* previous = nullptr; // not found

    while (node) {
      previous = node;
      node = node->getRight();
    }
    return previous;
  }

  /**
    Adds a value to the binary tree.

    @param value The value to be added.
    @return The value if it already exists. nullptr if the value was not found in the tree.
  */
  Value* add(const Value& value)
  {
    Node* node = BinaryTree<Value>::getRoot();

    if (!node) {
      this->elements = new typename BinaryTree<Value>::BinaryTreeImpl(new Node(nullptr, nullptr, nullptr, value)); // attach root node
      return nullptr;
    }

    while (true) {
      int result = compare(value, *node->getValue());
      if (result < 0) {
        if (node->getLeft()) {
          node = node->getLeft();
        } else { // attach left child node
          node->setLeft(new Node(node, nullptr, nullptr, value));
          return nullptr;
        }
      } else if (result > 0) {
        if (node->getRight()) {
          node = node->getRight();
        } else { // attach right child node
          node->setRight(new Node(node, nullptr, nullptr, value));
          return nullptr;
        }
      } else {
        return node->getValue(); // node with this value already exists
      }
    }
  }

  /**
    Removes the specified node from the binary tree. Raises InvalidNode if
    the node is invalid.

    @param node The node to be removed.
  */
  void remove(Node* node) throw(InvalidNode) {
    if (!node) {
      throw InvalidNode();
    }

    this->elements.copyOnWrite();

    if (node == BinaryTree<Value>::getRoot()) { // set new root
      if (node->getLeft()) {
        BinaryTree<Value>::setRoot(node->getLeft());
      } else if (node->getRight()) {
        BinaryTree<Value>::setRoot(node->getRight());
      }
    }

    while (true) {
      if (node->getLeft()) {
        this->elements->rotateRight(node);
      } else if (node->getRight()) {
        this->elements->rotateLeft(node);
      } else {
        break; // stop when the node is a leave in the tree
      }
    }

    Node* parent = node->getParent();
    if (parent) {
      if (parent->getLeft() == node) {
        // node if left child node of parent
        parent->setLeft(nullptr); // detach node
      } else {
        // node if right child node of parent
        parent->setRight(nullptr); // detach node
      }
      delete node;
    } else { // node is the only element of the tree
      BinaryTree<TYPE>::removeAll();
    }
  }

  /**
    Removes all the nodes from the tree.
  */
  void removeAll() noexcept {
    BinaryTree<TYPE>::removeAll();
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
