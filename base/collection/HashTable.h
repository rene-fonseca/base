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
#include <base/math/Math.h>
#include <base/mem/Reference.h>
#include <base/string/FormatOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  A hash table implementation.
  
  @short Hash table collection.
  @ingroup collections
  @version 1.2
*/

template<class KEY, class VALUE>
class HashTable : public Collection {
public:
  
  /** The type of the key. */
  typedef KEY Key;
  /** The type of the value. */
  typedef VALUE Value;
  /** The type of an association in the hash table. */
  typedef Association<Key, Value> HashTableAssociation;
  
  /** The minimum capacity. */
  static constexpr MemorySize MINIMUM_CAPACITY = 16;
  /** The maximum capacity. */
  static constexpr MemorySize MAXIMUM_CAPACITY = PrimitiveTraits<unsigned int>::MAXIMUM/2;
  /** The default capacity. */
  static constexpr MemorySize DEFAULT_CAPACITY = 16;
  
  /*
    Reference to an element within a hash table.
  */
  class Element {
    friend class HashTable;
  private:
    
    HashTable& hashTable;
    const Key& key;
    Element(const Element& copy) noexcept;
    Element& operator=(const Element& assign) noexcept;

    inline Element(HashTable& _hashTable, const Key& _key) noexcept
      : hashTable(_hashTable),
        key(_key)
    {
    }
  public:

    /**
      Sets the value of the element.
    */
    inline Element& operator=(const Value& value)
    {
      hashTable.add(key, value);
      return *this;
    }

    /**
      Returns the value of the element.
    */
    inline operator const Value&() const
    {
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
    inline Node(unsigned long _hash, const Key& _key, const Value& _value) noexcept
      : hash(_hash),
        value(_key, _value)
    {
    }
    
    /**
      Initializes a node.
    */
    inline Node(unsigned long _hash, const Key& _key, const Value& _value, Node* _next) noexcept
      : hash(_hash),
        value(_key, _value),
        next(_next)
    {
    }
    
    /**
      Returns the hash value of the node.
    */
    inline unsigned long getHash() const noexcept
    {
      return hash;
    }
    
    /**
      Returns the key of the node.
    */
    inline const Association<Key, Value>& getKeyValue() const noexcept
    {
      return value;
    }
    
    /**
      Returns the key of the node.
    */
    inline const Key& getKey() const noexcept
    {
      return value.getKey();
    }
    
    /**
      Returns the value of the node.
    */
    inline Value& getValue() noexcept
    {
      return value.getValue();
    }
    
    /**
      Returns the value of the node.
    */
    inline const Value& getValue() const noexcept
    {
      return value.getValue();
    }

    /**
      Returns the next node.
    */
    inline Node* getNext() noexcept
    {
      return next;
    }
    
    /**
      Returns the next node.
    */
    inline const Node* getNext() const noexcept
    {
      return next;
    }
    
    /**
      Sets the next node.
    */
    inline void setNext(Node* node) noexcept
    {
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
    MemorySize capacity = 0; // use size in table attribute
    /** Cache for (capacity - 1). */
    MemorySize mask = 0;
    /** Base 2 logarithm of the capacity. */
    MemorySize log2OfCapacity = 0;
    /** The number of elements in the table. */
    MemorySize size = 0;
    
    /**
      Returns the hash value of the key.
    */
    static inline unsigned long getHash(const Key& key) noexcept
    {
      Hash<Key> hash; // Hash is a functor
      return hash(key);
    }
  public:

    /**
      Returns the buckets for modifying access.
    */
    inline Node** getBuckets() noexcept
    {
      return table.getElements();
    }

    /**
      Returns the buckets for non-modifying access.
    */
    inline const Node* const* getBuckets() const noexcept
    {
      return table.getElements();
    }
    
    /**
      Initializes the hash table with the specified capacity.
    */
    HashTableImpl(MemorySize capacity)
    {
      capacity = maximum(capacity, MINIMUM_CAPACITY);
      capacity = minimum(capacity, MAXIMUM_CAPACITY);
      log2OfCapacity = Math::iLog2(capacity);
      if ((static_cast<MemorySize>(1) << log2OfCapacity) < capacity) {
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
    HashTableImpl(const HashTableImpl& copy)
      : table(copy.capacity),
        capacity(copy.capacity),
        mask(copy.mask),
        log2OfCapacity(copy.log2OfCapacity),
        size(copy.size)
    {
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
    inline void grow()
    {
      if (capacity < MAXIMUM_CAPACITY) {
        capacity *= 2;
        mask = capacity - 1;
        table.setSize(capacity); // upper entries set below
        ++log2OfCapacity;
        
        // unfold buckets
        Node** lowerBucket = getBuckets();
        Node** upperBucket = lowerBucket + capacity/2;
        const Node* const* endBucket = upperBucket;
        const MemorySize bitMask = capacity/2; // the bit that desides the half
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
                // destNode->setNext(nullptr); // only set for last node
                
                while (srcNode) {
                  Node* nextNode = srcNode->getNext();
                  if (srcNode->getHash() & bitMask) {
                    parentSrcNode->setNext(nextNode); // unlink
                    destNode->setNext(srcNode);
                    destNode = destNode->getNext();
                    // destNode->setNext(nullptr); // only set for last node
                  }
                  parentSrcNode = srcNode;
                  srcNode = nextNode;
                }
                destNode->setNext(nullptr);
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
                // destNode->setNext(nullptr); // only set for last node
                
                while (srcNode) {
                  Node* nextNode = srcNode->getNext();
                  if ((srcNode->getHash() & bitMask) == 0) {
                    parentSrcNode->setNext(nextNode); // unlink
                    destNode->setNext(srcNode);
                    destNode = destNode->getNext();
                    // destNode->setNext(nullptr); // only set for last node
                  }
                  parentSrcNode = srcNode;
                  srcNode = nextNode;
                }
                destNode->setNext(nullptr); // terminate linked list
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
    inline void shrink() noexcept
    {
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
    inline MemorySize getCapacity() const noexcept
    {
      return capacity;
    }
    
    /**
      Returns the number of elements in the hash table.
    */
    inline MemorySize getSize() const noexcept
    {
      return size;
    }

    /**
      Returns true if the specified value is a key in the hash table.
    */
    inline bool hasKey(const Key& key) noexcept
    {
      const unsigned long hash = getHash(key);
      Node** bucket = getBuckets() + (hash & mask);
      
      Node* parent = nullptr;
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
    inline bool hasKey(const Key& key) const noexcept
    {
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

      @param key The key of the value.

      @return nullptr is key doesn't exist.
    */
    inline Value* find(const Key& key)
    {
      const unsigned long hash = getHash(key);
      Node** bucket = getBuckets() + (hash & mask);
      Node* child = *bucket;
      while (child &&
             ((child->getHash() != hash) || (child->getKey() != key))) {
        child = child->getNext();
      }
      return child ? &(child->getValue()) : nullptr;
    }

    /**
      Returns the value associated with the specified key.

      @param key The key of the value.

      @return nullptr is key doesn't exist.
    */
    inline const Value* find(const Key& key) const
    {
      const unsigned long hash = getHash(key);
      const Node* const* bucket = getBuckets() + (hash & mask);
      const Node* child = *bucket;
      while (child &&
             ((child->getHash() != hash) || (child->getKey() != key))) {
        child = child->getNext();
      }
      return child ? &(child->getValue()) : nullptr;
    }
    
    /**
      Returns the value associated with the specified key.
    */
    inline Value& getValue(const Key& key)
    {
      const unsigned long hash = getHash(key);
      Node** bucket = getBuckets() + (hash & mask);
      Node* grandparent = nullptr;
      Node* parent = nullptr;
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
      if (!child) {
        _throw InvalidKey(this);
      }
      return *child->getValue();
    }
    
    /**
      Returns the value associated with the specified key.
    */
    inline const Value& getValue(const Key& key) const
    {
      const unsigned long hash = getHash(key);
      const Node* const* bucket = getBuckets() + (hash & mask);
      const Node* child = *bucket;
      while (child &&
             ((child->getHash() != hash) || (child->getKey() != key))) {
        child = child->getNext();
      }
      if (!child) {
        _throw InvalidKey(this);
      }
      return child->getValue();
    }
    
    /**
      Adds the element to the table.
    */
    inline void add(const Key& key, const Value& value)
    {
      const unsigned long hash = getHash(key);
      Node** buckets = getBuckets() + (hash & mask);
      if (*buckets) {
        Node* parent = nullptr;
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

    void dump()
    {
      Node** bucket = getBuckets();
      const Node* const* endBucket = bucket + capacity;
      MemorySize index = 0;
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
    inline void remove(const Key& key)
    {
      const unsigned long hash = getHash(key);
      Node** bucket = getBuckets() + (hash & mask);
      Node* child = *bucket;
      if (!child) {
        _throw InvalidKey(this);
      }
      if ((child->getHash() == hash) && (child->getKey() == key)) {
        *bucket = child->getNext(); // unlink first node (next could be 0)
      } else {
        Node* parent = child;
        child = child->getNext();
        while (child && ((child->getHash() != hash) || (child->getKey() != key))) {
          parent = child;
          child = child->getNext();
        }
        if (!child) {
          _throw InvalidKey(this);
        }
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
    inline ~HashTableImpl()
    {
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
    MemorySize numberOfElements = 0;
  public:
      
    /**
      Initializes enumeration of hash table.
      
      @param hashTable The hash table.
    */
    inline HashTableEnumerator(
      Reference<HashTableImpl> _impl) noexcept
      : impl(_impl),
        bucket(impl->getBuckets()),
        node(*bucket),
        numberOfElements(impl->getSize())
    {
    }

    /**
      Returns true if there is more elements in this enumeration.
    */
    inline bool hasNext() const noexcept
    {
      return numberOfElements;
    }

    /**
      Returns the current value and increments the position. Raises
      EndOfEnumeration if the end has been reached.
    */
    Pointer next()
    {
      if (!numberOfElements) {
        _throw EndOfEnumeration(this);
      }
      while (!node) {
        ++bucket;
        node = *bucket;
      }
      const Value& result = node->getKeyValue();
      node = node->getNext();
      --numberOfElements;
      return &result; // TAG: change type
    }

    inline ~HashTableEnumerator() noexcept
    {
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
    MemorySize numberOfElements = 0;
  public:

    /**
      Initializes enumeration of hash table.
      
      @param hashTable The hash table.
    */
    inline HashTableValueEnumerator(Reference<HashTableImpl> _impl) noexcept
      : impl(_impl),
        bucket(impl->getBuckets()),
        node(*bucket),
        numberOfElements(impl->getSize())
    {
    }

    /**
      Returns true if there is more elements in this enumeration.
    */
    inline bool hasNext() const noexcept
    {
      return numberOfElements;
    }

    /**
      Returns the current value and increments the position. Raises
      EndOfEnumeration if the end has been reached.
    */
    Pointer next()
    {
      if (!numberOfElements) {
        _throw EndOfEnumeration(this);
      }
      while (!node) {
        ++bucket;
        node = *bucket;
      }
      Value& result = node->getValue();
      node = node->getNext();
      --numberOfElements;
      return &result; // TAG: change result type
    }
    
    inline ~HashTableValueEnumerator() noexcept
    {
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
  inline void copyOnWrite()
  {
    impl.copyOnWrite();
  }
public:

  /**
    Initializes an hash table.
  */
  HashTable()
    : impl(new HashTableImpl(DEFAULT_CAPACITY))
  {
  }
  
  /**
    Initializes the hash table with the specified initial capacity.
  */
  HashTable(MemorySize capacity)
    : impl(new HashTableImpl(capacity))
  {
  }
  
  HashTable(std::initializer_list<HashTableAssociation> values)
    : impl(new HashTableImpl(DEFAULT_CAPACITY))
  {
    for (const auto& value : values) {
      add(value);
    }
  }
  
  /**
    Initializes hash table from other hash table.
  */
  inline HashTable(const HashTable& copy) noexcept
    : impl(copy.impl)
  {
  }
  
  /**
    Assignment of hash table by hash table.
  */
  inline HashTable& operator=(const HashTable& assign) noexcept
  {
    impl = assign.impl;
    return *this;
  }

  void dump() noexcept
  {
    impl->dump();
  }
  
  /**
    Returns the capacity of the hash table.
  */
  inline MemorySize getCapacity() const noexcept
  {
    return impl->getCapacity();
  }
  
  /**
    Returns the number of elements in the hash table.
  */
  inline MemorySize getSize() const noexcept
  {
    return impl->getSize();
  }
  
  /**
    Returns true if the hash table is empty.
  */
  inline bool isEmpty() const noexcept
  {
    return impl->getSize() == 0;
  }

  /**
    Returns true if the specified value is a key in the table.
  */
  inline bool hasKey(const Key& key) const noexcept
  {
    return impl->hasKey(key);
  }

  /**
    Returns the value associated with the specified key.

    @param key The key of the value.

    @return nullptr is key doesn't exist.
  */
  inline Value* find(const Key& key)
  {
    return impl->find(key);
  }

  /**
    Returns the value associated with the specified key.

    @param key The key of the value.

    @return nullptr is key doesn't exist.
  */
  inline const Value* find(const Key& key) const
  {
    return impl->find(key);
  }
  
  /**
    Returns the value associated with the specified key. Raises InvalidKey
    if the specified key doesn't exist in this hash table.
    
    @param key The key of the value.
  */
  inline const Value& getValue(const Key& key) const
  {
    return impl->getValue(key);
  }
  
  /**
    Adds the key and value to the table.
  */
  inline void add(const Key& key, const Value& value)
  {
    copyOnWrite();
    impl->add(key, value);
  }

  /**
    Adds the key and value to the table.
  */
  inline void add(const Key& key, Value&& value)
  {
    copyOnWrite();
    impl->add(key, moveObject(value));
  }

  /**
    Adds the key and value to the table.
  */
  inline void add(Key&& key, Value&& value)
  {
    copyOnWrite();
    impl->add(moveObject(key), moveObject(value));
  }
  
  /**
    Adds the key and value to the table.
  */
  inline void add(const HashTableAssociation& node)
  {
    copyOnWrite();
    impl->add(node.getKey(), node.getValue());
  }
  
  /**
    Adds the key and value to the table.
  */
  inline void add(HashTableAssociation&& node)
  {
    copyOnWrite();
    impl->add(node.getKey(), moveObject(node.getValue()));
  }
  
  /**
    Removes the specified key and its associated value from this hash table.
    Raises InvalidKey if the key doesn't exist in this hash table.
  */
  inline void remove(const Key& key)
  {
    copyOnWrite();
    impl->remove(key);
  }
  
  /**
    Removes all the keys from this hash table.
  */
  void removeAll() noexcept
  {
    impl = new HashTableImpl(DEFAULT_CAPACITY); // initial capacity is unknown
  }
  
  /**
    Returns a enumerator of the hash table for non-modifying access.
  */
  ReadEnumerator getReadEnumerator() const noexcept
  {
    return ReadEnumerator(impl);
  }
  
  /**
    Returns a enumerator of the values of the hash table for modifying access.
  */
  ValueEnumerator getValueEnumerator() noexcept
  {
    return ValueEnumerator(impl);
  }
  
  /**
    Returns a enumerator of the values of the hash table for non-modifying access.
  */
  ReadValueEnumerator getReadValueEnumerator() const noexcept
  {
    return ReadValueEnumerator(impl);
  }
  
  /**
    Returns the value associated with the specified key when used as 'rvalue'.
    When used as 'lvalue' the key is associated with the specified value.
  */
  inline Element operator[](const Key& key)
  {
    return Element(*this, key);
  }
  
  /**
    Returns the value associated with the specified key.
  */
  inline const Value& operator[](const Key& key) const
  {
    return getValue(key);
  }
  
  /** Returns true is non-empty. */
  inline operator bool() const noexcept
  {
    return impl->getSize() != 0;
  }
};

/**
  Writes the hash table to the format output stream.
*/
template<class KEY, class VALUE>
FormatOutputStream& operator<<(
  FormatOutputStream& stream,
  const HashTable<KEY, VALUE>& value) {
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

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
