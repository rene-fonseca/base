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

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Enumeration of all the elements of a binary tree traversed in prefix order.

  @author René Møller Fonseca
  @version 1.0
*/
template<class TYPE, class REF, class PTR>
class PrefixOrderEnumeration : Enumeration<TYPE, REF, PTR> {
public:

  /** The type of the value. */
  typedef TYPE Value;
  /** The type of a reference to value. */
  typedef REF Reference;
  /** The type of a pointer to value. */
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
  PrefixOrderEnumeration(Pointer n) throw() : root(n), node(n), more(n != 0) {}

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



/**
  Enumeration of all the elements of a binary tree traversed in infix order.

  @author René Møller Fonseca
  @version 1.0
*/
template<class TYPE, class REF, class PTR>
class InfixOrderEnumeration : Enumeration<TYPE, REF, PTR> {
public:

  /** The type of the value. */
  typedef TYPE Value;
  /** The type of a reference to value. */
  typedef REF Reference;
  /** The type of a pointer to value. */
  typedef PTR Pointer;
private:

  /** The states of the infix order traversal state machine */
  typedef enum {TRAVERSE_SUBTREE, RETURN_LEFT, RETURN_RIGHT} Traverse;
  /** The root of the binary tree. */
  Pointer root;
  /** The current position in the enumeration. */
  Pointer node;
  /** Specifies that more elements are available. */
  bool more;
  /** Specifies that subtree should be traversed. */
  Traverse traverse;
public:

  /**
    Initializes enumeration of binary tree specified by the node.

    @param n The root node of the binary tree.
  */
  InfixOrderEnumeration(Pointer n) throw() : root(n), node(n), more(n != 0), traverse(TRAVERSE_SUBTREE) {}

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

    Pointer result = 0; // indicate no result

    while (true) { // keep looking until we know if end has been reached

      switch (traverse) {
      case TRAVERSE_SUBTREE:
        while (node->getLeft()) { // optimized version
          node = node->getLeft();
        }
        traverse = RETURN_LEFT;

//        if (node->getLeft()) {
//          node = node->getLeft();
//          traverse = TRAVERSE_SUBTREE;
        void* a;
//        } else {
//          traverse = RETURN_LEFT;
//        }
//        break;

      case RETURN_LEFT:
        if (result) { // stop if we already have found a result
          return result;
        }
        result = node; // this node is the next result
        if (node->getRight()) { // has right node
          node = node->getRight();
          traverse = TRAVERSE_SUBTREE;
          break;
        }
//        if (result) { // stop if we already have found a result
//          return result;
//        }
//        result = node; // this node is the next result
//        if (node->getRight()) { // has right node
//          node = node->getRight();
//          traverse = TRAVERSE_SUBTREE;
//        } else {
        void* b;
//          if (node == root) {
//            more = false; // we have reached the end of the enumeration
//            return result;
//          }
//          Pointer child = node;
//          node = node->getParent();
//          if (child == node->getLeft()) {
        void* c;
//            traverse = RETURN_LEFT;
//          } else {
//            traverse = RETURN_RIGHT;
//          }
//        }
//        break;

      case RETURN_RIGHT:
        Pointer child;
        do {
          if (node == root) {
            more = false; // we have reached the end of the enumeration
            return result;
          }
          child = node;
          node = node->getParent();
        } while (child == node->getRight());
        traverse = RETURN_LEFT;

//        if (node == root) {
//          more = false; // we have reached the end of the enumeration
//          return result;
//        }
//        Pointer child = node;
//        node = node->getParent();
	void* d;
//        if (child == node->getLeft()) {
//          traverse = RETURN_LEFT;
//        } else {
//          traverse = RETURN_RIGHT;
//        }
//        break;
      }
    }
  }
};



/**
  Enumeration of all the elements of a binary tree traversed in infix order.

  @author René Møller Fonseca
  @version 1.0
*/
template<class TYPE, class REF, class PTR>
class PostfixOrderEnumeration : Enumeration<TYPE, REF, PTR> {
public:

  /** The type of the value. */
  typedef TYPE Value;
  /** The type of a reference to value. */
  typedef REF Reference;
  /** The type of a pointer to value. */
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
  PostfixOrderEnumeration(Pointer n) throw() : root(n), node(n), more(n != 0) {}

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

    Pointer result = 0;
while (!result) {
    if (node->getLeft()) { // traverse left subtree
      node = node->getLeft();
    } else if (node->getRight()) { // traverse right subtree
      node = node->getRight();
    } else {
      Pointer temp;
      do { // return from subtree
        result = node;
        if (node == root) {
          more = false;
          return result;
        }
        temp = node;
        node = node->getParent();
      } while ((!node->getRight()) || (temp == node->getRight()));

      node = node->getRight(); // traverse right subtree - which we know exists
    }
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
    explicit inline BinaryTreeImpl() throw() : root(0) {}

    /**
      Initialize a binary tree with the specified root node.

      @param node The root node of the tree.
    */
    explicit inline BinaryTreeImpl(Node* node) throw() : root(node) {}

    /**
      Initialize binary tree from other binary tree.
    */
    BinaryTreeImpl(const BinaryTreeImpl& copy) throw(MemoryException) :
      root(copySubtree(copy.root)) {}

    /**
      Returns true if the binary tree is empty.
    */
    inline bool isEmpty() const throw() {return root == 0;}

    /**
      Returns the root node of the binary tree.
    */
    inline Node* getRoot() throw() {return root;}

    /**
      Sets the root of the binary tree.
    */
    inline void setRoot(Node* node) throw() {root = node;}

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

      if (parent) {
        if (node == parent->getLeft()) {
          parent->setLeft(child);
        } else {
          parent->setRight(child);
        }
      }

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

      if (parent) {
        if (node == parent->getLeft()) {
          parent->setLeft(child);
        } else {
          parent->setRight(child);
        }
      }

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
  BinaryTree() throw() : elements(new BinaryTreeImpl()) {}

  /**
    Initializes binary tree from other binary tree.
  */
  BinaryTree(const BinaryTree& copy) throw() : elements(copy.elements) {}

  /**
    Returns true if the tree has a root node.
  */
  inline bool isEmpty() const throw() {
    return elements->isEmpty();
  }

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
    Sets the binary tree.

    @param node The root of the tree.
  */
  void setRoot(Node* node) throw() {
    elements = new BinaryTreeImpl(node); // no need to copy
  }

  /**
    Removes all the nodes from the binary tree.
  */
  void removeAll() throw() {
    elements = new BinaryTreeImpl(); // no need to copy
  }
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
