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

class Attributes : public Object {
public:

  /**
    @short XML attribute.
    @ingroup xml
    @version 1.0
  */
  class Attribute : public Object {
  private:
    
    /** The name. */
    String name;
    /** The value. */
    String value;
  public:
    
    /**
      Initializes attribute.
    */
    inline Attribute() throw() {
    }
    
    /**
      Initializes attribute.
    */
    inline Attribute(const String& _name, const String& _value) throw()
      : name(_name), value(_value) {
    }
    
    /**
      Initializes attribute from attribute.
    */
    inline Attribute(const Attribute& copy) throw()
      : name(copy.name), value(copy.value) {
    }

    /**
      Assignment of attribute by attribute.
    */
    inline Attribute& operator=(const Attribute& eq) throw() {
      name = eq.name;
      value = eq.value;
      return *this;
    }
    
    /**
      Returns the name of the attribute.
    */
    inline const String& getName() const throw() {
      return name;
    }
    
    /**
      Returns the value of the attribute.
    */
    inline const String& getValue() const throw() {
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
  inline Attributes() throw() {
  }
  
  /**
    Initializes the attributes.
  */
  inline Attributes(
    const Array<Attribute>& _attributes) throw()
    : attributes(_attributes) {
  }
  
  /**
    Initializes XML attributes from other XML attributes.
  */
  inline Attributes(const Attributes& copy) throw()
    : attributes(copy.attributes) {
  }
  
  /**
    Assignemnt of attributes by attributes.
  */
  inline Attributes& operator=(const Attributes& eq) throw() {
    attributes = eq.attributes;
    return *this;
  }

  /**
    Returns a hash table mapping attribute names to indices.
  */
  inline HashTable<String, unsigned int> getHashTable() const throw(MemoryException) {
    HashTable<String, unsigned int> result;
    for (unsigned int i = 0; i < attributes.getSize(); ++i) {
      result.add(attributes[i].getName(), i);
    }
    return result;
  }
  
//   /**
//     Look up the index of an attribute by XML 1.0 qualified name.
//   */
//   unsigned int getIndex(const String& qualifiedName) throw() {
//     return byName[name];
//   }
  
//   /**
//     Look up the index of an attribute by Namespace name.

//     @return -1 of not found.
//   */
//   int getIndex(const String& uri, const String& localPart) throw();

  /**
    Returns the number of attributes in the list.
  */
  inline unsigned int getSize() const throw() {
    return attributes.getSize();
  }
  
//   /**
//     Look up an attribute's local name by index.
//   */
//   String getLocalName(unsigned int index) const throw();
  
//   /**
//     Look up an attribute's XML 1.0 qualified name by index.
//   */
//   String getQName(unsigned int index) const throw();
  
//   /**
//     Look up an attribute's type by index.
//   */
//   String getType(unsigned int index) throw();
  
//   /**
//     Look up an attribute's type by XML 1.0 qualified name.
//   */
//   String getType(String qName) throw();
  
//   /**
//     Look up an attribute's type by Namespace name.
//   */
//   String getType(String uri, String localName) throw();
  
//   /**
//     Look up an attribute's Namespace URI by index.
//   */
//   String getURI(unsigned int index) throw();

  /**
    Returns the attributes at the specified index.
  */
  inline const Attribute& getAttribute(unsigned int index) const throw() {
    return attributes[index];
  }
  
  /**
    Returns the attributes at the specified index.
  */
  inline const Attribute& operator[](unsigned int index) const throw() {
    return attributes[index];
  }
  
//   /**
//     Look up an attribute's value by XML 1.0 qualified name.
//   */
//   String getValue(const String& qName) throw() {
//     return byName[index].getValue();
//     return String();
//   }

  /**
    Look up an attribute's value by Namespace name.
  */
  //String getValue(const String& uri, const String& localName) throw();
};

_COM_AZURE_DEV__BASE__LEAVE_NAMESPACE
