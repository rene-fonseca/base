/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__MAP_H
#define _DK_SDU_MIP__BASE_COLLECTION__MAP_H

#include "base/Object.h"
#include "Collection.h"
#include "Enumeration.h"
#include "base/Exception.h"
#include "base/MemoryException.h"
#include "InvalidKey.h"

/**
  Map.

  @author René Møller Fonseca
  @version 1.0
*/

template<class KEY, class VALUE>
class Map : public Object /*, public Collection*/ {
protected:

  /**
    Association node of map.
  */
  class MapNode {
  protected:
    MapNode* next;
    KEY key;
    VALUE value;
  public:
    inline MapNode(MapNode* n, const KEY& k, const VALUE& v) throw() : next(n), key(k), value(v) {}
    inline MapNode* getNext() const throw() {return next;}
    inline void setNext(MapNode* next) throw() {this->next = next;}
    inline const KEY& getKey() const throw() {return key;}
    inline const VALUE& getValue() const throw() {return value;}
    inline void setValue(VALUE& value) throw() {this->value = value;}
  };

  class MapEnumerationNode {
  protected:
    MapNode* node;
  public:
    explicit inline MapEnumerationNode(MapNode* n) throw() : node(n) {}
    inline const KEY& getKey() const throw() {return node->getKey();}
    inline const VALUE& getValue() const throw() {return node->getValue();}
    inline void setValue(VALUE& value) throw() {node->setValue(value);}
  };

  /**
    Enumeration of all the associations in a map.
  */
  class MapEnumeration {
  private:

    MapNode* node;
  public:

    inline MapEnumeration(MapNode* n) throw() : node(n) {}

    inline bool hasNext() const throw() {
      return node && node->getNext();
    }

    inline const MapEnumerationNode next() throw(EndOfEnumeration) {
      if (!node) {
        throw EndOfEnumeration();
      }
      MapEnumerationNode temp(node);
      node = node->getNext();
      return temp;
    }
  };

  /**
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

  /** The first node in the map. */
  MapNode* first;
  /** The number of nodes in the map. */
  unsigned int size;
public:

  /**
    Initializes an empty map.
  */
  Map() throw();

  /**
    Initializes map from other map.
  */
  Map(const Map& copy) throw(MemoryException);

  /**
    Returns the number of associations in this map.
  */
  inline unsigned int getSize() const throw() {return size;}

  /**
    Returns true if this map is empty (i.e. has no associations).
  */
  inline bool isEmpty() const throw() {return size == 0;}

  /**
    Returns an enumeration of all the associations in this map.
  */
  MapEnumeration getEnumeration() throw();

  /**
    Returns true if the specified key is associated with a value in this map.
  */
  bool isKey(const KEY& key) const throw();

  /**
    Returns the value associated with the specified key. Throws 'InvalidKey'
    if the specified key doesn't exist in this map.

    @param key The key of the value.
  */
  VALUE getValue(const KEY& key) const throw(InvalidKey);

  /**
    Associates the specified key with the specified value in this map. If the
    key already is associated with a value, the value is overridden.

    @param key The key.
    @param value The value.
  */
  void add(const KEY& key, const VALUE& value) throw(MemoryException);

  /**
    Removes the specified key and its associated value from this map. Throws
    'InvalidKey' if the key doesn't exist in this map.
  */
  void remove(const KEY& key) throw(InvalidKey);

  /**
    Removes all the keys from this map.
  */
  void removeAll() throw();

  /**
    Returns the value associated with the specified key when used as 'rvalue'.
    When used as 'lvalue' the key is associated with the specified value.
  */
  inline Index operator[](const KEY& key) throw(InvalidKey, MemoryException) {return Index(*this, key);}

  /**
    Destroys the map.
  */
  ~Map() throw();
};

#endif
