/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2006 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/collection/Collection.h>
#include <base/collection/Enumeration.h>
#include <base/collection/InvalidEnumeration.h>
#include <base/collection/InvalidNode.h>
#include <base/MemoryException.h>
#include <base/mem/Reference.h>
#include <base/string/FormatOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A node in a list.

  @short A node in a list.
  @ingroup collections
  @version 1.0
*/

template<class TYPE>
class _DK_SDU_MIP__BASE__API ListNode {
protected:

  /** The next node in the list. */
  ListNode* next = nullptr;
  /** The previous node in the list. */
  ListNode* previous = nullptr;
  /** The value associated with the node. */
  TYPE value;
public:

  inline ListNode(ListNode* _next, ListNode* _previous, const TYPE& _value)
    : next(_next),
      previous(_previous),
      value(_value) {
  }
  
  inline ListNode* getNext() const throw() {
    return next;
  }
  
  inline void setNext(ListNode* next) throw() {
    this->next = next;
  }
  
  inline ListNode* getPrevious() const throw() {
    return previous;
  }

  inline void setPrevious(ListNode* previous) throw() {
    this->previous = previous;
  }

  inline TYPE* getValue() throw() {
    return &value;
  }

  inline const TYPE* getValue() const throw() {
    return &value;
  }

  inline void setValue(TYPE value) throw() {
    this->value = value;
  }
};



/**
  Enumeration of elements in a list.
  
  @short Enumerator of list with modification access.
  @ingroup collections
  @version 1.0
*/

template<class TRAITS>
class _DK_SDU_MIP__BASE__API ListEnumerator : public Enumerator<TRAITS> {
public:

  typedef typename Enumerator<TRAITS>::Value Value;
  typedef typename Enumerator<TRAITS>::Pointer Pointer;
private:
  
  /** The current position in the enumeration. */
  ListNode<Value>* current;
public:

  /**
    Initializes an enumeration of all the elements of a list.

    @param begin Specifies the beginning of the enumeration.
  */
  explicit inline ListEnumerator(ListNode<Value>* begin) throw()
    : current(begin) {
  }

  /**
    Initializes enumeration from other enumeration.
  */
  inline ListEnumerator(const ListEnumerator& copy) throw()
    : current(copy.current) {
  }

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
    bassert(current != 0, EndOfEnumeration());
    Pointer temp = current->getValue();
    current = current->getNext();
    return temp;
  }

  /**
    Returns true if the enumerations are pointing to the same position.
  */
  inline bool operator==(const ListEnumerator& eq) const throw() {
    return current == eq.current;
  }

  /**
    Returns true if the enumerations aren't pointing to the same position.
  */
  inline bool operator!=(const ListEnumerator& eq) const throw() {
    return current != eq.current;
  }
};

/**
  Enumeration of elements in a list.

  @short Non-modifying enumerator of list.
  @version 1.0
*/

template<class TRAITS>
class _DK_SDU_MIP__BASE__API ListReadEnumerator : public Enumerator<TRAITS> {
protected:

  typedef typename Enumerator<TRAITS>::Value Value;
  typedef typename Enumerator<TRAITS>::Pointer Pointer;

  /** The current position in the enumeration. */
  const ListNode<Value>* current;
public:

  /**
    Initializes an enumeration of all the elements of a list.

    @param begin Specifies the beginning of the enumeration.
  */
  explicit inline ListReadEnumerator(const ListNode<Value>* begin) throw()
    : current(begin) {
  }

  /**
    Initializes enumeration from other enumeration.
  */
  inline ListReadEnumerator(const ListReadEnumerator& copy) throw()
    : current(copy.current) {
  }

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
    bassert(current != 0, EndOfEnumeration());
    Pointer temp = current->getValue();
    current = current->getNext();
    return temp;
  }

  /**
    Returns true if the enumerations are pointing to the same position.
  */
  inline bool operator==(const ListReadEnumerator& eq) const throw() {
    return current == eq.current;
  }

  /**
    Returns true if the enumerations aren't pointing to the same position.
  */
  inline bool operator!=(const ListReadEnumerator& eq) const throw() {
    return current != eq.current;
  }
};



