/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__LIST_H
#define _DK_SDU_MIP__BASE_COLLECTION__LIST_H

#include <base/collection/Collection.h>
#include <base/collection/Enumeration.h>
#include <base/collection/InvalidEnumeration.h>
#include <base/collection/InvalidNode.h>
#include <base/MemoryException.h>
#include <base/mem/ReferenceCountedObjectPointer.h>
#include <base/mem/ReferenceCountedObject.h>
#include <base/string/FormatOutputStream.h>

/**
  A node in a list.

  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE>
class ListNode {
protected:

  /** The next node in the list. */
  ListNode* next;
  /** The previous node in the list. */
  ListNode* previous;
  /** The value associated with the node. */
  TYPE value;
public:

  inline ListNode(ListNode* n, ListNode* p, const TYPE& v) : next(n), previous(p), value(v) {}
  inline ListNode* getNext() const throw() {return next;}
  inline void setNext(ListNode* next) throw() {this->next = next;}
  inline ListNode* getPrevious() const throw() {return previous;}
  inline void setPrevious(ListNode* previous) throw() {this->previous = previous;}
  inline TYPE* getValue() throw() {return &value;}
  inline const TYPE* getValue() const throw() {return &value;}
  inline void setValue(TYPE value) throw() {this->value = value;}
};



template<class TYPE>
class ListNodePointer {
private:

  ListNode<TYPE>* node;
public:

  ListNodePointer(ListNode<TYPE>* node) : node(node) {}

  ListNodePointer(const ListNodePointer& copy) : node(copy.node) {}

  ListNodePointer& operator=(const ListNodePointer& eq) {
    node = eq.node;
    return *this;
  }

  void operator--() {
    node = node->getPrevious();
  }

  void operator--(int) {
    ListNode<TYPE>* temp = node;
    node = node->getPrevious();
    return temp;
  }

  void operator++() {
    node = node->getNext();
  }

  void operator++(int) {
    ListNode<TYPE>* temp = node;
    node = node->getNext();
    return temp;
  }

  TYPE* operator*() {
    return node->getValue();
  }

  TYPE* operator->() {
    return node->getValue();
  }

  inline bool operator==(const ListNodePointer& eq) const throw() {
    return node == eq.node;
  }

  inline bool operator!=(const ListNodePointer& eq) const throw() {
    return node != eq.node;
  }
};



/**
  Enumeration of elements in a list.

  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE, class REF, class PTR, class NODEPTR>
class ListEnumeration : public Enumeration<TYPE, REF, PTR> {
public:

  /** The type of the values being enumerated. */
  typedef TYPE Value;
  /** The type of a reference to a value. */
  typedef REF Reference;
  /** The type of a pointer to a value. */
  typedef PTR Pointer;
  /** The type of a pointer to a value. */
  typedef NODEPTR NodePointer;
protected:

  /** The current position in the enumeration. */
  NodePointer current;
  /** The end of the enumeration. */
  NodePointer end;
public:

  /**
    Initializes an enumeration of all the elements specified by 'begin' and 'end'.

    @param begin Specifies the beginning of the enumeration.
    @param end Specifies the end of the enumeration.
  */
  inline ListEnumeration(NodePointer begin, NodePointer end) throw() : current(begin), end(end) {}

  /**
    Initializes enumeration from other enumeration.
  */
  inline ListEnumeration(const ListEnumeration& copy) throw() : current(copy.current), end(copy.end) {}

  /**
    Returns true if the enumeration still contains elements.
  */
  inline bool hasNext() const throw() {
    return current != end;
  }

  /**
    Returns the next element and advances the position of this enumeration.
  */
  inline Pointer next() throw(EndOfEnumeration) {
    if (current == end) {
      throw EndOfEnumeration();
    }
    Pointer temp = current->getValue();
    ++current;
    return temp;
  }

  /**
    Returns true if the enumerations are pointing to the same position.
  */
  inline bool operator==(const ListEnumeration& eq) const throw() {
    return current == eq.current;
  }

  /**
    Returns true if the enumerations aren't pointing to the same position.
  */
  inline bool operator!=(const ListEnumeration& eq) const throw() {
    return current != eq.current;
  }
};



