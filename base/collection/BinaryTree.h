/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__BINARY_TREE_H
#define _DK_SDU_MIP__BASE_COLLECTION__BINARY_TREE_H

#include <base/collection/Collection.h>
#include <base/collection/BinaryNode.h>
#include <base/collection/InvalidNode.h>
#include <base/collection/Enumeration.h>
#include <base/mem/ReferenceCountedObjectPointer.h>
#include <base/mem/ReferenceCountedObject.h>

/**
  Enumeration of all the elements of a binary tree traversed in prefix order.
*/
template<class TYPE, class REF, class PTR>
class PrefixOrderEnumeration : Enumeration<TYPE, REF, PTR> {
public:

  typedef TYPE Value;
  typedef REF Reference;
  typedef PTR Pointer;
private:

  /** The root of the binary tree. */
  Pointer root;
  /** The current position in the enumeration. */
  Pointer node;
  /** Specifies that more elements are available. */
  bool more;
public:

  /**
    Initializes enumeration of binary tree specified by the node.

    @param n The root node of the binary tree.
  */
  PrefixOrderEnumeration(Pointer n) throw() : root(n), node(n), more(!n) {}

  /**
    Returns true if there is more elements in this enumeration.
  */
  inline bool hasNext() const throw() {return more;}

  /**
    Returns the current value and increments the position. Throws
    'EndOfEnumeration' if the end has been reached.
  */
  Pointer next() throw(EndOfEnumeration) {
    if (!more) {
      throw EndOfEnumeration();
    }

    Pointer result = node;

    if (node->getLeft()) { // traverse left subtree
      node = node->getLeft();
    } else if (node->getRight()) { // traverse right subtree
      node = node->getRight();
    } else {
      Pointer temp;
      do { // return from subtree
        if (node == root) {
          more = false;
          return result;
        }
        temp = node;
        node = node->getParent();
      } while ((!node->getRight()) || (temp == node->getRight()));

      node = node->getRight(); // traverse right subtree - which we know exists
    }

    return result;
  }
};



//  /**
//    Invocates the specified unary operation for each element (non-modifying operation).
//  */
//  template<class TYPE, class UNOPR>
//  void traversePrefixOrder(const BinaryNode* root, UNOPR& function) {
//    const BinaryNode* node = root;
//    while (true) {
//      function(node);
//
//      if (node->getLeft()) { // traverse left subtree
//        node = node->getLeft();
//      } else if (node->getRight()) { // traverse right subtree
//        node = node->getRight();
//      } else {
//        do { // return from subtree
//          if (node == root) {
//            return;
//          }
//          const BinaryNode* temp = node;
//          node = node->getParent();
//        while ((!node->getRight()) || (temp == node->getRight()));
//
//        node = node->getRight(); // traverse right subtree - which we know exists
//      }
//    }
//  }
//
//  template<class TYPE, class UNOPR>
//  void traverseInfixOrder(const BinaryNode* root, UNOPR& function) {
//    const BinaryNode* node = root;
//    while (true) {
//      if (node->getLeft()) { // traverse left subtree
//        node = node->getLeft();
//      } else if (node->getRight()) { // traverse right subtree
//        function(node);
//        node = node->getRight();
//      } else {
//        do { // return from subtree
//          if (node == root) {
//            return;
//          }
//          const BinaryNode* temp = node;
//          node = node->getParent();
//        while ((!node->getRight()) || (temp == node->getRight()));
//
//        function(node);
//        node = node->getRight(); // traverse right subtree - which we know exists
//      }
//    }
//  }
//
//  template<class TYPE, class UNOPR>
//  void traversePostfixOrder(const BinaryNode* root, UNOPR& function) {
//    const BinaryNode* node = root;
//    while (true) {
//      if (node->getLeft()) { // traverse left subtree
//        node = node->getLeft();
//      } else if (node->getRight()) { // traverse right subtree
//        node = node->getRight();
//      } else {
//        do { // return from subtree
//          function(node);
//          if (node == root) {
//            return;
//          }
//          const BinaryNode* temp = node;
//          node = node->getParent();
//        while ((!node->getRight()) || (temp == node->getRight()));
//
//        node = node->getRight(); // traverse right subtree - which we know exists
//      }
//    }
//  }

