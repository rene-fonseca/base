/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    See COPYRIGHT.txt for details.

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/collection/Collection.h>
#include <base/collection/Association.h>
#include <base/collection/InvalidKey.h>
#include <base/MemoryException.h>
#include <base/mathematics/Math.h>
#include <base/mem/Reference.h>
#include <base/string/FormatOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  A hash table implementation.
  
  @short Hash table collection.
  @ingroup collections
  @version 1.2
*/

template<class KEY, class VALUE>
class _DK_SDU_MIP__BASE__API HashTable : public Collection {
public:
  
  /** The type of the key. */
  typedef KEY Key;
  /** The type of the value. */
  typedef VALUE Value;
  /** The type of an association in the hash table. */
  typedef Association<Key, Value> HashTableAssociation;
  
  /** The minimum capacity. */
  static const unsigned int MINIMUM_CAPACITY = 16;
  /** The maximum capacity. */
  static const unsigned int MAXIMUM_CAPACITY = PrimitiveTraits<unsigned int>::MAXIMUM/2;
  /** The default capacity. */
  static const unsigned int DEFAULT_CAPACITY = 16;
  
  /*
    Reference to an element within a hash table.
  */
  class Element {
    friend class HashTable;
  private:
    
    HashTable& hashTable;
    const Key key;
    Element(const Element& copy) throw();
    Element& operator=(const Element& eq) throw();

    inline Element(HashTable& _hashTable, const Key& _key) throw()
      : hashTable(_hashTable),
        key(_key) {
    }
  public:

    /**
      Sets the value of the element.
    */
    inline Element& operator=(Value value) throw(MemoryException) {
      hashTable.add(key, value);
      return *this;
    }

    /**
      Returns the value of the element.
    */
    inline operator Value() const throw(InvalidKey) {
      return hashTable.getValue(key);
    }
  };

  /*
    Node in the single linked list of a bucket.
  */
  class Node : public Object {
    friend class HashTableImpl;
  private:
    
    /** The hash of the element. */
    unsigned long hash = 0;
    /** The key and value of the element. */
    Association<Key, Value> value;
    /** The next node in the single linked list. */
    Node* next = nullptr;
  public:
    
    /**
      Initializes a node.
    */
    inline Node(unsigned long _hash, const Key& _key, const Value& _value) throw()
      : hash(_hash),
        value(_key, _value) {
    }
    
    /**
      Initializes a node.
    */
    inline Node(unsigned long _hash, const Key& _key, const Value& _value, Node* _next) throw()
      : hash(_hash),
        value(_key, _value),
        next(_next) {
    }
    
    /**
      Returns the hash value of the node.
    */
    inline unsigned long getHash() const throw() {
      return hash;
    }
    
    /**
      Returns the key of the node.
    */
    inline const Association<Key, Value>& getKeyValue() const throw() {
      return value;
    }
    
    /**
      Returns the key of the node.
    */
    inline const Key& getKey() const throw() {
      return value.getKey();
    }
    
    /**
      Returns the value of the node.
    */
    inline Value& getValue() throw() {
      return value.getValue();
    }
    
    /**
      Returns the value of the node.
    */
    inline const Value& getValue() const throw() {
      return value.getValue();
    }

    /**
      Returns the next node.
    */
    inline Node* getNext() throw() {
      return next;
    }
    
    /**
      Returns the next node.
    */
    inline const Node* getNext() const throw() {
      return next;
    }
    
    /**
      Sets the next node.
    */
    inline void setNext(Node* node) throw() {
      next = node;
    }
  };
  
  /*
    Hash table implementation.
  */
  class HashTableImpl : public ReferenceCountedObject {
  private:
    
    /** Lookup table. */
    Allocator<Node*> table;
    /** The current capacity of the table. */
    unsigned int capacity = 0; // use size in table attribute
    /** Cache for (capacity - 1). */
    unsigned int mask = 0;
    /** Base 2 logarithm of the capacity. */
    unsigned int log2OfCapacity = 0;
    /** The number of elements in the table. */
    unsigned int size = 0;
    
    /**
      Returns the hash value of the key.
    */
    static inline unsigned long getHash(const Key& key) throw() {
      Hash<Key> hash; // Hash is a functor
      return hash(key);
    }
  public:

    /**
      Returns the buckets for modifying access.
    */
    inline Node** getBuckets() throw() {
      return table.getElements();
    }

