/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__BINARY_NODE_H
#define _DK_SDU_MIP__BASE_COLLECTION__BINARY_NODE_H

#include <base/features.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A binary linked node. This class is used by the binary tree collection.

  @short Binary node.
  @see BinaryTree
  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE>
class BinaryNode {
private:

  /** The parent node of this node. */
  BinaryNode* parent;
  /** The left child node. */
  BinaryNode* left;
  /** The right child node. */
  BinaryNode* right;
  /** The value associated with the node. */
  TYPE value;
public:

  /**
    Initializes a binary node.

    @param p The parent node of this node.
    @param l The left child node.
    @param r The right child node.
    @param v The value to be associated with the node.
  */
  inline BinaryNode(BinaryNode* p, BinaryNode* l, BinaryNode* r, const TYPE& v) :
    parent(p), left(l), right(r), value(v) {}

  /**
    Returns the parent node.
  */
  inline BinaryNode* getParent() const throw() {return parent;}

  /**
    Sets the parent node.
  */
  inline void setParent(BinaryNode* parent) throw() {this->parent = parent;}

  /**
    Returns the left child node.
  */
  inline BinaryNode* getLeft() const throw() {return left;}

  /**
    Sets the left child node.
  */
  inline void setLeft(BinaryNode* left) throw() {this->left = left;}

  /**
    Returns the right child node.
  */
  inline BinaryNode* getRight() const throw() {return right;}

  /**
    Sets the right child node.
  */
  inline void setRight(BinaryNode* right) throw() {this->right = right;}

  /**
    Returns the value of the node.
  */
  inline TYPE* getValue() throw() {return &value;}

  /**
    Returns the value of the node.
  */
  inline const TYPE* getValue() const throw() {return &value;}

  /**
    Sets the value of the node.
  */
  inline void setValue(TYPE value) throw() {this->value = value;}
};

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
