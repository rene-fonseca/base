/***************************************************************************
    copyright   : (C) 2000 by René Møller Fonseca
    email       : fonseca@mip.sdu.dk
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_COLLECTION__ASSOCIATION_H
#define _DK_SDU_MIP__BASE_COLLECTION__ASSOCIATION_H

#include <base/features.h>
#include <base/string/FormatOutputStream.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  Association of value with other value. An association has the properties of
  its key (i.e. the key is the only significant when comparing associations
  with each other).

  @see Map
  @author René Møller Fonseca
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
  Association& operator=(const Association& eq);
public:

  /**
    Initializes an association with the specified key and uninitalized value.

    @param key The
  */
  inline Association(const KEY& k) throw() : key(k) {}

  /**
    Initializes association from other association.
  */
  inline Association(const Association& copy) throw() : key(copy.key), value(copy.value) {}

  /**
    Initializes an association with the specified key and value.

    @param key The
    @param value The value of the association.
  */
  inline Association(const KEY& k, const VALUE& v) throw() : key(k), value(v) {}

  /**
    Returns the key value of the association.
  */
  inline KEY getKey() const throw() {return key;}

  /**
    Returns the value of the association.
  */
  inline VALUE getValue() const throw() {return value;}

  /**
    Sets the value of the association.

    @param value The new value.
  */
  inline void setValue(const VALUE& value) throw() {this->value = value;}

  /**
    Returns true if this association is less than the specified association.
  */
  inline bool operator<(const Association& eq) const throw() {return key < eq.key;}

  /**
    Returns true if this association is greater than the specified association.
  */
  inline bool operator>(const Association& eq) const throw() {return key > eq.key;}

  /**
    Returns true if the associations are equal.
  */
  inline bool operator==(const Association& eq) const throw() {return key == eq.key;}
};

/**
  Writes association to format output stream.
*/
template<class KEY, class VALUE>
FormatOutputStream& operator<<(FormatOutputStream& stream, const Association<KEY, VALUE>& value) {
  return stream << '[' << value.getKey() << ']' << '=' << value.getValue();
}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
