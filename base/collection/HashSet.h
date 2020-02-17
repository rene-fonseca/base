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
#include <base/collection/InvalidNode.h>
#include <base/collection/Enumeration.h>
#include <base/MemoryException.h>
#include <base/math/Math.h>
#include <base/mem/Reference.h>
#include <base/string/StringOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  A hash set implementation.
  
  @short Hash set collection.
  @ingroup collections
  @version 1.2
*/

template<class TYPE>
class HashSet : public Collection {
public:
  
  /** The type of the value. */
  typedef TYPE Value;
  
  /** The minimum capacity. */
  static constexpr MemorySize MINIMUM_CAPACITY = 16;
  /** The maximum capacity. */
  static constexpr MemorySize MAXIMUM_CAPACITY = PrimitiveTraits<MemorySize>::MAXIMUM/2;
  /** The default capacity. */
  static constexpr MemorySize DEFAULT_CAPACITY = 16;
  
  /*
    Node in the single linked list of a bucket.
  */
  class Node : public Object {
    friend class HashSetImpl;
  private:

    /** The hash of the element. */
    unsigned long hash = 0;
    /** The value of the element. */
    Value value;
    /** The next node in the single linked list. */
    Node* next = nullptr;
  public:
    
    /**
      Initializes a node.
    */
    inline Node(unsigned long _hash, const Value& _value) noexcept
      : hash(_hash),
        value(_value),
        next(nullptr)
    {
    }
    
    /**
      Initializes a node.
    */
    inline Node(unsigned long _hash, const Value& _value, Node* _next) noexcept
      : hash(_hash),
        value(_value),
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
      Returns the value of the node.
    */
    inline Value& getValue() noexcept
    {
      return value;
    }
    
