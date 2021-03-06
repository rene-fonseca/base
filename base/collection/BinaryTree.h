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

#include <base/collection/Collection.h>
#include <base/collection/BinaryNode.h>
#include <base/collection/InvalidNode.h>
#include <base/collection/Enumeration.h>
#include <base/collection/Pair.h>
#include <base/string/FormatOutputStream.h>
#include <base/mem/Reference.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/** Binary tree iterator in the order root, left, and right. */
template<class TRAITS>
class PrefixOrderIterator : public Iterator<TRAITS> {
public:

  typedef typename Iterator<TRAITS>::Value Value;
  typedef typename Iterator<TRAITS>::Reference Reference;
  typedef typename Iterator<TRAITS>::Pointer Pointer;
  typedef typename Iterator<TRAITS>::Distance Distance;
  typedef ForwardIterator Category;
private:
  
  /** The root of the binary tree. */
  Pointer root = nullptr;
  /** The current position of the iterator. */
  Pointer node = nullptr;
public:

  inline PrefixOrderIterator() noexcept
  {
  }

  inline PrefixOrderIterator(Pointer _root) noexcept
    : root(_root), node(_root)
  {
    // starting with root
  }

  inline bool operator==(const PrefixOrderIterator& compare) const noexcept
  {
    return node == compare.node;
  }

  inline bool operator!=(const PrefixOrderIterator& compare) const noexcept
  {
    return node != compare.node;
  }

  inline PrefixOrderIterator& operator++()
  {
    if (!node) {
      _throw EndOfEnumeration();
    }

    if (auto left = node->getLeft()) { // traverse left subtree
      node = left;
    } else if (auto right = node->getRight()) { // traverse right subtree
      node = right;
    } else { // leaf
      Pointer child = nullptr;
      do { // return from subtree
        if (node == root) {
          node = nullptr;
          return *this; // end of tree
        }
        child = node;
        node = node->getParent();
      } while (!node->getRight() || (child == node->getRight()));

      node = node->getRight(); // traverse right subtree - which we know exists
    }

    return *this;
  }
  
  inline PrefixOrderIterator operator++(int)
  {
    PrefixOrderIterator result(*this);
    ++result;
    return result;
  }

  inline Pointer operator->() const noexcept
  {
    return node;
  }

  inline Reference operator*() const noexcept
  {
    return *node;
  }

  inline operator bool() const noexcept
  {
    return node;
  }
};

/** Binary tree iterator in the order left, right, and root. */
template<class TRAITS>
class PostfixOrderIterator : public Iterator<TRAITS> {
public:

  typedef typename Iterator<TRAITS>::Value Value;
  typedef typename Iterator<TRAITS>::Reference Reference;
  typedef typename Iterator<TRAITS>::Pointer Pointer;
  typedef typename Iterator<TRAITS>::Distance Distance;
  typedef ForwardIterator Category;
private:

  /** The states of the traversal state machine */
  enum Traverse {TRAVERSE_SUBTREE, RETURN_LEFT, RETURN_RIGHT};
  /** The root of the binary tree. */
  Pointer root = nullptr;
  /** The internal position. */
  Pointer node = nullptr;
  /** The current position. */
  Pointer current = nullptr;
  /** Specifies that subtree should be traversed. */
  Traverse traverse = TRAVERSE_SUBTREE;

  Pointer next()
  {
    if (!node) {
      _throw EndOfEnumeration();
    }

    while (true) { // keep looking until we know if end has been reached
      switch (traverse) {
      case TRAVERSE_SUBTREE:
        while (auto left = node->getLeft()) {
          node = left;
        }
        traverse = RETURN_LEFT;
        // [[fallthrough]]
      case RETURN_LEFT:
        if (auto right = node->getRight()) { // has right node
          node = right;
          traverse = TRAVERSE_SUBTREE;
          break;
        }
        traverse = RETURN_RIGHT;
        // [[fallthrough]]
      case RETURN_RIGHT:
        Pointer result = node; // this node is the next result
        if (node == root) {
          node = nullptr; // we have reached the end of the enumeration
          return result;
        }
        Pointer child = node;
        node = node->getParent();
        if (child == node->getLeft()) {
          traverse = RETURN_LEFT;
        }
        return result;
      }
    }
  }

#if 0
      if (auto left = node->getLeft()) { // traverse left subtree
        node = left;
      } else if (auto right = node->getRight()) { // traverse right subtree
        node = right;
      } else { // leaf
        Pointer child = nullptr;
        do { // return from subtree
          if (result) {
            return result;
          }
          result = node;
          if (node == root) {
            node = nullptr;
            return result;
          }
          child = node;
          node = node->getParent();
        } while (!node->getRight() || (child == node->getRight()));

        node = node->getRight(); // traverse right subtree - which we know exists
      }
#endif
public:

