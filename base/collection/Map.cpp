/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include "Map.h"

template Map<void*, void*>;

template<class KEY, class VALUE>
Map<KEY, VALUE>::Map() throw() : first(NULL) {
}

template<class KEY, class VALUE>
Map<KEY, VALUE>::Map(const Map& copy) throw(MemoryException) : first(NULL) {
  MapNode* node = first;
  while (node) {
    add(node->getKey(), node->getValue());
    node = node->getNext();
  }
}

template<class KEY, class VALUE>
Map<KEY, VALUE>::MapEnumeration Map<KEY, VALUE>::getEnumeration() throw() {
  return MapEnumeration(first);
}

template<class KEY, class VALUE>
bool Map<KEY, VALUE>::isKey(const KEY& key) const throw() {
  const MapNode* node = first;
  while (node) {
    if (node->getKey() == key) {
      return true;
    }
    node = node->getNext();
  }
  return false;
}

template<class KEY, class VALUE>
VALUE Map<KEY, VALUE>::getValue(const KEY& key) const throw(InvalidKey) {
  const MapNode* node = first;
  while (node) {
    if (node->getKey() == key) {
      return node->getValue();
    }
    node = node->getNext();
  }
  throw InvalidKey();
}

template<class KEY, class VALUE>
void Map<KEY, VALUE>::add(const KEY& key, const VALUE& value) throw(MemoryException) {
  if (!isKey(key)) {
    first = new MapNode(first, key, value);
    ++size;
  }
}

template<class KEY, class VALUE>
void Map<KEY, VALUE>::remove(const KEY& key) throw(InvalidKey) {
  MapNode* node = first;
  MapNode* previous = NULL;

  while (node) {
    if (node->getKey() == key) {
      if (previous == NULL) {
        first = node->getNext();
      } else {
        previous->setNext(node->getNext());
      }
      --size;
      delete node;
      return;
    }
    previous = node;
    node = node->getNext();
  }

  throw InvalidKey();
}

template<class KEY, class VALUE>
void Map<KEY, VALUE>::removeAll() throw() {
  while (first) {
    MapNode* temp = first;
    first = first->getNext();
    --size;
    delete temp;
  }
}

template<class KEY, class VALUE>
Map<KEY, VALUE>::~Map() throw() {
  removeAll();
}