    /**
      Returns the buckets for non-modifying access.
    */
    inline const Node* const* getBuckets() const throw() {
      return table.getElements();
    }
    
    /**
      Initializes the hash table with the specified capacity.
    */
    inline HashTableImpl(unsigned int capacity) throw(MemoryException) {
      capacity = maximum(capacity, MINIMUM_CAPACITY);
      capacity = minimum(capacity, MAXIMUM_CAPACITY);
      log2OfCapacity = Math::iLog2(capacity);
      if ((1U << log2OfCapacity) < capacity) {
        ++log2OfCapacity;
        capacity *= 2;
      }
      this->capacity = capacity;
      mask = capacity - 1;
      size = 0;
      table.setSize(capacity);
      fill<Node*>(table.getElements(), table.getSize(), 0);
    }
    
    /**
      Initializes the hash table from another hash table.
    */
    inline HashTableImpl(const HashTableImpl& copy) throw(MemoryException)
      : table(copy.capacity),
        capacity(copy.capacity),
        mask(copy.mask),
        log2OfCapacity(copy.log2OfCapacity),
        size(copy.size) {
      table.setSize(capacity); // entries set below
      
      // copy all buckets
      const Node* const* src = copy.getBuckets();
      const Node* const* end = src + copy.capacity;
      Node** dest = getBuckets();
      while (src != end) {
        const Node* srcNode = *src++;
        if (srcNode) {
          Node* firstNode = new Node(
            srcNode->getHash(),
            srcNode->getKey(),
            srcNode->getValue()
          );
          *dest++ = firstNode;
          Node* destNode = firstNode;
          srcNode = srcNode->getNext();
          while (srcNode) {
            destNode->setNext(
              new Node(srcNode->getHash(), srcNode->getKey(), srcNode->getValue())
            );
            destNode = destNode->getNext();
            srcNode = srcNode->getNext();
          }
        } else {
          *dest++ = 0;
        }
      }
    }
    
    /**
      Increases the capacity of the hash table.
    */
    inline void grow() throw(MemoryException) {
      if (capacity < MAXIMUM_CAPACITY) {
        capacity *= 2;
        mask = capacity - 1;
        table.setSize(capacity); // upper entries set below
        ++log2OfCapacity;
        
        // unfold buckets
        Node** lowerBucket = getBuckets();
        Node** upperBucket = lowerBucket + capacity/2;
        const Node* const* endBucket = upperBucket;
        const unsigned int bitMask = capacity/2; // the bit that desides the half
        while (lowerBucket != endBucket) {
          Node* srcNode = *lowerBucket;
          if (srcNode) {
            // keep order and do NOT construct or destroy nodes
            if ((srcNode->getHash() & bitMask) == 0) {
              // move from first to second bucket
              Node* parentSrcNode = srcNode;
              srcNode = srcNode->getNext();
              while (srcNode && ((srcNode->getHash() & bitMask) == 0)) {
                parentSrcNode = srcNode;
                srcNode = srcNode->getNext();
              }
              if (srcNode) {
                parentSrcNode->setNext(srcNode->getNext()); // unlink
                *upperBucket = srcNode;
                srcNode = srcNode->getNext();
                
                Node* destNode = *upperBucket;
                // destNode->setNext(0); // only set for last node
                
                while (srcNode) {
                  Node* nextNode = srcNode->getNext();
                  if (srcNode->getHash() & bitMask) {
                    parentSrcNode->setNext(nextNode); // unlink
                    destNode->setNext(srcNode);
                    destNode = destNode->getNext();
                    // destNode->setNext(0); // only set for last node
                  }
                  parentSrcNode = srcNode;
                  srcNode = nextNode;
                }
                destNode->setNext(0);
              }
            } else {
              // move from second to first bucket
              *upperBucket = *lowerBucket;
              *lowerBucket = 0;
              
              Node* parentSrcNode = srcNode;
              srcNode = srcNode->getNext();
              while (srcNode && (srcNode->getHash() & bitMask)) {
                parentSrcNode = srcNode;
                srcNode = srcNode->getNext();
              }
              if (srcNode) {
                parentSrcNode->setNext(srcNode->getNext()); // unlink
                *lowerBucket = srcNode;
                srcNode = srcNode->getNext();
                
                Node* destNode = *lowerBucket;
                // destNode->setNext(0); // only set for last node
                
                while (srcNode) {
                  Node* nextNode = srcNode->getNext();
                  if ((srcNode->getHash() & bitMask) == 0) {
                    parentSrcNode->setNext(nextNode); // unlink
                    destNode->setNext(srcNode);
                    destNode = destNode->getNext();
                    // destNode->setNext(0); // only set for last node
                  }
                  parentSrcNode = srcNode;
                  srcNode = nextNode;
                }
                destNode->setNext(0); // terminate linked list
              }
            }
          } else {
            *upperBucket = 0; // lower bucket already empty
          }
          ++lowerBucket;
          ++upperBucket;
        }
      }
    }
    
