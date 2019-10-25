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

#include <base/collection/OrderedBinaryTree.h>
#include <base/collection/Association.h>
#include <base/MemoryException.h>
#include <base/collection/InvalidKey.h>
#include <base/string/FormatOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Map collection implemented using an ordered binary tree.

  @short Map collection.
  @ingroup collections
  @version 1.1
*/

template<class KEY, class VALUE>
class Map : public Collection {
public:

  /** The type of a key in the map. */
  typedef KEY Key;
  /** The type of a value in the map. */
  typedef VALUE Value;
  /** The type of an association in the map. */
  typedef Association<Key, Value> Node;
private:

  /** The associations of the map. */
  OrderedBinaryTree<Node> elements;
  /** The number of associations in the map. */
  MemorySize size = 0;
public:

  /**
    @short Non-modifying enumerator of map.
  */
  class ReadEnumerator : public Enumerator<ReadEnumeratorTraits<Node> > {
  private:

    typedef typename Enumerator<ReadEnumeratorTraits<Node> >::Pointer Pointer;
    typename OrderedBinaryTree<Node>::ReadEnumerator enu;
  public:

    inline ReadEnumerator(
      typename OrderedBinaryTree<Node>::ReadEnumerator _enu) noexcept
      : enu(_enu)
    {
    }

    inline bool hasNext() const noexcept
    {
      return enu.hasNext();
    }

    inline Pointer next() throw(EndOfEnumeration)
    {
      return enu.next()->getValue();
    }
  };

  /**
    @short Modifying enumerator of values of map.
    @version 1.0
  */
  class ValueEnumerator : public Enumerator<EnumeratorTraits<Value> > {
  private:

    typedef typename ValueEnumerator::Pointer Pointer;
    typename OrderedBinaryTree<Node>::Enumerator enu;
  public:

    inline ValueEnumerator(
      typename OrderedBinaryTree<Node>::Enumerator _enu) noexcept
      : enu(_enu)
    {
    }

    inline bool hasNext() const noexcept
    {
      return enu.hasNext();
    }

    inline Pointer next() throw(EndOfEnumeration)
    {
      return &enu.next()->getValue()->getValue();
    }
  };

  /*
    Reference to an element within a map.
  */
  class Element {
    friend class Map;
  private:
    
    Map& map;
    const Key& key;
    Element(const Element& copy) noexcept;
    Element& operator=(const Element& eq) noexcept;
    
    inline Element(Map& _map, const Key& _key) noexcept
      : map(_map), key(_key) {
    }
  public:
    
    inline Element& operator=(const Value& value)
    {
      map.add(key, value);
      return *this;
    }
    
    inline operator const Value&() const throw(InvalidKey)
    {
      return map.getValue(key);
    }
  };
public:

  /**
    Initializes an empty map.
  */
  Map() noexcept
  {
  }

  /**
    Initializes map from other map.
  */
  Map(const Map& copy) noexcept
    : elements(copy.elements),
      size(copy.size)
  {
  }

  /**
    Assignment of map to map.
  */
  Map& operator=(const Map& eq)
  {
    elements = eq.elements;
    size = eq.size;
    return *this;
  }

  /**
    Returns the number of associations in the map.
  */
  inline MemorySize getSize() const noexcept
  {
    return size;
  }

  /**
    Returns true if the map is empty.
  */
  inline bool isEmpty() const noexcept
  {
    return size == 0;
  }

  /**
    Returns a modifying enumerator of the ordered binary tree.
  */
  inline ValueEnumerator getValueEnumerator() noexcept
  {
    return elements.getEnumerator();
  }

  /**
    Returns a non-modifying enumerator of the ordered binary tree.
  */
  inline ReadEnumerator getReadEnumerator() const noexcept
  {
    return elements.getReadEnumerator();
  }

  /**
    Returns true if the specified key is associated with a value in this map.

    @param key The value to search for.
  */
  bool isKey(const Key& key) const noexcept
  {
    return elements.find(Association<Key, Value>(key));
  }

  /**
    Returns the value associated with the specified key. Raises InvalidKey
    if the specified key doesn't exist in this map.

    @param key The key of the value.
  */
  const Value& getValue(const Key& key) const throw(InvalidKey)
  {
    const typename OrderedBinaryTree<Association<Key, Value> >::Node* node =
      elements.find(Association<Key, Value>(key));
    if (!node) {
      throw InvalidKey();
    }
    return node->getValue()->getValue();
  }

  /**
    Associates the specified key with the specified value in this map. If the
    key already is associated with a value, the value is overridden.

    @param key The key.
    @param value The value.
  */
  void add(const Key& key, const Value& value)
  {
    Node* result = elements.add(Association<Key, Value>(key, value));
    if (result) {
      // key already exists
      result->setValue(value); // set the new value
    } else {
      // key did not exist
      ++size;
    }
  }

  /**
    Removes the specified key and its associated value from this map. Raises
    InvalidKey if the key doesn't exist in this map.
  */
  void remove(const Key& key) throw(InvalidKey)
  {
    elements.remove(elements.find(Association<Key, Value>(key)));
    --size; // never ends up here if the key doesn't exist
  }

  /**
    Removes all the keys from this map.
  */
  void removeAll() noexcept
  {
    elements.removeAll();
    size = 0;
  }

  /**
    Returns the value associated with the specified key when used as 'rvalue'.
    When used as 'lvalue' the key is associated with the specified value.
  */
  inline Element operator[](const Key& key) throw(InvalidKey)
  {
    return Element(*this, key);
  }

  /**
    Returns the value associated with the specified key.
  */
  inline const Value& operator[](const Key& key) const throw(InvalidKey)
  {
    return getValue(key);
  }
};

/**
  Writes a string representation of a map to a format stream.

  @relates Map
*/
template<class KEY, class VALUE>
FormatOutputStream& operator<<(
  FormatOutputStream& stream, const Map<KEY, VALUE>& value) throw(IOException)
{
  typename Map<KEY, VALUE>::ReadEnumerator enu = value.getReadEnumerator();
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
