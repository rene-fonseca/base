/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2002-2003 by Rene Moeller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#pragma once

#include <base/collection/HashTable.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Raised if unable to load the value from CacheProvider.
*/

class _DK_SDU_MIP__BASE__API CacheException : public Exception {
public:
  
};

template<class KEY, class VALUE>
class _DK_SDU_MIP__BASE__API CacheProvider {
public:
  
  /** Returns the value for the specified key. */
  virtual VALUE getValue(KEY key) throw(CacheException) = 0;
};

/**
  Cache implementation.
  
  @short Cache.
  @ingroup collections
  @version 1.0
*/

template<class KEY, class VALUE>
class _DK_SDU_MIP__BASE__API Cache : public Collection {
public:

  typedef KEY Key;
  typedef VALUE Value;
  /** The values in the cache. */
  HashTable<KEY, VALUE> elements;
public:
  
  // time out - need active object
  // need time attribute or cost
  
  /**
    Initializes the cache.
  */
  Cache() throw(MemoryException) {
  }
  
  /**
    Initializes the cache with the specified initial capacity.
  */
  Cache(unsigned int capacity) throw(OutOfDomain, MemoryException)
    : elements(capacity) {
  }

  /**
    Initializes cache from other cache.
  */
  inline Cache(const Cache& copy) throw() : elements(copy.elements) {
  }
  
  /**
    Assignment of cache by cache.
  */
  inline Cache& operator=(const Cache& eq) throw() {
    elements = eq.elements;
    return *this;
  }
  
  /**
    Returns the number of elements in the cache.
  */
  inline unsigned int getSize() const throw() {
    return elements.getSize();
  }
  
  /**
    Returns true if the cache is empty.
  */
  inline bool isEmpty() const throw() {
    return elements.getSize() == 0;
  }

  /**
    Returns true if the specified object is in the cache.
  */
  inline bool isCached(const Key& key) const throw() {
    return elements.isKey(key);
  }

  /**
    Returns the value associated with the specified key. Raises InvalidKey
    if the specified key doesn't exist in the cache.
    
    @param key The key of the value.
  */
  inline Value getValue(const Key& key) const throw(CacheException) {
    if (!elements.isKey(key)) {
      // Value value = provider->getValue();
      // elements.add(key, value);
      // return value;
    }
    return elements.getValue(key);
  }
  
  /**
    Adds the key and value to the cache.
  */
  inline void add(const Key& key, const Value& value) throw(MemoryException) {
    elements.add(key, value);
  }
  
  /**
    Removes the specified key and its associated value from this cache.
    Raises InvalidKey if the key doesn't exist in the cache.
  */
  inline void remove(const Key& key) throw(InvalidKey, MemoryException) {
    elements.remove(key);
  }
  
  /**
    Removes all the keys from the cache.
  */
  inline void removeAll() throw() {
    elements = HashTable<KEY, VALUE>();
  }

#if 0
  /**
    Returns a enumerator of the cache for non-modifying access.
  */
  inline ReadEnumerator getReadEnumerator() const throw() {
    return elements.getReadEnumerator();
  }
  
  /**
    Returns a enumerator of the values of the cache for modifying access.
  */
  inline ValueEnumerator getValueEnumerator() throw() {
    return elements.getValueEnumerator();
  }
  
  /**
    Returns a enumerator of the values of the cache for non-modifying access.
  */
  inline ReadValueEnumerator getReadValueEnumerator() const throw() {
    return elements.getReadValueEnumerator();
  }
#endif

  /**
    Returns the value associated with the specified key when used as 'rvalue'.
    When used as 'lvalue' the key is associated with the specified value.
  */
  inline typename HashTable<KEY, VALUE>::Element operator[](const Key& key) throw(InvalidKey, MemoryException) {
    return elements[key];
  }
  
  /**
    Returns the value associated with the specified key.
  */
  inline Value operator[](const Key& key) const throw(InvalidKey) {
    return elements.getValue(key);
  }
};

/**
  Writes the content of the cache to the format output stream.

  @relates Cache
*/
template<class KEY, class VALUE>
FormatOutputStream& operator<<(
  FormatOutputStream& stream, const Cache<KEY, VALUE>& value) throw(IOException) {
  return stream << value;
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
