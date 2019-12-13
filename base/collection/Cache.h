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

#include <base/collection/HashTable.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Raised if unable to load the value from CacheProvider.
*/

class _COM_AZURE_DEV__BASE__API CacheException : public Exception {
public:
  
  _COM_AZURE_DEV__BASE__EXCEPTION_THIS_TYPE()
};

template<class KEY, class VALUE>
class CacheProvider {
public:
  
  /** Returns the value for the specified key. */
  virtual const VALUE& getValue(const KEY& key) throw(CacheException) = 0;
};

/**
  Cache implementation.
  
  @short Cache.
  @ingroup collections
  @version 1.0
*/

template<class KEY, class VALUE>
class Cache : public Collection {
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
  Cache() noexcept
  {
  }
  
  /**
    Initializes the cache with the specified initial capacity.
  */
  Cache(MemorySize capacity) throw(OutOfDomain)
    : elements(capacity)
  {
  }

  /**
    Initializes cache from other cache.
  */
  inline Cache(const Cache& copy) noexcept
    : elements(copy.elements)
  {
  }
  
  /**
    Assignment of cache by cache.
  */
  inline Cache& operator=(const Cache& assign) noexcept
  {
    elements = assign.elements;
    return *this;
  }
  
  /**
    Returns the number of elements in the cache.
  */
  inline MemorySize getSize() const noexcept
  {
    return elements.getSize();
  }
  
  /**
    Returns true if the cache is empty.
  */
  inline bool isEmpty() const noexcept
  {
    return elements.getSize() == 0;
  }

  /**
    Returns true if the specified object is in the cache.
  */
  inline bool isCached(const Key& key) const noexcept
  {
    return elements.hasKey(key);
  }

  /**
    Returns the value associated with the specified key. Raises InvalidKey
    if the specified key doesn't exist in the cache.
    
    @param key The key of the value.
  */
  inline const Value& getValue(const Key& key) const throw(CacheException)
  {
    if (!elements.hasKey(key)) {
      // Value value = provider->getValue();
      // elements.add(key, value);
      // return value;
    }
    return elements.getValue(key);
  }
  
  /**
    Adds the key and value to the cache.
  */
  inline void add(const Key& key, const Value& value)
  {
    elements.add(key, value);
  }
  
  /**
    Removes the specified key and its associated value from this cache.
    Raises InvalidKey if the key doesn't exist in the cache.
  */
  inline void remove(const Key& key) throw(InvalidKey)
  {
    elements.remove(key);
  }
  
  /**
    Removes all the keys from the cache.
  */
  inline void removeAll() noexcept {
    elements = HashTable<KEY, VALUE>();
  }

#if 0
  /**
    Returns a enumerator of the cache for non-modifying access.
  */
  inline ReadEnumerator getReadEnumerator() const noexcept
  {
    return elements.getReadEnumerator();
  }
  
  /**
    Returns a enumerator of the values of the cache for modifying access.
  */
  inline ValueEnumerator getValueEnumerator() noexcept
  {
    return elements.getValueEnumerator();
  }
  
  /**
    Returns a enumerator of the values of the cache for non-modifying access.
  */
  inline ReadValueEnumerator getReadValueEnumerator() const noexcept
  {
    return elements.getReadValueEnumerator();
  }
#endif

  /**
    Returns the value associated with the specified key when used as 'rvalue'.
    When used as 'lvalue' the key is associated with the specified value.
  */
  inline typename HashTable<KEY, VALUE>::Element operator[](const Key& key) throw(InvalidKey)
  {
    return elements[key];
  }
  
  /**
    Returns the value associated with the specified key.
  */
  inline const Value& operator[](const Key& key) const throw(InvalidKey)
  {
    return elements.getValue(key);
  }
};

/**
  Writes the content of the cache to the format output stream.

  @relates Cache
*/
template<class KEY, class VALUE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Cache<KEY, VALUE>& value)
{
  return stream << value;
}

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
