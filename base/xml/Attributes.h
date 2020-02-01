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

#include <base/Object.h>
#include <base/string/String.h>
#include <base/collection/Array.h>
#include <base/collection/HashTable.h>

_COM_AZURE_DEV__BASE__ENTER_NAMESPACE

/**
  @short List of XML attributes.
  @ingroup xml
  @version 1.0
*/

class _COM_AZURE_DEV__BASE__API Attributes : public Object {
public:

  /**
    @short XML attribute.
    @ingroup xml
    @version 1.0
  */
  class _COM_AZURE_DEV__BASE__API Attribute : public Object {
  private:
    
    /** The name. */
    String name;
    /** The value. */
    String value;
  public:
    
    /**
      Initializes attribute.
    */
    inline Attribute() noexcept {
    }
    
    /**
      Initializes attribute.
    */
    inline Attribute(const String& _name, const String& _value) noexcept
      : name(_name), value(_value) {
    }
    
    /**
      Initializes attribute from attribute.
    */
    inline Attribute(const Attribute& copy) noexcept
      : name(copy.name), value(copy.value)
    {
    }

    /**
      Assignment of attribute by attribute.
    */
    inline Attribute& operator=(const Attribute& assign) noexcept
    {
      if (&assign != this) {
        name = assign.name;
        value = assign.value;
      }
      return *this;
    }
    
    /**
      Returns the name of the attribute.
    */
    inline const String& getName() const noexcept
    {
      return name;
    }
    
    /**
      Returns the value of the attribute.
    */
    inline const String& getValue() const noexcept
    {
      return value;
    }
  };
private:

  /** Attributes by index. */
  Array<Attribute> attributes;
public:

  /**
    Initializes the attributes.
  */
  inline Attributes() noexcept
  {
  }
  
  /**
    Initializes the attributes.
  */
  inline Attributes(
    const Array<Attribute>& _attributes) noexcept
    : attributes(_attributes)
  {
  }
  
  /**
    Initializes XML attributes from other XML attributes.
  */
  inline Attributes(const Attributes& copy) noexcept
    : attributes(copy.attributes)
  {
  }
  
  /**
    Assignemnt of attributes by attributes.
  */
  inline Attributes& operator=(const Attributes& assign) noexcept
  {
    if (&assign != this) {
      attributes = assign.attributes;
    }
    return *this;
  }

  /**
    Returns a hash table mapping attribute names to indices.
  */
  inline HashTable<String, unsigned int> getHashTable() const
  {
    HashTable<String, unsigned int> result;
    for (unsigned int i = 0; i < attributes.getSize(); ++i) {
      result.add(attributes[i].getName(), i);
    }
    return result;
  }
  
//   /**
//     Look up the index of an attribute by XML 1.0 qualified name.
//   */
//   unsigned int getIndex(const String& qualifiedName) noexcept
//   {
//     return byName[name];
//   }
  
//   /**
//     Look up the index of an attribute by Namespace name.

//     @return -1 of not found.
//   */
//   int getIndex(const String& uri, const String& localPart) noexcept;

  /**
    Returns the number of attributes in the list.
  */
  inline MemorySize getSize() const noexcept
  {
    return attributes.getSize();
  }
  
//   /**
//     Look up an attribute's local name by index.
//   */
//   String getLocalName(unsigned int index) const noexcept;
  
//   /**
//     Look up an attribute's XML 1.0 qualified name by index.
//   */
//   String getQName(unsigned int index) const noexcept;
  
//   /**
//     Look up an attribute's type by index.
//   */
//   String getType(unsigned int index) noexcept;
  
//   /**
//     Look up an attribute's type by XML 1.0 qualified name.
//   */
//   String getType(String qName) noexcept;
  
//   /**
//     Look up an attribute's type by Namespace name.
//   */
//   String getType(String uri, String localName) noexcept;
  
//   /**
//     Look up an attribute's Namespace URI by index.
//   */
//   String getURI(unsigned int index) noexcept;

  /**
    Returns the attributes at the specified index.
  */
  inline const Attribute& getAttribute(unsigned int index) const noexcept
  {
    return attributes[index];
  }
  
  /**
    Returns the attributes at the specified index.
  */
  inline const Attribute& operator[](unsigned int index) const noexcept
  {
    return attributes[index];
  }
  
//   /**
//     Look up an attribute's value by XML 1.0 qualified name.
//   */
//   String getValue(const String& qName) noexcept
//   {
//     return byName[index].getValue();
//     return String();
//   }

  /**
    Look up an attribute's value by Namespace name.
  */
  //String getValue(const String& uri, const String& localName) noexcept;
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