  inline PostfixOrderIterator() noexcept
  {
  }

  inline PostfixOrderIterator(Pointer _root) noexcept
    : root(_root), node(_root)
  {
    current = node ? next() : nullptr; // get first node
  }

  inline bool operator==(const PostfixOrderIterator& compare) const noexcept
  {
    return current == compare.current;
  }

  inline bool operator!=(const PostfixOrderIterator& compare) const noexcept
  {
    return current != compare.current;
  }

  inline PostfixOrderIterator& operator++()
  {
    current = node ? next() : nullptr;
    return *this;
  }
  
  inline PostfixOrderIterator operator++(int)
  {
    PostfixOrderIterator result(*this);
    ++result;
    return result;
  }

  inline Pointer operator->() const noexcept
  {
    return current;
  }

  inline Reference operator*() const noexcept
  {
    return *current;
  }

  inline operator bool() const noexcept
  {
    return current;
  }
};

/** Binary tree iterator in the order left, root, and right. */
template<class TRAITS>
class InfixOrderIterator : public Iterator<TRAITS> {
public:

  typedef typename Iterator<TRAITS>::Value Value;
  typedef typename Iterator<TRAITS>::Reference Reference;
  typedef typename Iterator<TRAITS>::Pointer Pointer;
  typedef typename Iterator<TRAITS>::Distance Distance;
  typedef ForwardIterator Category;
private:
  
  /** The states of the traversal state machine */
  enum Traverse {TRAVERSE_SUBTREE, RETURN_LEFT, RETURN_RIGHT};
  /** The root of the binary tree. */
  Pointer root = nullptr;
  /** The internal position. */
  Pointer node = nullptr;
  /** The current position. */
  Pointer current = nullptr;
  /** Specifies that subtree should be traversed. */
  Traverse traverse = TRAVERSE_SUBTREE;

  Pointer next()
  {
    if (!node) {
      _throw EndOfEnumeration();
    }

    Pointer result = nullptr; // indicate no result
    while (true) { // keep looking until we know if end has been reached
      switch (traverse) {
      case TRAVERSE_SUBTREE:
        while (auto left = node->getLeft()) {
          node = left;
        }
        traverse = RETURN_LEFT;
        // [[fallthrough]]
      case RETURN_LEFT:
        if (result) { // stop if we already have found a result
          return result;
        }
        result = node; // this node is the next result
        if (auto right = node->getRight()) { // has right node
          node = right;
          traverse = TRAVERSE_SUBTREE;
          break;
        }
        traverse = RETURN_RIGHT;
        // [[fallthrough]]
      case RETURN_RIGHT:
        Pointer child = nullptr;
        do {
          if (node == root) {
            node = nullptr; // we have reached the end of the enumeration
            return result;
          }
          child = node;
          node = node->getParent();
        } while (child == node->getRight());
        traverse = RETURN_LEFT;
      }
    }
    return result;
  }
public:

  InfixOrderIterator() noexcept
  {
  }

  /**
    Initializes iterator of binary tree specified by the node.

    @param root The root node of the binary tree.
  */
  InfixOrderIterator(Pointer _root) noexcept
    : root(_root), node(_root)
  {
    current = node ? next() : nullptr; // get first node
  }

  inline bool operator==(const InfixOrderIterator& compare) const noexcept
  {
    return current == compare.current;
  }

  inline bool operator!=(const InfixOrderIterator& compare) const noexcept
  {
    return current != compare.current;
  }