/**
  Linked list.

  You can enumerate the elements of a list like this (myMember is a member function of MyClass):
  <pre>
    List<MyClass> myList;
    List<MyClass>::Enumeration enu(myList);
    while (enu.hasNext()) {
      enu.next()->myMember();
    }
  </pre>

  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE>
class List : public Collection {
public:

  typedef TYPE Value;
  typedef ListNode<Value> Node;

  /**
    Enumeration of all the elements of an array.
  */
  class Enumeration : public ListEnumeration<Value, Value&, Value*, Node*> {
  public:

    /**
      Initializes an enumeration of all the elements of the specified list.

      @param list The list being enumerated.
    */
    Enumeration(List& list) throw() :
      ListEnumeration<Value, Value&, Value*, Node*>(list.getFirst(), list.getLast()) {}
  };

  /**
    Non-modifying enumeration of all the elements of an array.
  */
  class ReadOnlyEnumeration : public ListEnumeration<Value, const Value&, const Value*, const Node*> {
  public:

    /**
      Initializes a non-modifying enumeration of all the elements of the specified list.

      @param list The list being enumerated.
    */
    ReadOnlyEnumeration(const List& list) throw() :
      ListEnumeration<Value, const Value&, const Value*, const Node*>(list.getFirst(), list.getLast()) {}
  };
protected:

  /**
    Internal list implementation.

    @author René Møller Fonseca
    @version 1.0
  */
  class ListImpl : public ReferenceCountedObject {
  private:

    /** The first node in the list. */
    Node* first;
    /** The last node in the list. */
    Node* last;
    /** The number of elements in the list. */
    unsigned int size;
  protected:

    /**
      Inserts the value at the specified node of this list.

      @param node Node specifying the position. Must not be NULL.
      @param value The value to be inserted.
    */
    inline void insert(Node* node, const TYPE& value) throw() {
      Node* previous = node->getPrevious();
      Node* temp = new Node(node, previous, value);
      node->setPrevious(temp);
      if (previous) {
        previous->setNext(temp);
      }
      ++size;
    }

    /**
      Removes the specified node from this list.

      @param node The node to be removed. Must not be NULL.
    */
    inline void remove(Node* node) throw() {
      Node* next = node->getNext();
      Node* previous = node->getPrevious();
      if (next) {
        next->setPrevious(previous);
      }
      if (previous) {
        previous->setNext(next);
      }
      --size;
      delete node;
    }
  public:

    /**
      Initializes an empty list.
    */
    ListImpl() throw() : first(0), last(0), size(0) {}

    /**
      Initializes list from other list.
    */
    ListImpl(const ListImpl& copy) throw() : first(0), last(0), size(0) {
      const Node* node = copy.getFirst();
      while (node) {
        append(*node->getValue());
        node = node->getNext();
      }
    }

    /**
      Returns the number of elements of the list.
    */
    inline unsigned int getSize() const throw() {return size;}

    /**
      Returns the first node of the list.
    */
    inline Node* getFirst() throw() {
      return first;
    }

    /**
      Returns the first node of the list.
    */
    inline const Node* getFirst() const throw() {
      return first;
    }

    /**
      Returns the last node of the list.
    */
    inline Node* getLast() throw() {
      return last;
    }

    /**
      Returns the last node of the list.
    */
    inline const Node* getLast() const throw() {
      return last;
    }

    /**
      Appends the value to the end of this list.
    */
    void append(const TYPE& value) throw() {
      if (size) {
        Node* node = new Node(0, last, value);
        last->setNext(node);
        last = node;
        ++size;
      } else { // list is empty
        Node* node = new Node(0, 0, value);
        first = node;
        last = node;
        ++size;
      }
    }

    /**
      Prepends the value to the beginning of this list.
    */
    void prepend(const TYPE& value) throw() {
      if (size) {
        Node* node = new Node(first, 0, value);
        first->setPrevious(node);
        first = node;
        ++size;
      } else { // list is empty
        Node* node = new Node(0, 0, value);
        first = node;
        last = node;
        ++size;
      }
    }

    /**
      Inserts the value at the specified position.

      @param enu Enumeration of this list specifying the position.
      @param value The value to be inserted.
    */
/*    void insert(Enumeration& enu, const TYPE& value) throw(InvalidEnumeration) {
      if (enu.getOwner() != this) {
        throw InvalidEnumeration();
      }
  //    insert(enu.node, value);
    }*/

    /**
      Removes this first node of this list.
    */
    void removeFirst() throw(InvalidNode) {
      if (!first) {
        throw InvalidNode();
      }
      remove(first);
    }

    /**
      Removes the last node of this list.
    */
    void removeLast() throw(InvalidNode) {
      if (!last) {
        throw InvalidNode();
      }
      remove(last);
    }

    /**
      Destroys the list.
    */
    ~ListImpl() throw() {
      removeAll();
    }
  };

  /**
    The elements of the list.
  */
  ReferenceCountedObjectPointer<ListImpl<TYPE> > elements;

  /**
    Makes a new copy of the internal representation of the elements if shared
    by several lists. This member function must be called explicitly before
    most modifications to the list.
  */
  inline void copyOnWrite() throw(MemoryException) {
    if (elements.isMultiReferenced()) { // do we have the elements for our self
      elements = new ListImpl<TYPE>(*elements); // make a copy of the elements
    }
  }

  /**
    Returns the first node of the list.
  */
  inline Node* getFirst() throw() {
    copyOnWrite();
    return elements->getFirst();
  }

  /**
    Returns the first node of the list.
  */
  inline const Node* getFirst() const throw() {
    return elements->getFirst();
  }

  /**
    Returns the last node of the list.
  */
  inline Node* getLast() throw() {
    copyOnWrite();
    return elements->getLast();
  }

  /**
    Returns the last node of the list.
  */
  inline const Node* getLast() const throw() {
    return elements->getLast();
  }
