/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__SET_H
#define _DK_SDU_MIP__BASE_COLLECTION__SET_H

#include "Collection.h"
#include "Enumeration.h"
#include "InvalidKey.h"
#include "base/MemoryException.h"

/**
  Set.

  @author René Møller Fonseca
  @version 1.0
*/

template<class KEY>
class Set : public Collection {
protected:

  /**
    A key in a set.
  */
  class SetNode {
  protected:
    SetNode* next;
    KEY key;
  public:
    inline SetNode(SetNode* n, const KEY& k) throw() : next(n), key(k) {}
    inline SetNode* getNext() const throw() {return next;}
    inline void setNext(SetNode* next) throw() {this->next = next;}
    inline const KEY* getKey() const throw() {return &key;}
    inline void setKey(KEY& key) throw() {this->key = key;}
  };

  /**
    Enumeration of all the keys in a set.
  */
  class SetEnumeration {
  private:

    SetNode* node;
  public:

    inline SetEnumeration(SetNode* n) throw() : node(n) {}

    inline bool hasNext() const throw() {
      return node;
    }

    inline const KEY* const next() throw(EndOfEnumeration) {
      if (!node) {
        throw EndOfEnumeration();
      }
      const KEY* temp = node->getKey();
      node = node->getNext();
      return temp;
    }
  };

  /** The first node in the set. */
  SetNode* first;
public:

  /**
    Initializes an empty set.
  */
  Set() throw();

  /**
    Initializes set from other set.
  */
  Set(const Set& copy) throw(MemoryException);

  /**
    Returns true if the specified key is present is this set.
  */
  bool isKey(const KEY& key) const throw();

  /**
    Returns an enumeration of all the keys in this set.
  */
  SetEnumeration getEnumeration() throw();

  /**
    Adds the specified key to the set.

    @param key The key to be added to the set.
  */
  void add(const KEY& key) throw(MemoryException);

  /**
    Removes the specified key from this set. Throws 'InvalidKey' if the key
    doesn't exist in the set.
  */
  void remove(const KEY& key) throw(InvalidKey);

  /**
    Removes all the keys from this set.
  */
  void removeAll() throw();

  /**
    Destroys the set.
  */
  ~Set() throw();
};

#endif