/**
  Linked list data structure.

  You can enumerate the elements of a list like this (myMember is a member
  function of MyClass):
  @code
    List<MyClass> myList;
    List<MyClass>::Enumeration enu(myList);
    while (enu.hasNext()) {
      enu.next()->myMember();
    }
  @endcode

  @short List collection.
  @ingroup collections
  @version 1.0
*/

template<class TYPE>
class _DK_SDU_MIP__BASE__API List : public Collection {
public:

  /** The type of a value. */
  typedef TYPE Value;
  /** The type of a node. */
  typedef ListNode<Value> Node;

  typedef ListEnumerator<EnumeratorTraits<TYPE> > Enumerator;
  typedef ListReadEnumerator<ReadEnumeratorTraits<TYPE> > ReadEnumerator;
protected:

  /*
    Internal list implementation.
    
    @version 1.0
  */
  class _DK_SDU_MIP__BASE__API ListImpl : public ReferenceCountedObject {
  private:

    /** The first node in the list. */
    Node* first = nullptr;
    /** The last node in the list. */
    Node* last = nullptr;
    /** The number of elements in the list. */
    unsigned int size = 0;
  protected:

    /**
      Inserts the value at the specified node of this list.

      @param node Node specifying the position. Must not be 0.
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
    inline ListImpl() throw() {
    }
    
    /**
      Initializes list from other list.
    */
    ListImpl(const ListImpl& copy) throw() {
      const Node* node = copy.getFirst();
      while (node) {
        append(*node->getValue());
        node = node->getNext();
      }
    }

    /**
      Returns the number of elements of the list.
    */
    inline unsigned int getSize() const throw() {
      return size;
    }

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
  Reference<ListImpl> elements;

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
  List() throw(MemoryException) : elements(new ListImpl()) {
  }

  /**
    Initializes list from other list.
  */
  inline List(const List& copy) throw()
    : elements(copy.elements) {
  }
  
  /**
    Assignment of list by list.
  */
  inline List& operator=(const List& eq) throw() {
    elements = eq.elements;
    return *this;
  }
  
  // Node read-only

  /**
    Returns the number of elements in the list.
  */
  inline unsigned int getSize() const throw() {
    return elements->getSize();
  }

  /**
    Returns true if the list is empty.
  */
  inline bool isEmpty() const throw() {
    return elements->isEmpty();
  }

  /**
    Returns a modifying enumerator of the list.
  */
  inline Enumerator getEnumerator() throw() {
    return Enumerator(getFirst());
  }

  /**
    Returns a non-modifying enumerator of the list.
  */
  inline ReadEnumerator getReadEnumerator() const throw() {
    return ReadEnumerator(getFirst());
  }

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
  void removeFirst() throw(InvalidNode, MemoryException) {
    elements.copyOnWrite();
    elements->removeFirst();
  }

  /**
    Removes the last node of this list.
  */
  void removeLast() throw(InvalidNode, MemoryException) {
    elements.copyOnWrite();
    elements->removeLast();
  }

  /**
    Removes all the elements from this list.
  */
  void removeAll() throw(MemoryException) {
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
    enu.next(); // raises exception if end has been reached
    remove(node);
  }*/
};

/**
  Writes a string representation of a list to a format stream.

  @relates List
*/
template<class TYPE>
FormatOutputStream& operator<<(
  FormatOutputStream& stream, const List<TYPE>& value) throw(IOException) {
  typename List<TYPE>::ReadEnumerator enu = value.getReadEnumerator();
  stream << '{';
  while (enu.hasNext()) {
    stream << *enu.next();
    if (enu.hasNext()) {
      stream << ';';
    }
  }
  stream << '}';
  return stream;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
