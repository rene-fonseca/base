/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2006 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

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
#include <base/mathematics/Math.h>
#include <base/mem/Reference.h>
#include <base/string/FormatOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

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
  static const unsigned int MINIMUM_CAPACITY = 16;
  /** The maximum capacity. */
  static const unsigned int MAXIMUM_CAPACITY = PrimitiveTraits<unsigned int>::MAXIMUM/2;
  /** The default capacity. */
  static const unsigned int DEFAULT_CAPACITY = 16;
  
  /*
    Node in the single linked list of a bucket.
  */
  class Node : public Object {
    friend class HashSetImpl;
  private:

    /** The hash of the element. */
    unsigned long hash;
    /** The value of the element. */
    Value value;
    /** The next node in the single linked list. */
    Node* next;
    // TAG: could have been bidirectional
  public:
    
    /**
      Initializes a node.
    */
    inline Node(unsigned long _hash, const Value& _value) throw()
      : hash(_hash),
        value(_value),
        next(0) {
    }
    
    /**
      Initializes a node.
    */
    inline Node(unsigned long _hash, const Value& _value, Node* _next) throw()
      : hash(_hash),
        value(_value),
        next(_next) {
    }

    /**
      Returns the hash value of the node.
    */
    inline unsigned long getHash() const throw() {
      return hash;
    }
    
    /**
      Returns the value of the node.
    */
    inline Value& getValue() throw() {
      return value;
    }
    
    /**
      Returns the value of the node.
    */
    inline const Value& getValue() const throw() {
      return value;
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
    Hash set implementation.
  */
  class HashSetImpl : public ReferenceCountedObject {
  private:
    
    /** Lookup table. */
    Allocator<Node*> table;
    /** The current capacity of the set. */
    unsigned int capacity = 0;
    /** Cache for (capacity - 1). */
    unsigned int mask = 0;
    /** Base 2 logarithm of the capacity. */
    unsigned int log2OfCapacity = 0;
    /** The number of elements in the set. */
    unsigned int size = 0;
    
    /**
      Returns the hash value of the value.
    */
    static inline unsigned long getHash(const Value& value) throw() {
      Hash<Value> hash; // Hash is a functor
      return hash(value);
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
      Initializes the hash set with the specified capacity.
    */
    inline HashSetImpl(unsigned int capacity) throw(MemoryException) {
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
      Initializes the hash set from another hash set.
    */
    inline HashSetImpl(const HashSetImpl& copy) throw(MemoryException)
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
      Reduces the capacity of the hash set.
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
      Returns the capacity of the hash set.
    */
    inline unsigned int getCapacity() const throw() {
      return capacity;
    }
    
    /**
      Returns the number of elements in the hash set.
    */
    inline unsigned int getSize() const throw() {
      return size;
    }

    /**
      Returns true if the specified value is in the hash set.
    */
    inline bool hasValue(const Value& key) throw() {
      const unsigned long hash = getHash(key);
      Node** bucket = getBuckets() + (hash & mask);
      
      Node* parent = 0;
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
    inline bool hasValue(const Value& value) const throw() {
      const unsigned long hash = getHash(value);
      const Node* const* bucket = getBuckets() + (hash & mask);
      const Node* child = *bucket;
      while (child &&
             ((child->getHash() != hash) || child->getValue() != value)) {
        child = child->getNext();
      }
      return child;
    }    
    
    /**
      Adds the element to the set.
    */
    inline void add(const Value& value) throw(MemoryException) {
      const unsigned long hash = getHash(value);
      Node** buckets = getBuckets() + (hash & mask);
      if (*buckets) {
        Node* parent = 0;
        Node* child = *buckets;
        while (child && ((child->getHash() != hash) || (child->getValue() != value))) {
          parent = child;
          child = child->getNext();
        }
        if (!child) { // if value not already in list
          child = new Node(hash, value);
          if (parent) {
            parent->setNext(child);
          }
          ++size;
        }
      } else {
        *buckets = new Node(hash, value);
        ++size;
      }
      if (size > capacity) { // TAG: find simple rules that works
        grow();
      }
    }
    
    /**
      Removes the specified value from the set.
    */
    inline void remove(const Value& value) throw(InvalidNode) {
      const unsigned long hash = getHash(value);
      Node** bucket = getBuckets() + (hash & mask);
      Node* child = *bucket;
      bassert(child, InvalidNode(this));      
      if ((child->getHash() == hash) && (child->getValue() == value)) {
        *bucket = child->getNext(); // unlink first node (next could be 0)
      } else {
        Node* parent = child;
        child = child->getNext();
        while (child && ((child->getHash() != hash) || (child->getValue() != value))) {
          parent = child;
          child = child->getNext();
        }
        bassert(child, InvalidNode(this));      
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
    inline ~HashSetImpl() throw() {
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
    
    typedef typename Enumerator<TRAITS>::Pointer Pointer;
    typedef typename Enumerator<TRAITS>::Value Value;
    
    /** The hash set implementation. */
    Reference<HashSetImpl> impl;
    /** The current bucket. */
    Node** bucket;
    /** The current position of the enumeration. */
    Node* node;
    /** The number of elements. */
    unsigned int numberOfElements;
  public:
      
    /**
      Initializes enumeration of hash set.
      
      @param hashSet The hash set.
    */
    inline HashSetEnumerator(Reference<HashSetImpl> _impl) throw()
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
      Pointer result = &node->getValue();
      node = node->getNext();
      --numberOfElements;
      return result;
    }

    inline ~HashSetEnumerator() throw() {
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
  inline void copyOnWrite() throw(MemoryException) {
    impl.copyOnWrite();
  }
public:

  /**
    Initializes an hash set.
  */
  HashSet() throw(MemoryException) : impl(new HashSetImpl(DEFAULT_CAPACITY)) {
  }
  
  /**
    Initializes the hash set with the specified initial capacity.
  */
  HashSet(unsigned int capacity) throw(MemoryException)
    : impl(new HashSetImpl(capacity)) {
  }
  
  /**
    Initializes hash set from another hash set.
  */
  HashSet(const HashSet& copy) throw() : impl(copy.impl) {
  }
  
  /**
    Assignment of hash set by hash set.
  */
  HashSet& operator=(const HashSet& eq) throw() {
    impl = eq.impl;
    return *this;
  }
  
  /**
    Returns the capacity of the hash set.
  */
  inline unsigned int getCapacity() const throw() {
    return impl->getCapacity();
  }

  /**
    Returns the number of elements in the hash set.
  */
  inline unsigned int getSize() const throw() {
    return impl->getSize();
  }
  
  /**
    Returns true if the hash set is empty.
  */
  inline bool isEmpty() const throw() {
    return impl->getSize() == 0;
  }

  /**
    Returns true if the specified value is in the set.
  */
  bool hasValue(const Value& value) const throw() {
    return impl->hasValue(value);
  }
  
  /**
    Adds the value to the set.
  */
  void add(const Value& value) throw(MemoryException) {
    copyOnWrite();
    impl->add(value);
  }
  
  /**
    Removes the specified value from this hash set. Raises InvalidNode if the
    value doesn't exist in the set.
  */
  void remove(const Value& value) throw(InvalidNode, MemoryException) {
    copyOnWrite();
    impl->remove(value);
  }
 
  /**
    Removes all the values from the hash set.
  */
  void removeAll() throw() {
    impl = new HashSetImpl(DEFAULT_CAPACITY); // initial capacity is unknown
  }
  
  /**
    Returns the enumerator of the hash set.
  */
  Enumerator getEnumerator() throw() {
    return Enumerator(impl);
  }

  /**
    Returns the read enumerator of the hash set.
  */
  ReadEnumerator getReadEnumerator() const throw() {
    return ReadEnumerator(impl);
  }
};

/**
  Writes the hash table to the format output stream.
*/
template<class TYPE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const HashSet<TYPE>& value) throw(IOException) {
  typename HashSet<TYPE>::ReadEnumerator enu = value.getReadEnumerator();
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