/**
  Binary tree.

  @short Binary tree.
  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE>
class BinaryTree : public Collection {
public:

  /** The type of the value. */
  typedef TYPE Value;
  /** The type of a node. */
  typedef BinaryNode<TYPE> Node;

  /**
    Enumeration of all the elements of a binary tree.

    @author René Møller Fonseca
    @version 1.0
  */
  class Enumeration : public PrefixOrderEnumeration<Node, Node&, Node*> {
  public:

    /**
      Initializes an enumeration of all the elements of the specified binary
      tree.

      @param tree The binary tree being enumerated.
    */
    inline Enumeration(BinaryTree& tree) throw() :
      PrefixOrderEnumeration<Node, Node&, Node*>(tree.getRoot()) {}
  };

  /**
    Non-modifying enumeration of all the elements of a binary tree.

    @author René Møller Fonseca
    @version 1.0
  */
  class ReadOnlyEnumeration : public PrefixOrderEnumeration<Node, const Node&, const Node*> {
  public:

    /**
      Initializes a non-modifying enumeration of all the elements of the
      specified binary tree.

      @param tree The binary tree being enumerated.
    */
    inline ReadOnlyEnumeration(const BinaryTree& tree) throw() :
      PrefixOrderEnumeration<Node, const Node&, const Node*>(tree.getRoot()) {}
  };

  /**
    Internal binary tree implementation.

    @author René Møller Fonseca
    @version 1.0
  */
  class BinaryTreeImpl : public ReferenceCountedObject {
  private:

    /** The root node of the binary tree. */
    Node* root;
  protected:

    /**
      Makes a copy of a subtree specified by the top node.

      @param node The top node of the subtree to be copied.
      @return The top node of the new subtree.
    */
    Node* copySubtree(const Node* node) throw() {
      if (node) {
        Node* left = copySubtree(node->getLeft());
        Node* right = copySubtree(node->getRight());
        Node* result = new Node(0, left, right, *node->getValue());
        if (left) {
          left->setParent(result);
        }
        if (right) {
          right->setParent(result);
        }
        return result;
      } else {
        return 0;
      }
    }

    /**
      Destroys the subtree specified by the node. Helper function only used by
      the destouctor.

      @param node The root node of the subtree to be destroyed.
    */
    void destroySubtree(Node* node) throw() {
      if (node) {
        destroySubtree(node->getLeft());
        destroySubtree(node->getRight());
        delete node;
      }
    }
  public:

    /**
      Initialize an empty binary tree.
    */
    inline BinaryTreeImpl() throw() : root(0) {}

    /**
      Initialize binary tree from other binary tree.
    */
    BinaryTreeImpl(const BinaryTreeImpl& copy) throw(MemoryException) :
      root(copySubtree(copy.root)) {}

    /**
      Returns the root node of the binary tree.
    */
    inline Node* getRoot() throw() {return root;}

    /**
      Makes a left child node.

      @return The new left child node.
    */
    Node* makeLeft(Node* node, const TYPE& value) throw(MemoryException) {
      Node* child = new Node(node, 0, 0, value);
      node->setLeft(child);
      return child;
    }

    /**
      Makes a right child node.

      @return The new right child node.
    */
    Node* makeRight(Node* node, const TYPE& value) throw(MemoryException) {
      Node* child = new Node(node, 0, 0, value);
      node->setRight(child);
      return child;
    }

    /**
      Rotates the specified subtree to the left. Throws 'InvalidNode' if the
      specified node cannot be rotated (i.e. the specified node does not have
      a right child node).

      @param node The subtree to be rotated.
      @return The new parent of the subtree.
    */
    Node* rotateLeft(Node* node) throw(InvalidNode) {
      if (!node || !node->getRight()) {
        throw InvalidNode();
      }
      Node* child = node->getRight();
      node->setRight(child->getLeft());
      child->setLeft(node);

      Node* parent = node->getParent();
      node->setParent(child);
      child->setParent(parent);

      return child;
    }

    /**
      Rotates the specified subtree to the right. Throws 'InvalidNode' if the
      specified node cannot be rotated (i.e. the specified node does not have
      a left child node).

      @param node The subtree to be rotated.
      @return The new parent of the subtree.
    */
    Node* rotateRight(Node* node) throw(InvalidNode) {
      if (!node || !node->getLeft()) {
        throw InvalidNode();
      }
      Node* child = node->getLeft();
      node->setLeft(child->getRight());
      child->setRight(node);

      Node* parent = node->getParent();
      node->setParent(child);
      child->setParent(parent);

      return child;
    }

    /**
      Destroys the binary tree.
    */
    ~BinaryTreeImpl() throw() {
      destroySubtree(root);
      root = 0;
    }
  };
protected:

  /**
    The elements of the binary tree.
  */
  ReferenceCountedObjectPointer<BinaryTreeImpl<TYPE> > elements;
public:

  /**
    Initializes an empty binary tree.
  */
  BinaryTree() throw() {}

  /**
    Initializes binary tree from other binary tree.
  */
  BinaryTree(const BinaryTree& copy) throw() : elements(copy.elements) {}

  /**
    Returns the root node of the binary tree for modifying access.
  */
  Node* getRoot() throw() {
    elements.copyOnWrite();
    return elements->getRoot();
  }

  /**
    Returns the root node of the binary tree for non-modifying access.
  */
  const Node* getRoot() const throw() {
    return elements->getRoot();
  }

  /**
    Removes all the nodes from the binary tree.
  */
  void removeAll() throw() {
    elements = 0; // no need to copy
  }
};

#endif
