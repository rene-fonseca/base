/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#include <base/collection/Set.h>

template Set<void*>;

template<class KEY>
Set<KEY>::Set() throw() : first(0) {
}

template<class KEY>
Set<KEY>::Set(const Set& copy) throw(MemoryException) : first(0) {
  const SetNode* node = copy.first;
  while (node) {
    add(*node->getKey());
    node = node->getNext();
  }
}

template<class KEY>
bool Set<KEY>::isKey(const KEY& key) const throw() {
  const SetNode* node = first;
  while (node) {
    if (*node->getKey() == key) {
      return true;
    }
    node = node->getNext();
  }
  return false;
}

template<class KEY>
Set<KEY>::SetEnumeration Set<KEY>::getEnumeration() throw() {
  return SetEnumeration(first);
}

template<class KEY>
void Set<KEY>::add(const KEY& key) throw(MemoryException) {
  if (!isKey(key)) { // is key in set
    first = new SetNode(first, key);
    ++size;
  }
}

template<class KEY>
void Set<KEY>::remove(const KEY& key) throw(InvalidKey) {
  SetNode* node = first;
  SetNode* previous = 0;

  while (node) {
    if (*node->getKey() == key) {
      if (!previous) {
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

template<class KEY>
void Set<KEY>::removeAll() throw() {
  while (first) {
    SetNode* temp = first;
    first = first->getNext();
    --size;
    delete temp;
  }
}

template<class KEY>
Set<KEY>::~Set() throw() {
  removeAll();
}
