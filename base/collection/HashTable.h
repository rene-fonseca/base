/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2000-2002 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__HASH_TABLE_H
#define _DK_SDU_MIP__BASE_COLLECTION__HASH_TABLE_H

#include <base/collection/Collection.h>
#include <base/collection/Association.h>
#include <base/collection/InvalidKey.h>
#include <base/OutOfRange.h>
#include <base/MemoryException.h>
#include <base/mem/ReferenceCountedObject.h>
#include <base/mem/ReferenceCountedObjectPointer.h>
#include <base/string/FormatOutputStream.h>
#include <base/mathematics/Math.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

// need hash set
// need support for propagating when element is accessed
// enumerator (read and write)
// iterator (read and write)
// TAG: need support for other hash functions

/**
  A hash table implementation.
  
  @short Hash table.
  @ingroup collections
  @author Rene Moeller Fonseca <fonseca@mip.sdu.dk>
  @version 1.2
*/

template<class KEY, class VALUE>
class HashTable : public Collection {
public:

  /** The type of the key. */
  typedef KEY Key;
  /** The type of the value. */
  typedef VALUE Value;
  
  /** The minimum capacity. */
  static const unsigned int MINIMUM_CAPACITY = 16;
  /** The default capacity. */
  static const unsigned int DEFAULT_CAPACITY = 256;

  /**
    Reference to an element within a hash table.
  */
  class Reference {
    friend class HashTable;
  private:
    
    HashTable& hashTable;
    const Key key;
    Reference(const Reference& copy); // prohibit default copy initialization
    Reference& operator=(const Reference& eq); // prohibit default assignment

    inline Reference(HashTable& _hashTable, const Key& _key)
      : hashTable(_hashTable),
        key(_key) {
    }
  public:
    
    inline Reference& operator=(Value value) throw(MemoryException) {
      hashTable.add(key, value);
      return *this;
    }
    
    inline operator Value() const throw(InvalidKey) {
      return hashTable.getValue(key);
    }
  };
  
  class Node {
  private:
    
    /** The key of the element. */
    Key key;
    /** The value of the element. */
    Value value;
    /** The next node in the single linked list. */
    Node* nextNode;
  public:

    /** Initializes a node. */
    inline Node(const Key& _key, const Value& _value, Node* _nextNode) throw()
      : key(_key),
        value(_value),
        nextNode(_nextNode) {
    }
  };

  class Bucket {
  private:

    Node* firstNode;
  public:

    inline Bucket() throw() : firstNode(0) {
    }
    
    inline void addNode(Node* node) throw() {
      node->next = firstNode; // could be 0
      firstNode = node;
    }

    // TAG: propagation should be controlled by the user
    inline Reference getValue(const Key& key) throw(InvalidKey) {
      Node* grandparent = 0;
      Node* parent = 0;
      Node* child = firstNode;
      while (child && (child->key != key)) {
        grandparent = parent;
        parent = child;
        child = child->next;
      }
      assert(child, InvalidKey(this));
      if (grandparent) {
        grandparent->next = child;
        parent->next = child->next;
        child->next = parent;
      } else if (parent) { // propagate towards entry
        // parent is the first node
        parent->next = child->next;
        child->next = firstNode;
        firstNode = child;
      } else {
        // child is the first node
      }
    }
    
    inline void removeAll() throw() {
      Node* node = firstNode;
      firstNode = 0;
      while (node) {
        Node* temp = node;
        node = node->next;
        delete temp;
      }
    }
  };
  
  class HashTableImpl : public ReferenceCountedObject {
  private:

    /** Lookup table. */
    Allocator<Bucket> table;
    /** The current capacity of the table. */
    unsigned int capacity; // use CapacityAllocator
    /** . */
    unsigned int log2OfCapacity;
    /** Cache for (Math::log2(capacity) - 1). */
    unsigned int mask;
    /** The number of elements in the table. */
    unsigned int size;
  public:

    HashTableImpl(unsigned int capacity) throw(OutOfDomain, MemoryException) {
      // round up to nearest power of 2
      assert(
        Math::isPowerOf2(capacity) && (capacity >= MINIMUM_CAPACITY),
        OutOfDomain(this)
      );
      this->capacity = capacity;
      log2OfCapacity = Math::ilog2(capacity);
      size = 0;
      
      // mark all entries as empty
      fill<Bucket>(table.getElements(), table.getSize(), Bucket());
    }

    void grow() throw(MemoryException) {
      capacity *= 2;
      ++log2OfCapacity;
      mask = log2OfCapacity - 1;
      table.setSize(capacity);
      // clear top
      // move lower half to new places
    }
    
    void shrink() throw() {
      capacity /= 2;
      --log2OfCapacity;
      mask = log2OfCapacity - 1;
      // move upper half to lower half
      table.setSize(capacity);
    }
    
    /**
      Returns the number of elements in the hash table.
    */
    inline unsigned int getSize() throw() {
      return size;
    }

    void validate() throw() {
      // check if valid table
    }