    /**
      Reduces the capacity of the hash table.
    */
    inline void shrink() throw() {
      if (capacity > MINIMUM_CAPACITY) {
        // fold upper half buckets into lower buckets
        Node** lowerBucket = getBuckets();
        Node** upperBucket = lowerBucket + capacity/2;
        const Node* const* endBucket = upperBucket;
        while (lowerBucket != endBucket) {
          // merge buckets
          if (!*upperBucket) {
            // nothing to do
          } else if (!*lowerBucket) {
            *lowerBucket = *upperBucket;
          } else {
            // TAG: bucket could hold link to last node
            Node* node = *lowerBucket;
            while (node->getNext()) { // go to last node
              node = node->getNext();
            }
            node->setNext(*upperBucket);
          }
          ++lowerBucket;
          ++upperBucket;
        }
        capacity /= 2;
        mask = capacity - 1;
        --log2OfCapacity;
        table.setSize(capacity);
      }
    }
    
    /**
      Returns the capacity of the hash table.
    */
    inline unsigned int getCapacity() const throw() {
      return capacity;
    }
    
    /**
      Returns the number of elements in the hash table.
    */
    inline unsigned int getSize() const throw() {
      return size;
    }

    /**
      Returns true if the specified value is a key in the hash table.
    */
    inline bool isKey(const Key& key) throw() {
      const unsigned long hash = getHash(key);
      Node** bucket = getBuckets() + (hash & mask);
      
      Node* parent = 0;
      Node* child = *bucket;
      if (child) {
        if ((child->getHash() == hash) && (child->getKey() == key)) {
          return true; // this case should be fairly likely
        }
        parent = child;
        child = child->getNext();
        while (child &&
               ((child->getHash() != hash) || (child->getKey() != key))) {
          parent = child;
          child = child->getNext();
        }
        if (child) {
          Node* temp = child->getNext();
          Node* first = *bucket;
          child->setNext(first); // cyclic loop to first
          *bucket = child; // move to first - still cyclic loop
          parent->setNext(temp); // detach child
        }
      }
      return child;
    }
    
    /**
      Returns true if the specified value is a key in the hash table.
    */
    inline bool isKey(const Key& key) const throw() {
      const unsigned long hash = getHash(key);
      const Node* const* bucket = getBuckets() + (hash & mask);
      const Node* child = *bucket;
      while (child &&
             ((child->getHash() != hash) || (child->getKey() != key))) {
        child = child->getNext();
      }
      return child;
    }
    
    /**
      Returns the value associated with the specified key.
    */
    inline Value& getValue(const Key& key) throw(InvalidKey) {
      const unsigned long hash = getHash(key);
      Node** bucket = getBuckets() + (hash & mask);
      Node* grandparent = 0;
      Node* parent = 0;
      Node* child = *bucket;
      while (child && ((child->getHash() != hash) || (child->getKey() != key))) {
        grandparent = parent;
        parent = child;
        child = child->getNext();
      }
      if (child) {
        if (grandparent) {
          grandparent->setNext(child);
          parent->setNext(child->getNext());
          child->setNext(parent);
        } else if (parent) {
          // parent is the first node
          parent->setNext(child->getNext());
          child->setNext(*bucket);
          *bucket = child;
        } else {
          // child is the first node
        }
      }
      bassert(child, InvalidKey(this));
      return *child->getValue();
    }
    
    /**
      Returns the value associated with the specified key.
    */
    inline const Value& getValue(const Key& key) const throw(InvalidKey) {
      const unsigned long hash = getHash(key);
      const Node* const* bucket = getBuckets() + (hash & mask);
      const Node* child = *bucket;
      while (child &&
             ((child->getHash() != hash) || (child->getKey() != key))) {
        child = child->getNext();
      }
      bassert(child, InvalidKey(this));
      return child->getValue();
    }
    