  inline InfixOrderIterator& operator++()
  {
    current = node ? next() : nullptr;
    return *this;
  }

  inline InfixOrderIterator operator++(int)
  {
    InfixOrderIterator result(*this);
    ++result;
    return result;
  }

  inline Pointer operator->() const noexcept
  {
    return current;
  }

  inline Reference operator*() const noexcept
  {
    return *current;
  }

  inline operator bool() const noexcept
  {
    return current;
  }
};

/**
  Enumeration of all the elements of a binary tree traversed in prefix order.
  
  @short Prefix order enumerator
  @ingroup collections
*/
template<class TRAITS>
class PrefixOrderEnumerator : public Enumerator<TRAITS> {
private:

  typedef typename Enumerator<TRAITS>::Value Value;
  typedef typename Enumerator<TRAITS>::Reference Reference;
  typedef typename Enumerator<TRAITS>::Pointer Pointer;
  
  /** The root of the binary tree. */
  Pointer root = nullptr;
  /** The current position of the enumeration. */
  Pointer node = nullptr;
public:

  /**
    Initializes enumeration of binary tree specified by the node.

    @param node The root node of the binary tree.
  */
  PrefixOrderEnumerator(Pointer _node) noexcept
    : root(_node), node(_node)
  {
  }

  /**
    Returns true if there is more elements in this enumeration.
  */
  inline bool hasNext() const noexcept
  {
    return node;
  }

  /**
    Returns the current value and increments the position. Raises
    EndOfEnumeration if the end has been reached.
  */
  Reference next()
  {
    if (!node) {
      _throw EndOfEnumeration();
    }

    Pointer result = node;

    if (auto left = node->getLeft()) { // traverse left subtree
      node = left;
    } else if (auto right = node->getRight()) { // traverse right subtree
      node = right;
    } else {
      Pointer child = nullptr;
      do { // return from subtree
        if (node == root) {
          node = nullptr;
          return result;
        }
        child = node;
        node = node->getParent();
      } while (!node->getRight() || (child == node->getRight()));

      node = node->getRight(); // traverse right subtree - which we know exists
    }

    return result;
  }
};



/**
  Enumeration of all the elements of a binary tree traversed in infix order.

  @short Infix order enumerator
  @ingroup collections
*/
template<class TRAITS>
class InfixOrderEnumerator : public Enumerator<TRAITS> {
private:

  typedef typename Enumerator<TRAITS>::Value Value;
  typedef typename Enumerator<TRAITS>::Reference Reference;
  typedef typename Enumerator<TRAITS>::Pointer Pointer;

  /** The states of the traversal state machine */
  enum Traverse {TRAVERSE_SUBTREE, RETURN_LEFT, RETURN_RIGHT};
  /** The root of the binary tree. */
  Pointer root = nullptr;
  /** The current position in the enumeration. */
  Pointer node = nullptr;
  /** Specifies that subtree should be traversed. */
  Traverse traverse = TRAVERSE_SUBTREE;
public:

  InfixOrderEnumerator() noexcept
  {
  }

  /**
    Initializes enumeration of binary tree specified by the node.

    @param node The root node of the binary tree.
  */
  InfixOrderEnumerator(Pointer _node) noexcept
    : root(_node), node(_node)
  {
  }

  /**
    Returns true if there is more elements in this enumeration.
  */
  inline bool hasNext() const noexcept
  {
    return node;
  }

  /**
    Returns the current value and increments the position. Raises
    EndOfEnumeration if the end has been reached.
  */
  Reference next()
  {
    if (!node) {
      _throw EndOfEnumeration();
    }

    Pointer result = nullptr; // indicate no result
    while (true) { // keep looking until we know if end has been reached
      switch (traverse) {
      case TRAVERSE_SUBTREE:
        while (auto left = node->getLeft()) {
          node = left;
        }
        traverse = RETURN_LEFT;
        // [[fallthrough]]
      case RETURN_LEFT:
        if (result) { // stop if we already have found a result
          return *result;
        }
        result = node; // this node is the next result
        if (auto right = node->getRight()) { // has right node
          node = right;
          traverse = TRAVERSE_SUBTREE;
          break;
        }
        traverse = RETURN_RIGHT;
        // [[fallthrough]]
      case RETURN_RIGHT:
        Pointer child = nullptr;
        do {
          if (node == root) {
            node = nullptr; // we have reached the end of the enumeration
            return *result;
          }
          child = node;
          node = node->getParent();
        } while (child == node->getRight());
        traverse = RETURN_LEFT;
      }
    }
  }
};



