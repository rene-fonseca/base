/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__SET_H
#define _DK_SDU_MIP__BASE_COLLECTION__SET_H

#include <base/collection/OrderedBinaryTree.h>
#include <base/string/FormatOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Set collection implemented using an ordered binary tree.

  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.1
*/

template<class KEY>
class Set : public Collection {
private:

  /** The elements of the set. */
  OrderedBinaryTree<KEY> elements;
  /** The number of elements in the set. */
  unsigned int size;
public:

  /** Enumerator of set. */
  template<class TRAITS, class ENU>
  class SetEnumerator : public Enumerator<TRAITS> {
  private:

    ENU enu;
  public:

    inline SetEnumerator(ENU e) : enu(e) {}

    inline bool hasNext() const throw() {
      return enu.hasNext();
    }

    inline Pointer next() throw(EndOfEnumeration) {
      return enu.next()->getValue();
    }
  };

  /** Modifying enumerator. */
  typedef SetEnumerator<EnumeratorTraits<KEY>, OrderedBinaryTree<KEY>::Enumerator> Enumerator;
  /** Non-modifying enumerator. */
  typedef SetEnumerator<ReadEnumeratorTraits<KEY>, OrderedBinaryTree<KEY>::ReadEnumerator> ReadEnumerator;

//  /** Non-modifying enumerator. */
//  class ReadEnumerator : public Enumerator<ReadEnumeratorTraits<KEY> > {
//  private:
//
//    OrderedBinaryTree<KEY>::ReadEnumerator enu;
//  public:
//
//    inline ReadEnumerator(OrderedBinaryTree<KEY>::ReadEnumerator e) : enu(e) {}
//
//    inline bool hasNext() const throw() {
//      return enu.hasNext();
//    }
//
//    inline const KEY* next() throw(EndOfEnumeration) {
//      return enu.next()->getValue();
//    }
//  };

//  /** Modifying enumerator. */
//  class Enumerator : public OrderedBinaryTree<KEY>::Enumerator {
//  public:
//
//    Enumerator(OrderedBinaryTree<KEY>::Enumerator enu) : OrderedBinaryTree<KEY>::Enumerator(enu) {}
//
//    KEY* next() throw(EndOfEnumeration) {
//      return OrderedBinaryTree<KEY>::Enumerator::next()->getValue();
//    }
//  };
//
//  /** Non-modifying enumerator. */
//  class ReadEnumerator : public OrderedBinaryTree<KEY>::ReadEnumerator {
//  public:
//
//    ReadEnumerator(OrderedBinaryTree<KEY>::ReadEnumerator enu) : OrderedBinaryTree<KEY>::ReadEnumerator(enu) {}
//
//    const KEY* next() throw(EndOfEnumeration) {
//      return OrderedBinaryTree<KEY>::ReadEnumerator::next()->getValue();
//    }
//  };

//  class Enumeration;
//  friend class Enumeration;
//  class ReadOnlyEnumeration;
//  friend class ReadOnlyEnumeration;
//
//  /**
//    Enumeration of all the elements of a set.
//
//    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
//    @version 1.0
//  */
//  class Enumeration : public OrderedBinaryTree<KEY>::Enumeration {
//  public:
//
//    /**
//      Initializes an enumeration of all the elements of the specified set.
//
//      @param set The set being enumerated.
//    */
//    inline Enumeration(Set& set) throw() :
//      OrderedBinaryTree<KEY>::Enumeration(set.elements) {}
//  };
//
//  /**
//    Non-modifying enumeration of all the elements of a set.
//
//    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
//    @version 1.0
//  */
//  class ReadOnlyEnumeration : public OrderedBinaryTree<KEY>::ReadOnlyEnumeration {
//  public:
//
//    /**
//      Initializes a non-modifying enumeration of all the elements of the
//      specified set.
//
//      @param set The set being enumerated.
//    */
//    inline ReadOnlyEnumeration(const Set& set) throw() :
//      OrderedBinaryTree<KEY>::ReadOnlyEnumeration(set.elements) {}
//  };
public:

  /**
    Initializes an empty set.
  */
  Set() throw() : size(0) {}

  /**
    Initializes set from other set.
  */
  Set(const Set& copy) throw(MemoryException) : elements(copy.elements) {}

  /**
    Returns the number of elements in the collection.
  */
  inline unsigned int getSize() const throw() {return size;}

  /**
    Returns true if the collection is empty.
  */
  inline bool isEmpty() const throw() {return size != 0;}

  /**
    Returns a modifying enumerator of the ordered binary tree.
  */
  inline Enumerator getEnumerator() throw() {
    return Enumerator(elements.getEnumerator());
  }

  /**
    Returns a non-modifying enumerator of the ordered binary tree.
  */
  inline ReadEnumerator getReadEnumerator() const throw() {
    return ReadEnumerator(elements.getReadEnumerator());
  }

  /**
    Returns true if the specified key is present is this set.

    @param key The value to search for.
  */
  bool isKey(const KEY& key) const throw() {
    return elements.find(key);
  }

  /**
    Adds the specified key to the set.

    @param key The key to be added to the set.
  */
  void add(const KEY& key) throw(MemoryException) {
    KEY* result = elements.add(key);
    if (!result) {
      ++size; // key did not exist
    }
  }

  /**
    Removes the specified key from this set. Throws 'InvalidNode' if the key
    doesn't exist in the set.

    @param key The value to be removed from the set.
  */
  void remove(const KEY& key) throw(InvalidNode) {
    elements.remove(elements.find(key));
    --size;
  }

  /**
    Removes all the keys from this set.
  */
  void removeAll() throw() {
    elements.removeAll();
    size = 0;
  }
};

template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Set<TYPE>& value) {
  Set<TYPE>::ReadEnumerator enu = value.getReadEnumerator();
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

#endif
