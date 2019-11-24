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
#include <base/collection/Pair.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Binary tree with the nodes ordered. All the values of the left and right
  subtrees are respectively less than and greater than the value for any node.

  @short Ordered binary tree
  @ingroup collections
  @version 1.0
*/

template<class TYPE, class KEY = TYPE>
class OrderedBinaryTree : protected BinaryTree<TYPE> {
public:

  /** The type of the value. */
  typedef TYPE Value;
  /** The type of the key. */
  typedef KEY Key;
  /** The type of the node. */
  typedef typename BinaryTree<TYPE>::Node Node;

  /** Modifying enumerator. */
  typedef InfixOrderEnumerator<EnumeratorTraits<Node> > Enumerator;
  /** Non-modifying enumerator. */
  typedef InfixOrderEnumerator<ReadEnumeratorTraits<Node> > ReadEnumerator;
  /** Non-modifying iterator. */
  // typedef InfixOrderIterator<ReadIteratorTraits<Node> > ReadIterator;

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
  OrderedBinaryTree() noexcept
  {
  }

  /**
    Initializes binary tree from other binary tree.
  */
  OrderedBinaryTree(const OrderedBinaryTree& copy)
    : BinaryTree<TYPE>(copy)
  {
  }

  /**
    Assignment of ordered binary tree to ordered binary tree.
  */
  OrderedBinaryTree& operator=(const OrderedBinaryTree& copy)
  {
    BinaryTree<TYPE>::operator=(copy);
    return *this;
  }

  /**
    Returns a modifying enumerator of the ordered binary tree.
  */
  inline Enumerator getEnumerator() noexcept
  {
    return Enumerator(getRoot());
  }

  /**
    Returns a non-modifying enumerator of the ordered binary tree.
  */
  inline ReadEnumerator getReadEnumerator() const noexcept
  {
    return ReadEnumerator(getRoot());
  }