/**
  Enumeration of all the elements of a binary tree traversed in postfix order.

  @short Postfix order enumerator
  @ingroup collections
*/
template<class TRAITS>
class PostfixOrderEnumerator : public Enumerator<TRAITS> {
private:

  typedef typename Enumerator<TRAITS>::Value Value;
  typedef typename Enumerator<TRAITS>::Reference Reference;
  typedef typename Enumerator<TRAITS>::Pointer Pointer;

  /** The root of the binary tree. */
  Pointer root = nullptr;
  /** The current position in the enumeration. */
  Pointer node = nullptr;
public:

  /**
    Initializes enumeration of binary tree specified by the node.

    @param node The root node of the binary tree.
  */
  PostfixOrderEnumerator(Pointer _node) noexcept
    : root(_node), node(_node)
  {
  }

  /**
    Returns true if there is more elements in this enumeration.
  */
  inline bool hasNext() const noexcept
  {
    return node;
  }

  /**
    Returns the current value and increments the position. Raises
    EndOfEnumeration if the end has been reached.
  */
  Reference next()
  {
    if (!node) {
      _throw EndOfEnumeration();
    }

    Pointer result = nullptr;
    while (!result) {
      if (auto left = node->getLeft()) { // traverse left subtree
        node = left;
      } else if (auto right = node->getRight()) { // traverse right subtree
        node = right;
      } else {
        Pointer child = nullptr;
        do { // return from subtree
          if (result) {
            return *result;
          }
          result = node;
          if (node == root) {
            node = nullptr;
            return *result;
          }
          child = node;
          node = node->getParent();
        } while (!node->getRight() || (child == node->getRight()));

        node = node->getRight(); // traverse right subtree - which we know exists
      }
    }
    return *result;
  }
};



//  /**
//    Invocates the specified unary operation for each element (non-modifying operation).
//  */
//  template<class TYPE, class UNOPR>
//  void traversePrefixOrder(const BinaryNode* root, UNOPR& function) noexcept
//  {
//    const BinaryNode* node = root;
//    while (true) {
//      function(node);
//
//      if (auto left = node->getLeft()) { // traverse left subtree
//        node = left;
//      } else if (auto right = node->getRight()) { // traverse right subtree
//        node = right;
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
//  void traverseInfixOrder(const BinaryNode* root, UNOPR& function) noexcept
//  {
//    const BinaryNode* node = root;
//    while (true) {
//      if (auto left = node->getLeft()) { // traverse left subtree
//        node = left;
//      } else if (auto right = node->getRight()) { // traverse right subtree
//        function(node);
//        node = right;
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
//  void traversePostfixOrder(const BinaryNode* root, UNOPR& function) noexcept
//  {
//    const BinaryNode* node = root;
//    while (true) {
//      if (auto left = node->getLeft()) { // traverse left subtree
//        node = left;
//      } else if (auto right = node->getRight()) { // traverse right subtree
//        node = right;
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
  Binary tree data structure.

  @short Binary tree
  @ingroup collections
*/

template<class TYPE>
class BinaryTree : public Collection {
public:

  /** The type of the value. */
  typedef TYPE Value;
  /** The type of a node. */
  typedef BinaryNode<TYPE> Node;

  typedef PrefixOrderEnumerator<EnumeratorTraits<Node> > Enumerator;
  typedef PrefixOrderEnumerator<ReadEnumeratorTraits<Node> > ReadEnumerator;
  typedef PrefixOrderIterator<ReadIteratorTraits<Node> > ReadIterator;

  /*
    Internal binary tree implementation.
  */
  class BinaryTreeImpl : public ReferenceCountedObject {
  private:

