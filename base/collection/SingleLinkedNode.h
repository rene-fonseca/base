/***************************************************************************
    copyright   : (C) 2000 by Ren� M�ller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__SINGLE_LINKED_NODE_H
#define _DK_SDU_MIP__BASE_COLLECTION__SINGLE_LINKED_NODE_H

/**
  A single linked node. This class is used by collections that only require
  unidirectional traversal of elements.

  @short Single linked node.
  @author Ren� M�ller Fonseca
  @version 1.0
*/

template<class TYPE>
class SingleLinkedNode {
protected:

  /** The next node in the container. */
  SingleLinkedNode* next;
  /** The value associated with the node. */
  TYPE value;
public:

  /**
    Initializes a linked node.

    @param n The next node in the container.
    @param v The value to be associated with the node.
  */
  inline SingleLinkedNode(SingleLinkedNode* n, const TYPE& v) :
    next(n), value(v) {}

  /**
    Returns the next node.
  */
  inline SingleLinkedNode* getNext() const throw() {return next;}

  /**
    Sets the next node.
  */
  inline void setNext(SingleLinkedNode* next) throw() {this->next = next;}

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

#endif
