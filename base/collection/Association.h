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

#include <base/string/FormatOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Association of value with other value. An association has the properties of
  its key (i.e. the key is the only significant when comparing associations
  with each other). The value of the association is the associated value.

  @short Association of value with value
  @see Map
  @ingroup collections
  @version 1.0
*/
template<class KEY, class VALUE>
class Association {
private:

  /** The key of the association. */
  KEY key;
  /** The value associated with the key. */
  VALUE value;

  /* Disable the default assignment operator. */
  Association& operator=(const Association& eq) throw();
public:

  /**
    Initializes an association with the specified key and uninitalized value.

    @param key The
  */
  inline Association(const KEY& _key) throw() : key(_key) {
  }

  /**
    Initializes association from other association.
  */
  inline Association(const Association& copy) throw()
    : key(copy.key), value(copy.value) {
  }

  /**
    Initializes an association with the specified key and value.

    @param key The
    @param value The value of the association.
  */
  inline Association(const KEY& _key, const VALUE& _value) throw()
    : key(_key), value(_value) {
  }

  /**
    Returns the key value of the association.
  */
  /*
  inline const KEY& getKey() throw() {
    return key;
  }
  */

  /**
    Returns the key value of the association.
  */
  inline const KEY& getKey() const throw() {
    return key;
  }

  /**
    Returns the value of the association.
  */
  inline VALUE& getValue() throw() {
    return value;
  }

  /**
    Returns the value of the association.
  */
  inline const VALUE& getValue() const throw() {
    return value;
  }

  /**
    Sets the value of the association.

    @param value The new value.
  */
  inline void setValue(const VALUE& value) throw() {
    this->value = value;
  }

  /**
    Returns true if the associations are equal.
  */
  inline bool operator==(const Association& eq) const throw() {
    return key == eq.key;
  }

  /**
    Returns true if this association is less than the specified association.
  */
  inline bool operator<(const Association& eq) const throw() {
    return key < eq.key;
  }

  /**
    Compares the specified associations with each other.
  */
  friend int compare<>(const Association& left, const Association& right) throw();
};

template<class KEY, class VALUE>
inline int compare(
  const Association<KEY, VALUE>& left,
  const Association<KEY, VALUE>& right) throw() {
  return compare(left.getKey(), right.getKey());
}

/**
  Writes association to format output stream.
*/
template<class KEY, class VALUE>
FormatOutputStream& operator<<(
  FormatOutputStream& stream,
  const Association<KEY, VALUE>& value) throw(IOException) {
  return stream << '[' << value.getKey() << ']' << '=' << value.getValue();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE
