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

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

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



//template<class TYPE>
//class ListNodePointer {
//private:
//
//  ListNode<TYPE>* node;
//public:
//
//  ListNodePointer(ListNode<TYPE>* node) : node(node) {}
//
//  ListNodePointer(const ListNodePointer& copy) : node(copy.node) {}
//
//  ListNodePointer& operator=(const ListNodePointer& eq) {
//    node = eq.node;
//    return *this;
//  }
//
//  void operator--() {
//    node = node->getPrevious();
//  }
//
//  void operator--(int) {
//    ListNode<TYPE>* temp = node;
//    node = node->getPrevious();
//    return temp;
//  }
//
//  void operator++() {
//    node = node->getNext();
//  }
//
//  void operator++(int) {
//    ListNode<TYPE>* temp = node;
//    node = node->getNext();
//    return temp;
//  }
//
//  TYPE* operator*() {
//    return node->getValue();
//  }
//
//  TYPE* operator->() {
//    return node->getValue();
//  }
//
//  inline bool operator==(const ListNodePointer& eq) const throw() {
//    return node == eq.node;
//  }
//
//  inline bool operator!=(const ListNodePointer& eq) const throw() {
//    return node != eq.node;
//  }
//};



/**
  Enumeration of elements in a list.

  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE, class REF, class PTR>
class ListEnumeration : public Enumeration<TYPE, REF, PTR> {
public:

  /** The type of a pointer to a value. */
  typedef PTR Pointer;
protected:

  /** The current position in the enumeration. */
  ListNode<TYPE>* current;
public:

  /**
    Initializes an enumeration of all the elements of a list.

    @param begin Specifies the beginning of the enumeration.
  */
  explicit inline ListEnumeration(ListNode<TYPE>* begin) throw() : current(begin) {}

  /**
    Initializes enumeration from other enumeration.
  */
  inline ListEnumeration(const ListEnumeration& copy) throw() : current(copy.current) {}

  /**
    Returns true if the enumeration still contains elements.
  */
  inline bool hasNext() const throw() {
    return current != 0;
  }

  /**
    Returns the next element and advances the position of this enumeration.
  */
  inline Pointer next() throw(EndOfEnumeration) {
    if (current == 0) {
      throw EndOfEnumeration();
    }
    Pointer temp = current->getValue();
    current = current->getNext();
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
  Enumeration of elements in a list.

  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE, class REF, class PTR>
class ListReadOnlyEnumeration : public Enumeration<TYPE, REF, PTR> {
public:

  /** The type of a pointer to a value. */
  typedef PTR Pointer;
protected:

  /** The current position in the enumeration. */
  const ListNode<TYPE>* current;
public:

  /**
    Initializes an enumeration of all the elements of a list.

    @param begin Specifies the beginning of the enumeration.
  */
  explicit inline ListReadOnlyEnumeration(const ListNode<TYPE>* begin) throw() : current(begin) {}

  /**
    Initializes enumeration from other enumeration.
  */
  inline ListReadOnlyEnumeration(const ListReadOnlyEnumeration& copy) throw() : current(copy.current) {}

  /**
    Returns true if the enumeration still contains elements.
  */
  inline bool hasNext() const throw() {
    return current != 0;
  }

  /**
    Returns the next element and advances the position of this enumeration.
  */
  inline Pointer next() throw(EndOfEnumeration) {
    if (current == 0) {
      throw EndOfEnumeration();
    }
    Pointer temp = current->getValue();
    current = current->getNext();
    return temp;
  }

  /**
    Returns true if the enumerations are pointing to the same position.
  */
  inline bool operator==(const ListReadOnlyEnumeration& eq) const throw() {
    return current == eq.current;
  }

  /**
    Returns true if the enumerations aren't pointing to the same position.
  */
  inline bool operator!=(const ListReadOnlyEnumeration& eq) const throw() {
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

  /** The type of a value. */
  typedef TYPE Value;
  /** The type of a node. */
  typedef ListNode<Value> Node;

  class Enumeration;
  friend class Enumeration;
  class ReadOnlyEnumeration;
  friend class ReadOnlyEnumeration;

  /**
    Enumeration of all the elements of an array.
  */
  class Enumeration : public ListEnumeration<Value, Value&, Value*> {
  public:

    /**
      Initializes an enumeration of all the elements of the specified list.

      @param list The list being enumerated.
    */
    Enumeration(List& list) throw() :
      ListEnumeration<Value, Value&, Value*>(list.getFirst()) {}
  };

  /**
    Non-modifying enumeration of all the elements of an array.
  */
  class ReadOnlyEnumeration : public ListReadOnlyEnumeration<Value, const Value&, const Value*> {
  public:

    /**
      Initializes a non-modifying enumeration of all the elements of the specified list.

      @param list The list being enumerated.
    */
    ReadOnlyEnumeration(const List& list) throw() :
      ListReadOnlyEnumeration<Value, const Value&, const Value*>(list.getFirst()) {}
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
    void append(const TYPE& value) throw(MemoryException) {
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
    void prepend(const TYPE& value) throw(MemoryException) {
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
      while (first) {
        Node* node = first;
        first = first->getNext();
        delete node;
      }
    }
  };

  /**
    The elements of the list.
  */
  ReferenceCountedObjectPointer<ListImpl> elements;

  /**
    Returns the first node of the list.
  */
  inline Node* getFirst() throw(MemoryException) {
    elements.copyOnWrite();
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
  inline Node* getLast() throw(MemoryException) {
    elements.copyOnWrite();
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
  List() throw() : elements(new ListImpl()) {}

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
    elements.copyOnWrite();
    elements->append(value);
  }

  /**
    Prepends the value to the beginning of this list.

    @param value The value to be prepended to the list.
  */
  void prepend(const TYPE& value) throw(MemoryException) {
    elements.copyOnWrite();
    elements->prepend(value);
  }

  /**
    Appends the specified value to the end of this list.

    @param value The value to be appended to the list.
  */
  void add(const TYPE& value) throw(MemoryException) {
    elements.copyOnWrite();
    elements->append(value);
  }

  /**
    Inserts the value at the specified position.

    @param enu Enumeration of this list specifying the position.
    @param value The value to be inserted.
  */
/*  void insert(Enumeration& enu, const TYPE& value) throw(InvalidEnumeration) {
    elements.copyOnWrite();
    if (enu.getOwner() != this) {
      throw InvalidEnumeration();
    }
//    insert(enu.node, value);
  }*/

  /**
    Removes this first node of this list.
  */
  void removeFirst() throw(InvalidNode) {
    elements.copyOnWrite();
    elements->removeFirst();
  }

  /**
    Removes the last node of this list.
  */
  void removeLast() throw(InvalidNode) {
    elements.copyOnWrite();
    elements->removeLast();
  }

  /**
    Removes all the elements from this list.
  */
  void removeAll() throw() {
    elements = new ListImpl(); // copyOnWrite is not required
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
    stream << *enu.next();
    if (enu.hasNext()) {
      stream << ";";
    }
  }
  stream << "}";
  return stream;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