    /**
      Adds the element to the table.
    */
    inline void add(const Key& key, const Value& value) throw(MemoryException) {
      const unsigned long hash = getHash(key);
      Node** buckets = getBuckets() + (hash & mask);
      if (*buckets) {
        Node* parent = 0;
        Node* child = *buckets;
        while (child && ((child->getHash() != hash) || (child->getKey() != key))) {
          parent = child;
          child = child->getNext();
        }
        if (!child) { // if value not already in list
          child = new Node(hash, key, value);
          if (parent) {
            parent->setNext(child);
          }
          ++size;
        }
      } else {
        *buckets = new Node(hash, key, value);
        ++size;
      }
      if (size > capacity) { // TAG: find simple rules that works
        grow();
      }
    }

    void dump() throw() {
      Node** bucket = getBuckets();
      const Node* const* endBucket = bucket + capacity;
      unsigned int index = 0;
      fout << "Capacity: " << capacity << ENDL;
      while (bucket != endBucket) {
        Node* srcNode = *bucket;
        fout << index << ':' << ' ';
        if (srcNode) {
          while (srcNode) {
            fout << srcNode->getHash() << ' ';
            srcNode = srcNode->getNext();
          }
        } else {
          fout << "EMPTY";
        }
        fout << ENDL;
        ++index;
        ++bucket;
      }
    }
    
    /**
      Removes the element with the specified key from the table.
    */
    inline void remove(const Key& key) throw(InvalidKey) {
      const unsigned long hash = getHash(key);
      Node** bucket = getBuckets() + (hash & mask);
      Node* child = *bucket;
      bassert(child, InvalidKey(this));
      if ((child->getHash() == hash) && (child->getKey() == key)) {
        *bucket = child->getNext(); // unlink first node (next could be 0)
      } else {
        Node* parent = child;
        child = child->getNext();
        while (child && ((child->getHash() != hash) || (child->getKey() != key))) {
          parent = child;
          child = child->getNext();
        }
        bassert(child, InvalidKey(this));
        parent->setNext(child->getNext()); // unlink node from linked list
      }
      --size;
      delete child;
      if (size < capacity/2) { // TAG: find simple rules that works
        shrink();
      }
    }
    
    /**
      Destroys the hash table.
    */
    inline ~HashTableImpl() throw() {
      Node** bucket = getBuckets();
      const Node* const* endBucket = bucket + capacity;
      while (bucket != endBucket) {
        Node* node = *bucket;
        while (node) {
          Node* temp = node;
          node = node->getNext();
          delete temp;
        }
        ++bucket;
      }
    }
  };

  /**
    Enumeration of all the elements of a hash set.
    
    @short Hash set enumerator
    @ingroup collections
    @version 1.0
  */
  
  template<class TRAITS>
  class HashTableEnumerator : public Enumerator<TRAITS> {
    friend class HashTableImpl;
  private:
    
    typedef typename Enumerator<TRAITS>::Pointer Pointer;
    typedef typename Enumerator<TRAITS>::Value Value;
    
    /** The hash set implementation. */
    Reference<HashTableImpl> impl;
    /** The current bucket. */
    Node** bucket = nullptr;
    /** The current position of the enumeration. */
    Node* node = nullptr;
    /** The number of elements. */
    unsigned int numberOfElements = 0;
  public:
      
    /**
      Initializes enumeration of hash table.
      
      @param hashTable The hash table.
    */
    inline HashTableEnumerator(
      Reference<HashTableImpl> _impl) throw()
      : impl(_impl),
        bucket(impl->getBuckets()),
        node(*bucket),
        numberOfElements(impl->getSize()) {
    }

    /**
      Returns true if there is more elements in this enumeration.
    */
    inline bool hasNext() const throw() {
      return numberOfElements;
    }

    /**
      Returns the current value and increments the position. Raises
      EndOfEnumeration if the end has been reached.
    */
    Pointer next() throw(EndOfEnumeration) {
      bassert(numberOfElements, EndOfEnumeration(this));
      while (!node) {
        ++bucket;
        node = *bucket;
      }
      const Value& result = node->getKeyValue();
      node = node->getNext();
      --numberOfElements;
      return &result; // TAG: change type
    }

    inline ~HashTableEnumerator() throw() {
    }
  };

  /**
    Value enumerator of values of hash table.

    @short Hash table value enumerator
    @see HashTable
    @version 1.0
  */
  template<class TRAITS>
  class HashTableValueEnumerator : public Enumerator<TRAITS> {
    friend class HashTableImpl;
  private:
    