    /**
      Returns the capacity of the hash table.
    */
    inline unsigned int getCapacity() throw() {
      return capacity;
    }

    void* lookupKey(Key key) throw() {
      // get hash value
      // entry = entries[hash & mask]
      // for all in bucket find matching key
    }

    bool isKey(Key key) const throw() {
      // get hash value
      // entry = entries[hash & mask]
      // for all in bucket find matching key
    }

    void add(Key key, Value value) throw() {
      // get hash value
      // entry = entries[hash & mask]
      // if not empty buckey
      //   for all in bucket find matching key
      //     if found then error
      // if not found add to list
      // if size > capacity then grow
    }

    /**
      Returns the hash value of the key.
    */
    inline unsigned long getHashValue(const Key& key) throw() {
      return Hash<Key>(key); // Hash is a functor
    }

    /**
      Removes the elements with the specified key from the table.
    */
    void remove(Key key) throw(InvalidKey) {
      const unsigned long hash = getHash(key);
      const MemorySize entry = hash & mask;
      Bucket* bucket = table.getElements();
      Node* node = bucket->firstNode;

      if (node->key == key) {
        bucket->firstNode = node->next; // next could be 0
      } else {
        Node* previousNode = 0;
        while (node) {
          if (node->key == key) {
            break;
          }
          previousNode = node;
          node = node->next;
        }
        assert(node, InvalidKey(this));
        previousNode->next = node->next; // unlink node from linked list
      }
      
      delete node;
      --size;
      
      if (size < capacity/2) { // TAG: find simple hysteresis that works
        shrink();
      }
    }
    
    ~HashTableImpl() throw() {
      // destroy all elements
    }
  };
  
  ReferenceCountedObjectPointer<HashTableImpl> impl;
  
  inline void copyOnWrite() throw(MemoryException) {
    // TAG: fixme
  }
public:

  /**
    Initializes an hash table.
  */
  HashTable() throw(MemoryException)
    : impl(new HashTableImpl(DEFAULT_CAPACITY)) {
  }
  
  /**
    Initializes the hash table with the specified initial capacity.
  */
  HashTable(unsigned int capacity) throw(OutOfDomain, MemoryException)
    : impl(new HashTableImpl(capacity)) {
  }

  /**
    Initializes hash table from other hash table.
  */
  inline HashTable(const HashTable& copy) throw()
    : impl(copy.impl) {
  }
  
  /**
    Assignment of hash table by hash table.
  */
  inline HashTable& operator=(const HashTable& eq) throw() {
    impl = eq.impl;
    return *this;
  }
  
  /**
    Returns the number of elements in the hash table.
  */
  inline unsigned int getSize() const throw() {
    return size;
  }
  
  /**
    Returns true if the hash table is empty.
  */
  inline bool isEmpty() const throw() {
    return size == 0;
  }

  /**
    Returns true if the specified value is a key in the table.
  */
  inline bool isKey(Key key) const throw() {
    return impl->isKey(key);
  }

  /**
    Returns the value associated with the specified key. Raises InvalidKey
    if the specified key doesn't exist in this hash table.
    
    @param key The key of the value.
  */
  inline Value getValue(const Key& key) const throw(InvalidKey) {
    return impl->getValue();
  }
  
  /**
    Adds the key and value to the table.
  */
  inline void add(const Key& key, const Value& value) throw(MemoryException) {
    copyOnWrite();
    impl->add(key, value);
  }
  
  /**
    Removes the specified key and its associated value from this hash table.
    Raises InvalidKey if the key doesn't exist in this hash table.
  */
  inline void remove(const Key& key) throw(InvalidKey, MemoryException) {
    copyOnWrite();
    impl->remove(key);
  }
 
  /**
    Removes all the keys from this hash table.
  */
  void removeAll() throw() {
    impl = new HashTableImpl(DEFAULT_CAPACITY); // initial capacity is unknown
  }

  typedef int* Iterator; // TAG: fixme
  typedef const int* ReadIterator; // TAG: fixme
  typedef int* Enumerator; // TAG: fixme
  typedef const int* ReadEnumerator; // TAG: fixme
  
  Iterator getBeginIterator() throw(MemoryException) {
    copyOnWrite();
    // TAG: fixme
  }

  Iterator getEndIterator() throw(MemoryException) {
    copyOnWrite();
    // TAG: fixme
  }

  ReadIterator getReadBeginIterator() const throw() {
    // TAG: fixme
  }

  ReadIterator getReadEndIterator() const throw() {
    // TAG: fixme
  }
  
  Enumerator getEnumerator() throw(MemoryException) {
    copyOnWrite();
    // TAG: fixme
  }
  
  ReadEnumerator getReadEnumerator() const throw() {
    // TAG: fixme
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
FormatOutputStream& operator<<(FormatOutputStream& stream, const HashTable<KEY, VALUE>& value) throw(IOException) {
  typename HashTable<KEY, VALUE>::ReadEnumerator enu = value.getReadEnumerator();
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