    /** The root node of the binary tree. */
    Node* root = nullptr;
  protected:

    /**
      Makes a copy of a subtree specified by the top node.

      @param node The top node of the subtree to be copied.
      @return The top node of the new subtree.
    */
    Node* copySubtree(const Node* node)
    {
      if (node) {
        Node* left = copySubtree(node->getLeft());
        Node* right = copySubtree(node->getRight());
        Node* result = new Node(nullptr, left, right, node->getValue());
        if (left) {
          left->setParent(result);
        }
        if (right) {
          right->setParent(result);
        }
        return result;
      } else {
        return nullptr;
      }
    }

    /**
      Destroys the subtree specified by the node. Helper function only used by
      the destouctor.

      @param node The root node of the subtree to be destroyed.
    */
    void destroySubtree(Node* node)
    {
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
    explicit inline BinaryTreeImpl() noexcept
    {
    }

    /**
      Initialize a binary tree with the specified root node.

      @param node The root node of the tree.
    */
    explicit inline BinaryTreeImpl(Node* node) noexcept
      : root(node)
    {
    }

    /**
      Initialize binary tree from other binary tree.
    */
    BinaryTreeImpl(const BinaryTreeImpl& copy)
      : root(copySubtree(copy.root))
    {
    }

    /**
      Returns true if the binary tree is empty.
    */
    inline bool isEmpty() const noexcept
    {
      return root == nullptr;
    }

    /**
      Returns the root node of the binary tree.
    */
    inline Node* getRoot() noexcept
    {
      return root;
    }

    /**
      Returns the root node of the binary tree.
    */
    inline const Node* getRoot() const noexcept
    {
      return root;
    }

    /**
      Sets the root of the binary tree.
    */
    inline void setRoot(Node* node) noexcept
    {
      root = node;
    }

    /**
      Makes a left child node.

      @return The new left child node.
    */
    static Node* makeLeft(Node* node, const TYPE& value)
    {
      Node* child = new Node(node, nullptr, nullptr, value);
      node->setLeft(child);
      return child;
    }

    /**
      Makes a left child node.

      @return The new left child node.
    */
    static Node* makeLeft(Node* node, TYPE&& value)
    {
      Node* child = new Node(node, nullptr, nullptr, moveObject(value));
      node->setLeft(child);
      return child;
    }

    /**
      Makes a right child node.

      @return The new right child node.
    */
    static Node* makeRight(Node* node, const TYPE& value)
    {
      Node* child = new Node(node, nullptr, nullptr, value);
      node->setRight(child);
      return child;
    }

    /**
      Makes a right child node.

      @return The new right child node.
    */
    static Node* makeRight(Node* node, TYPE&& value)
    {
      Node* child = new Node(node, nullptr, nullptr, moveObject(value));
      node->setRight(child);
      return child;
    }

    /**
      Rotates the specified subtree to the left. Raises InvalidNode if the
      specified node cannot be rotated (i.e. the specified node does not have
      a right child node).

      @param node The subtree to be rotated.
      @return The new parent of the subtree.
    */
    static Node* rotateLeft(Node* node)
    {
      if (!node || !node->getRight()) {
        _throw InvalidNode();
      }
      Node* parent = node->getParent();

      Node* child = node->getRight();
      node->setRight(child->getLeft());
      if (auto left = child->getLeft()) {
        left->setParent(node);
      }
      child->setLeft(node);
      node->setParent(child);
      child->setParent(parent);

      if (parent) {
        if (node == parent->getLeft()) {
          parent->setLeft(child);
        }
        else {
          parent->setRight(child);
        }
      }

      return child;
    }

    /**
      Rotates the specified subtree to the right. Raises InvalidNode if the
      specified node cannot be rotated (i.e. the specified node does not have
      a left child node).

      @param node The subtree to be rotated.
      @return The new parent of the subtree.
    */
    static Node* rotateRight(Node* node)
    {
      if (!node || !node->getLeft()) {
        _throw InvalidNode();
      }
      Node* parent = node->getParent();

      Node* child = node->getLeft();
      node->setLeft(child->getRight());
      if (auto right = child->getRight()) {
        right->setParent(node);
      }
      child->setRight(node);
      node->setParent(child);
      child->setParent(parent);

      if (parent) {
        if (node == parent->getLeft()) {
          parent->setLeft(child);
        }
        else {
          parent->setRight(child);
        }
      }

      return child;
    }

    /** Returns the size of the given subtree. */
    static MemorySize getSize(const Node* node) noexcept
    {
      if (!node) {
        return 0;
      }
      return 1 + getSize(node->getLeft()) + getSize(node->getRight());
    }

#if 0
    static Node** getNodesImpl(Node** dest, Node* node)
    {
      if (auto left = node->getLeft()) {
        dest = getNodesImpl(dest, left);
      }
      *dest++ = node;
      if (auto right = node->getRight()) {
        dest = getNodesImpl(dest, right);
      }
      return dest;
    }
#endif

    /** Gets all the nodes for the given node. */
    static void getNodes(Allocator<Node*>& buffer, Node* node)
    {
      const MemorySize size = getSize(node);
      buffer.setSize(size);
      if (node) {
        Node** dest = buffer.getElements();
        for (InfixOrderIterator<IteratorTraits<Node> > src(node); src; ++dest, ++src) { // no need for end iterator
          *dest = &*src;
        }
        // getNodesImpl(dest, node);
      }
    }

    static Node* buildBalancedTree(Span<Node*> span)
    {
      const auto size = span.getSize();
      const auto m = size / 2;
      Node* parent = span[m];
      auto left = (m > 0) ? buildBalancedTree(Span<Node*>(span.begin(), m)) : nullptr;
      parent->setLeft(left);
      if (left) {
        left->setParent(parent);
      }      
      auto right = ((m + 1) < size) ? buildBalancedTree(Span<Node*>(span.begin() + m + 1, span.end())) : nullptr;
      parent->setRight(right);
      if (right) {
        right->setParent(parent);
      }
      return parent;
    }

    /** Builds a balanced tree. */
    static Node* buildBalancedTree(Allocator<Node*>& buffer)
    {
      if (buffer.getSize() == 0) {
        return nullptr;
      }
      auto parent = buildBalancedTree(buffer.getSpan());
      parent->setParent(nullptr); // root
      return parent;
    }

    /** Builds a balanced tree. But uses buffer and rebuilds entire tree. */
    void balanceTree()
    {
      if (root) {
        Allocator<Node*> buffer; // also need balancing without using buffer
        getNodes(buffer, root);
        root = buildBalancedTree(buffer);
      }
    }

    static Pair<bool, MemorySize> isBalancedImpl(const Node* node) noexcept
    {
      if (!node) {
        return Pair<bool, MemorySize>(true, 0);
      }
      const auto left = isBalanced(node->getLeft());
      if (!left.first) {
        return left; // early stop
      }
      const auto right = isBalanced(node->getRight());
      if (!right.first) {
        return right; // early stop
      }
      const bool balanced = (left.second <= right.second) ?
        ((right.second - left.second) <= 1) : ((left.second - right.second) <= 1);
      return Pair<bool, MemorySize>(
        /*left.first && right.first &&*/ balanced,
        maximum(left.second, right.second) + 1
        );
    }

    /** Returns true if the tree is balanced. */
    static bool isBalanced(const Node* node) noexcept
    {
      return isBalancedImpl(node).first;
    }

    /** Returns how balanced the given tree is. */
    static Pair<MemorySize, MemorySize> getHowBalancedImpl(const Node* node)
    {
      // how much 'work' must be done to make it balanced
      if (!node) {
        return Pair<MemorySize, MemorySize>(0, 0);
      }
      const auto left = getHowBalancedImpl(node->getLeft());
      const auto right = getHowBalancedImpl(node->getRight());
      const MemorySize size = left.first + right.first + 1;
      const MemorySize work = left.second + right.second;
      if (left.first <= right.first) {
        work += right.first - left.first;
      }
      else {
        work += left.first - right.first;
      }
      return Pair<MemorySize, MemorySize>(size, work);
    }

    /** Returns how balanced the given tree is. */
    static Pair<MemorySize, MemorySize> getHowBalanced(const Node* node)
    {
      return getHowBalancedImpl(node).second;
    }

    /** Validates the tree for the given predicate. */
    template<class PREDICATE>
    static bool validate(const Node* node, PREDICATE predicate)
    {
#if 0
      const Node* previous = nullptr;
      for (const Node* node : iterable(node)) {
        if (previous) {
          if (!predicate(previous, node)) {
            return false;
          }
        }
        previous = node;
      }
      return true;
#endif
      if (node) { // avoid recursion
        if (auto left = node->getLeft()) {
          if (!predicate(left, node)) {
            return false;
          }
          if (!validate(left, predicate)) {
            return false;
          }
        }
        if (auto right = node->getRight()) {
          if (!predicate(node, right)) {
            return false;
          }
          if (!validate(right, predicate)) {
            return false;
          }
        }
      }
      return true;
    }

#if 0
    /** Builds a balanced tree. */
    MemorySize rebalanceTree(Node* node)
    {
      // TAG: we only want to do the minimum work
    }
#endif
    
    /**
      Destroys the binary tree.
    */
    ~BinaryTreeImpl()
    {
      destroySubtree(root);
      root = nullptr;
    }
  };
protected:

