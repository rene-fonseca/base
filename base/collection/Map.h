/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__MAP_H
#define _DK_SDU_MIP__BASE_COLLECTION__MAP_H

#include <base/collection/OrderedBinaryTree.h>
#include <base/collection/Association.h>
#include <base/Exception.h>
#include <base/MemoryException.h>
#include <base/collection/InvalidKey.h>
#include <base/string/FormatOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Map collection implemented using an ordered binary tree.

  @ingroup collections
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.1
*/
template<class KEY, class VALUE>
class Map : public Collection {
public:

  /** The type of a key in the map. */
  typedef KEY Key;
  /** The type of a value in the map. */
  typedef VALUE Value;
  /** The type of an association in the map. */
  typedef Association<Key, Value> Node;
private:

  /** The associations of the map. */
  OrderedBinaryTree<Node> elements;
  /** The number of associations in the map. */
  unsigned int size;
public:

//  /** Enumerator of Map. */
//  template<class TRAITS, class ENU>
//  class MapEnumerator : public Enumerator<TRAITS> {
//  private:
//
//    ENU enu;
//  public:
//
//    inline MapEnumerator(ENU e) : enu(e) {}
//
//    inline bool hasNext() const throw() {
//      return enu.hasNext();
//    }
//
//    inline Pointer next() throw(EndOfEnumeration) {
//      return enu.next()->getValue();
//    }
//  };

  /** Enumerator of map. */
  class ReadEnumerator : public Enumerator<ReadEnumeratorTraits<Node> > {
  private:

    OrderedBinaryTree<Node>::ReadEnumerator enu;
  public:

    inline ReadEnumerator(OrderedBinaryTree<Node>::ReadEnumerator e) : enu(e) {}

    inline bool hasNext() const throw() {
      return enu.hasNext();
    }

    inline Pointer next() throw(EndOfEnumeration) {
      return enu.next()->getValue();
    }
  };

  /**
    Modifying enumerator of values of map.

    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
  class ValueEnumerator : public Enumerator<EnumeratorTraits<Value> > {
  private:

    OrderedBinaryTree<Node>::Enumerator enu;
  public:

    inline ValueEnumerator(OrderedBinaryTree<Node>::Enumerator e) : enu(e) {}

    inline bool hasNext() const throw() {
      return enu.hasNext();
    }

    inline Pointer next() throw(EndOfEnumeration) {
      return enu.next()->getValue()->getValue();
    }
  };

  /** Modifying enumerator. */
//  typedef MapEnumerator<EnumeratorTraits<Node>, OrderedBinaryTree<Node>::Enumerator> Enumerator;
  /** Non-modifying enumerator. */
//  typedef MapEnumerator<ReadEnumeratorTraits<Node>, OrderedBinaryTree<Node>::ReadEnumerator> ReadEnumerator;


  /**
    Enumeration of all the elements of a map.

    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
    @version 1.0
  */
//  class Enumeration : public OrderedBinaryTree<Node>::Enumeration {
//  public:
//
//    /**
//      Initializes an enumeration of all the elements of the specified map.
//
//      @param map The map being enumerated.
//    */
//    inline Enumeration(Map& map) throw() :
//      OrderedBinaryTree<Node>::Enumeration(map.elements) {}
//  };

//  /**
//    Non-modifying enumeration of all the elements of a map.
//
//    @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
//    @version 1.0
//  */
//  class ReadOnlyEnumeration : public OrderedBinaryTree<Node>::ReadOnlyEnumeration {
//  public:
//
//    /**
//      Initializes a non-modifying enumeration of all the elements of the
//      specified map.
//
//      @param map The map being enumerated.
//    */
//    inline ReadOnlyEnumeration(const Map& map) throw() :
//      OrderedBinaryTree<Node>::ReadOnlyEnumeration(map.elements) {}
//  };

  /**
    Reference to an element within a map.
  */
  class Reference {
  private:
    friend class Map;
    Map& map;
    const Key key;
    Reference(const Reference& copy); // prohibit default copy initialization
    Reference& operator=(const Reference& eq); // prohibit default assignment
    inline Reference(Map& m, const Key& k) : map(m), key(k) {}
  public:
    inline Reference& operator=(Value value) throw(MemoryException) {map.add(key, value); return *this;}
    inline operator Value() const throw(InvalidKey) {return map.getValue(key);}
  };
public:

  /**
    Initializes an empty map.
  */
  Map() throw() : size(0) {}

  /**
    Initializes map from other map.
  */
  Map(const Map& copy) throw(MemoryException) : elements(copy.elements), size(copy.size) {}

  /**
    Assignment of map to map.
  */
  Map& operator=(const Map& eq) throw(MemoryException) {
    elements = eq.elements;
    size = eq.size;
    return *this;
  }

  /**
    Returns the number of associations in the map.
  */
  inline unsigned int getSize() const throw() {return size;}

  /**
    Returns true if the map is empty.
  */
  inline bool isEmpty() const throw() {return size == 0;}

  /**
    Returns a modifying enumerator of the ordered binary tree.
  */
  inline ValueEnumerator getValueEnumerator() throw() {
    return elements.getEnumerator();
  }

  /**
    Returns a non-modifying enumerator of the ordered binary tree.
  */
  inline ReadEnumerator getReadEnumerator() const throw() {
    return elements.getReadEnumerator();
  }

  /**
    Returns true if the specified key is associated with a value in this map.

    @param key The value to search for.
  */
  bool isKey(const Key& key) const throw() {
    return elements.find(Association<Key, Value>(key));
  }

  /**
    Returns the value associated with the specified key. Throws 'InvalidKey'
    if the specified key doesn't exist in this map.

    @param key The key of the value.
  */
  Value getValue(const Key& key) const throw(InvalidKey) {
    const OrderedBinaryTree<Association<Key, Value> >::Node* node = elements.find(Association<Key, Value>(key));
    if (!node) {
      throw InvalidKey();
    }
    return *node->getValue()->getValue();
  }

  /**
    Associates the specified key with the specified value in this map. If the
    key already is associated with a value, the value is overridden.

    @param key The key.
    @param value The value.
  */
  void add(const Key& key, const Value& value) throw(MemoryException) {
    Node* result = elements.add(Association<Key, Value>(key, value));
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
  void remove(const Key& key) throw(InvalidKey) {
    elements.remove(elements.find(Association<Key, Value>(key)));
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
  inline Reference operator[](const Key& key) throw(InvalidKey, MemoryException) {
    return Reference(*this, key);
  }

  /**
    Returns the value associated with the specified key.
  */
  inline Value operator[](const Key& key) const throw(InvalidKey) {
    return getValue(key);
  }
};

template<class KEY, class VALUE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Map<KEY, VALUE>& value) {
  Map<KEY, VALUE>::ReadEnumerator enu = value.getReadEnumerator();
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