  /**
    Searches for the specified value in this tree.

    @param value The value to look for.

    @return The node with the matching value. nullptr if not found.
  */
  Node* find(const Key& value) noexcept
  {
    Node* node = getRoot();
    while (node) {
      const int result = compare<const Key&>(value, node->getValue());
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

    @return The node with the matching value. nullptr if not found.
  */
  const Node* find(const Key& value) const noexcept
  {
    const Node* node = getRoot();
    while (node) {
      const int result = compare<const Key&>(value, node->getValue());
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

    @return nullptr if the tree is empty.
  */
  Node* getFirst() noexcept
  {
    Node* node = getRoot();
    Node* previous = nullptr; // not found
    while (node) {
      previous = node;
      node = node->getLeft();
    }
    return previous;
  }

  /**
    Returns the last/highest value of this tree.

    @return nullptr if the tree is empty.
  */
  Node* getLast() noexcept
  {
    Node* node = getRoot();
    Node* previous = nullptr; // not found
    while (node) {
      previous = node;
      node = node->getRight();
    }
    return previous;
  }

  void rebalance(Node* node) noexcept
  {
    Allocator<Node*> buffer;
    BinaryTree<TYPE>::BinaryTreeImpl::getNodes(buffer, node);
    setRoot(BinaryTree<TYPE>::BinaryTreeImpl::buildBalancedTree(buffer));
  }

  inline void rebalance() noexcept
  {
    rebalance(getRoot());
    if (!BinaryTree<TYPE>::BinaryTreeImpl::validate(
         getRoot(),
         [](const Node* a, const Node* b) {return compare<const Key&>(a->getValue(), b->getValue()); })
    ) {
      BASSERT(!"OrderedBinaryTree is not ordered correctly.");
    }
  }

  /** Rebalances the given subtree. */
  inline bool rebalance(Node* node, bool right) noexcept
  {
    // auto sl = BinaryTree<TYPE>::BinaryTreeImpl::getSize(node->getLeft());
    // auto sr = BinaryTree<TYPE>::BinaryTreeImpl::getSize(node->getRight());
#if 0
    if (!BinaryTree<TYPE>::BinaryTreeImpl::validate(
          node,
          [](const Node* a, const Node* b) {return compare<const Key&>(a->getValue(), b->getValue()); })
    ) {
      BASSERT(!"OrderedBinaryTree is not ordered correctly.");
    }
#endif
    return false;
  }

  /**
    Adds a value to the binary tree.

    @param value The value to be added.
    @return The node and boolean which is true if value already exists.
  */
  Pair<Node*, bool> add(const Value& value)
  {
    Node* node = getRoot();

    if (!node) {
      auto node = new Node(nullptr, nullptr, nullptr, value);
      this->elements = new typename BinaryTree<Value>::BinaryTreeImpl(node); // attach root node
      return Pair<Node*, bool>(node, true);
    }

    while (true) {
      const int result = compare<const Key&>(value, node->getValue());
      if (result < 0) {
        if (node->getLeft()) {
          node = node->getLeft();
        } else { // attach left child node
          Node* newNode = new Node(node, nullptr, nullptr, value);
          node->setLeft(newNode);
          rebalance(node, false);
          return Pair<Node*, bool>(newNode, true);
        }
      } else if (result > 0) {
        if (node->getRight()) {
          node = node->getRight();
        } else { // attach right child node
          Node* newNode = new Node(node, nullptr, nullptr, value);
          node->setRight(newNode);
          rebalance(node, true);
          return Pair<Node*, bool>(newNode, true);
        }
      } else {
        if (!std::is_same<Value, Key>()) {
          node->getValue() = value; // force assignment
        }
        return Pair<Node*, bool>(node, false); // node with this value already exists
      }
    }
  }

  /**
    Adds a value to the binary tree.

    @param value The value to be added.
    @return The value if it already exists. nullptr if the value was not found in the tree.
  */
  Value* add2(const Value& value)
  {
    Pair<Node*, bool> result = add(value);
    return !result.getSecond() ? &(result.getFirst()->getValue()) : nullptr;
  }
  
  /**
    Adds a value to the binary tree.

    @param value The value to be added.
    @return The node and boolean which is true if value already exists.
  */
  Pair<Node*, bool> add(Value&& value)
  {
    Node* node = getRoot();

    if (!node) {
      auto node = new Node(nullptr, nullptr, nullptr, std::move(value));
      this->elements = new typename BinaryTree<Value>::BinaryTreeImpl(node); // attach root node
      return Pair<Node*, bool>(node, true);
    }

    while (true) {
      const int result = compare<const Key&>(value, node->getValue());
      if (result < 0) {
        if (node->getLeft()) {
          node = node->getLeft();
        } else { // attach left child node
          Node* newNode = new Node(node, nullptr, nullptr, std::move(value));
          node->setLeft(newNode);
          rebalance(node, false);
          return Pair<Node*, bool>(newNode, true);
        }
      } else if (result > 0) {
        if (node->getRight()) {
          node = node->getRight();
        } else { // attach right child node
          Node* newNode = new Node(node, nullptr, nullptr, std::move(value));
          node->setRight(newNode);
          rebalance(node, true);
          return Pair<Node*, bool>(newNode, true);
        }
      } else {
        if (!std::is_same<Value, Key>()) {
          node->getValue() = std::move(value); // force assignment
        }
        return Pair<Node*, bool>(node, false); // node with this value already exists
      }
    }
  }

  /**
    Adds a value to the binary tree.

    @param value The value to be added.
    @return The value if it already exists. nullptr if the value was not found in the tree.
  */
  Value* add2(Value&& value)
  {
    Pair<Node*, bool> result = add(std::move(value));
    return !result.getSecond() ? &(result.getFirst()->getValue()) : nullptr;
  }
  
  /**
    Removes the specified node from the binary tree. Raises InvalidNode if
    the node is invalid.

    @param node The node to be removed.
  */
  void remove(Node* node) throw(InvalidNode)
  {
    if (!node) {
      throw InvalidNode();
    }

    this->elements.copyOnWrite();

    if (node == getRoot()) { // set new root
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
        break; // stop when the node is a leaf in the tree
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

  template<class PREDICATE>
  MemorySize removeImpl(Node* node, PREDICATE predicate)
  {
    if (!node) {
      return 0;
    }
    MemorySize count = 0;
    if (auto left = node->getLeft()) {
      count = removeImpl(left, predicate);
    }
    if (auto right = node->getRight()) {
      count += removeImpl(right, predicate);
    }
    if (predicate(node)) {
      remove(node);
      ++count;
    }
    return count;
  }

  /** Removes all elements matching the given predicate. */
  template<class PREDICATE>
  MemorySize remove(PREDICATE predicate)
  {
    return removeImpl(getRoot(), predicate);
  }

  /**
    Removes all the nodes from the tree.
  */
  void removeAll()
  {
    BinaryTree<TYPE>::removeAll();
  }
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
