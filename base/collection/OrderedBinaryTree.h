/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2006 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__ORDERED_BINARY_TREE_H
#define _DK_SDU_MIP__BASE_COLLECTION__ORDERED_BINARY_TREE_H

#include <base/collection/BinaryTree.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Binary tree with the nodes ordered. All the values of the left and right
  subtrees are respectively less than and greater than the value for any node.

  @short Ordered binary tree
  @ingroup collections
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
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
//    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
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
//    inline Enumeration(OrderedBinaryTree& tree) throw() :
//      InfixOrderEnumeration<Value, Value&, Value*, Node*>(tree.getRoot()) {
//    }
//  };
//
//  /**
//    Non-modifying enumeration of all the elements of an ordered binary tree.
//
//    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
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
//    inline ReadOnlyEnumeration(const OrderedBinaryTree& tree) throw() :
//      InfixOrderEnumeration<Value, const Value&, const Value*, const Node*>(tree.getRoot()) {
//    }
//  };

  using BinaryTree<TYPE>::getRoot;

  /**
    Initializes an empty ordered binary tree.
  */
  OrderedBinaryTree() throw() {
  }

  /**
    Initializes binary tree from other binary tree.
  */
  OrderedBinaryTree(const OrderedBinaryTree& copy) throw()
    : BinaryTree<TYPE>(copy) {
  }

  /**
    Assignment of ordered binary tree to ordered binary tree.
  */
  OrderedBinaryTree& operator=(const OrderedBinaryTree& eq) throw() {
    BinaryTree<TYPE>::operator=(eq);
    return *this;
  }

  /**
    Returns a modifying enumerator of the ordered binary tree.
  */
  inline Enumerator getEnumerator() throw() {
    return Enumerator(getRoot());
  }

  /**
    Returns a non-modifying enumerator of the ordered binary tree.
  */
  inline ReadEnumerator getReadEnumerator() const throw() {
    return ReadEnumerator(getRoot());
  }

  /**
    Searches for the specified value in this tree.

    @param value The value to look for.

    @return The node with the matching value. 0 if not found.
  */
  Node* find(const Value& value) throw() {
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
    return 0; // not found
  }

  /**
    Searches for the specified value in this tree.

    @param value The value to look for.

    @return The node with the matching value. 0 if not found.
  */
  const Node* find(const Value& value) const throw() {
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
    return 0; // not found
  }

  /**
    Returns the first/smallest value of this tree.

    @return 0 if the tree is empty.
  */
  Node* getFirst() throw() {
    Node* node = BinaryTree<Value>::getRoot();
    Node* previous = 0; // not found

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
  Node* getLast() throw() {
    Node* node = BinaryTree<Value>::getRoot();
    Node* previous = 0; // not found

    while (node) {
      previous = node;
      node = node->getRight();
    }
    return previous;
  }

  /**
    Adds a value to the binary tree.

    @param value The value to be added.
    @return The value if it already exists. 0 if the value was not found in the tree.
  */
  Value* add(const Value& value) throw(MemoryException) {
    Node* node = BinaryTree<Value>::getRoot();

    if (!node) {
      this->elements = new typename BinaryTree<Value>::BinaryTreeImpl(new Node(0, 0, 0, value)); // attach root node
      return 0;
    }

    while (true) {
      int result = compare(value, *node->getValue());
      if (result < 0) {
        if (node->getLeft()) {
          node = node->getLeft();
        } else { // attach left child node
          node->setLeft(new Node(node, 0, 0, value));
          return 0;
        }
      } else if (result > 0) {
        if (node->getRight()) {
          node = node->getRight();
        } else { // attach right child node
          node->setRight(new Node(node, 0, 0, value));
          return 0;
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
        parent->setLeft(0); // detach node
      } else {
        // node if right child node of parent
        parent->setRight(0); // detach node
      }
      delete node;
    } else { // node is the only element of the tree
      BinaryTree<TYPE>::removeAll();
    }
  }

  /**
    Removes all the nodes from the tree.
  */
  void removeAll() throw() {
    BinaryTree<TYPE>::removeAll();
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