    typedef typename Enumerator<TRAITS>::Pointer Pointer;
    typedef typename Enumerator<TRAITS>::Value Value;
    
    /** The hash set implementation. */
    Reference<HashTableImpl> impl;
    /** The current bucket. */
    Node** bucket = nullptr;
    /** The current position of the enumeration. */
    Node* node = nullptr;
    /** The number of elements. */
    unsigned int numberOfElements = 0;
  public:

    /**
      Initializes enumeration of hash table.
      
      @param hashTable The hash table.
    */
    inline HashTableValueEnumerator(
      Reference<HashTableImpl> _impl) throw()
      : impl(_impl),
        bucket(impl->getBuckets()),
        node(*bucket),
        numberOfElements(impl->getSize()) {
    }

    /**
      Returns true if there is more elements in this enumeration.
    */
    inline bool hasNext() const throw() {
      return numberOfElements;
    }

    /**
      Returns the current value and increments the position. Raises
      EndOfEnumeration if the end has been reached.
    */
    Pointer next() throw(EndOfEnumeration) {
      bassert(numberOfElements, EndOfEnumeration(this));
      while (!node) {
        ++bucket;
        node = *bucket;
      }
      Value& result = node->getValue();
      node = node->getNext();
      --numberOfElements;
      return &result; // TAG: change result type
    }
    
    inline ~HashTableValueEnumerator() throw() {
    }
  };
  
  
  
  /** Non-modifying enumerator. */
  typedef HashTableEnumerator<ReadEnumeratorTraits<Association<Key, Value> > > ReadEnumerator;
  /** Modifying value enumerator. */
  typedef HashTableValueEnumerator<EnumeratorTraits<Value> > ValueEnumerator;
  /** Non-modifying value enumerator. */
  typedef HashTableValueEnumerator<ReadEnumeratorTraits<Value> > ReadValueEnumerator;
  
  
  
  /** Hash table implementation. */
  Reference<HashTableImpl> impl;

  /**
    Copies the hash set if referenced by multiple automation pointers.
  */
  inline void copyOnWrite() throw(MemoryException) {
    impl.copyOnWrite();
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

  void dump() throw() {
    impl->dump();
  }
  
  /**
    Returns the capacity of the hash table.
  */
  inline unsigned int getCapacity() const throw() {
    return impl->getCapacity();
  }
  
  /**
    Returns the number of elements in the hash table.
  */
  inline unsigned int getSize() const throw() {
    return impl->getSize();
  }
  
  /**
    Returns true if the hash table is empty.
  */
  inline bool isEmpty() const throw() {
    return impl->getSize() == 0;
  }

  /**
    Returns true if the specified value is a key in the table.
  */
  inline bool isKey(const Key& key) const throw() {
    return impl->isKey(key);
  }

  /**
    Returns the value associated with the specified key. Raises InvalidKey
    if the specified key doesn't exist in this hash table.
    
    @param key The key of the value.
  */
  inline Value getValue(const Key& key) const throw(InvalidKey) {
    return impl->getValue(key);
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
  
  /**
    Returns a enumerator of the hash table for non-modifying access.
  */
  ReadEnumerator getReadEnumerator() const throw() {
    return ReadEnumerator(impl);
  }
  
  /**
    Returns a enumerator of the values of the hash table for modifying access.
  */
  ValueEnumerator getValueEnumerator() throw() {
    return ValueEnumerator(impl);
  }
  
  /**
    Returns a enumerator of the values of the hash table for non-modifying access.
  */
  ReadValueEnumerator getReadValueEnumerator() const throw() {
    return ReadValueEnumerator(impl);
  }
  
  /**
    Returns the value associated with the specified key when used as 'rvalue'.
    When used as 'lvalue' the key is associated with the specified value.
  */
  inline Element operator[](const Key& key) throw(InvalidKey, MemoryException) {
    return Element(*this, key);
  }
  
  /**
    Returns the value associated with the specified key.
  */
  inline Value operator[](const Key& key) const throw(InvalidKey) {
    return getValue(key);
  }
};

/**
  Writes the hash table to the format output stream.
*/
template<class KEY, class VALUE>
FormatOutputStream& operator<<(
  FormatOutputStream& stream,
  const HashTable<KEY, VALUE>& value) throw(IOException) {
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
