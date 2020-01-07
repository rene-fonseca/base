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

#include <base/collection/Array.h>
#include <base/collection/Association.h>
#include <base/collection/InvalidKey.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Map using Array container. Use this for returning a limited size set of values from functions.
  
  Linear search for key! But fine as long as only a few items are added.

  @short Association of value with value
  @see Map
  @ingroup collections
*/

template<class KEY, class VALUE>
class ArrayMap : public Array<Association<KEY, VALUE> > {
public:

  typedef Association<KEY, VALUE> Node;
  typedef Array<Association<KEY, VALUE> > Base;

  /**
    Initializes an empty map.
  */
  inline ArrayMap() noexcept
  {
  }

  /**
    Initializes map with given values.
  */
  ArrayMap(std::initializer_list<Node> values)
  {
    Base::ensureCapacity(values.size());
    for (auto& value : values) {
      Base::append(value); // C++: https://isocpp.org/files/papers/N4166.pdf
    }
  }
  
  /**
    Initializes map from other map.
  */
  inline ArrayMap(const ArrayMap& copy)
    : Base(copy)
  {
  }

  /**
    Initializes map from other map.
  */
  inline ArrayMap(ArrayMap&& move)
    : Base(moveObject(move))
  {
  }

  /**
    Assign map.
  */
  inline ArrayMap& operator=(const ArrayMap& assign)
  {
    Base::operator=(assign);
    return *this;
  }

  /**
    Assign map.
  */
  inline ArrayMap& operator=(ArrayMap&& assign)
  {
    Base::operator=(moveObject(assign));
    return *this;
  }

  /** Returns reference to the item if found by the given value. */
  Node* find(const KEY& key)
  {
    const auto _end = Base::end();
    for (auto src = Base::begin(); src != _end; ++src) {
      if (*src == key) {
        return &*src;
      }
    }
    return nullptr;
  }

  /** Returns reference to the item if found by the given value. */
  const Node* find(const KEY& key) const
  {
    const auto _end = Base::cend();
    for (auto src = Base::cbegin(); src != _end; ++src) {
      if (*src == key) {
        return &*src;
      }
    }
    return nullptr;
  }

  /**
    Returns true if the key exists.
  */
  inline bool hasKey(const KEY& key) const noexcept
  {
    return find(key) != nullptr;
  }

  /**
    Returns the value of associated with the given key. Throws InvalidKey
    is key does not exist.
  */
  VALUE& getValue(const KEY& key)
  {
    if (auto node = find(key)) {
      return node->getValue();
    }
    _throw InvalidKey("Key does not exist.");
  }

  /**
    Returns the value of associated with the given key. Throws InvalidKey
    is key does not exist.
  */
  const VALUE& getValue(const KEY& key) const
  {
    if (auto node = find(key)) {
      return node->getValue();
    }
    _throw InvalidKey("Key does not exist.");
  }

  /**
    Associates the specified key with the specified value in this map. If the
    key already is associated with a value, the value is overridden. Linear
    search for key.

    @param key The key.
    @param value The value.
   
    @return Returns true if new key was added.
  */
  bool add(const KEY& key, const VALUE& value)
  {
    if (auto node = find(key)) {
      node->setValue(value);
      return false;
    }
    Base::setSize(Base::getSize() + 1, Node(key, value));
    return true;
  }

 #if 0 // not allowed due to protected key of Association - we could use Pair instead of Association
  /**
    Removes the specified key and its associated value from this map. Raises
    InvalidKey if the key doesn't exist in this map.
  */
  void remove(const KEY& key)
  {
    const auto _end = Base::end();
    for (auto src = Base::begin(); src != _end; ++src) {
      if (*src == key) {
        Base::remove(src - _begin);
        return;
      }
    }
    _throw InvalidKey();
  }
#endif
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