  /**
    The elements of the binary tree.
  */
  Reference<BinaryTreeImpl> elements;
public:

  /**
    Initializes an empty binary tree.
  */
  BinaryTree()
    : elements(new BinaryTreeImpl())
  {
  }

  /**
    Initializes binary tree from other binary tree.
  */
  inline BinaryTree(const BinaryTree& copy) noexcept
    : elements(copy.elements)
  {
  }

  /**
    Assignment of binary tree to binary tree.
  */
  inline BinaryTree& operator=(const BinaryTree& copy) noexcept
  {
    elements = copy.elements;
    return *this;
  }
  
  /**
    Returns true if the tree has a root node.
  */
  inline bool isEmpty() const noexcept
  {
    return elements->isEmpty();
  }

  /**
    Returns the root node of the binary tree for modifying access.
  */
  Node* getRoot()
  {
    elements.copyOnWrite();
    return elements->getRoot();
  }

  /**
    Returns the root node of the binary tree for non-modifying access.
  */
  inline const Node* getRoot() const noexcept
  {
    return elements->getRoot();
  }

  /**
    Returns a modifying enumerator of the binary tree.
  */
  inline Enumerator getEnumerator() noexcept
  {
    return Enumerator(getRoot());
  }

  /**
    Returns a non-modifying enumerator of the binary tree.
  */
  inline ReadEnumerator getReadEnumerator() const noexcept
  {
    return ReadEnumerator(getRoot());
  }

