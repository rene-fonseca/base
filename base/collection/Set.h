/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__SET_H
#define _DK_SDU_MIP__BASE_COLLECTION__SET_H

#include <base/collection/OrderedBinaryTree.h>
#include <base/string/FormatOutputStream.h>

/**
  Set collection implemented using an ordered binary tree.

  @author René Møller Fonseca
  @version 1.1
*/

template<class KEY>
class Set : public Collection {
private:

  /** The elements of the set. */
  OrderedBinaryTree<KEY> elements;
  /** The number of elements in the set. */
  unsigned int size;
protected:

  /**
    Enumeration of all the elements of a set.

    @author René Møller Fonseca
    @version 1.0
  */
  class Enumeration : public OrderedBinaryTree<KEY>::Enumeration {
  public:

    /**
      Initializes an enumeration of all the elements of the specified set.

      @param set The set being enumerated.
    */
    inline Enumeration(Set& set) throw() :
      OrderedBinaryTree<KEY>::Enumeration(set.elements) {}
  };

  /**
    Non-modifying enumeration of all the elements of a set.

    @author René Møller Fonseca
    @version 1.0
  */
  class ReadOnlyEnumeration : public OrderedBinaryTree<KEY>::ReadOnlyEnumeration {
  public:

    /**
      Initializes a non-modifying enumeration of all the elements of the
      specified set.

      @param set The set being enumerated.
    */
    inline ReadOnlyEnumeration(const Set& set) throw() :
      OrderedBinaryTree<KEY>::ReadOnlyEnumeration(set.elements) {}
  };
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
  Set<TYPE>::ReadOnlyEnumeration enu(value);
  stream << '{';
  while (enu.hasNext()) {
    stream << *enu.next()->getValue();
    if (enu.hasNext()) {
      stream << ";";
    }
  }
  stream << '}';
  return stream;
}

#endif
