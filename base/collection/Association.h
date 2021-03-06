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

#include <base/string/StringOutputStream.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

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
public:

  /**
    Default initializes association.
  */
  Association()
  {
  }

  /**
    Initializes association with the specified key and uninitalized value.

    @param key The key.
  */
  Association(const KEY& _key)
    : key(_key)
  {
  }

  /**
    Initializes association with the specified key and uninitalized value.

    @param key The key.
  */
  Association(KEY&& _key)
    : key(moveObject(_key))
  {
  }

  /**
    Initializes association from other association.
  */
  Association(const Association& copy)
    : key(copy.key),
      value(copy.value)
  {
  }

  /**
    Initializes association from other association.
  */
  Association(Association&& move)
    : key(moveObject(move.key)),
      value(moveObject(move.value))
  {
  }

  /**
    Initializes association with the specified key and value.

    @param key The key.
    @param value The value of the association.
  */
  Association(const KEY& _key, const VALUE& _value)
    : key(_key),
      value(_value)
  {
  }

  /**
    Initializes association with the specified key and value.

    @param key The key.
    @param value The value of the association.
  */
  Association(const KEY& _key, VALUE&& _value)
    : key(_key),
      value(moveObject(_value))
  {
  }

  /**
    Initializes association with the specified key and value.

    @param key The key.
    @param value The value of the association.
  */
  Association(KEY&& _key, VALUE&& _value)
    : key(moveObject(_key)),
      value(moveObject(_value))
  {
  }

  /**
    Initializes association with the specified key and value.

    @param key The key.
    @param value The value of the association.
  */
  Association(KEY&& _key, const VALUE& _value)
    : key(moveObject(_key)),
      value(_value)
  {
  }

  /* Assigns new value. The key is NOT changed. */
  Association& operator=(const Association& assign)
  {
    BASSERT(key == assign.key);
    // key is not changed!
    value = assign.value;
    return *this;
  }

  /* Assigns new value. The key is NOT changed. */
  Association& operator=(Association&& assign)
  {
    BASSERT(key == assign.key);
    value = moveObject(assign.value);
    return *this;
  }

#if 0 // do not allow key to be changed in-place
  /**
    Returns the key value of the association.
  */
  inline KEY& getKey() noexcept
  {
    return key;
  }
#endif

  /**
    Returns the key value of the association.
  */
  inline const KEY& getKey() const noexcept
  {
    return key;
  }

  /**
    Returns the value of the association.
  */
  inline VALUE& getValue() noexcept
  {
    return value;
  }

  /**
    Returns the value of the association.
  */
  inline const VALUE& getValue() const noexcept
  {
    return value;
  }

  /**
    Sets the value of the association.

    @param value The new value.
  */
  inline void setValue(const VALUE& value)
  {
    this->value = value;
  }

  /**
    Sets the value of the association.

    @param value The new value.
  */
  inline void setValue(VALUE&& value)
  {
    this->value = moveObject(value);
  }

  /**
    Returns true if the associations are equal.
  */
  inline bool operator==(const Association& compare) const
  {
    return key == compare.key;
  }

  /**
    Returns true if the associations are not equal.
  */
  inline bool operator!=(const Association& compare) const
  {
    return !(operator==(compare));
  }

  /**
    Returns true if this association is less than the specified association.
  */
  inline bool operator<(const Association& compare) const
  {
    return key < compare.key;
  }

  /**
    Returns true if this association is less than or equal to the specified association.
  */
  inline bool operator<=(const Association& compare) const
  {
    return key <= compare.key;
  }

  /**
    Returns true if this association is greater than or equal to the specified association.
  */
  inline bool operator>=(const Association& compare) const
  {
    return !(operator<(compare));
  }

  /**
    Returns true if this association is greater that the specified association.
  */
  inline bool operator>(const Association& compare) const
  {
    return !(operator<=(compare));
  }

  /**
    Returns true if the associations are equal.
  */
  inline bool operator==(const KEY& compare) const
  {
    return key == compare;
  }

  /**
    Returns true if the associations are not equal.
  */
  inline bool operator!=(const KEY& compare) const
  {
    return !operator==(compare);
  }

  /**
    Returns true if this association is less than the specified association.
  */
  inline bool operator<(const KEY& compare) const
  {
    return key < compare;
  }

  /**
    Returns true if this association is less than or equal to the specified association.
  */
  inline bool operator<=(const KEY& compare) const
  {
    return key <= compare;
  }

  /**
    Returns true if this association is less than or equal to the specified association.
  */
  inline bool operator>=(const KEY& compare) const
  {
    return !(operator<(compare));
  }

  /**
    Returns true if this association is greater than the specified association.
  */
  inline bool operator>(const KEY& compare) const
  {
    return !(operator<=(compare));
  }

  /**
    Returns the key value of the association. Requires for implicit comparison.
  */
  inline operator const KEY&() const noexcept
  {
    return key;
  }

  /**
    Compares the specified associations with each other.
  */
  friend int compare<>(const Association& left, const Association& right);
};

template<class KEY, class VALUE>
inline int compare(
  const Association<KEY, VALUE>& left,
  const Association<KEY, VALUE>& right)
{
  return compare(left.getKey(), right.getKey());
}

/**
  Writes association to format output stream.
*/
template<class KEY, class VALUE>
FormatOutputStream& operator<<(
  FormatOutputStream& stream,
  const Association<KEY, VALUE>& value)
{
  return stream << '[' << value.getKey() << ']' << '=' << value.getValue();
}

// TAG: move to proper place
namespace association {

/** Returns HTML <tr> header. */
inline FormatOutputStream& getHeaderAsTR(FormatOutputStream& stream, const String& text)
{
  return stream << "<tr>" << "<th colspan=\"2\" style=\"text-align: center\">"
    << text << "</th>" << "</tr>";
}

/** Returns Type and Value as HTML <tr>. */
template<class TYPE>
FormatOutputStream& getTypeValueAsTR(FormatOutputStream& stream, const TYPE& value)
{
  return stream << "<tr>" << "<td style=\"text-align: right\">"
    << TypeInfo::getTypename(Type::getType<TYPE>()) << "</td>" << "<td style=\"text-align: left\">"
    << HTMLEncode<TYPE>::map(value) << "</td>" << "</tr>";
}

}

/** Returns Association as HTML. */
template<class KEY, class VALUE>
String getAssociationAsHTML(const Association<KEY, VALUE>& value)
{
  StringOutputStream stream;
  stream << "<table>";
  association::getHeaderAsTR(stream, TypeInfo::getTypename(value));
  association::getTypeValueAsTR(stream, value.getKey());
  association::getTypeValueAsTR(stream, value.getValue());
  stream << "<table>";
  return stream.getString();
}

#if (_COM_AZURE_DEV__BASE__COMPILER == _COM_AZURE_DEV__BASE__COMPILER_CLING)
template<class KEY, class VALUE> \
inline ClingMimeBundle _COM_AZURE_DEV__BASE__CLING_GET_MIME_BUNDLE_ID(const Association<KEY, VALUE>& v) \
{ \
  return cling_getHTMLMimeBundle(getAssociationAsHTML(v)); \
}
#endif

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
