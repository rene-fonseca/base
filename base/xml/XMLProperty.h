/***************************************************************************
    The Base Framework
    A framework for developing platform independent applications

    Copyright (C) 2001 by René Møller Fonseca <fonseca@mip.sdu.dk>

    This framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    For the licensing terms refer to the file 'LICENSE'.
 ***************************************************************************/

#ifndef _DK_SDU_MIP__BASE_XML__XML_PROPERTY_H
#define _DK_SDU_MIP__BASE_XML__XML_PROPERTY_H

#include <base/Object.h>
#include <base/string/String.h>

_DK_SDU_MIP__BASE__ENTER_NAMESPACE

/**
  @short XML property.
  @author René Møller Fonseca
*/

class XMLProperty : public Object {
private:

  /** The name of the XML property. */
  String name;
  /** The value of the XML property. */
  String value;
public:

  /**
    Initialize XML property with the specified name and empty value.

    @param name The desired name of the property.
  */
  XMLProperty(const String& name) throw();

  /**
    Initializes XML property with name and value.

    @param name The desired name of the property.
    @param value The desired value of the property.
  */
  XMLProperty(const String& name, const String& value) throw();

  /**
    Initializes XML attribute from other XML attribute.
  */
  inline XMLProperty(const XMLProperty& copy) throw() : name(copy.name), value(copy.value) {}

  /**
    Assignemnt of attribute by attribute.
  */
  inline XMLProperty& operator=(const XMLProperty& eq) throw() {
    name = eq.name;
    value = eq.value;
    return *this;
  }

  /**
    Returns the name of the property.
  */
  inline const String& getName() const throw() {return name;}

  /**
    Returns the value of the property.
  */
  inline const String& getValue() const throw() {return value;}

  /**
    Sets the value of the property.
  */
  inline const String& setValue(const String& value) {this->value = value;}
};

inline XMLProperty::XMLProperty(const String& n) throw() : name(n) {}

inline XMLProperty::XMLProperty(const String& n, const String& v) throw() : name(n), value(v) {}

_DK_SDU_MIP__BASE__LEAVE_NAMESPACE

#endif