    /**
      Returns the value of the node.
    */
    inline const Value& getValue() const noexcept
    {
      return value;
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
    Hash set implementation.
  */
  class HashSetImpl : public ReferenceCountedObject {
  private:
    
    /** Lookup table. */
    Allocator<Node*> table;
    /** The current capacity of the set. */
    MemorySize capacity = 0;
    /** Cache for (capacity - 1). */
    MemorySize mask = 0;
    /** Base 2 logarithm of the capacity. */
    MemorySize log2OfCapacity = 0;
    /** The number of elements in the set. */
    MemorySize size = 0;
    
    /**
      Returns the hash value of the value.
    */
    static inline unsigned long getHash(const Value& value) noexcept
    {
      Hash<Value> hash; // Hash is a functor
      return hash(value);
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
      Initializes the hash set with the specified capacity.
    */
    HashSetImpl(MemorySize capacity)
    {
      capacity = maximum(capacity, MINIMUM_CAPACITY);
      capacity = minimum(capacity, MAXIMUM_CAPACITY);
      log2OfCapacity = Math::iLog2(capacity);
      if ((static_cast<MemorySize>(1U) << log2OfCapacity) < capacity) {
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
      Initializes the hash set from another hash set.
    */
    inline HashSetImpl(const HashSetImpl& copy)
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
          Node* firstNode = new Node(srcNode->getHash(), srcNode->getValue());
          *dest++ = firstNode;
          Node* destNode = firstNode;
          srcNode = srcNode->getNext();
          while (srcNode) {
            destNode->setNext(new Node(srcNode->getHash(), srcNode->getValue()));
            destNode = destNode->getNext();
            srcNode = srcNode->getNext();
          }
        } else {
          *dest++ = 0;
        }
      }
    }
    
    /**
      Increases the capacity of the hash set.
    */
    void grow()
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
      Reduces the capacity of the hash set.
    */
    inline void shrink()
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
      Returns the capacity of the hash set.
    */
    inline MemorySize getCapacity() const noexcept
    {
      return capacity;
    }
    
    /**
      Returns the number of elements in the hash set.
    */
    inline MemorySize getSize() const noexcept
    {
      return size;
    }

    /**
      Returns true if the specified value is in the hash set.
    */
    bool hasValue(const Value& key) noexcept
    {
      const unsigned long hash = getHash(key);
      Node** bucket = getBuckets() + (hash & mask);
      
      Node* parent = nullptr;
      Node* child = *bucket;
      if (child) {
        if ((child->getHash() == hash) && (*child->getKey() == key)) {
          return true; // this case should be fairly likely
        }
        parent = child;
        child = child->getNext();
        while (child &&
               ((child->getHash() != hash) || (*child->getKey() != key))) {
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
      Returns true if the specified value is in the hash set.
    */
    bool hasValue(const Value& value) const noexcept
    {
      const unsigned long hash = getHash(value);
      const Node* const* bucket = getBuckets() + (hash & mask);
      const Node* child = *bucket;
      while (child &&
             ((child->getHash() != hash) || child->getValue() != value)) {
        child = child->getNext();
      }
      return child;
    }    

    inline bool hasKey(const Value& key) const noexcept
    {
      return hasValue(key);
    }

    /**
      Adds the element to the set.
    */
    void add(const Value& value)
    {
      const unsigned long hash = getHash(value);
      Node** buckets = getBuckets() + (hash & mask);
      if (*buckets) {
        Node* parent = nullptr;
        Node* child = *buckets;
        while (child && ((child->getHash() != hash) || (child->getValue() != value))) {
          parent = child;
          child = child->getNext();
        }
        if (child) { // found value
          child->getValue() = value; // force assignment to allow association
        } else { // if value not already in list
          child = new Node(hash, value);
          if (parent) {
            parent->setNext(child);
          } else {
            *buckets = child;
          }
          ++size;
        }
      } else {
        *buckets = new Node(hash, value);
        ++size;
      }
      if (size > capacity) { // what is the best criteria
        grow();
      }
    }
    
    /**
      Removes the specified value from the set.
    */
    void remove(const Value& value)
    {
      const unsigned long hash = getHash(value);
      Node** bucket = getBuckets() + (hash & mask);
      Node* child = *bucket;
      if (!child) {
        _throw InvalidNode(this);
      }
      if ((child->getHash() == hash) && (child->getValue() == value)) {
        *bucket = child->getNext(); // unlink first node (next could be 0)
      } else {
        Node* parent = child;
        child = child->getNext();
        while (child && ((child->getHash() != hash) || (child->getValue() != value))) {
          parent = child;
          child = child->getNext();
        }
        if (!child) {
          _throw InvalidNode(this);
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
      Destroys the hash set.
    */
    ~HashSetImpl()
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
  class HashSetEnumerator : public base::Enumerator<TRAITS> {
    friend class HashSetImpl;
  private:
    
    typedef typename Enumerator<TRAITS>::Value Value;
    typedef typename Enumerator<TRAITS>::Reference Reference;
    typedef typename Enumerator<TRAITS>::Pointer Pointer;
    
    /** The hash set implementation. */
    R<HashSetImpl> impl;
    /** The current bucket. */
    Node** bucket = nullptr;
    /** The current position of the enumeration. */
    Node* node = nullptr;
    /** The number of elements. */
    MemorySize numberOfElements = 0;
  public:
      
    /**
      Initializes enumeration of hash set.
      
      @param hashSet The hash set.
    */
    inline HashSetEnumerator(R<HashSetImpl> _impl) noexcept
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
    Reference next()
    {
      if (!numberOfElements) {
        _throw EndOfEnumeration(this);
      }
      while (!node) {
        ++bucket;
        node = *bucket;
      }
      Reference result = node->getValue();
      node = node->getNext();
      --numberOfElements;
      return result;
    }

    inline ~HashSetEnumerator() noexcept
    {
    }
  };
  
  
  
  /** Modifying enumerator. */
  typedef HashSetEnumerator<EnumeratorTraits<Value> > Enumerator;
  /** Non-modifying enumerator. */
  typedef HashSetEnumerator<ReadEnumeratorTraits<Value> > ReadEnumerator;
  
  
  
  /** Hash set implementation. */
  Reference<HashSetImpl> impl;

  /**
    Copies the hash set if referenced by multiple automation pointers.
  */
  void copyOnWrite()
  {
    impl.copyOnWrite();
  }
public:

  /**
    Initializes an hash set.
  */
  HashSet()
    : impl(new HashSetImpl(DEFAULT_CAPACITY))
  {
  }
  
  HashSet(std::initializer_list<TYPE> values)
    : impl(new HashSetImpl(DEFAULT_CAPACITY))
  {
    for (const auto& value : values) {
      add(value);
    }
  }
  
  /**
    Initializes the hash set with the specified initial capacity.
  */
  HashSet(MemorySize capacity) 
    : impl(new HashSetImpl(capacity))
  {
  }
  
  /**
    Initializes hash set from another hash set.
  */
  HashSet(const HashSet& copy) noexcept
    : impl(copy.impl)
  {
  }
  
  /**
    Assignment of hash set by hash set.
  */
  HashSet& operator=(const HashSet& assign) noexcept
  {
    impl = assign.impl;
    return *this;
  }
  
  /**
    Returns the capacity of the hash set.
  */
  inline MemorySize getCapacity() const noexcept
  {
    return impl->getCapacity();
  }

  /**
    Returns the number of elements in the hash set.
  */
  inline MemorySize getSize() const noexcept
  {
    return impl->getSize();
  }
  
  /**
    Returns true if the hash set is empty.
  */
  inline bool isEmpty() const noexcept
  {
    return impl->getSize() == 0;
  }

  /**
    Returns true if the specified value is in the set.
  */
  bool hasValue(const Value& value) const noexcept
  {
    return impl->hasValue(value);
  }

  /**
    Returns true if the specified value is in the set.
  */
  inline bool hasKey(const Value& key) const noexcept
  {
    return hasValue(key);
  }

  /**
    Adds the value to the set.
  */
  void add(const Value& value)
  {
    copyOnWrite();
    impl->add(value);
  }
  
  /**
    Removes the specified value from this hash set. Raises InvalidNode if the
    value doesn't exist in the set.
  */
  void remove(const Value& value)
  {
    copyOnWrite();
    impl->remove(value);
  }
 
  /**
    Removes all the values from the hash set.
  */
  void removeAll()
  {
    impl = new HashSetImpl(DEFAULT_CAPACITY); // initial capacity is unknown
  }
  
  /**
    Returns the enumerator of the hash set.
  */
  Enumerator getEnumerator() noexcept
  {
    return Enumerator(impl);
  }

  /**
    Returns the read enumerator of the hash set.
  */
  ReadEnumerator getReadEnumerator() const noexcept
  {
    return ReadEnumerator(impl);
  }
  
  /** Returns true is non-empty. */
  inline operator bool() const noexcept
  {
    return impl->getSize() != 0;
  }
  
  /** Adds value. */
  inline HashSet& operator<<(const Value& key)
  {
    add(key);
    return *this;
  }

  /** Adds value. */
  inline HashSet& operator<<(Value&& key)
  {
    add(moveObject(key));
    return *this;
  }
};

/**
  Writes the hash table to the format output stream.
*/
template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const HashSet<TYPE>& value)
{
  typename HashSet<TYPE>::ReadEnumerator enu = value.getReadEnumerator();
  stream << '{';
  while (enu.hasNext()) {
    stream << enu.next();
    if (enu.hasNext()) {
      stream << ';';
    }
  }
  stream << '}';
  return stream;
}

_COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_CONTAINER(HashSet)

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
