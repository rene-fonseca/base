/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__MAP_H
#define _DK_SDU_MIP__BASE_COLLECTION__MAP_H

#include <base/collection/OrderedBinaryTree.h>
#include <base/collection/Association.h>
#include <base/Exception.h>
#include <base/MemoryException.h>
#include <base/collection/InvalidKey.h>
#include <base/string/FormatOutputStream.h>

/**
  Map collection implemented using an ordered binary tree.

  @author René Møller Fonseca
  @version 1.1
*/
template<class KEY, class VALUE>
class Map : public Collection {
public:

  /** The type of a node in the map. */
  typedef Association<KEY, VALUE> Node;
private:

  /** The associations of the map. */
  OrderedBinaryTree<Association<KEY, VALUE> > elements;
  /** The number of associations in the map. */
  unsigned int size;
protected:

  /**
    Enumeration of all the elements of a map.

    @author René Møller Fonseca
    @version 1.0
  */
  class Enumeration : public OrderedBinaryTree<Node>::Enumeration {
  public:

    /**
      Initializes an enumeration of all the elements of the specified map.

      @param map The map being enumerated.
    */
    inline Enumeration(Map& map) throw() :
      OrderedBinaryTree<Node>::Enumeration(map.elements) {}

    /**
      Returns the next element of the enumeration.
    */
//    inline Node* next() throw(EndOfEnumeration) {
//      return OrderedBinaryTree<Node>::next()->getValue();
//    }
  };

  /**
    Non-modifying enumeration of all the elements of a map.

    @author René Møller Fonseca
    @version 1.0
  */
  class ReadOnlyEnumeration : public OrderedBinaryTree<Node>::ReadOnlyEnumeration {
  public:

    /**
      Initializes a non-modifying enumeration of all the elements of the
      specified map.

      @param map The map being enumerated.
    */
    inline ReadOnlyEnumeration(const Map& map) throw() :
      OrderedBinaryTree<Node>::ReadOnlyEnumeration(map.elements) {}
  };

  /*
    Used to implement 'operator[](const KEY& key)'
  */
  class Index {
  protected:
    Map& map;
    const KEY& key;
  public:
    inline Index(Map& m, const KEY& k) throw() : map(m), key(k) {}
    inline VALUE operator=(const VALUE& value) throw(MemoryException) {map.add(key, value); return value;}
    inline operator VALUE() const throw(InvalidKey) {return map.getValue(key);}
  };
public:

  /**
    Initializes an empty map.
  */
  Map() throw() : size(0) {}

  /**
    Initializes map from other map.
  */
  Map(const Map& copy) throw(MemoryException) : elements(copy.elements) {}

  /**
    Returns the number of associations in the map.
  */
  inline unsigned int getSize() const throw() {return size;}

  /**
    Returns true if the map is empty.
  */
  inline bool isEmpty() const throw() {return size == 0;}

  /**
    Returns true if the specified key is associated with a value in this map.

    @param key The value to search for.
  */
  bool isKey(const KEY& key) const throw() {
    return elements.find(Association<KEY, VALUE>(key));
  }

  /**
    Returns the value associated with the specified key. Throws 'InvalidKey'
    if the specified key doesn't exist in this map.

    @param key The key of the value.
  */
  VALUE getValue(const KEY& key) const throw(InvalidKey) {
    const OrderedBinaryTree<Association<KEY, VALUE> >::Node* node = elements.find(Association<KEY, VALUE>(key));
    if (!node) {
      throw InvalidKey();
    }
    return node->getValue()->getValue();
  }

  /**
    Associates the specified key with the specified value in this map. If the
    key already is associated with a value, the value is overridden.

    @param key The key.
    @param value The value.
  */
  void add(const KEY& key, const VALUE& value) throw(MemoryException) {
    Node* result = elements.add(Association<KEY, VALUE>(key, value));
    if (result) {
      // key already exists
      result->setValue(value); // set the new value
    } else {
      // key did not exist
      ++size;
    }
  }

  /**
    Removes the specified key and its associated value from this map. Throws
    'InvalidKey' if the key doesn't exist in this map.
  */
  void remove(const KEY& key) throw(InvalidKey) {
    elements.remove(elements.find(Association<KEY, VALUE>(key)));
    --size; // never ends up here if the key doesn't exist
  }

  /**
    Removes all the keys from this map.
  */
  void removeAll() throw() {
    elements.removeAll();
    size = 0;
  }

  /**
    Returns the value associated with the specified key when used as 'rvalue'.
    When used as 'lvalue' the key is associated with the specified value.
  */
  inline Index operator[](const KEY& key) throw(InvalidKey, MemoryException) {
    return Index(*this, key);
  }
};

template<class KEY, class VALUE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Map<KEY, VALUE>& value) {
  Map<KEY, VALUE>::ReadOnlyEnumeration enu(value);
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
