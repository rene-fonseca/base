/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__LIST_H
#define _DK_SDU_MIP__BASE_COLLECTION__LIST_H

#include <base/collection/Collection.h>
#include <base/collection/Enumeration.h>
#include <base/MemoryException.h>

/**
  List.

  You can enumerate the elements of a list like this:
  <pre>
    myList::Enumeration myEnum = myList.getEnumeration();
    while (myEnum.hasNext()) {
      myEnum.next();
    }
  </pre>

  @author René Møller Fonseca
  @version 1.0
*/

template<class TYPE>
class List : public Collection {
protected:

  /**
    A node in a list.
  */
  class ListNode {
  protected:

    ListNode* next;
    ListNode* previous;
    TYPE value;
  public:

    inline ListNode(ListNode* n, ListNode* p, const TYPE& v) : next(n), previous(p), value(v) {}
    inline ListNode* getNext() const throw() {return next;}
    inline void setNext(ListNode* next) throw() {this->next = next;}
    inline ListNode* getPrevious() const throw() {return previous;}
    inline void setPrevious(ListNode* previous) throw() {this->previous = previous;}
    inline TYPE* getValue() throw() {return &value;}
    inline void setValue(TYPE value) throw() {this->value = value;}
  };

  /**
    Enumeration of elements in a list.
  */
  class ListEnumeration {
  protected:

    ListNode* node;
  public:

    inline ListEnumeration(ListNode* n) throw() : node(n) {}

    inline bool hasNext() const throw() {
      return node;
    }

    inline const TYPE* const next() throw(EndOfEnumeration) {
      if (!node) {
        throw EndOfEnumeration();
      }
      const TYPE* temp = node->getValue();
      node = node->getNext();
      return temp;
    }
  };

  /** The first node in the list. */
  ListNode* first;
  /** The last node in the list. */
  ListNode* last;
public:

  /**
    Initializes an empty list.
  */
  List() throw();

  /**
    Initializes list from other list.
  */
  List(const List& copy) throw(MemoryException);



  // Node read-only



  /**
    Returns the index of the specified node in this list.

    @return -1 if the node isn't in the list.
  */
  int indexOf(const ListNode& node) const throw();
//  int indexOf(const Iterator& i) const throw();
//  int indexOf(const TYPE& value) const throw();

  /**
    Gets an enumeration of all the elements in this list.
  */
  ListEnumeration getEnumeration() const throw();



  // Node manip



  /**
    Appends the value to the end of this list.
  */
  void append(const TYPE& value) throw();
  void append(Enumeration<TYPE>& enu) throw();

  /**
    Prepends the value to the beginning of this list.
  */
  void prepend(const TYPE& value) throw();
  void prepend(Enumeration<TYPE>& enu) throw();

  /**
    Adds the value to the end of the list.
  */
  void add(const TYPE& value) throw();
  void add(Enumeration<TYPE>& enu) throw();

  /**
    Inserts the value at the specified position.
  */
  void insert(ListNode& pos, const TYPE& value) throw();
//  void insert(const Iterator& pos, const TYPE& value) throw();

  /**
    Removes the node from the list.
  */
  void remove(ListNode* node) throw();
//  void remove(const Iterator& pos) throw();// iterator must still be valid - point to next node

  /**
    Removes all the nodes in the list.
  */
  void removeAll() throw();

  /**
    Destroys the list.
  */
  ~List();

  // Friend section
// TYPE does not have to implement operator<<
//  friend FormatOutputStream& operator<< <>(FormatOutputStream& stream, const List& value);
};

#endif
