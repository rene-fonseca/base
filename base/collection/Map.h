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
  typedef OrderedBinaryTree<Node, Key> Tree;
  typedef typename Tree::Iterator Iterator; // iteration over Association
  typedef typename Tree::ReadIterator ReadIterator; // iteration over Association
private:

  /** The associations of the map. */
  Tree elements;
  /** The number of associations in the map. */
  MemorySize size = 0; // belongs in tree
public:

  /**
    @short Non-modifying enumerator of map.
  */
  class ReadEnumerator : public Enumerator<ReadEnumeratorTraits<Node> > {
  private:

    typedef typename Enumerator<ReadEnumeratorTraits<Node> >::Pointer Pointer;
    typename Tree::ReadEnumerator enu;
  public:

    inline ReadEnumerator(typename Tree::ReadEnumerator _enu) noexcept
      : enu(_enu)
    {
    }

    inline bool hasNext() const noexcept
    {
      return enu.hasNext();
    }

    inline Pointer next()
    {
      return &(enu.next()->getValue());
    }
  };

  /**
    @short Modifying enumerator of values of map.
    @version 1.0
  */
  class ValueEnumerator : public Enumerator<EnumeratorTraits<Value> > {
  private:

    typedef typename ValueEnumerator::Pointer Pointer;
    // typedef typename ValueEnumerator::Reference Reference;
    typename Tree::Enumerator enu;
  public:

    inline ValueEnumerator(typename Tree::Enumerator _enu) noexcept
      : enu(_enu)
    {
    }

    inline bool hasNext() const noexcept
    {
      return enu.hasNext();
    }

    // TAG: return by value instead
    inline Pointer next()
    {
      return &(enu.next()->getValue().getValue());
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

    Element& operator=(const Element& assign) noexcept = delete;
    
    inline Element(Map& _map, const Key& _key)
      : map(_map), key(_key)
    {
    }
  public:

    inline Element(const Element& copy) noexcept
      : map(copy.map),
        key(copy.key)
    {
    }

    inline Element& operator=(const Value& value)
    {
      map.add(key, value);
      return *this;
    }

    inline operator Value&()
    {
      return map.getValue(key);
    }

    inline operator const Value&() const
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
    Initializes map with given values.
  */
  Map(std::initializer_list<Node> values)
  {
    for (const auto& value : values) {
      add(value);
    }
  }
  
  /**
    Initializes map from other map.
  */
  Map(const Map& copy)
    : elements(copy.elements),
      size(copy.size)
  {
  }

  /**
    Initializes map from other map.
  */
  Map(Map&& move)
    : elements(moveObject(move.elements)),
      size(moveObject(move.size))
  {
  }

  /**
    Assignment of map to map.
  */
  Map& operator=(const Map& copy)
  {
    if (this != &copy) {
      elements = copy.elements;
      size = copy.size;
    }
    return *this;
  }
  
  /**
    Assignment of map to map.
  */
  Map& operator=(Map&& move)
  {
    if (this != &move) {
      elements = moveObject(move.elements);
      size = moveObject(move.size);
      move.size = 0;
    }
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
    return elements.getRoot() == nullptr;
  }

  /**
    Returns true if the map is not empty.
  */
  inline operator bool() const noexcept
  {
    return elements.getRoot();
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
  bool hasKey(const Key& key) const noexcept
  {
    return elements.find(key);
  }

  /**
    Returns the value associated with the specified key.

    @param key The key of the value.

    @return nullptr is key doesn't exist.
  */
  Value* find(const Key& key)
  {
    typename Tree::Node* node = elements.find(key);
    if (!node) {
      return nullptr;
    }
    Node& association = node->getValue();
    return &(association.getValue());
  }

  /**
    Returns the value associated with the specified key.

    @param key The key of the value.

    @return nullptr is key doesn't exist.
  */
  const Value* find(const Key& key) const
  {
    const typename Tree::Node* node = elements.find(key);
    if (!node) {
      return nullptr;
    }
    const Node& association = node->getValue();
    return &(association.getValue());
  }

  /**
    Returns the value associated with the specified key. Raises InvalidKey
    if the specified key doesn't exist in this map.

    @param key The key of the value.
  */
  Value& getValue(const Key& key)
  {
    typename Tree::Node* node = elements.find(key);
    if (!node) {
      _throw InvalidKey();
    }
    Node& association = node->getValue();
    return association.getValue();
  }

  /**
    Returns the value associated with the specified key. Raises InvalidKey
    if the specified key doesn't exist in this map.

    @param key The key of the value.
  */
  const Value& getValue(const Key& key) const
  {
    const typename Tree::Node* node = elements.find(key);
    if (!node) {
      _throw InvalidKey();
    }
    const Node& association = node->getValue();
    return association.getValue();
  }

  void rebalance()
  {
    elements.rebalance();
  }

  /**
    Associates the specified key with the specified value in this map. If the
    key already is associated with a value, the value is overridden.

    @param key The key.
    @param value The value.
   
    @return Returns true if new key was added.
  */
  bool add(const Key& key, const Value& value)
  {
    // TAG: rotate when new node is added? but we need to know length of chains
    Pair<typename Tree::Node*, bool> result = elements.add(Node(key, value));
    if (!result.getSecond()) {
      // key already exists
      typename Tree::Node* node = result.getFirst();
      Node& association = node->getValue();
      association.setValue(value); // set the new value
      return false;
    } else {
      // key did not exist
      ++size;
      return true;
    }
  }

  /**
    Associates the specified key with the specified value in this map. If the
    key already is associated with a value, the value is overridden.

    @param key The key.
    @param value The value.
   
    @return Returns true if new key was added.
  */
  bool add(const Key& key, Value&& value)
  {
    // auto result = elements.add(Node(key, moveObject(value)));
    typename Tree::Node* node = elements.find(key);
    if (node) { // key already exists
      Node& association = node->getValue();
      association.setValue(moveObject(value)); // set the new value
      return false;
    } else { // key does not exist
      // annoying to search tree twice
      auto result = elements.add(Node(key, moveObject(value)));
      ++size;
      return true;
    }
  }

  bool add(const Node& value)
  {
    Pair<typename Tree::Node*, bool> result = elements.add(value);
    if (!result.getSecond()) {
      // key already exists
      typename Tree::Node* node = result.getFirst();
      Node& association = node->getValue();
      association.setValue(value.getValue()); // set the new value
      return false;
    } else {
      // key did not exist
      ++size;
      return true;
    }
  }

  /**
    Removes the specified key and its associated value from this map. Raises
    InvalidKey if the key doesn't exist in this map.
  */
  void remove(const Key& key)
  {
    typename Tree::Node* node = elements.find(key);
    if (!node) {
      _throw InvalidKey();
    }
    elements.remove(node);
    --size; // never ends up here if the key doesn't exist
  }
  
  void remove(const Iterator& it)
  {
    elements.remove(it);
    --size;
  }

  template<class PREDICATE>
  class NodePredicate {
  private:
    
    PREDICATE& predicate;
  public:
    
    inline NodePredicate(PREDICATE& _predicate) noexcept : predicate(_predicate)
    {
    }
    
    inline bool operator()(const typename Tree::Node* node)
    {
      return predicate(node->getValue());
    }
  };
  
  template<class PREDICATE>
  void removeByPredicate(PREDICATE predicate)
  {
    NodePredicate<PREDICATE> _predicate(predicate);
    MemorySize removed = elements.removeByPredicate(_predicate);
    size -= removed;
  }
  
  /**
    Removes all the keys from this map.
  */
  void removeAll()
  {
    elements.removeAll();
    size = 0;
  }

#if 0
  /**
    Returns the value associated with the specified key when used as 'rvalue'.
    When used as 'lvalue' the key is associated with the specified value.
  */
  inline Element operator[](const Key& key)
  {
    return Element(*this, key);
  }
#else
  /**
    Returns the value associated with the specified key.
  */
  inline Value& operator[](const Key& key)
  {
    return getValue(key);
  }
#endif

  /**
    Returns the value associated with the specified key.
  */
  inline const Value& operator[](const Key& key) const
  {
    return getValue(key);
  }

  inline const Tree& getTree() const noexcept
  {
    return elements;
  }
  
  inline Iterator begin() noexcept
  {
    // elements.copyOnWrite();
    return Iterator(elements.getRoot());
  }

  inline Iterator end() noexcept
  {
    // elements.copyOnWrite();
    return Iterator();
  }
  
  inline ReadIterator begin() const noexcept
  {
    return ReadIterator(elements.getRoot());
  }
  
  inline ReadIterator end() const noexcept
  {
    return ReadIterator();
  }
  
  /** Adds value. */
  inline Map& operator<<(const Node& value)
  {
    add(value);
    return *this;
  }

  /** Adds value. */
  inline Map& operator<<(Node&& value)
  {
    add(moveObject(value));
    return *this;
  }
};

/**
  Writes a string representation of a map to a format stream.

  @relates Map
*/
template<class KEY, class VALUE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Map<KEY, VALUE>& value)
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
