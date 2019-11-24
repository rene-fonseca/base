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
#include <base/collection/Enumeration.h>
#include <base/string/FormatOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  Set collection implemented using an ordered binary tree.

  @short Set collection.
  @ingroup collections
  @version 1.1
*/

template<class KEY>
class Set : public Collection {
private:

  /** The elements of the set. */
  OrderedBinaryTree<KEY> elements;
  /** The number of elements in the set. */
  MemorySize size = 0;
public:

  /* Enumerator of set. */
  template<class TRAITS, class ENU>
  class SetEnumerator : public base::Enumerator<TRAITS> {
  public:

    typedef typename Enumerator<TRAITS>::Pointer Pointer;
    typedef typename Enumerator<TRAITS>::Value Value;
  private:

    ENU enu;
  public:

    inline SetEnumerator(const ENU& _enu) noexcept
      : enu(_enu)
    {
    }
    
    inline bool hasNext() const noexcept
    {
      return enu.hasNext();
    }
    
    inline Pointer next() throw(EndOfEnumeration)
    {
      return &(enu.next()->getValue());
    }
  };

  /** Modifying enumerator. */
  typedef SetEnumerator<EnumeratorTraits<KEY>, typename OrderedBinaryTree<KEY>::Enumerator> Enumerator;
  /** Non-modifying enumerator. */
  typedef SetEnumerator<ReadEnumeratorTraits<KEY>, typename OrderedBinaryTree<KEY>::ReadEnumerator> ReadEnumerator;
public:

  /**
    Initializes an empty set.
  */
  inline Set() noexcept
  {
  }

  /**
    Initializes set from other set.
  */
  inline Set(const Set& copy) noexcept
    : elements(copy.elements)
  {
  }
  
  /**
    Returns the number of elements in the collection.
  */
  inline MemorySize getSize() const noexcept
  {
    return size;
  }

  /**
    Returns true if the collection is empty.
  */
  inline bool isEmpty() const noexcept
  {
    return size != 0;
  }

  /**
    Returns a modifying enumerator of the ordered binary tree.
  */
  inline Enumerator getEnumerator() noexcept
  {
    return Enumerator(elements.getEnumerator());
  }

  /**
    Returns a non-modifying enumerator of the ordered binary tree.
  */
  inline ReadEnumerator getReadEnumerator() const noexcept
  {
    return ReadEnumerator(elements.getReadEnumerator());
  }

  /**
    Returns true if the specified key is present is this set.

    @param key The value to search for.
  */
  bool hasKey(const KEY& key) const noexcept
  {
    return elements.find(key);
  }

  /**
    Adds the specified key to the set.

    @param key The key to be added to the set.
  */
  const KEY* add(const KEY& key)
  {
    const KEY* result = elements.add2(key);
    if (!result) {
      ++size; // key did not exist
    }
    return result;
  }

  /**
    Removes the specified key from this set. Raises InvalidNode if the key
    doesn't exist in the set.

    @param key The value to be removed from the set.
  */
  void remove(const KEY& key) throw(InvalidNode)
  {
    elements.remove(elements.find(key));
    --size;
  }

  // add template<class PREDICATE> Set::remove(PREDICATE& predicate);

  /**
    Removes all the keys from this set.
  */
  void removeAll() noexcept
  {
    elements.removeAll();
    size = 0;
  }
  
  inline operator bool() const noexcept
  {
    return size != 0;
  }
};

/**
  Writes a string representation of a set to a format stream.

  @relates Set
*/
template<class TYPE>
FormatOutputStream& operator<<(
  FormatOutputStream& stream, const Set<TYPE>& value)  throw(IOException)
{
  typename Set<TYPE>::ReadEnumerator enu = value.getReadEnumerator();
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
