/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__ORDERED_BINARY_TREE_H
#define _DK_SDU_MIP__BASE_COLLECTION__ORDERED_BINARY_TREE_H

#include <base/collection/BinaryTree.h>

/**
  Binary tree with the nodes ordered. All the values of the left and right
  subtrees are respectively less than and greater than the value for any node.

  @short Ordered binary tree.
  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE>
class OrderedBinaryTree : protected BinaryTree<TYPE> {
public:

  /**
    Enumeration of all the elements of an ordered binary tree.

    @author René Møller Fonseca
    @version 1.0
  */
  class Enumeration : public InfixOrderEnumeration<Node, Node&, Node*> {
  public:

    /**
      Initializes an enumeration of all the elements of the specified ordered
      binary tree.

      @param tree The ordered binary tree being enumerated.
    */
    inline Enumeration(OrderedBinaryTree& tree) throw() :
      InfixOrderEnumeration<Node, Node&, Node*>(tree.getRoot()) {}
  };

  /**
    Non-modifying enumeration of all the elements of an ordered binary tree.

    @author René Møller Fonseca
    @version 1.0
  */
  class ReadOnlyEnumeration : public InfixOrderEnumeration<Node, const Node&, const Node*> {
  public:

    /**
      Initializes a non-modifying enumeration of all the elements of the
      specified ordered binary tree.

      @param tree The ordered binary tree being enumerated.
    */
    inline ReadOnlyEnumeration(const OrderedBinaryTree& tree) throw() :
      InfixOrderEnumeration<Node, const Node&, const Node*>(tree.getRoot()) {}
  };

  /**
    Initializes an empty ordered binary tree.
  */
  OrderedBinaryTree() throw() {}

  /**
    Initializes binary tree from other binary tree.
  */
  OrderedBinaryTree(const OrderedBinaryTree& copy) throw() {}

  /**
    Searches for the specified value in this tree.

    @param value The value to look for.

    @return The node with the matching value. 0 if not found.
  */
  Node* find(const TYPE& value) throw() {
    Node* node = BinaryTree<TYPE>::getRoot();

    while (node) {
      if (value < *node->getValue()) {
        node = node->getLeft();
      } else if (value > *node->getValue()) {
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
  const Node* find(const TYPE& value) const throw() {
    const Node* node = BinaryTree<TYPE>::getRoot();

    while (node) {
      if (value < *node->getValue()) {
        node = node->getLeft();
      } else if (value > *node->getValue()) {
        node = node->getRight();
      } else {
        return node;
      }
    }
    return 0; // not found
  }

  /**
    Adds a value to the binary tree.

    @param value The value to be added.
    @return The value if it already exists. 0 if the value was not found in the tree.
  */
  Value* add(const TYPE& value) throw(MemoryException) {
    Node* node = BinaryTree<TYPE>::getRoot();

    if (!node) {
      BinaryTree<TYPE>::setRoot(new Node(0, 0, 0, value)); // attach root node
      return 0;
    }

    while (true) {
      if (value < *node->getValue()) {
        if (node->getLeft()) {
          node = node->getLeft();
        } else { // attach left child node
          node->setLeft(new Node(node, 0, 0, value));
          return 0;
        }
      } else if (value > *node->getValue()) {
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
    Removes the specified node from the binary tree. Throws 'InvalidNode' if
    the node is invalid.

    @param node The node to be removed.
  */
  void remove(Node* node) throw(InvalidNode) {
    if (!node) {
      throw InvalidNode();
    }

    elements.copyOnWrite();

    while (true) {
      if (node->getLeft()) {
        elements->rotateRight(node);
      } else if (node->getRight()) {
        elements->rotateLeft(node);
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

#endif