  /**
    Sets the binary tree.

    @param node The root of the tree.
  */
  void setRoot(Node* node)
  {
    elements.copyOnWrite();
    elements->setRoot(node);
  }

  /**
    Removes all the nodes from the binary tree.
  */
  void removeAll()
  {
    elements = new BinaryTreeImpl(); // no need to copy
  }
};

template<class TYPE>
class OutputBinaryTree {
private:

  FormatOutputStream& stream;
  unsigned int i = 0;
  bool left = false;
public:
  
  OutputBinaryTree(FormatOutputStream& _stream)
    : stream(_stream)
  {
  }

  void writeNode(const BinaryNode<TYPE>* node)
  {
    if (!node) {
      return;
    }
    const bool temp = left;
    if (node->getLeft()) {
      left = true;
      ++i;
      writeNode(node->getLeft()); // /--
      --i;
    }
    const bool leaf = !node->getLeft() && !node->getRight();
    stream << indent(i * 2) << "| " << node->getValue() << (leaf ? "*" : "") << EOL;
    if (node->getRight()) {
      left = false;
      ++i;
      writeNode(node->getRight()); // \--
      --i;
    }
    left = temp;
  }
};

template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const BinaryNode<TYPE>* node)
{
  if (node) {
    OutputBinaryTree<TYPE> outputBinaryTree(stream);
    outputBinaryTree.writeNode(node, indent);
  }
  return stream;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