public:

  /**
    Initializes an empty list.
  */
  List() throw() {}

  /**
    Initializes list from other list.
  */
  List(const List& copy) throw(MemoryException) : elements(copy.elements) {}

  // Node read-only

  /**
    Returns the number of elements in the list.
  */
  inline unsigned int getSize() const throw() {return elements->getSize();}

  /**
    Returns true if the list is empty.
  */
  inline bool isEmpty() const throw() {return elements->isEmpty();}

  /**
    Returns the index of the node with the specified value in this list.

    @return -1 if the node isn't in the list.
  */
/*  int indexOf(const TYPE& value) const throw() {
    const ListNode* current = first;
    int index = 0;
    while (current) {
      if (current->getValue() == value) {
        return index;
      }
      current = current->getNext();
    }
    return -1; // value not found
  }*/

  // Node manip

  /**
    Appends the specified value to the end of this list.

    @param value The value to be appended to the list.
  */
  void append(const TYPE& value) throw(MemoryException) {
    copyOnWrite();
    elements->append(value);
  }

  /**
    Prepends the value to the beginning of this list.

    @param value The value to be prepended to the list.
  */
  void prepend(const TYPE& value) throw(MemoryException) {
    copyOnWrite();
    elements->prepend(value);
  }

  /**
    Appends the specified value to the end of this list.

    @param value The value to be appended to the list.
  */
  void add(const TYPE& value) throw(MemoryException) {
    copyOnWrite();
    elements->append(value);
  }

  /**
    Inserts the value at the specified position.

    @param enu Enumeration of this list specifying the position.
    @param value The value to be inserted.
  */
/*  void insert(Enumeration& enu, const TYPE& value) throw(InvalidEnumeration) {
    copyOnWrite();
    if (enu.getOwner() != this) {
      throw InvalidEnumeration();
    }
//    insert(enu.node, value);
  }*/

  /**
    Removes this first node of this list.
  */
  void removeFirst() throw(InvalidNode) {
    copyOnWrite();
    elements->removeFirst();
  }

  /**
    Removes the last node of this list.
  */
  void removeLast() throw(InvalidNode) {
    copyOnWrite();
    elements->removeLast();
  }

  /**
    Removes all the elements from this list.
  */
  void removeAll() throw() {
    elements = 0; // copyOnWrite is not required
  }

  /**
    Removes the node specified by the enumeration from this list.

    @param enu Enumeration specifying the element to be removed.
  */
/*  void remove(ListEnumeration& enu) throw(InvalidEnumeration, EndOfEnumeration) {
    if (enu.getOwner() != this) {
      throw InvalidEnumeration();
    }
    ListNode* node = 0; //enu.node;
    enu.next(); // throws exception if end has been reached
    remove(node);
  }*/
};

/**
  Writes a string representation of a list to a format stream.
*/
template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const List<TYPE>& value) {
  List<TYPE>::ReadOnlyEnumeration enu(value);
  stream << "{";
  while (enu.hasNext()) {
    stream << *enu.next()->getValue();
    if (enu.hasNext()) {
      stream << ";";
    }
  }
  stream << "}";
  return stream;
}

#endif
